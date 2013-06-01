//g++ -o csvtest cvstest.cpp navsec.cpp db.cpp Parser.cpp -I/usr/local/mysql/include -L/usr/local/mysql/lib -lmysqlclient -lm -lz

#include <stdio.h>
#include <vector>
#include "navsec.h"
#include "Parser.h"
#include <time.h>
#include "db.h"
#include <mysql.h>
#include <string>

int print_mysql_info();     //Print mysql info
int start_connection();     //Start a connection
int select_db();            //Select navsec db
std::vector<float> select_expert_users();   //Select active users and return an vector of active users
std::vector< std::vector<float> > select_expert_sessions();
float min_cost_session(std::vector<float> asession);
float max_cost_session(std::vector<float> session);

void printSessions(std::vector< std::vector<float> > pos2);
MYSQL *conn;


int main(){
    /*START TIME*/
        const clock_t begin_time = clock();

    /*VARIABLES*/
        navSec ML;                              //ML Object
        Parser expertVectorParser;              //ML Object
        db dbConnector;
        char storageFilename[] = "test.csv";    //filename
        std::vector<double> data;               //Parser object           
        float euclideanDistance;
        std::vector<float>  euclideanDistanceSessions;
        std::vector<float>  cosSimSessions;
        float cosineSimilarity;
        int i = 0, j=0;                                     //counter
        std::vector< std::vector<float> > sessions;          //sessions
    
    
    /*START A DB CONNECTION*/
        start_connection();
        print_mysql_info();
    
    /*SELECT NAVSEC DATABASE*/
        if(select_db()){
            printf( "navsec database selected...\n");
        }
    
    /*PRINT ALL EXPERT SESSIONS*/
        printf("--------------------Expert Sessions-----------------.\n");
        sessions = select_expert_sessions();            //SELECT ALL EXPERT USER VECTORS
       printf("Number of Expert Sessions: %lu \n", sessions.size());
    
    /*COMPARE USERS*/
        float active_user[] = {1,1,1,1,0,1,1,0};
        std::vector<float> pos1 (active_user, active_user + sizeof(active_user) / sizeof(float) ); // Active Vector 2
    
        float expert_user[] = {2,2,2,2,0,2,1,1};
        std::vector<float> pos2 (expert_user, expert_user + sizeof(expert_user) / sizeof(float) ); // Expert Vector 2
    
       // data = expertVectorParser.ReadCoordinates(&storageFilename[0]);     //You allocated new mem
    
    /*PRINT SESSIONS*/
        printSessions(sessions);

    /*CALCULATE  */    
        // My squared  euclidean distance similarity
        //euclideanDistance = ML.computeEuclidDistance(pos1, pos2);
    
        euclideanDistanceSessions = ML.computeEuclidDistanceAllVectors(pos1, sessions);
        cosSimSessions = ML.computeCosineSimAllVectors(pos1, sessions);
    
        // My squared  cosine similarity
        //cosineSimilarity = ML.computeCosineSimilarity(pos1, pos2);
    
        printf("\n");
        printf("--------------------Active User-----------------.\n");
   
    /*PRINT ACTIVE USER*/
        printf("Active User = [");
        for(unsigned int i = 2; i < pos1.size(); ++i)
        {
            printf(" %f ", pos1[i]);
        }
        printf("] \n\n");
    
        printf("--------------------Euclidean Distance-----------------.\n");
    
    /*PRINT DISTANCE VECTOR */
        printf("Euclidean Distance Vector= [");
        for(unsigned int i = 0; i < euclideanDistanceSessions.size(); ++i)
        {
            printf(" %f ", euclideanDistanceSessions[i]);
        }
        printf("] \n");
        //printf("Euclidean Distance: %f.\n", euclideanDistance);
        //printf("Cosine Similarity: %f.\n", cosineSimilarity);
    
    /*PRINT MINIMUM EUCLIDEAN DISTANCE*/
        float cost = min_cost_session(euclideanDistanceSessions);
        printf("Minimum Euclidean Distance= %f \n",cost);
    
    /*PRINT MOST SIMILAR EUCLIDEAN DISTANCE*/
        printf("Recommending session ids:[" );
        for(unsigned int i = 0; i < euclideanDistanceSessions.size(); ++i)
        {
            if(cost - euclideanDistanceSessions[i]==0)
            {
                printf(" %f ", sessions[i][0]);
            }
        }
        printf("]\n" );
    
        printf("--------------------Cosine Similarity -----------------.\n");
    /*PRINT MINIMUM EUCLIDEAN DISTANCE*/
        cost = min_cost_session(euclideanDistanceSessions);
        printf("Minimum Euclidean Distance= %f \n",cost);
    
    
    /*PRINT COSINE SIMILARITY*/
        printf("Cosine Similarity Vector= [");
        for(unsigned int i = 0; i < cosSimSessions.size(); ++i)
        {
            printf(" %f ", cosSimSessions[i]);
        }
        printf("] \n");
    
    /*PRINT COSINE SIMILAR MOST SIMILAR*/
        cost = max_cost_session(cosSimSessions);
        printf(" Best Cosine Simarity Cost= %f \n",cost);
    
    /*PRINT MOST SIMILAR EUCLIDEAN DISTANCE*/
        printf("Recommending sessions: [" );
        for(unsigned int i = 0; i < cosSimSessions.size(); ++i)
        {
            if(cost - cosSimSessions[i]==0)
            {
                printf(" %f ", sessions[i][0]);
            }
        }
        printf("] \n" );
    
    /*CLOSE MYSQL CONNECTION*/
        mysql_close(conn);
    
    /*END TIME*/
        float end_time = ( clock () - begin_time );
        printf("Computational Time: %f. us\n", end_time);
    
        
    
        for (i = 0 ; i < ML.num_coord - 1; i=i+2)
        {
            //Print coordinate
            //printf("x[%d] = %f, y[%d] = %f\n",j, data[i], j, data[i+1]);
            //j++;
        }
        
        
}

// --------------------------------------------------------------------
/* Select All Sessions*/
/*------------------------------------*/
    float min_cost_session(std::vector<float> session){

 
    float val = *(std::min_element(session.begin(),session.end()));//get max
    //printf("Min: %f",*val);
    
    return val;
        
    }


// --------------------------------------------------------------------
/* Select All Sessions*/
/*------------------------------------*/
float max_cost_session(std::vector<float> session){
    
    
    float val = *(std::max_element(session.begin(),session.end()));//get max
    //printf("Min: %f",*val);
    
    return val;
    
}

// --------------------------------------------------------------------
/* Select All Sessions*/
/*------------------------------------*/
    std::vector< std::vector<float> > select_expert_sessions(){
    /*VARIABLES*/
        unsigned long *lengths;
        
    /*QUERY*/    
        if (mysql_query(conn, "SELECT * FROM session_vectors"))
        {
            printf("Query Error %u: %s\n", mysql_errno(conn), mysql_error(conn)); //print error
            exit(1);            //detects an error and abort program
        }
        
    /*RESULT*/ 
        MYSQL_RES *result = mysql_store_result(conn);
        
        if (result == NULL)
        {
            printf("Query Error %u: %s\n", mysql_errno(conn), mysql_error(conn)); //print error
            exit(1);            //detects an error and abort program
        }
      
      
        int num_rows = mysql_num_rows(result);          //The number of values in the row
        int num_fields = mysql_num_fields(result);
        
        std::vector< std::vector<float> >sessions(num_rows);
        std::vector<float> arow(num_fields);              //A row vector with size as number of fields.
        int j = 0;                                      //counter
        
    /*PRINT*/
        MYSQL_ROW row;                              //an array of string
        
        
        while ((row = mysql_fetch_row(result)))     //Retrieves the next row of a result set.
        {
            
            lengths = mysql_fetch_lengths(result);
            
            
            for(int i = 0; i < num_fields; i++)
            {
                //printf("Column %u is %lu bytes in length with value %s.\n", i, lengths[i], row[i] ? row[i] : "NULL");
                
                arow[i] = atof(row[i]); //convert from char* to int
                
            }
            
            //printf("\n");
            sessions[j]=arow;
            j++;                    //add one to session counter

        }
    
    /*PRINT FIELDS*/
       /* MYSQL_FIELD *fields;
        fields = mysql_fetch_fields(result);
        for(int i = 0; i < num_fields; i++)
        {
            printf("Field %u is %s\n", i, fields[i].name);
        }*/
        
     /*CLEANUP*/  
        mysql_free_result(result);
    /*RETURN*/
        return sessions;
        
    }



/*PRINT MYSQL INFO*/
/*---------------------------*/
    int print_mysql_info(){
        
        printf("\n\n--------------MySQL Info--------------------\n");
        
        printf("MySQL client version: %s\n", mysql_get_client_info());              //Client verison and info
        printf("MySQL server info: %s\n", mysql_get_server_info(conn));             //
        printf("MySQL server version: %lu\n", mysql_get_server_version(conn));      //Server Version
        printf("MySQL host info: %s\n", mysql_get_host_info(conn));                 // type of connection in use
        printf("MySQL protocol version: %u\n", mysql_get_proto_info(conn));         // protocol version
        printf("-----------------------------------------\n\n");
        
        return TRUE;
        
    }



/*START A CONNECTION*/
/*---------------------------*/
    int start_connection()
    {
        
    /*VARIABLES*/
        static const std::string MY_USERNAME = "root";
        static const std::string MY_PASSWORD = "root";
        static const std::string MY_SOCKET   = "/Applications/MAMP/tmp/mysql/mysql.sock";
        static const std::string MY_HOSTNAME = "localhost";
        int MY_PORT = 8889;                //db port number
        
        
    /*INIT*/
        conn = mysql_init(NULL);       // Allocates or initializes a MYSQL object
        if (conn == NULL)
        {
            printf("Connection Object Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
            exit(1);
        }
        
    /*CONNECT*/
        //attempts to establish a connection to a MySQL database using an UNIX Socket
        if (mysql_real_connect(conn, MY_HOSTNAME.c_str(), MY_USERNAME.c_str(), MY_PASSWORD.c_str(), NULL, MY_PORT, MY_SOCKET.c_str(), 0) == NULL)
        {
            printf("Connection Error %u: %s\n", mysql_errno(conn), mysql_error(conn)); //print error
            exit(1);    //detects an error and abort program
        }
        
    /*RETURN*/
        return TRUE;
        
    }

/*SELECT A DATABASE*/
/*---------------------------*/
    int select_db(){
    
    /*SELECT*/
        if(mysql_select_db(conn,"navsec")){
            printf( "Database connection error %u: %s\n", mysql_errno(conn), mysql_error(conn));
            exit(1);            ////detects an error and abort program
            
        }
    
    /*RETURN*/
        return TRUE;
    }

/*PRINT SESSIONS */
/*---------------------------*/
    void printSessions(std::vector< std::vector<float> > pos2)
    {
    /*COMPUTE*/
        for(unsigned int j = 0; j < pos2.size(); ++j)
        {
    
        /*PRINT SESSION VECTOR*/
            printf("Session id %f for Expert User %f  = [",pos2[j][0], pos2[j][1]);
            for(unsigned int i = 2; i < pos2[j].size(); ++i)
            {
                printf(" %f ", pos2[j][i]);
            }
            printf("] \n");
        }

    }

/*SELECT EXPERTS*/
/*---------------------------*/
    std::vector<float> select_expert_users(){
        
        std::vector<float> expert_users;
     /*RETURN*/                      
        return expert_users;

    }





