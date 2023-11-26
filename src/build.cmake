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

set(src_dir src)

set(src_cpp
        Application.cpp
        Config/Config.cpp
        Config/ConfigLoader.cpp
        Model/Agency.cpp
        Model/Prediction.cpp
        AgencyTypes/BusTime3/BusTime3Agency.cpp
        Backend/DataManagerThread.cpp
)

set(src_h
        Application.h
        Portable.h
        Utilities/Factory.h
        Config/Config.h
        Config/ConfigLoader.h
        Model/Agency.h
        Model/Stop.h
        Model/Route.h
        Model/Prediction.h
        AgencyTypes/BusTime3/BusTime3Agency.h
        Backend/DataManagerThread.h
)

set (src ${src_cpp} ${src_h})
list (TRANSFORM src PREPEND "${src_dir}/")