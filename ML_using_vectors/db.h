
/*Title:
 *Created By: Troy Nunnally
 *Copyright 2013 Communication Assurance Performance (CAP)
 *Description:
 *
 *
 */


#ifndef db_H
#define db_H
/*
*
*
* gcc mysql.c -o version  `mysql_config --cflags --libs`
*
* MySQL Connector/C is a C client library for client/server communication. 
* It is a standalone replacement for the MySQL Client Library shipped with 
* the MySQL Server. At one time, in order to use a client library to connect 
* to a mmysql server, we would have to compile and install the MySQL server
* package. Now, we only have to install the client connector.
* 
*/
 
//Commonly used MySQL functions
//mysql_errno - Returns the error number for the most recently invoked MySQL function
//mysql_error - Returns the error message for the most recently invoked MySQL function


/*Declare a variable*/

//Define true and false
//bool does not exist as a keyword pre-C99

#include <mysql.h>      //mysql functions
#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <vector>       //vector
#include <string>       //String


#define TRUE 1
#define FALSE 0



class db
{
    
public:
    db();                   //Constructor
    int drop_db();          //Drop a database
    int create_db();        //Create a database
    int select_db();        //Select a database
    int select();           //Select a row
    int insert();           //Insert a row
    int update();           //Update a row
    int closeconnection();  //Close a connection
    

    
    
private:
    int debug;      //Set Debug flag to be zero
    
/*CONNECTION INFO*/
    static const std::string MY_USERNAME;      //db Username
    static const std::string MY_PASSWORD;      //db password
    static const std::string MY_HOSTNAME;      //db hostname
    static const std::string MY_SOCKET;       //db socket
    int MY_PORT;            //db port number
 
    
};

#endif //end db




