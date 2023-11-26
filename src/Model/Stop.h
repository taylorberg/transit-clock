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

#ifndef TRANSIT_CLOCK_STOP_H
#define TRANSIT_CLOCK_STOP_H

#include <string>
#include <vector>
#include "Prediction.h"

class Stop
{
public:
    Stop() { }

    explicit Stop(const std::string & stop_id) :
        mStopId(stop_id) { }

    Stop(const std::string & stop_id, const std::string & stop_name) :
        mStopId(stop_id),
        mStopName(stop_name) { }

    ~Stop() = default;

    const std::string & stop_id() const { return mStopId; }

    const std::string & stop_name() const { return mStopName; }

    void set_stop_id(const std::string & new_id) { mStopId = new_id; }

    void set_stop_name(const std::string & new_name) { mStopName = new_name; }

private:
    std::string mStopId;
    std::string mStopName;
};


#endif //TRANSIT_CLOCK_STOP_H
