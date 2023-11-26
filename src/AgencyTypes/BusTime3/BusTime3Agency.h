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

#ifndef TRANSIT_CLOCK_BUSTIME3AGENCY_H
#define TRANSIT_CLOCK_BUSTIME3AGENCY_H

#include <chrono>
#include <graphics.h>
#include <Model/Agency.h>
#include <Model/Stop.h>
#include "Model/Route.h"

namespace cpr
{
    class Parameters;
}

class BusTime3Agency : public Agency
{
public:
    BusTime3Agency() = default;
    virtual ~BusTime3Agency();

    [[nodiscard]]
    const std::string & agency_name() const override;

    [[nodiscard]]
    const std::string & agency_type() const override;

    void load_from_config(const std::string & name, const nlohmann::json & config_object) override;

    void update_data() override;

private:
    nlohmann::json make_api_request(const std::string & method, const cpr::Parameters & parameters) const;

    void update_time_offset();

    void load_from_api();

    rgb_matrix::Color hex_to_color(const std::string & hex_string);

    std::string get_stop_id_list() const;

    std::chrono::system_clock::time_point system_time_from_api_timestamp(const std::string & timestamp) const;

    static const std::string mType;
    std::string mName;
    std::string mEndpoint;
    std::string mKey;
    std::string mDataFeed;

    std::chrono::milliseconds mCurrentTimeOffset{0};

    std::map<std::string, Stop> mStops;
    std::map<std::string, Route> mRoutes;
};


#endif //TRANSIT_CLOCK_BUSTIME3AGENCY_H
