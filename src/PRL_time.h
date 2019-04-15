#ifndef PRL_CLASS_TIME_H_INCLUDED
#define PRL_CLASS_TIME_H_INCLUDED

#include <iostream>


class PRL_Stopwatch
{
public:
	//! Constructor.
    PRL_Stopwatch();
    //! Destructor.
    ~PRL_Stopwatch();

	//! Get the elapsed time in microseconds (us).
	long long us() const;
	//! Get the elapsed time in milliseconds (ms).
	int ms() const;

	//! Start the stopwatch.
	/*!
	If the stopwatch is already started, nothing will happen. However if it was stopped
	it simply continues without reset.
	*/
	void start();
	//! Stop the stopwatch.
	/*!
	When stopped, the last value is held. If the stopwatch is already stopped or just reseted, nothing will happen.
	*/
	void stop();
	//! Reset the stopwatch.
	/*!
	This action causes the time measured to be set to 0 again, and stops the stopwatch.
	*/
	void reset();

private:
    long long startingTime;//!< Time at which the stopwatch have been set (in us).
    long long stopDuration;//!< Time of the cumulated stops (in us).
    long long timeAtStop;//!< Time at the previous stop (in us).
    bool reseted;//!< Tell if the stopwatch has just been reseted.
    bool stopped;//!< Tell if the stopwatch is stopped.
};

//! Classic delay function.
/*!
Use this function to pause the thread in which it is called.
NB: The values of the desired time must be positive!
@param [in] us Specify how long you want the thread to be paused.
*/
void PRL_Delay(long long us);
//! Get the number of microseconds since the launch of the program.
long long PRL_GetTicks();
//! Get a time stamp as a string.
std::string PRL_TimeStamp();


/*
*!
This class stores an elapsed time since it has been created or reseted.
You need to call the method update() in order to make the internal time match the real elapsed time.
You can introduce a shift in the timer if needed.
*
class PRL_Timer
{
private:
    std::chrono::high_resolution_clock::time_point start; //!< Reference time point, set to be the zero of the timer.
    long long time_us;//!< Time stored in microseconds (us).
    long long time_ms;//!< Time stored in milliseconds (ms).
    long long shift_us;//!< Time shift (0 by default).

    static int timers_count;//!< Keep track of how many timers are present in the program.

public:
    //! Constructor.
    PRL_Timer();
    //! Destructor.
    ~PRL_Timer();

    //! Update the timer and get the latest time value.
    void update();
    //! Reset the timer.
    *!
    This method resets the reference time point and the time shift, just as if the class had just been created.
    *
    void reset();
    //! Set a time shift (us).
    *!
    The time shift is implemented via this formula:
    t_timer = t_true - shift.

    Using shifting could be useful if your program has been paused for instance.
    NB: The shift can be positive or negative!
    @param [in] us The desired time shift in microseconds.
    *
    void shift(long long us);
    //! Get the elapsed time in microseconds.
    long long us() const;
    //! Get the elapsed time in milliseconds.
    long long ms() const;
    //! Update the timer and then get the elapsed time in microseconds.
    long long updateAndGetTime_us();
    //! Update the timer and then get the elapsed time in milliseconds.
    long long updateAndGetTime_ms();
    //! Get how many timers are currently present.
    static int getCount();
};*/

#endif // PRL_CLASS_TIME_H_INCLUDED
