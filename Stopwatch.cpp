#ifndef _SCRIPT_PAD_STOP_WATCH_CPP
#define _SCRIPT_PAD_STOP_WATCH_CPP

#include "Stopwatch.h"

namespace scriptpad {
    void Stopwatch::Start() {
        m_start = system_clock::now();
        m_bRunning = true;
    }

    void Stopwatch::Stop() {
        m_end = system_clock::now();
        m_bRunning = false;
    }

    void Stopwatch::StopMs(double* result) {
        Stop();
        *result = elapsedMilliSeconds();
    }

    void Stopwatch::StopS(double* result) {
        Stop();
        *result = elapsedSeconds();
    }

    double Stopwatch::elapsedMilliSeconds() {
        time_point<system_clock> endTime;

        if (m_bRunning) {
            endTime = system_clock::now();
        }
        else {
            endTime = m_end;
        }

        return duration_cast<milliseconds>(endTime - m_start).count();
    }

    double Stopwatch::elapsedSeconds() {
        return elapsedMilliSeconds() / 1000.0;
    }
}   //namespace stopwatch

#endif  //_SCRIPT_PAD_STOP_WATCH_CPP