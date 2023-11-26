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

#include "Agency.h"

AgencyAPIError::AgencyAPIError(const std::string & message) noexcept :
        std::runtime_error(message)
{

}

AgencyAPIError::AgencyAPIError(const std::string & message, const nlohmann::json & response) noexcept :
        std::runtime_error(message + "\n" + response.dump(4, ' ', true)),
        mResponseObject(response)
{

}

const nlohmann::json & AgencyAPIError::response_object() const
{
    return mResponseObject;
}

std::vector<Prediction> Agency::predictions()
{
    std::lock_guard<std::mutex> lock(mDataMutex);
    return mDisplayedPredictions;
}

std::vector<Route> Agency::routes()
{
    std::lock_guard<std::mutex> lock(mDataMutex);
    return mDisplayedRoutes;
}

std::vector<Stop> Agency::stops()
{
    std::lock_guard<std::mutex> lock(mDataMutex);
    return mDisplayedStops;
}

void Agency::update_predictions(const std::vector<Prediction> & predictions)
{
    std::lock_guard<std::mutex> lock(mDataMutex);
    mDisplayedPredictions = predictions;
    mLastUpdate = std::chrono::system_clock::now();
}

void Agency::update_routes(const std::vector<Route> & routes)
{
    std::lock_guard<std::mutex> lock(mDataMutex);
    mDisplayedRoutes = routes;
    mLastUpdate = std::chrono::system_clock::now();
}

void Agency::update_stops(const std::vector<Stop> & stops)
{
    std::lock_guard<std::mutex> lock(mDataMutex);
    mDisplayedStops = stops;
    mLastUpdate = std::chrono::system_clock::now();
}

bool Agency::due_for_update() const
{
    return (std::chrono::system_clock::now() - mLastUpdate) > mUpdateInterval;
}