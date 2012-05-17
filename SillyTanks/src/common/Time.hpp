/**
 * time.hpp
 * 
 */

#ifndef GRAPHICSLAB_TIME_HPP
#define GRAPHICSLAB_TIME_HPP


// Global definitions include
#include "Definitions.hpp"

// Includes
#ifdef WIN32

#include <windows.h>
#define TIMETYPE FILETIME
#define GETTIME( a ) GetSystemTimeAsFileTime( &a )

#else

#include <sys/time.h>
#define TIMETYPE timeval
#define GETTIME( a ) gettimeofday( &a, NULL )

#endif


namespace game_space {


/** Class to measure passed time */
class Time
{
public:
    /** Constructor */
    Time();
    
	/** Restart the time */
    void start();
    
	/** 
	 * Get milliseconds from the last restart of the timer.
	 * 
	 * @return		Milliseconds from last restart
	 */
    float getMilliseconds() const;
    
private:
	/** Internal time data structure */
    TIMETYPE _time;
    
}; // class Time


}


#endif // GRAPHICSLAB_TIME_HPP
