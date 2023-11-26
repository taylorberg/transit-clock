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

#include "DataManagerThread.h"

#include <Model/Agency.h>
#include <thread>

DataManagerThread::DataManagerThread(const std::vector<Agency *> agency_list) :
        mAgencyList(agency_list)
{

}

DataManagerThread::~DataManagerThread()
{
    request_to_exit();
    mThread.join();
}

void DataManagerThread::start()
{
    mThread = std::thread(&DataManagerThread::thread_main, this);
}

void DataManagerThread::request_to_exit()
{
    mExitRequested = true;
}

void DataManagerThread::thread_main()
{
    while (!mExitRequested)
    {
        std::chrono::system_clock::time_point next_update = std::chrono::system_clock::time_point::max();

        for (auto & agency : mAgencyList)
        {
            // Check to see if we need to do the update, and if so, do it!
            if (agency->due_for_update())
            {
                agency->update_data();
            }

            // Figure out which agency needs to be updated next
            if (agency->last_update() + agency->update_interval() < next_update)
            {
                next_update = agency->last_update() + agency->update_interval();
            }
        }

        std::this_thread::sleep_until(next_update);
    }
}

void DataManagerThread::new_predictions(const std::vector<Prediction> & prediction_list)
{
    std::lock_guard<std::mutex> guard(mDataLock);
    mPredictionList = prediction_list;
}

std::vector<Prediction> DataManagerThread::prediction_list()
{
    std::lock_guard<std::mutex> guard(mDataLock);
    return mPredictionList;
}
