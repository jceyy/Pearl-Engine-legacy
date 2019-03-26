#include <iostream>
#include <chrono>
#include <thread>

#include "PRL_time.h"

std::chrono::high_resolution_clock::time_point tickStart = std::chrono::high_resolution_clock::now();

long long PRL_GetTicks()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tickStart).count();
}


PRL_Stopwatch :: PRL_Stopwatch() : startingTime(0), stopDuration(0), timeAtStop(0), reseted(true),
stopped(true)
{
	;
}

PRL_Stopwatch :: ~PRL_Stopwatch()
{
	;
}

long long PRL_Stopwatch :: us() const
{
	if (stopped)
		return timeAtStop - startingTime - stopDuration;
	else
		return PRL_GetTicks() - startingTime - stopDuration;
}

int PRL_Stopwatch :: ms() const
{
    return (int)(us() / 1000);
}

void PRL_Stopwatch :: reset()
{
    reseted = true;
    stopped = true;
}

void PRL_Stopwatch :: start()
{
	if (reseted)
	{
		startingTime = PRL_GetTicks();
		stopDuration = 0;
		reseted = false;
	}
	else if (stopped)
	{
		stopDuration += PRL_GetTicks() - timeAtStop;
	}
}

void PRL_Stopwatch :: stop()
{
	if (!reseted && !stopped)
	{
		timeAtStop = PRL_GetTicks();
	}
}

using namespace std::this_thread;

void PRL_Delay(long long us)
{
    sleep_for(std::chrono::microseconds(us));
}

time_t rawtime;
struct tm * timeinfo;
char buffer[80];

std::string PRL_TimeStamp()
{
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    std::string str(buffer);

    return str;
}

/*int PRL_Timer :: timers_count = 0;

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
}*/

