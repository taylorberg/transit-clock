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

#ifndef TRANSIT_CLOCK_CONFIGLOADER_H
#define TRANSIT_CLOCK_CONFIGLOADER_H

#include <string>

#include "nlohmann/json.hpp"

class Config;

class ConfigLoader
{
public:
    ConfigLoader(const std::string & config_file);
    virtual ~ConfigLoader();

    static const uint32_t CONFIG_FORMAT_VERSION = 1;

private:
    nlohmann::json open_and_load(const std::string & config_file);

    void parse_config(const nlohmann::json & config_data);
    void parse_display_config(const nlohmann::json & display_config);
    void parse_agency_config(const std::string & name, const nlohmann::json & agency_config);

    uint64_t mFileVersion = 0;
    Config * mCurrentConfig = nullptr;

    std::string mHardwareMapping;
    std::string mLedRgbSequence;
    std::string mPixelMapperConfig;
    std::string mPanelType;
};


#endif //TRANSIT_CLOCK_CONFIGLOADER_H
