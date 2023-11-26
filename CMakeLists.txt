#[[
 | TransitClock
 | Copyright (C) 2023-2024 Taylor Berg
 |
 | This program is free software: you can redistribute it and/or modify
 | it under the terms of the GNU General Public License as published by
 | the Free Software Foundation, either version 3 of the License, or
 | (at your option) any later version.
 |
 | This program is distributed in the hope that it will be useful,
 | but WITHOUT ANY WARRANTY; without even the implied warranty of
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 | GNU General Public License for more details.
 |
 | You should have received a copy of the GNU General Public License
 | along with this program.  If not, see <http://www.gnu.org/licenses/>.
]]

cmake_minimum_required(VERSION 3.25)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(transit-clock LANGUAGES C CXX)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)

include(build/matrix.cmake)
include(src/build.cmake)

add_executable(transitclock ${src})
target_include_directories(transitclock PRIVATE src)
target_link_libraries(transitclock PRIVATE
        Boost::program_options
        Boost::headers
        rgbmatrix
        nlohmann_json::nlohmann_json
        cpr::cpr
)