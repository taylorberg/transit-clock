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

#ifndef TRANSIT_CLOCK_DATAMANAGERTHREAD_H
#define TRANSIT_CLOCK_DATAMANAGERTHREAD_H

#include <vector>
#include <mutex>
#include <thread>
#include "Model/Prediction.h"

class Agency;

class DataManagerThread
{
public:
    explicit DataManagerThread(const std::vector<Agency *> agency_list);
    ~DataManagerThread();

    void start();

    void request_to_exit();

    std::vector<Prediction> prediction_list();

private:
    void thread_main();

    void new_predictions(const std::vector<Prediction> & prediction_list);

    bool mExitRequested = false;
    std::mutex mDataLock;
    std::vector<Agency *> mAgencyList;
    std::thread mThread;
    std::vector<Prediction> mPredictionList;
};


#endif //TRANSIT_CLOCK_DATAMANAGERTHREAD_H
