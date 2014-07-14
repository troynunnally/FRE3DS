/* Title: Training
 * Created By: Troy Nunnally and Nashlie Sephus
 * Copyright 2013 Partpic
 * Description: This file trains and saves descriptors for SURF features in a yml file when the image, imageID, and Hessian Threshold are passed in.
 * Last Updated: 9/26/2013
 * Version: Opencv 2.4.6
 *
 */

/*INCLUDES*/  
    #include "Timer.h"

/*---
 * Constructor
-----------------------------------*/
Timer::Timer (Db* database)
	:	_db(database)					// Init Db
{
	m_startTime = 0;
	m_stopTime = 0;
}


/*---
 * func: start
 * desc: Start the timer
-----------------------------------*/
void Timer::start()
{
    /*VARIABLES*/
        //startTime = (double)clock();
        //printf("%f \n",startTime)
	ULONGLONG current_time = GetTickCount64();

	m_startTime = double(GetTickCount64());
		
	wchar_t buf[2048];
	swprintf(buf,L"Start time: %f...\n", m_startTime);
	OutputDebugStringW(buf);
}



/*---
 * func: unsigned long elapsedTime
 * desc: Stop the Timer
-----------------------------------*/
void Timer::stop()
{
    /*VARIABLES*/
        m_stopTime    = double(GetTickCount64());
		wchar_t buf[2048];
		swprintf(buf,L"Stop time: %f...\n", m_stopTime);
		OutputDebugStringW(buf);

        diffTime = elapsedTime();
}


/*---
 * func: start
 * desc: Start the timer
-----------------------------------*/
void Timer::uploadTime()
{
    /*VARIABLES*/
        //startTime = (double)clock();
        //printf("%f \n",startTime)
	_db->upload_time(1, diffTime);		//Perform a Get Request to NAVSEC (sid, iid)
		
}

/*---
 * func: unsigned long elapsedTime
 * desc: Get Elapse time
-----------------------------------*/
double Timer::elapsedTime()
{
    /*RETURN*/
		wchar_t buf[2048];
		swprintf(buf,L"Elasp Time: %f \n",(m_stopTime - m_startTime)/1000);
		OutputDebugStringW(buf);

        
        return (m_stopTime - m_startTime);
}



/*---
 * func: isTimeout(unsigned long seconds)
 * desc: is the timer pass a certain number of seconds?
-----------------------------------*/

bool Timer::isTimeout(unsigned long seconds)
{
     /*RETURN*/
        return seconds >= elapsedTime();
}

