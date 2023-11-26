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
#include <led-matrix.h>

#include "Application.h"

#include "Portable.h"
#include "Config/ConfigLoader.h"

#include "boost/program_options.hpp"
namespace po = boost::program_options;

Application::Application(int argc, char ** argv)
{
    rgb_matrix::RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;

    matrix_options.rows = 32;
    matrix_options.cols = 64;
    matrix_options.chain_length = 2;
    matrix_options.brightness = 50;

    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt))
    {
        display_usage(argv[0]);
    }

    po::options_description desc("Options");
    desc.add_options()
                ("config", po::value<std::string>(), "Config file location");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("config")) {
        mConfigFileLocation = vm["config"].as<std::string>();
    }

    try
    {
        ConfigLoader config_loader(mConfigFileLocation);

        std::cout << "Application ready!" << std::endl;
    }
    catch(std::exception & ex)
    {
        std::cout << "An error occurred during initialization: " << ex.what() << std::endl;
    }
}

void Application::display_usage(const char * program_name) const
{
    std::cerr << "usage: " << program_name << " <options> -C <config-file-path>" << std::endl;
    rgb_matrix::PrintMatrixFlags(stderr);
}

void Application::run()
{

}

const std::string & Application::config_file_path() const
{
    return mConfigFileLocation;
}

Application::~Application() = default;

int main(int argc, char * argv[])
{
    Application application(argc, argv);
    return 0;
}