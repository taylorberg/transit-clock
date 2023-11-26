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

#include <fstream>
#include <iostream>
#include "ConfigLoader.h"
#include "Config.h"

#include <Utilities/Factory.h>
#include <Model/Agency.h>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

ConfigLoader::ConfigLoader(const std::string & config_file)
{
    json data = open_and_load(config_file);
    parse_config(data);
}

ConfigLoader::~ConfigLoader()
{
    if (mCurrentConfig)
    {
        delete mCurrentConfig;
    }
}

json ConfigLoader::open_and_load(const std::string & config_file)
{
    std::ifstream file(config_file);
    if (!file.is_open())
    {
        throw std::runtime_error("Configuration file could not be opened");
    }
    return json::parse(file);
}

void ConfigLoader::parse_config(const nlohmann::json & config_data)
{
    // Check version number
    mFileVersion = config_data["version"].get<uint64_t>();

    std::cout << "Config version: " << mFileVersion << std::endl;

    if (mFileVersion != CONFIG_FORMAT_VERSION)
    {
        std::string error_msg("An invalid configuration file was loaded: expected version ");
        error_msg.append(std::to_string(mFileVersion));
        error_msg.append(", got ");
        error_msg.append(std::to_string(CONFIG_FORMAT_VERSION));

        throw std::runtime_error(error_msg);
    }

    mCurrentConfig = new Config();

    std::cout << "Loading display configuration" << std::endl;
    parse_display_config(config_data["display"]);

    for (auto & [name, agency_data] : config_data["agencies"].items())
    {
        std::cout << "Loading agency " << name << " of type " << agency_data["type"] << std::endl;
        parse_agency_config(name, agency_data);
        std::cout << "Finished loading agency " << name << std::endl;
    }

    mCurrentConfig->lock_config();
}

void ConfigLoader::parse_display_config(const nlohmann::json & display_config)
{
    // Load runtime options
    rgb_matrix::RuntimeOptions runtimeOptions;
    runtimeOptions.do_gpio_init = true;
    runtimeOptions.drop_privileges = 1;
    runtimeOptions.daemon = 0;
    runtimeOptions.gpio_slowdown = display_config.value("gpio-slowdown", 4);

    mCurrentConfig->set_runtime_options(runtimeOptions);

    // Load matrix options
    rgb_matrix::RGBMatrix::Options matrixOptions;

    mHardwareMapping = display_config.value("hardware-mapping", "regular");
    matrixOptions.hardware_mapping = mHardwareMapping.c_str();

    matrixOptions.rows = display_config.value("rows", 32);
    matrixOptions.cols = display_config.value("columns", 64);
    matrixOptions.chain_length = display_config.value("chain", 1);
    matrixOptions.parallel = display_config.value("parallel-chains", 1);
    matrixOptions.pwm_bits = display_config.value("pwm-bits", 11);
    matrixOptions.pwm_lsb_nanoseconds = display_config.value("pwm-lsb-nanoseconds", 130);
    matrixOptions.pwm_dither_bits = display_config.value("pwm-dither-bits", 0);
    matrixOptions.brightness = display_config.value("brightness", 50);
    matrixOptions.scan_mode = display_config.value("scan-mode", 0);
    matrixOptions.row_address_type = display_config.value("row-address-type", 0);
    matrixOptions.multiplexing = display_config.value("multiplexing", 0);
    matrixOptions.disable_hardware_pulsing = display_config.value("disable-hardware-pulsing", false);
    matrixOptions.show_refresh_rate = display_config.value("show-refresh-rate", false);
    matrixOptions.inverse_colors = display_config.value("inverse-colors", false);

    mLedRgbSequence = display_config.value("led-rgb-sequence", "RGB");
    matrixOptions.led_rgb_sequence = mLedRgbSequence.c_str();

    mPixelMapperConfig = display_config.value("pixel-mapper-config", "");
    matrixOptions.pixel_mapper_config = mPixelMapperConfig.empty() ? nullptr : mPixelMapperConfig.c_str();

    mPanelType = display_config.value("panel-type", "");
    matrixOptions.panel_type = mPanelType.empty() ? nullptr : mPanelType.c_str();

    mCurrentConfig->set_matrix_options(matrixOptions);
}

void ConfigLoader::parse_agency_config(const std::string & name, const nlohmann::json & agency_config)
{
    const std::string agency_type = agency_config["type"].get<std::string>();
    Agency * my_agency = Factory<Agency>::instance()->construct(agency_type);

    if (my_agency == nullptr)
    {
        throw std::runtime_error("Agency factory returned nullptr for " + agency_type);
    }

    my_agency->load_from_config(name, agency_config);

    mCurrentConfig->add_agency(my_agency);
}
