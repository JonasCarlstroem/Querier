#pragma once

#ifndef _SCRIPT_PAD_STOP_WATCH_H
    #define _SCRIPT_PAD_STOP_WATCH_H

#include <chrono>

namespace scriptpad {
    using namespace std::chrono;
    class Stopwatch {
    public:
        void Start();
        void Stop();
        void StopMs(double* result);
        void StopS(double* result);
        double elapsedMilliSeconds();
        double elapsedSeconds();

    private:
        time_point<system_clock> m_start;
        time_point<system_clock> m_end;
        bool m_bRunning = false;
    };

}   //namespace stopwatch

#endif  //_SCRIPT_PAD_STOP_WATCH_H