/*Title:
 *Created By: Troy Nunnally
 *Copyright 2013 Communication Assurance Performance (CAP)
 *Description:
 *
 *
 */
#include "Db.h"

Db::Db(){
    
    Db::debug = TRUE;               //Enable debug
}

/*DROP A DATABASE*/
/*---------------------------*/
int Db::getrequest(int sid,char * iid)
	{
		 CURL *curl;
		 CURLcode res;
 
		 char html_link[256];
		sprintf(html_link, "http://localhost/navsec/index.php/navsec/add_interaction?sid=%u&iid=%s", sid, iid);

		 curl = curl_easy_init();
		  if(curl) {
			curl_easy_setopt(curl, CURLOPT_URL, html_link);
			/* example.com is redirected, so we tell libcurl to follow redirection */ 
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		 
			/* Perform the request, res will get the return code */ 
			res = curl_easy_perform(curl);
			/* Check for errors */ 
			if(res != CURLE_OK)
			  fprintf(stderr, "curl_easy_perform() failed: %s\n",
					  curl_easy_strerror(res));
		 
			/* always cleanup */ 
			curl_easy_cleanup(curl);
		  }
		  return TRUE;
	}

/*DROP A DATABASE*/
/*---------------------------*/
    int Db::drop_db()
        {
        /*RETURN*/
            return TRUE;

            
        }



/*CREATE A DATABASE*/
/*---------------------------*/
    int Db::create_db()
        {
            
        /*RETURN*/
            return TRUE;
      
            
        }



/*SELECT A DATABASE*/
/*---------------------------*/
    int Db::select_db()
        {
        
        /*RETURN*/
            return TRUE;

        }


/*SELECT A ROW*/
/*---------------------------*/
    int Db::select()
        {
        /*RETURN*/
            return TRUE;
     
        }

/*INSERT A ROW*/
/*---------------------------*/
    int Db::insert()
        {
        
        /*RETURN*/
            return TRUE;
        }


/*UPDATE A ROW*/
/*---------------------------*/
    int Db::update()
        {
        /*RETURN*/
            return TRUE;
            
        }

/*DELETE A ROW*/
/*---------------------------*/
    int Db::closeconnection()
        {
        /*RETURN*/
            return TRUE;
        }


	void Db::shutdown()
	{
	}





