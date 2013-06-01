
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <vector>
#include <cstdlib>
#include "navsec.h"
#include <math.h>

/// <summary>loads a CSV record from the stream is</summary>
/// <remarks>
/// * leading and trailing white space is removed outside of
//    quoted sections when trimWhiteSpace is true
/// * line breaks are preserved in quoted sections
/// * quote literals consist of two adjacent quote characters
/// * quote literals must be in quoted sections
/// </remarks>
/// <param name=is>input stream for CSV records</param>
/// <param name=trimWhiteSpace>trims white space on unquoted fields</param>
/// <param name=fieldDelim>field delimiter. defaults to ',' for CSV</param>
/// <param name=recordDelim>record delimiter. defaults to '\n' for CSV</param>
/// <param name=quote>delimiter for quoted fields. defaults to '"'</param>
/// <returns>a list of fields in the record</returns>

//gcc mysql.c -o version  `mysql_config --cflags --libs`

navSec::navSec()
{
    num_coord = 0;          //Initialize the number  coordinates
}

void navSec::printrecords(char* filename)
{
    //Return file descriptor
    FILE *in=fopen(filename,"r");
    
    printf("*****Printing records in file %s ***** \n", filename);
    
    if(in==NULL)
    {
        printf("./parser file.csv \n");
        perror("File open error");
        //exit(EXIT_FAILURE);
    }
    else{
    }
    
    fclose(in);

}

void navSec::printProperties()
{
    printf("*****Printing properties***** \n");
    //printf("Field Delimiter: %s \n", navSec::fieldDelim);
    //printf("Record Delimiter: %s \n", recordDelim);
}

std::vector<double> navSec::ReadCoordinates(char* filename){

    int x;              //counter
    int size=0;         //
    char* data;
    int i = 0;          //counter
    
    FILE *fp=fopen(filename, "r");
    
    
    if (fp == NULL){
	 perror ("Error opening file");
    }
    
    while  (( x = fgetc(fp)) != EOF ) {  //Returns the character currently pointed by the internal file position indicator
		size++;     //Number of characters in the csv file
	}
    
	rewind(fp);                         //Sets the position indicator to the beginning of the file
	printf("size is %d.\n", size);      //print
        
    data = new char[23];                //Each line is 23 bytes (characters) long
	size = (size/23) * 2;               //number of x, y coordinates
	
    coordinates.resize (size);           //allocate memory for an array of coordinates, need to be freed somewhere
           
	
	num_coord = size;                   //num_coord is public
	
    //fgets (data, size, fp);
	//printf("data is %c.\n", *data);
        
	for(x=0; x<size; x++){
		fgets (data, size, fp);
		coordinates[i] = atof(&data[0]);          //convert string to double
		coordinates[i+1] = atof(&data[11]);       //convert string to double
		i = i+2;
	}
    
   
    delete[] data;
    
    fclose (fp);
    
    return coordinates;
}


bool navSec::cleanup()
{

}


bool navSec::writeCoordinates(float coordinates[], int size, char* filename)
{
    
    //Return file descriptor
    FILE *fp=fopen(filename,"a"); //open for appending (file need not exist)  
    
   
   //Write coordinate array to file
    for (int i=0; i<size; i++)
        fprintf(fp, "%f,", coordinates[i]);
    
    //Put a new line at the end of the file 
    fprintf(fp, "\n");
    
    if(fp != NULL)
    {
        fclose(fp);
        return true;   
    }
    else
    {
        perror("File open error");
        return false;
        //exit(EXIT_FAILURE);
    }
    
}

bool navSec::writeRecord(int userID, char* bioID, char* filename)
{
    //Return file descriptor
    FILE *fp=fopen(filename,"a"); //open for appending (file need not exist)  
    bool success;
    
    if(fp != NULL)
    {
        fprintf(fp, "%u, %s\n", userID, bioID);
        fclose(fp);
        success =true;
        
    }
    else
    {
        perror("File open error");
        success = false;
        //exit(EXIT_FAILURE);
    }
    return success;
}

// My squared  euclidean distance similarity
    float navSec::computeEuclidDistance(std::vector<float> pos1, std::vector<float> pos2)
    {
        float sum = 0;
        
        for(unsigned int i = 0; i < pos1.size(); ++i)
        {
            sum += pow( (pos1[i] - pos2[i]), 2.0 );
        }
        
        return sqrt(sum);
    }


/*COMPUTE EUCLIDEAN DISTANCE OF ALL SESSION VECTORS*/
/*---------------------------*/
    std::vector<float> navSec::computeEuclidDistanceAllVectors(std::vector<float> pos1, std::vector< std::vector<float> > pos2)
    {
    /*VARIABLES*/   
        std::vector<float> distance_vec(pos2.size());        //Vector of scores (distance
        float sum;
        
    /*COMPUTE*/
        for(unsigned int j = 0; j < pos2.size(); ++j)
        {
            sum = 0;
            for(unsigned int i = 2; i < pos2[j].size(); ++i)
            {
                sum += pow( (pos1[i] - pos2[j][i]), 2.0 ); 
            }

            distance_vec[j] = sqrt(sum);
        }

    /*RETURN*/
        return distance_vec;
    }


std::vector<float> navSec::computeCosineSimAllVectors(std::vector<float> pos1, std::vector< std::vector<float> > pos2)
{
    /*VARIABLES*/
        std::vector<float> distance_vec(pos2.size());        //Vector of scores (distance
        float sum;
        float sum0 = 0, sum1 = 0, sum2 = 0;
    
    /*COMPUTE*/
        for(unsigned int j = 0; j < pos2.size(); ++j)
        {
            for(unsigned int i = 2; i < pos2[j].size(); ++i)
            {
                sum0 += pos1[i] * pos2[j][i];  //DOT Product
                sum1 += (pos1[i]*pos1[i]);  //Magnitude Squared of Vector 1
                sum2 += (pos2[j][i]*pos2[j][i]);  //Magnitude Squared of Vector 2
            }
            
            distance_vec[j] = sum0 / ( sqrt(sum1) * sqrt(sum2) ); //Dot Product over the magtitudes
            
        }
    /*RETURN*/
        return distance_vec;

}



/*COMPUTE COSINE SIMILARITY*/
/*---------------------------*/
float navSec::computeCosineSimilarity(std::vector<float> pos1, std::vector<float> pos2)
{
    float sum0 = 0, sum1 = 0, sum2 = 0;
    
    for(unsigned int i = 0; i < pos1.size(); ++i)
    {
        sum0 += pos1[i] * pos2[i];  //DOT Product
        sum1 += (pos1[i]*pos1[i]);  //Magnitude Squared of Vector 1
        sum2 += (pos2[i]*pos2[i]);  //Magnitude Squared of Vector 2
    }
    
    float similarity = sum0 / ( sqrt(sum1) * sqrt(sum2) ); //Dot Product over the magtitudes
    
    return similarity;
}
