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

#include <stdexcept>
#include "Config.h"

#include "led-matrix.h"

#include <Model/Agency.h>

Config::Config()
{

}

Config::~Config()
{
    for (auto agency : mAgencies)
    {
        delete agency;
    }
}

const rgb_matrix::RuntimeOptions & Config::get_runtime_options() const
{
    return mRuntimeOptions;
}

const rgb_matrix::RGBMatrix::Options & Config::get_matrix_options() const
{
    return mMatrixOptions;
}

const std::vector<Agency *> & Config::get_agency_list() const
{
    return mAgencies;
}

void Config::set_runtime_options(const rgb_matrix::RuntimeOptions & options)
{
    check_lock();

    mRuntimeOptions = options;
}

void Config::set_matrix_options(const rgb_matrix::RGBMatrix::Options & options)
{
    check_lock();

    mMatrixOptions = options;
}

void Config::add_agency(Agency * mAgency)
{
    check_lock();

    mAgencies.push_back(mAgency);
}

void Config::lock_config()
{
    mLocked = true;
}

void Config::check_lock(const std::source_location & location) const
{
    if (mLocked)
    {
        throw std::domain_error(std::string(location.function_name()) + " called after config was locked!");
    }
}