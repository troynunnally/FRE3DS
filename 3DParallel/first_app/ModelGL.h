#ifndef _MODELGL_H
#define _MODELGL_H

#include <GL/gl.h>		//Needed for declaration fo gl variables
#include <vector>		//Needed for Vertex Buffer Object (VBO)
#include "Parser.h"

//Use the vector space within the standard library (std)
using std::vector;

#define PI 3.141592653589793238462643

//Degrees to Radians
#define DTOR            0.0174532925

//Define cross product
#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

//Vector Info
typedef struct {
   double x,y,z;
} XYZ;

//Camera Info
typedef struct {
   XYZ vp;              /* View position           */
   XYZ vd;              /* View direction vector   */
   XYZ vu;              /* View up direction       */
   XYZ pr;              /* Point to rotate about   */
   double focallength;  /* Focal Length along vd   */
   double aperture;     /* Camera aperture         */
   double eyesep;       /* Eye separation          */
   int screenwidth,screenheight;
} CAMERA;


class Parser; //Declare our Parser class

class ModelGL 
{
public:
    
	Parser* m_parser;					//A pointer to the Parser class

	ModelGL();							//Constructor
	~ModelGL();                         //Destructor

	//*****Used for Preparing OpenGL Window*****//
    bool init();							//Initialize OpenGL states and scene
    void prepare(float dt);					//Currently - DNE			
    void render();							//Draw the scene
    void shutdown();						//Currently - DNE			
	void onResize(int width, int height);	//Adjust the viewport and perspective
	void clearScreen();						//Clears the opengl screen
	
	//*****Use for adjusting the camera*****//
	bool loadPcapFile(char * pcapFileName);

	void setMouseLeft(bool flag) { mouseLeftDown = flag; };
    void setMouseRight(bool flag) { mouseRightDown = flag; };
    void setMousePosition(int x, int y) { mouseX = x; mouseY = y; };


	//*****Used for draw objects*****//
	void DrawCube(float xPos, float yPos, float zPos);
	void DrawLine(float x1, float y1, float z1, float x2, float y2, float z2);

	//*****Use for adjusting the camera*****//
	void CameraHome(int);					//Sets the camera to a default location
	void Normalise(XYZ *);					//Normalise vector so that the magntiude is equivalent to one
	void RotateCamera(int,int,int);			//Rotates camera in the direction of vd
	void TranslateCamera(int,int,int);		//Move the camera left, right, up, down, into screen, out of screen
	void AdjustEyeSeparation(int);			//Adjust Eye Separation of Left and Right Camera
	void AdjustFocalLength(int);			//Adjust the focal length of the camera
	void Zoom(int);							//Zooms the camera in the direction of vd



private:
	//Used for Preparing OpenGL Window
    void initLights();                              // add a white light to scene
	void loadTextures();							//Load Textures
    
	
	float m_rotationAngle;
	

	// members
    bool mouseLeftDown;
    bool mouseRightDown;
    int mouseX;
    int mouseY;

	float xspeed;			// X Rotation Speed
	float yspeed;			// Y Rotation Speed
	float ratio;			//Ratio of Width/Height
	float	z;				// Depth Into The Screen
	
	/*Flags*/
	int stereo;			//Stereoscopic 3D Flag
	int debug;			//debug bit = use to print info
	double dtheta;		//delta theta - 1 degree
	
	CAMERA camera;						//Instance of camera structure
	XYZ origin;							//Instance of the 3D location

	GLuint m_vertexBuffer;				//Vertex Buffer ID used to store the names of the buffer
	GLuint m_numVertexBufElements;		//

	
};

#endif