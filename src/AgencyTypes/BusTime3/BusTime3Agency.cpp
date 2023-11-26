/*
 * TransitClock
 * Copyright (C) 2023-2024 Taylor Berg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "BusTime3Agency.h"

#include "cpr/cpr.h"

[[maybe_unused]] Factory<Agency>::Registration<BusTime3Agency> registration("clever_bustime_v3");

const std::string BusTime3Agency::mType = "clever_bustime_v3";

BusTime3Agency::~BusTime3Agency()
{

}

const std::string & BusTime3Agency::agency_name() const
{
    return mName;
}

const std::string & BusTime3Agency::agency_type() const
{
    return mType;
}

void BusTime3Agency::load_from_config(const std::string & name, const nlohmann::json & config_object)
{
    mName = name;

    mEndpoint = config_object["endpoint"];
    mKey = config_object["key"];
    mDataFeed = config_object["data-feed"];

    update_time_offset();

    std::cout << "Time offset is " << mCurrentTimeOffset << std::endl;

    for (auto & stop_id : config_object["stop-ids"])
    {
        mStops.emplace(stop_id, Stop(stop_id));
    }

    load_from_api();
}

void BusTime3Agency::update_time_offset()
{
    auto result = make_api_request("gettime", cpr::Parameters({{"unixTime", "true"}}));

    std::chrono::system_clock::time_point system_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point api_time(
            std::chrono::milliseconds(std::stoll(result["tm"].get<std::string>()))
    );

    mCurrentTimeOffset = std::chrono::duration_cast<std::chrono::milliseconds>(system_time - api_time);
}

void BusTime3Agency::load_from_api()
{
    // Load stop info
    auto stop_id_list = get_stop_id_list();
    auto result = make_api_request("getstops", cpr::Parameters({{"stpid", stop_id_list}}));

    for (auto & stop_info : result["stops"])
    {
        mStops[stop_info["stpid"]] = Stop(stop_info["stpid"], stop_info["stpnm"]);
    }

    // Load route info
    result = make_api_request("getroutes", cpr::Parameters({}));

    for (auto & route : result["routes"])
    {
        mRoutes[route["rt"]] = Route(
                    route["rt"],
                    route["rtnm"],
                    hex_to_color(route["rtclr"]),
                    route["rtdd"]
                );
    }
}

void BusTime3Agency::update_data()
{
    auto result = make_api_request("getpredictions", cpr::Parameters({
        {"stpid", get_stop_id_list()},
        {"tmres", "s"}
    }));

    std::vector<Prediction> new_predictions;

    for (auto & prediction : result["prd"])
    {
        uint8_t load_percent = 255;

        if (prediction["psgld"] == "EMPTY")
        {
            load_percent = 0;
        }
        else if (prediction["psgld"] == "HALF_EMPTY")
        {
            load_percent = 50;
        }
        else if (prediction["psgld"] == "FULL")
        {
            load_percent = 100;
        }

        new_predictions.emplace_back(
                this,
                &mStops[prediction["stpid"]],
                &mRoutes[prediction["rt"]],
                system_time_from_api_timestamp(prediction["prdtm"]),
                system_time_from_api_timestamp(prediction["tmstmp"]),
                std::stoll(prediction["dstp"].get<std::string>()) * 30.48,
                prediction["dly"],
                load_percent);
    }
}

nlohmann::json BusTime3Agency::make_api_request(const std::string & method,
                                                const cpr::Parameters & parameters) const
{
    // Create a copy of our list of parameters and add some specifying that we want the response in JSON
    // and providing our key
    cpr::Parameters to_send = parameters;
    to_send.Add({"format", "json"});
    to_send.Add({"key", mKey.c_str()});

    if (method != "gettime" && method != "getrtpidatafeeds")
    {
        to_send.Add({"rtpidatafeed", mDataFeed.c_str()});
    }

    // Make the request synchronously
    cpr::Response r = cpr::Get(cpr::Url{mEndpoint + "/bustime/api/v3/" + method},
                               to_send,
                               cpr::UserAgent{"github.com/taylorberg/transitclock"});

    std::cout << "====== Request for " << mName << std::endl;
    std::cout << r.url << std::endl;

    // Check for an error - if found, raise an exception
    nlohmann::json my_json = nlohmann::json::parse(r.text)["bustime-response"];

    std::cout << my_json.dump(2, ' ', true) << std::endl;

    if (my_json.contains("error"))
    {
        throw AgencyAPIError(my_json["error"][0]["msg"], my_json);
    }

    // Otherwise return the result
    return my_json;
}

std::string BusTime3Agency::get_stop_id_list() const
{
    bool first = true;
    std::string result = "";

    for (auto & [key, value] : mStops)
    {
        if (!first)
        {
            result.append(",");
        }
        first = false;
        result.append(key);
    }

    return result;
}

rgb_matrix::Color BusTime3Agency::hex_to_color(const std::string & hex_string)
{
    rgb_matrix::Color color;

    // typical CSS style hex string: #3300cc
    color.r = std::stoi(hex_string.substr(1, 3), nullptr, 16);
    color.g = std::stoi(hex_string.substr(3, 5), nullptr, 16);
    color.b = std::stoi(hex_string.substr(5, 7), nullptr, 16);

    return color;
}

std::chrono::system_clock::time_point
BusTime3Agency::system_time_from_api_timestamp(const std::string & timestamp) const
{
    std::tm time;
    std::stringstream ss(timestamp);
    ss >> std::get_time(&time, "%Y%m%d %H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&time));

}

