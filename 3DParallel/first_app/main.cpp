/*****************************************************************************
main.cpp

*****************************************************************************/
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

#ifdef _WIN32

//function declarations of Windows system calls or the WinAPI. 
//The WinAPI has everything necessary for programming under windows.
#include <windows.h>
#include "glwindow.h"
#else
#include "glxwindow.h"
#endif

#include "ModelGL.h"


/*****************************************************************************
 WinMain()

 Windows entry point
*****************************************************************************/

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance,/* Handle to the current application instance */
                   HINSTANCE hPrevInstance,/* Handle to the previous application instance */
                   LPSTR cmdLine,  /* pointer to command line arguments */
                   int cmdShow)/* show state of the window (Maximized, Minimized, etc).*/
{
#else
int main(int argc, char** argv)
{
#endif
    //Set our window settings
    const int windowWidth = 1024;				//Window Width
    const int windowHeight = 768;				//Window Height
    const int windowBPP = 16;					//Bits per pixel
    const int windowFullscreen = false;			//Fullscreen mode

#ifdef _WIN32
    //Instantiate our helper class called program window located in glwindow.cpp
	//GLWindow creates a window from the application instance hInstance
    GLWindow programWindow(hInstance);
#else
    SimpleGLXWindow programWindow;
#endif
	//Instantiate the model - The model contains all application data
	//and implementations to tell how the application behaves. 
	//Contains the the example OpenGL code for rendering the P3D scene
	//like set get parsed info, set lighting, etc.
    ModelGL model;

    //Attach our example to our window
	//Set the model in GLwindow to point to address of the model 
    programWindow.attachModel(&model);

    //Attempt to register and create the window given the Width, Height, Bits per pixel, Fullscreen mode
	//Here is where it fills out class structure, register the class, create window, display window
    if (!programWindow.create(windowWidth, windowHeight, windowBPP, windowFullscreen))
    {
        //If it fails
#ifdef _WIN32
        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
#endif
        programWindow.destroy(); //Reset the display and exit
        return 1;
    }

    if (!model.init()) //Initialize our OpenGL example
    {
#ifdef _WIN32
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
#endif
        programWindow.destroy(); //Reset the display and exit
        return 1;
    }

    //This is the mainloop, we render frames until isRunning returns false
	//isRunning is true when, the message handler create(WM_CREATE) is called
    while(programWindow.isRunning())
    {
        programWindow.processEvents(); //Process any window events

        //We get the time that passed since the last frame
        float elapsedTime = programWindow.getElapsedSeconds();

        model.prepare(elapsedTime); //Do any pre-rendering logic
        model.render(); //Render the scene

        programWindow.swapBuffers();
    }

    model.shutdown(); //Free any resources
    programWindow.destroy(); //Destroy the program window

    return 0; //Return success
}
