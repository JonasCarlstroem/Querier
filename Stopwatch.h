#pragma once

#ifndef _STOP_WATCH_H
    #define _STOP_WATCH_H

#include <chrono>

namespace stopwatch {
    using namespace std::chrono;
    class Stopwatch {
    public:
        void Start() {
            m_start = system_clock::now();
            m_bRunning = true;
        }

        void Stop() {
            m_end = system_clock::now();
            m_bRunning = false;
        }

        void StopMs(double* result) {
            Stop();
            *result = elapsedMilliSeconds();
        }

        void StopS(double* result) {
            Stop();
            *result = elapsedSeconds();
        }

        double elapsedMilliSeconds() {
            time_point<system_clock> endTime;

            if (m_bRunning) {
                endTime = system_clock::now();
            }
            else {
                endTime = m_end;
            }

            return duration_cast<milliseconds>(endTime - m_start).count();
        }

        double elapsedSeconds() {
            return elapsedMilliSeconds() / 1000.0;
        }

    private:
        time_point<system_clock> m_start;
        time_point<system_clock> m_end;
        bool m_bRunning = false;
    };
}

#endif