#ifndef _GLWINDOW_H
#define _GLWINDOW_H

#include <windows.h>
#include <ctime>

class ModelGL; //Declare our ModelGL class

class GLWindow 
{
public:
    GLWindow(HINSTANCE hInstance); //default constructor

    bool create(int width, int height, int bpp, bool fullscreen);
    void destroy();
    void processEvents();
    void attachModel(ModelGL* model);
    

    bool isRunning(); //Is the window running?

    void swapBuffers() { SwapBuffers(m_hdc); }

    static LRESULT CALLBACK StaticWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

    float getElapsedSeconds();
private:
    ModelGL* m_model;			//A link to the ModelGL program
    bool m_isRunning;			//Is the window still running?
    bool m_isFullscreen; 

	//
    HWND m_hwnd;					//Window handle
    HGLRC m_hglrc;					//Rendering context
    HDC m_hdc;						//Device context
    RECT m_windowRect;				//Window bounds
    HINSTANCE m_hinstance;			//Application instance
    WNDCLASSEX m_windowClass;		//Window Class
    void setupPixelFormat(void);	//Setup Pixel Format
    ModelGL* getAttachedModel() { return m_model; }  //Returns the instance of the ModelGL PpenGL class

    float m_lastTime;

	//Interactions
	void HandleKeyPress(UINT wParam);
	
	//Mouse Interactions
	//POINT Structure - Represents an ordered pair of integer x- and y-coordinates that defines a point in a two-dimensional plane.
	POINT g_OrigCursorPos;	//Location of the Mouse cursor
	POINT g_LastCursorPos;	//Last Location of the Mouse cursor
	POINT g_OrigWndPos;		//Location fot the application window
	
	bool g_LeftButtonPressed;
	bool g_MiddleButtonPressed;

	bool debug;
};

#endif