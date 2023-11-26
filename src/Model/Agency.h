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

#ifndef TRANSIT_CLOCK_AGENCY_H
#define TRANSIT_CLOCK_AGENCY_H

#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <Utilities/Factory.h>
#include <Model/Route.h>
#include <Model/Prediction.h>
#include <Model/Stop.h>

class AgencyAPIError : std::runtime_error
{
public:
    AgencyAPIError(const std::string & message) noexcept;
    AgencyAPIError(const std::string & message, const nlohmann::json & response) noexcept;
    ~AgencyAPIError() = default;

    [[nodiscard]]
    const nlohmann::json & response_object() const;

private:
    nlohmann::json mResponseObject;
};


class Agency
{
protected:
    Agency() = default;

public:
    virtual ~Agency() = default;

    /**
     * Builds this object from the given parsed JSON configuration data object.
     * @param name The name of this Agency.
     * @param config_object The JSON object loaded from the system's configuration file.
     */
    virtual void load_from_config(const std::string & name, const nlohmann::json & config_object) = 0;

    /**
     * Updates the data associated with this object.
     */
    virtual void update_data() = 0;

    /**
     * @return This agency's name.
     */
    virtual const std::string & agency_name() const = 0;

    /**
     * @return The reference name of this agency's driver.
     */
    virtual const std::string & agency_type() const = 0;

    /**
     * Gets the list of routes for this agency.
     */
    std::vector<Route> routes();

    /**
     * Gets the current list of predictions.
     */
    std::vector<Prediction> predictions();

    /**
     * Gets the list of stops for this agency.
     */
    std::vector<Stop> stops();

    /**
     * Gets the update interval for this agency.
     */
    [[nodiscard]] std::chrono::milliseconds update_interval() const { return mUpdateInterval; }

    /**
     * Gets the last update time for this Agency.
     */
    [[nodiscard]] std::chrono::system_clock::time_point last_update() const { return mLastUpdate; }

    /**
     * Sets the update interval for this agency.
     */
    void update_interval(std::chrono::milliseconds interval) { mUpdateInterval = interval; }

    /**
     * Checks to see if this Agency is due for an update.
     */
    [[nodiscard]] bool due_for_update() const;

protected:
    /**
     * Updates the list of routes.
     * @note This erases the list of predictions!
     */
    void update_routes(const std::vector<Route> & routes);

    /**
     * Updates the list of predictions.
     * @note This erases the list of predictions!
     */
    void update_predictions(const std::vector<Prediction> & predictions);

    /**
     * Updates the list of routes.
     * @note This erases the list of predictions!
     */
    void update_stops(const std::vector<Stop> & stops);

private:
    std::chrono::system_clock::time_point mLastUpdate = std::chrono::system_clock::now();
    std::chrono::milliseconds mUpdateInterval = std::chrono::milliseconds(120000);
    std::mutex mDataMutex;
    std::vector<Route> mDisplayedRoutes;
    std::vector<Prediction> mDisplayedPredictions;
    std::vector<Stop> mDisplayedStops;
};


#endif //TRANSIT_CLOCK_AGENCY_H
