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

#ifndef TRANSIT_CLOCK_ROUTE_H
#define TRANSIT_CLOCK_ROUTE_H

#include <string>
#include <led-matrix.h>

class Route
{
public:
    Route() { };
    Route(const std::string & identifier,
          const std::string & name,
          const rgb_matrix::Color & color,
          const std::string & display_identifier = "") :
            mIdentifier(identifier),
            mName(name),
            mColor(color),
            mDisplayIdentifier(display_identifier.empty() ? identifier : display_identifier) { }

    const std::string & identifier() const { return mIdentifier; }
    const std::string & name() const { return mName; }
    const rgb_matrix::Color & color() const { return mColor; }
    const std::string & display_identifier() const { return mDisplayIdentifier; }

private:
    std::string mIdentifier;
    std::string mName;
    rgb_matrix::Color mColor;
    std::string mDisplayIdentifier;
};

#endif //TRANSIT_CLOCK_ROUTE_H
