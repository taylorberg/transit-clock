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

#ifndef TRANSIT_CLOCK_APPLICATION_H
#define TRANSIT_CLOCK_APPLICATION_H

#include <Portable.h>

#include <string>

class Config;

class Application
{
public:
    Application(int argc, char ** argv);
    ~Application();

    void display_usage(const char * program_name) const;

    const std::string & config_file_path() const;

    void run();

private:
    std::string mConfigFileLocation = "/etc/transitclock.json";
};

#ifndef NO_MAIN
int main(int argc, char * argv[]);
#endif


#endif //TRANSIT_CLOCK_APPLICATION_H
