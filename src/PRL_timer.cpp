#include <iostream>
#include <chrono>
#include <thread>

#include "PRL_timer.h"

int PRL_Timer :: timers_count = 0;

PRL_Timer :: PRL_Timer() : time_us(0), time_ms(0), shift_us(0)
{
    start = std::chrono::high_resolution_clock::now();
    timers_count++;
}

PRL_Timer :: ~PRL_Timer()
{
    timers_count--;
}

void PRL_Timer :: update()
{
    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
    time_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() - shift_us;
    time_ms = time_us * 0.001;
}

void PRL_Timer :: reset()
{
    start = std::chrono::high_resolution_clock::now();
    time_us = time_ms = 0;
    shift_us = 0;
}

void PRL_Timer :: shift(long long us)
{
    shift_us = us;
}

long long PRL_Timer :: ms() const
{
    return time_ms;
}

long long PRL_Timer :: us() const
{
    return time_us;
}

long long PRL_Timer :: updateAndGetTime_ms()
{
    update();
    return time_ms;
}

long long PRL_Timer :: updateAndGetTime_us()
{
    update();
    return time_us;
}

int PRL_Timer :: getCount()
{
    return timers_count;
}

using namespace std::this_thread; // sleep_for, sleep_until

void PRL_Delay(long long us)
{
    sleep_for(std::chrono::microseconds(us));
}

