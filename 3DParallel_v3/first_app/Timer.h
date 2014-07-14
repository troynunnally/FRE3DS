/*Title: Timer Class
 *Created By: Troy Nunnally
 *Copyright 2013 Communication Assurance Performance (CAP)
 *Description: Timer function allows use to measure the time spent on actions.
 *
 *
 */


#ifndef timer_H
#define timer_H

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "Db.h"				//Declare our Class to connect to Database

class Timer
{
    
public:
    Timer(Db* database);
    void start();                //Start
    void stop();                 //Start
	void uploadTime();                 //Start
    double elapsedTime(); //Get the elapsetime
    bool isTimeout(unsigned long seconds);
    double diffTime;    //stopTime

private:
	double   m_startTime;      //startTime
    double   m_stopTime;       //stopTime
	Db* _db;					// Connects to the web interface
    
};

#endif //end timer
