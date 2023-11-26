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

#ifndef TRANSIT_CLOCK_CONFIG_H
#define TRANSIT_CLOCK_CONFIG_H

#include <source_location>
#include <vector>
#include <led-matrix.h>

class Agency;

class Config
{
public:
    Config();
    virtual ~Config();

    [[nodiscard]]
    const rgb_matrix::RuntimeOptions & get_runtime_options() const;

    [[nodiscard]]
    const rgb_matrix::RGBMatrix::Options & get_matrix_options() const;

    [[nodiscard]]
    const std::vector<Agency *> & get_agency_list() const;

    void set_runtime_options(const rgb_matrix::RuntimeOptions & options);
    void set_matrix_options(const rgb_matrix::RGBMatrix::Options & options);
    void add_agency(Agency * mAgency);

    void lock_config();

private:
    /**
     * Checks the state of the lock, if this class is about to be modified. If the config is locked, throws an exception.
     * Otherwise returns.
     */
    void check_lock(const std::source_location & location = std::source_location::current()) const;

    bool mLocked = false;

    rgb_matrix::RuntimeOptions mRuntimeOptions;
    rgb_matrix::RGBMatrix::Options mMatrixOptions;

    std::vector<Agency *> mAgencies;
};


#endif //TRANSIT_CLOCK_CONFIG_H
