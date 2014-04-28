///////////////////////////////////////////////////////////////////////////////
// ModelGL.cpp
// =========
// Renders the OpenGL scene
// 
// AUTHOR: Troy Nunnally (troynunnally@gmail.com)
// CREATED: 2011-12-21
// 
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>
#include <math.h>
#pragma comment( lib, "opengl32" )
#pragma comment( lib, "glu32" )

#endif


#include <iostream>
#include <gl\gl.h>
#include <gl\glu.h>
#include "glext.h"
#include "tgaload.h"		//Used to load tga file
#include "ModelGL.h"

//Initialized stuff for the VBO Located in glext.h
PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;

//Define Textures
#define MAX_NO_TEXTURES 2
#define RIGHTSIDE_TEXTURE 0
#define LEFTSIDE_TEXTURE 1
GLuint texture_id[MAX_NO_TEXTURES];

/************************************************************************************
* Constructor - runs when the class is instantiated
*************************************************************************************/
ModelGL::ModelGL() : m_rotationAngle(0.0f), 
					z(0.0f), 
					leftplane(0),
					avgYtotalZ(0),
					stereo(true), 
					debug(false), 
					dtheta(1)			//Used to Rotate the camera by one degree
{
	

	//Set origin vector
	origin.x = 0.0;
	origin.y = 0.0;
	origin.z = 0.0;			//Instance of vector structure origin

	CameraHome(0);		//Bring the camera to home position 

	 	
}

/************************************************************************************
* Destructor
*************************************************************************************/
ModelGL::~ModelGL()
{

}


/************************************************************************************
*  init - initialize OpenGL states and scene
*************************************************************************************/
bool ModelGL::init()
{


	//wglGetProcAddress function returns the address of an OpenGL extension function for use with the current OpenGL rendering context.
	#ifdef _WIN32
		glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffers");
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	#else
		glGenBuffers = (PFNGLGENBUFFERSARBPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
		glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
	#endif

	  if (!glGenBuffers || !glBindBuffer || !glBufferData)
    {
        std::cerr << "VBOs are not supported by your graphics card" << std::endl;
        return false;
    }

	glEnable(GL_DEPTH_TEST);								// Enable Depth Testing
	glDepthFunc(GL_LEQUAL);									// Specifies the conditions under which the pixel will be drawn. Passes if the incoming depth value is less than or equal to the stored depth value.
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);					// Sets the background color to dark gray
	 
    glShadeModel(GL_SMOOTH);									// Enable Smooth Shading - computed colors of vertices to be interpolated as the primitive is rasterized
    glEnable ( GL_COLOR_MATERIAL );							// Makes it possible to change a subset of material parameters for each vertex using only the glColor command
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );	// Front parameters should track the current


	loadTextures();										//Load Textures


	initLights();										//Set up Ambient Light and Diffuse Light	
	
	

	//ModelGL::loadPcapFile("p01_port_confusion_DOS_and_scan.pcap");

	ModelGL::loadPcapFile("pcap/ftp_disguise.pcap");
	
	

    //Return success
    return true;
}

/************************************************************************************
* Load Textures
*************************************************************************************/
void ModelGL::loadTextures()
{
	/*********Load textures****************/
	glEnable ( GL_TEXTURE_2D );
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
    glGenTextures (2, texture_id);

    image_t   temp_image;

    glBindTexture ( GL_TEXTURE_2D, texture_id [0] );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    tgaLoad  ( "RightSideview.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );


    glBindTexture ( GL_TEXTURE_2D, texture_id [1] );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    tgaLoad  ( "LeftSideview.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );

}

/************************************************************************************
* initLights -intialize the lighting
*************************************************************************************/
void ModelGL::initLights()
{
	//Lighting
	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };


	/*********Set Lighting****************/
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);		// Position The Light
	glEnable(GL_LIGHT1);									// Enable Light One

}


void ModelGL::prepare(float dt)
{

}

/************************************************************************************
* Render - draw the openGL scene
*************************************************************************************/
void ModelGL::render()
{

	 XYZ r; //right camera

	 double ratio, radians, wd2, ndfl;
	 double left, right, top, bottom, near_plane=1, far_plane=200;
	
	 int count;
	/* Clip to avoid extreme stereo */

	if (stereo)
	{ 
			near_plane = camera.focallength / 5; //The rule thumb is one-fifth of the focal length
	}

   /* Misc stuff */
	 ratio  = camera.screenwidth / (double)camera.screenheight;
	 radians = DTOR * camera.aperture / 2;
	 wd2     = near_plane * tan(radians);			//half of the height
	 ndfl    = near_plane / camera.focallength;		//ratio to calculate assymetric frustum

	 if (stereo) {

      /* Derive the right eye positions */
      CROSSPROD(camera.vd,camera.vu,r);
      Normalise(&r);
      r.x *= camera.eyesep / 2.0;
      r.y *= camera.eyesep / 2.0;
      r.z *= camera.eyesep / 2.0;
	
	  //Calculate the frustum for right camera
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      left  = - ratio * wd2 - 0.5 * camera.eyesep * ndfl;
      right =   ratio * wd2 - 0.5 * camera.eyesep * ndfl;
      top    =   wd2;
      bottom = - wd2;
      
	  //produces a perspective projection by multiply the current matrix by a perspective matrix 
	  glFrustum(left,right,bottom,top,near_plane,far_plane);

	   //Calculate the viewing position and direction for right camera position
      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_RIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      
	  //Define the viewing transformation of the right camera
	  //Define the orientation and position of the right camera
	  gluLookAt(camera.vp.x + r.x,camera.vp.y + r.y,camera.vp.z + r.z,
                camera.vp.x + r.x + camera.vd.x,
                camera.vp.y + r.y + camera.vd.y,
                camera.vp.z + r.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);
      
	  //Render the Figure with the right camera
	  //Define the model transformation of the right camera
      
	  ModelGL::DrawCube(-6.0f, 0.0f, 0.0f);	//Draw 3D parallel Coordinate
	  ModelGL::DrawCube(6.0f, 0.0f, 0.0f);	//Draw 3D parallel Coordinate

	    if(leftplane>0)
		{
			ModelGL::DrawCube(-18.0f, 0.0f, 0.0f);
		}
	
	  /*      This is the bottom face*/
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);	

		//Set Line Width 
		glEnable(GL_LINE_SMOOTH);				//To enable line antialiasing,
		glLineWidth(1.5f);

		//char buf[2048];
		//sprintf(buf,"Total Number of Buffer: %u... \n", m_numVertexBufElements);
		//OutputDebugString(buf);

	  //Draw Vertex Buffer Object for the right camera
	  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[0]);
	   glVertexPointer(3, GL_DOUBLE, 0, 0);		//Tell OpenGL the vertices start at the beginning of this data

	  glEnableClientState(GL_VERTEX_ARRAY);		//Enable Vertex Array
	 
	  glDrawArrays(GL_LINES, 0, m_numVertexBufElements/3);			//Draw the line starting from index 0 with 12 elements

	  glDisableClientState(GL_VERTEX_ARRAY);	//Dissable the Vertex Array state
		
	  if(avgYtotalZ>0)
		{
		//Draw Vertex Buffer Object 2
		  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[1]);
		  glVertexPointer(3, GL_DOUBLE, 0, 0);							//Tell OpenGL the vertices start at the beginning of this data
		  glEnableClientState(GL_VERTEX_ARRAY);							//Enable Vertex Array
		   glDrawArrays(GL_LINES, 0, 100);	
		  glDrawArrays(GL_LINES, 0, m_numVertexBufElements2/3);			//Draw the line starting from index 0 with 12 elements
		}
	  
	  //glDisableClientState(GL_VERTEX_ARRAY);	//Dissable the Vertex Array state

	  //Calculate the frustum for left camera
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      left  = - ratio * wd2 + 0.5 * camera.eyesep * ndfl;
      right =   ratio * wd2 + 0.5 * camera.eyesep * ndfl;
      top    =   wd2;
      bottom = - wd2;
      glFrustum(left,right,bottom,top,near_plane,far_plane);
	
	   //Calculate the viewing position and direction for right camera position
      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_LEFT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      
	  //Define the viewing transformation of the left camera
	  //Define the orientation and position of the left camera
	  gluLookAt(camera.vp.x - r.x,camera.vp.y - r.y,camera.vp.z - r.z,
                camera.vp.x - r.x + camera.vd.x,
                camera.vp.y - r.y + camera.vd.y,
                camera.vp.z - r.z + camera.vd.z,
                camera.vu.x,camera.vu.y,camera.vu.z);
      
	  //Render the Figure with the left camera
	  //Define the model transformation of the left camera
      ModelGL::DrawCube(-6.0f, 0.0f, 0.0f);
	  ModelGL::DrawCube(6.0f, 0.0f, 0.0f);


	  if(leftplane>0)
		{
			ModelGL::DrawCube(-18.0f, 0.0f, 0.0f);
		}

	  glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	  //Set Line Width 
	  glEnable(GL_LINE_SMOOTH);				//To enable line antialiasing,
	  glLineWidth(1.5f);

	  //Draw Vertex Buffer Object for the left camera
	  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[0]);
	  glVertexPointer(3, GL_DOUBLE, 0, 0);		//Tell OpenGL the vertices start at the beginning of this data

	  glEnableClientState(GL_VERTEX_ARRAY);		//Enable Vertex Array
	  
	  glDrawArrays(GL_LINES, 0, m_numVertexBufElements/3);			//Draw the line starting from index 0 with 12 elements

 
	  glDisableClientState(GL_VERTEX_ARRAY);	//Dissable the Vertex Array state

	  if(avgYtotalZ>0)
		{

		//Draw Vertex Buffer Object 2
		  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[1]);
		  glVertexPointer(3, GL_DOUBLE, 0, 0);							//Tell OpenGL the vertices start at the beginning of this data
		  glEnableClientState(GL_VERTEX_ARRAY);							//Enable Vertex Array
		  //glDrawArrays(GL_LINES, 0, 100);	
		  glDrawArrays(GL_LINES, 0, m_numVertexBufElements2/3);		//Draw the line starting from index 0 with 12 elements
		}


	  } else {

      glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();							//Load Identity Model-View Matrix
      
	  //Define the viewing transformation
	  //Define the orientation and position of the camera
	  gluLookAt(camera.vp.x,camera.vp.y,camera.vp.z,	//Specifies the position of the eye point.
                camera.vp.x + camera.vd.x,				//Specifies the position of the x-value of reference point.
                camera.vp.y + camera.vd.y,				//Specifies the position of the y-value of reference point.
                camera.vp.z + camera.vd.z,				//Specifies the position of the z-value of reference point.
                camera.vu.x,camera.vu.y,camera.vu.z);	//Specifies the direction of the up vector.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the Figure for non-stereo mode
	//Define the model transformation for non-stereo mode					
	ModelGL::DrawCube(-6.0f, 0.0f, 0.0f);
	ModelGL::DrawCube(6.0f, 0.0f, 0.0f);
	
    if(leftplane>0)
    {
	    ModelGL::DrawCube(-18.0f, 0.0f, 0.0f);
    }


	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	
	//Set Line Width 
	glEnable(GL_LINE_SMOOTH);				//To enable line antialiasing,
	glLineWidth(1.5f);

	//Render Vertes Buffer Object (VBO) for non-stereo mode 
	  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[0]);
	  glVertexPointer(3, GL_DOUBLE, 0, 0);		//Tell OpenGL the vertices start at the beginning of this data

	  glEnableClientState(GL_VERTEX_ARRAY);		//Enable Vertex Array
	  
	  //glDrawArrays(GL_LINES, 0, 100);	
	 glDrawArrays(GL_LINES, 0, m_numVertexBufElements/3);			//Draw the line starting from index 0 with 12 elements


	 	  if(avgYtotalZ>0)
		{

		//Draw Vertex Buffer Object 2
		  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[1]);
		  glVertexPointer(3, GL_DOUBLE, 0, 0);							//Tell OpenGL the vertices start at the beginning of this data
		  glEnableClientState(GL_VERTEX_ARRAY);							//Enable Vertex Array
		  // glDrawArrays(GL_LINES, 0, 100);	
		  glDrawArrays(GL_LINES, 0, m_numVertexBufElements2/3);			//Draw the line starting from index 0 with 12 elements
		}



	  glDisableClientState(GL_VERTEX_ARRAY);	//Dissable the Vertex Array state
	}
}

/************************************************************************************
* DrawCube - Draw a 10x10x1 Cube given the point at the center of the cube
*************************************************************************************/
void ModelGL::DrawCube(float xPos, float yPos, float zPos)
{
        glPushMatrix();
        
		glBegin(GL_QUADS);

                /*This is the top face*/
				glColor4f(0.5f, 0.5f, 0.5f, 1.0f);	
                glVertex3f(xPos+0.5f, yPos+5.0f, zPos+5.0f);			//Front-Right
                glVertex3f(xPos-0.5f, yPos+5.0f, zPos+5.0f);		//Front-Left
                glVertex3f(xPos-0.5f, yPos+5.0f, zPos-5.0f);		//Back-Left
				glVertex3f(xPos+0.5f, yPos+5.0f, zPos-5.0f);	//Back-Right
               

                /*      This is the front face*/
				glColor4f(0.6f, 0.6f, 0.6f, 1.0f);		
                glVertex3f(xPos+0.5f, yPos+5.0f, zPos+5.0f);		//Top-Right 
                glVertex3f(xPos-0.5f, yPos+5.0f, zPos+5.0f);		//Top-Left
                glVertex3f(xPos-0.5f, yPos-5.0f, zPos+5.0f);		//Bottom Left
                glVertex3f(xPos+0.5f, yPos-5.0f, zPos+5.0f);		//Bottom-Right
				
				
                /*      This is the right face*/
				glBindTexture ( GL_TEXTURE_2D, texture_id[RIGHTSIDE_TEXTURE] );
				glColor4f(0.9f, 0.9f, 0.9f, 1.0f);		
				glTexCoord2f(1.0f, 1.0f); glVertex3f(xPos+0.5f, yPos+5.0f, zPos+5.0f);		//Top-Front 
                glTexCoord2f(0.0f, 1.0f); glVertex3f(xPos+0.5f, yPos+5.0f, zPos-5.0f);		//Top-Back
                glTexCoord2f(0.0f, 0.0f); glVertex3f(xPos+0.5f, yPos-5.0f, zPos-5.0f);		//Bottom-Back
				glTexCoord2f(1.0f, 0.0f); glVertex3f(xPos+0.5f, yPos-5.0f, zPos+5.0f);		//Bottom-front

				

                /*      This is the left face*/
				glBindTexture ( GL_TEXTURE_2D, texture_id[LEFTSIDE_TEXTURE] );
				glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(xPos-0.5f, yPos+5.0f, zPos+5.0f);		//Top-Front 
                glTexCoord2f(0.0f, 1.0f); glVertex3f(xPos-0.5f, yPos+5.0f, zPos-5.0f);		//Top-Back
                glTexCoord2f(0.0f, 0.0f); glVertex3f(xPos-0.5f, yPos-5.0f, zPos-5.0f);		//Bottom-Back
				glTexCoord2f(1.0f, 0.0f); glVertex3f(xPos-0.5f, yPos-5.0f, zPos+5.0f);		//Bottom-front
               


                /*      This is the bottom face*/
				glColor4f(0.5f, 0.5f, 0.5f, 1.0f);		
                glVertex3f(xPos+0.5f, yPos-5.0f, zPos+5.0f);			//Front-Right
                glVertex3f(xPos-0.5f, yPos-5.0f, zPos+5.0f);		//Front-Left
                glVertex3f(xPos-0.5f, yPos-5.0f, zPos-5.0f);		//Back-Left
				glVertex3f(xPos+0.5f, yPos-5.0f, zPos-5.0f);	//Back-Right
                

                /*      This is the back face*/
				glColor4f(0.6f, 0.6f, 0.6f, 1.0f);	
                glVertex3f(xPos+0.5f, yPos+5.0f, zPos-5.0f);		//Top-Right 
                glVertex3f(xPos-0.5f, yPos+5.0f, zPos-5.0f);		//Top-Left
                glVertex3f(xPos-0.5f, yPos-5.0f, zPos-5.0f);		//Bottom Left
                glVertex3f(xPos+0.5f, yPos-5.0f, zPos-5.0f);

        glEnd();
        glPopMatrix();
}

/************************************************************************************
* DrawLine - Draw a line given 2 points
*************************************************************************************/
void ModelGL::DrawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
        glPushMatrix();

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);		//Green
        glBegin(GL_LINES);
		    glVertex3f(x1, y1, z1) ; 
		    glVertex3f(x2, y2, z2);
	    glEnd();
		glPopMatrix();

}

void ModelGL::shutdown()
{

}

/************************************************************************************
* clearScreen - Clears the sceen
*************************************************************************************/
void ModelGL::clearScreen()
{
	wchar_t buf[2048];
	wsprintf(buf,L"Clearing the Screen... \n");
	OutputDebugStringW(buf);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clean the screen and the depth buffer
	glLoadIdentity();



}

/************************************************************************************
* loadPcap - Loads the pcap file
*************************************************************************************/
bool ModelGL::loadPcapFile(char * pcapFileName)
{

	//The parser for conversion from network data to GL coordinates
    Parser glParser;
	//m_parser = glParser;

	//glParser.readPacketsFromFile("port_source_confusion_r2.pcap");
	glParser.readPacketsFromFile(pcapFileName);
	
	
	glParser.showVertexIndex(0);
	glParser.showAveragePacketSize();
	glParser.showTotalNumberPackets();
	//glParser.glCalcCoord_averageY_totalZ();
	glParser.showVertexSize(); 
	int numVertexSize = glParser.getVertexSize();

	//Number of Vertex Elements in the Vertex Array
	m_numVertexBufElements = numVertexSize;

	vector<double> vertex;
	vector<double> vertex2;

	vertex.clear();
	vertex = glParser.gl_vertices;
	//Create the Vertex - array to a line

	glParser.glCalcCoord_averageY_totalZ();
	vertex2.clear();
	vertex2 = glParser.gl_vertices2;
	int numVertexSize2 = glParser.gl_vertices2.size();



	m_numVertexBufElements2 = numVertexSize2;

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[1]);//Bind buffer into GL_ARRAY_BUFFER is used for per-vertex data									//Bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * (numVertexSize2), &vertex2[0], GL_STATIC_DRAW); //Send the data to OpenGL

	glGenBuffers(1, &m_vertexBuffer[0]);			//Generate one buffer name and store it into the variable that can store one buffer name											//Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[0]);//Bind buffer into GL_ARRAY_BUFFER is used for per-vertex data									//Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * (numVertexSize), &vertex[0], GL_STATIC_DRAW); //Send the data to OpenGL


	

	return true;
}


/************************************************************************************
* onResize - Resizes the viewport
*************************************************************************************/
void ModelGL::onResize(int width, int height)
{
    glViewport(0, 0, width, height); //Viewport is the display area on the OpenGL's 2D application window


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	//x and y dimensions along the line of sight 
    gluPerspective(45.0f, float(width) / float(height),  0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*************************************************************************************************
*      CameraHome - Sets the camera to a default location 
**************************************************************************************************/

void ModelGL::CameraHome(int mode)
{
	
//The resolution of the screen is 1920 X 1080 (16:9 ratio)
   camera.screenwidth = 1920;
   camera.screenheight = 1080;


   camera.aperture = 50;
   camera.focallength = 20;
   camera.eyesep = camera.focallength / 20;
   camera.pr = origin;

/*
   camera.vp.x = camera.focallength; 
   camera.vp.y = 0; 
   camera.vp.z = 0;
   camera.vd.x = -1;
   camera.vd.y = 0;
   camera.vd.z = 0;
*/
   /* Special camera position so the beam crosses the view */
   camera.vp.x = 0;
   camera.vp.y = 0;
   camera.vp.z = 20;
   camera.vd.x =0; 
   camera.vd.y = 0; 
   camera.vd.z = -10;

   camera.vu.x = 0;  
   camera.vu.y = 1; 
   camera.vu.z = 0;



}


/*************************************************************************************************
*      AddLeftPlane - Add left plane
**************************************************************************************************/

	void ModelGL::AddLeftPlane(int number)
	{
		
		leftplane = number;
		OutputDebugStringW(L"Adding Left Plane. \n");

	}


/*************************************************************************************************
*      AddAvgYTotalPacketsZ - 
**************************************************************************************************/
	void ModelGL::AddAvgYTotalPacketsZ(int number)
	{
		
		avgYtotalZ = number;

	}
/*************************************************************************************************
*      Normalise - Divide the X, Y, Z component by magnitude of vector so that the magnituge of new vector is one 
**************************************************************************************************/
	void ModelGL::Normalise(XYZ *p)
	{
	   double length;

	   length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
	   if (length != 0) {
		  p->x /= length;
		  p->y /= length;
		  p->z /= length;
	   } else {
		  p->x = 0;
		  p->y = 0;
		  p->z = 0;
	   }
	}

/*************************************************************************************************
*      AdjustEyeSeparation - Increase or decrease eye separation
**************************************************************************************************/

	void ModelGL::AdjustEyeSeparation(int ix)
	{
		camera.eyesep += ix*.1;
	}

/*************************************************************************************************
*      AdjustFocalLength - Increase or decrease focal length
**************************************************************************************************/

	void ModelGL::AdjustFocalLength(int ix)
	{
		camera.focallength += ix*.1;

	}

/*************************************************************************************************
*      RotateCamera 
*	   -Rotate (ix,iy) or roll (iz) the camera about the focal point
*	   -ix,iy,iz are flags, 0 do nothing, +- 1 rotates in opposite directions
*      -Correctly updating all camera attributes
**************************************************************************************************/

	void ModelGL::RotateCamera(int ix,int iy,int iz)
	{
	   XYZ vp,vu,vd;			//Declare UP, Position, and Direction Vector
	   XYZ right;
	   XYZ newvp,newr;
	   double radius,dd,radians;
	   double dx,dy,dz;

	   //Calculate Direction and Up Unit Vectors
	   vu = camera.vu;
	   Normalise(&vu);
	   vp = camera.vp;
	   vd = camera.vd;
	   Normalise(&vd);
	   CROSSPROD(vd,vu,right);
	   Normalise(&right);
	   radians = dtheta * PI / 180.0;			//Convert degrees of rotation in radians

	   /* Handle the roll */
	   if (iz != 0) {
		  camera.vu.x += iz * right.x * radians;
		  camera.vu.y += iz * right.y * radians;
		  camera.vu.z += iz * right.z * radians;
		  Normalise(&camera.vu);
		  return;
	   }

	   /* Distance from the rotate point */  
	   dx = camera.vp.x - camera.pr.x;
	   dy = camera.vp.y - camera.pr.y;
	   dz = camera.vp.z - camera.pr.z;
	   radius = sqrt(dx*dx + dy*dy + dz*dz);

	   /* Determine the new view point */
	   dd = radius * radians;
	   newvp.x = vp.x + dd * ix * right.x + dd * iy * vu.x - camera.pr.x;
	   newvp.y = vp.y + dd * ix * right.y + dd * iy * vu.y - camera.pr.y;
	   newvp.z = vp.z + dd * ix * right.z + dd * iy * vu.z - camera.pr.z;
	   Normalise(&newvp);
	   camera.vp.x = camera.pr.x + radius * newvp.x;
	   camera.vp.y = camera.pr.y + radius * newvp.y;
	   camera.vp.z = camera.pr.z + radius * newvp.z;

	   /* Determine the new right vector */
	   newr.x = camera.vp.x + right.x - camera.pr.x;
	   newr.y = camera.vp.y + right.y - camera.pr.y;
	   newr.z = camera.vp.z + right.z - camera.pr.z;
	   Normalise(&newr);
	   newr.x = camera.pr.x + radius * newr.x - camera.vp.x;
	   newr.y = camera.pr.y + radius * newr.y - camera.vp.y;
	   newr.z = camera.pr.z + radius * newr.z - camera.vp.z;

	   camera.vd.x = camera.pr.x - camera.vp.x;
	   camera.vd.y = camera.pr.y - camera.vp.y;
	   camera.vd.z = camera.pr.z - camera.vp.z;
	   Normalise(&camera.vd);

	   /* Determine the new up vector */
	   CROSSPROD(newr,camera.vd,camera.vu);
	   Normalise(&camera.vu);

	}

/************************************************************************************
* Zoom the camera
*************************************************************************************/
	void ModelGL::Zoom(int ix)
	{

	   XYZ vp,vu,vd;
	   //XYZ right;
	   //XYZ newvp,newr;
	   //double radians;
	   double delta;

	   vu = camera.vu;
	   Normalise(&vu);
	   vp = camera.vp;
	   vd = camera.vd;
	   Normalise(&vd);
	   //CROSSPROD(vd,vu,right);
	   //Normalise(&right);
	   //radians = dtheta * PI / 180.0;
	   delta = dtheta * camera.focallength / 90.0;

	   camera.vp.x += ix * vd.x * delta;
	   camera.vp.y += ix * vd.y * delta;
	   camera.vp.z += ix * vd.z * delta;



	}

/*************************************************************************************************
*      TranslateCamera 
*	   
*		Translate (pan) the camera view point
*		In response to i,j,k,l keys
*	    Also move the camera rotate location in parallel
*
*		TranslateCamera(0,1);		//Translate camera up
*		TranslateCamera(0,-1);		//Translate camera down
*		TranslateCamera(-1,0);		//Translate camera left
*	    TranslateCamera(1,0);		//Translate camera right
**************************************************************************************************/

	void ModelGL::TranslateCamera(int ix,int iy, int iz)
	{
	   XYZ vp,vu,vd;
	   XYZ right;
	   //XYZ newvp;
	   //XYZ newr;
	   double radians,delta;

	   vu = camera.vu;
	   Normalise(&vu);
	   vp = camera.vp;
	   vd = camera.vd;
	   Normalise(&vd);
	   CROSSPROD(vd,vu,right);
	   Normalise(&right);
	   radians = dtheta * PI / 180.0;
	   delta = dtheta * camera.focallength / 90.0;

	   camera.vp.x += iy * vu.x * delta;
	   camera.vp.y += iy * vu.y * delta;
	   camera.vp.z += iy * vu.z * delta;
	   camera.pr.x += iy * vu.x * delta;
	   camera.pr.y += iy * vu.y * delta;
	   camera.pr.z += iy * vu.z * delta;

	   //
	   camera.vp.x += ix * right.x * delta;
	   camera.vp.y += ix * right.y * delta;
	   camera.vp.z += ix * right.z * delta;
	   camera.pr.x += ix * right.x * delta;
	   camera.pr.y += ix * right.y * delta;
	   camera.pr.z += ix * right.z * delta;

	   camera.vp.x += iz * vd.x * delta;
	   camera.vp.y += iz * vd.y * delta;
	   camera.vp.z += iz * vd.z * delta;
	   camera.pr.x += iz * vd.x * delta;
	   camera.pr.y += iz * vd.y * delta;
	   camera.pr.z += iz * vd.z * delta;

	}