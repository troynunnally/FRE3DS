#include <ctime>
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include "wglext.h"
#include "glwindow.h"
#include "ModelGL.h"

typedef HGLRC (APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

//Constructor
GLWindow::GLWindow(HINSTANCE hInstance):
m_isRunning(false),		  //application is running
m_model(NULL),			  // Initialize model to NULL
m_hinstance(hInstance),   // application instance
m_lastTime(0)
{
    g_LeftButtonPressed = false;	//Left Button Pressed
	g_MiddleButtonPressed = false;	//Middle Button is Pressed
	debug = false;					//Debug is false
}

/*****************************************************************************
 GLWindow::create()

// fill out the window class structure
//register the windows class
// Adjust Window To True Requested Size
//create our window
// display the window
// update the window
*****************************************************************************/
bool GLWindow::create(int width, int height, int bpp, bool fullscreen)
{
    DWORD      dwExStyle;			        // Window Extended Style
    DWORD      dwStyle;					    // Window Style

    m_isFullscreen = fullscreen;			// Store the fullscreen flag

    m_windowRect.left = (long)0;			// Set Left Value To 0
    m_windowRect.right = (long)width;		// Set Right Value To Requested Width
    m_windowRect.top = (long)0;				// Set Top Value To 0
    m_windowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

    // Fill out the window class structure
    m_windowClass.cbSize          = sizeof(WNDCLASSEX);
    m_windowClass.style           = CS_HREDRAW | CS_VREDRAW;
    m_windowClass.lpfnWndProc     = GLWindow::StaticWndProc; //We set our static method as the event handler
    m_windowClass.cbClsExtra      = 0;
    m_windowClass.cbWndExtra      = 0;
    m_windowClass.hInstance       = m_hinstance;
    m_windowClass.hIcon           = LoadIcon(NULL, IDI_APPLICATION);  // default icon
    m_windowClass.hCursor         = LoadCursor(NULL, IDC_ARROW);      // default arrow
    m_windowClass.hbrBackground   = NULL;                             // don't need background
    m_windowClass.lpszMenuName    = NULL;                             // no menu
    m_windowClass.lpszClassName   = "GLClass";
    m_windowClass.hIconSm         = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon

    // Register the windows class
    if (!RegisterClassEx(&m_windowClass))
    {
        return false;
    }

    if (m_isFullscreen) // If we are fullscreen, we need to change the display mode                             
    {
        DEVMODE dmScreenSettings;                   // Device mode
        
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings); 

        dmScreenSettings.dmPelsWidth = width;			// screen width
        dmScreenSettings.dmPelsHeight = height;         // screen height
        dmScreenSettings.dmBitsPerPel = bpp;            // bits per pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // setting display mode failed, switch to windowed
            MessageBox(NULL, "Display mode failed", NULL, MB_OK);
            m_isFullscreen = false; 
        }
    }

    if (m_isFullscreen)                           // Are We Still In Fullscreen Mode?
    {
        dwExStyle = WS_EX_APPWINDOW;              // Window Extended Style
        dwStyle = WS_POPUP;                       // Windows Style
        ShowCursor(false);                        // Hide Mouse Pointer
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
    }

    AdjustWindowRectEx(&m_windowRect, dwStyle, false, dwExStyle);     // Adjust Window To True Requested Size

    // class registered, so now create our window
    m_hwnd = CreateWindowEx(NULL,               // extended style
        "GLClass",							    // class name
        "3D Parallel Coordinates",				// app name
        dwStyle | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS,
        0, 0,									// x,y coordinate
        m_windowRect.right - m_windowRect.left,
        m_windowRect.bottom - m_windowRect.top, // width, height
        NULL,									// handle to parent
        NULL,									// handle to menu
        m_hinstance,						    // application instance
        this);									// we pass a pointer to the GLWindow here

    // check if window creation failed (hwnd would equal NULL)
    if (!m_hwnd)
        return 0;
	
	
    m_hdc = GetDC(m_hwnd);				  //Get the device context of the window

    ShowWindow(m_hwnd, SW_SHOW);          // display the window
    UpdateWindow(m_hwnd);                 // update the window

    m_lastTime = GetTickCount() / 1000.0f; //Initialize the time
    return true;
}


/*****************************************************************************
GLWindow::destroy()
Destroys the window
*****************************************************************************/
void GLWindow::destroy() 
{
    if (m_isFullscreen)
    {
        ChangeDisplaySettings(NULL, 0);          // If So Switch Back To The Desktop
        ShowCursor(true);                        // Show Mouse Pointer
    }
}



/*****************************************************************************
 GLWindow::attachExample(Example* example)
Sets the OpenGL rendering and attach it to a new variable called m_example
*****************************************************************************/
void GLWindow::attachModel(ModelGL* model)
{
    m_model = model;
}



bool GLWindow::isRunning()
{
    return m_isRunning;
}


/*****************************************************************************
  GLWindow::processEvents()
Process events sent by the Operating Sstem
*****************************************************************************/
void GLWindow::processEvents()
{
    MSG msg;

    //While there are messages in the queue, store them in msg
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        //Process the messages one-by-one
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


/*****************************************************************************
 GLWindow::setupPixelFormat
Sets the pixel format sued in the device context
*****************************************************************************/
void GLWindow::setupPixelFormat(void)
{
    int pixelFormat;

    PIXELFORMATDESCRIPTOR pfd =
    {   
        sizeof(PIXELFORMATDESCRIPTOR),  // size
            1,                          // version
            PFD_SUPPORT_OPENGL |        // OpenGL window
            PFD_DRAW_TO_WINDOW |        // render to window
            PFD_DOUBLEBUFFER |          // support double-buffering
			PFD_STEREO,					// support quad-buffering stereo
            PFD_TYPE_RGBA,              // color type
            32,                         // prefered color depth
            0, 0, 0, 0, 0, 0,           // color bits (ignored)
            0,                          // no alpha buffer
            0,                          // alpha bits (ignored)
            0,                          // no accumulation buffer
            0, 0, 0, 0,                 // accum bits (ignored)
            16,                         // depth buffer
            0,                          // no stencil buffer
            0,                          // no auxiliary buffers
            PFD_MAIN_PLANE,             // main layer
            0,                          // reserved
            0, 0, 0,                    // no layer, visible, damage masks
    };

	//Find the closes matching pixel format
    pixelFormat = ChoosePixelFormat(m_hdc, &pfd);

	//Set the pixel format for the device context
    SetPixelFormat(m_hdc, pixelFormat, &pfd);
	
	//Check the pixel format for Stereo
	int iPixelFormat= DescribePixelFormat(m_hdc, 1, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	int iStereoPixelFormats = 0;

	while (iPixelFormat) {
		DescribePixelFormat(m_hdc, iPixelFormat,sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		if (pfd.dwFlags& PFD_STEREO){
			iStereoPixelFormats++;
		}
	iPixelFormat--;
	}
	if (iStereoPixelFormats== 0){
		// no stereo pixel formats available
		OutputDebugString("No Stereo is available \n");
	}
	else{
		OutputDebugString("Stereo formats is available \n");
	}
}
/*****************************************************************************
 WndProc()

 Windows message handler
*****************************************************************************/
LRESULT GLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_CREATE:         // window creation
    {
        m_hdc = GetDC(hWnd);		//Get the device context handle
        setupPixelFormat();			//Set up the pixel format

        //Set the version that we want, in this case 3.0
        int attribs[] = {
	        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        0}; //zero indicates the end of the array

        //Create temporary context so we can get a pointer to the function
        HGLRC tmpContext = wglCreateContext(m_hdc);
        //Make it current context
        wglMakeCurrent(m_hdc, tmpContext);

        //Get the function pointer
        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");

        //If this is NULL then OpenGL 3.0 is not supported
        if (!wglCreateContextAttribsARB)
        {
			std::cerr << "OpenGL 3.0 is not supported, falling back to GL 2.1" << std::endl;
            m_hglrc = tmpContext;
        } 
		else
		{
			// Create an OpenGL 3.0 context using the new function
			m_hglrc = wglCreateContextAttribsARB(m_hdc, 0, attribs);
			//Delete the temporary context
			wglDeleteContext(tmpContext);
		}

        //Make the GL3 context current
        wglMakeCurrent(m_hdc, m_hglrc);

        m_isRunning = true; //Mark our window as running
    }
    break;
    case WM_DESTROY: // window destroy
    case WM_CLOSE: // windows is closing
        wglMakeCurrent(m_hdc, NULL);
        wglDeleteContext(m_hglrc);
        m_isRunning = false; //Stop the main loop
        PostQuitMessage(0); //Send a WM_QUIT message
        return 0;
    break;
    case WM_SIZE:
    {
        int height = HIWORD(lParam);        // retrieve width and height
        int width = LOWORD(lParam);
        getAttachedModel()->onResize(width, height); //Call the example's resize method
    }
    break;
	case WM_KEYDOWN:             
        {
            HandleKeyPress(wParam);   // KeyPress function acts on the key passed in
            break;
        }
   	case WM_LBUTTONDOWN:
		if (GetCursorPos(&g_OrigCursorPos))	//Retrieves the cursor's position, in screen coordinates.
		{
			RECT rt;						//defines the coordinates of the upper-left and lower-right corners of a rectangle.
			GetWindowRect(hWnd, &rt);		//Retrieves the dimensions of the bounding rectangle of the specified window. 
			g_OrigWndPos.x = rt.left;
			g_OrigWndPos.y = rt.top;
			g_LeftButtonPressed = true;
			SetCapture(hWnd);							//Sets the mouse capture to the specified window, hWnd, belonging to the current thread
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));	//Sets the cursor shape.Four-pointed arrow pointing north, south, east, and west
			
			g_LastCursorPos.x = g_OrigCursorPos.x;
			g_LastCursorPos.y = g_OrigCursorPos.y;

			if (debug){
			OutputDebugString("Left Button Pressed. \n");
			}
		}
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();						//Releases the mouse capture from a window in the current thread and restores normal mouse input processing.
		return 0;
	case WM_MBUTTONDOWN:
		if (GetCursorPos(&g_OrigCursorPos))	//Retrieves the cursor's position, in screen coordinates.
		{
			RECT rt;						//defines the coordinates of the upper-left and lower-right corners of a rectangle.
			GetWindowRect(hWnd, &rt);		//Retrieves the dimensions of the bounding rectangle of the specified window. 
			g_OrigWndPos.x = rt.left;
			g_OrigWndPos.y = rt.top;
			g_MiddleButtonPressed = true;
			SetCapture(hWnd);							//Sets the mouse capture to the specified window, hWnd, belonging to the current thread
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));	//Sets the cursor shape.Four-pointed arrow pointing north, south, east, and west
			
			g_LastCursorPos.x = g_OrigCursorPos.x;
			g_LastCursorPos.y = g_OrigCursorPos.y;

			if (debug){
			OutputDebugString("Left Button Pressed. \n");
			}
		}
		return 0;
	case WM_MBUTTONUP:
		ReleaseCapture();						//Releases the mouse capture from a window in the current thread and restores normal mouse input processing.
	return 0;
	case WM_CAPTURECHANGED:						//Sent to the window that is losing the mouse capture.
		g_LeftButtonPressed = (HWND)lParam == hWnd;	//A handle to the window gaining the mouse capture
		g_MiddleButtonPressed = (HWND)lParam == hWnd;	
	return 0;

	case WM_MOUSEMOVE:
		if (g_LeftButtonPressed)
		{
			POINT pt;
			if (GetCursorPos(&pt))
			{


				int dx,dy;
				
				dx = pt.x  - g_LastCursorPos.x;
				dy = pt.y - g_LastCursorPos.y;

				if (dx < 0)      dx = -1;//Mouse moved to the left
			    else if (dx > 0) dx =  1;//Mouse moved to the right
			    if (dy < 0)      dy = -1;//Mouse moved up
			    else if (dy > 0) dy =  1;//Mouse moved down
				
				//getAttachedModel()->RotateCamera(0,1,0);
				getAttachedModel()->RotateCamera(-dx,dy,0);

				//Position the Window
				/*int wnd_x = g_OrigWndPos.x + 
				  (pt.x - g_OrigCursorPos.x);
				int wnd_y = g_OrigWndPos.y + 
				  (pt.y - g_OrigCursorPos.y);
				SetWindowPos(hWnd, NULL, wnd_x, 
				  wnd_y, 0, 0, SWP_NOACTIVATE|
				  SWP_NOOWNERZORDER|SWP_NOZORDER|
				  SWP_NOSIZE);*/
				
				//Set Last Cursor Position of the mouse
				g_LastCursorPos.x=pt.x;
				g_LastCursorPos.y=pt.y;

				if (debug){
					char buf[2048];
					sprintf(buf,"g_OrigCursorPos = (%d, %d) \n",g_OrigCursorPos.x, g_OrigCursorPos.y);
					OutputDebugString(buf);

					sprintf(buf,"MovedCursorPos = (%d, %d) \n",pt.x, pt.y);
					OutputDebugString(buf);
				}
			}
		}
		if (g_MiddleButtonPressed)
		{
			POINT pt;
			if (GetCursorPos(&pt))
			{


				int dx,dy;
				
				dx = pt.x  - g_LastCursorPos.x;
				dy = pt.y - g_LastCursorPos.y;

				if (dx < 0)      dx = -1;//Mouse moved to the left
			    else if (dx > 0) dx =  1;//Mouse moved to the right
			    if (dy < 0)      dy = -1;//Mouse moved up
			    else if (dy > 0) dy =  1;//Mouse moved down
				
				getAttachedModel()->Zoom(-dy);

				//Position the Window
				/*int wnd_x = g_OrigWndPos.x + 
				  (pt.x - g_OrigCursorPos.x);
				int wnd_y = g_OrigWndPos.y + 
				  (pt.y - g_OrigCursorPos.y);
				SetWindowPos(hWnd, NULL, wnd_x, 
				  wnd_y, 0, 0, SWP_NOACTIVATE|
				  SWP_NOOWNERZORDER|SWP_NOZORDER|
				  SWP_NOSIZE);*/
				
				//Set Last Cursor Position of the mouse
				g_LastCursorPos.x=pt.x;
				g_LastCursorPos.y=pt.y;

				if (debug){
					char buf[2048];
					sprintf(buf,"g_OrigCursorPos = (%d, %d) \n",g_OrigCursorPos.x, g_OrigCursorPos.y);
					OutputDebugString(buf);

					sprintf(buf,"MovedCursorPos = (%d, %d) \n",pt.x, pt.y);
					OutputDebugString(buf);
				}
			}
		}
	return 0;

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*****************************************************************************
 GLWindow::HandleKeyPress

 Windows message handler
******************************************************************************/
void GLWindow::HandleKeyPress(UINT wParam)
{
    if (wParam == VK_ESCAPE) //If the escape key was pressed
    {
        DestroyWindow(m_hwnd); //Send a WM_DESTROY message
    }
	if (wParam == VK_LEFT) //If the LEFT ARROW key was pressed
    {
         getAttachedModel()->TranslateCamera(-1,0,0);		// Translate camera left
    }
	if (wParam == VK_RIGHT) //If the RIGHT ARROW key was pressed
    {
        getAttachedModel()->TranslateCamera(1,0,0);		//Translate camera right
    }
	if (wParam == VK_UP) //If the UP ARROW key was pressed
    {
        getAttachedModel()->TranslateCamera(0,1,0);		// Translate camera up
    }
	if (wParam == VK_DOWN) //If the DOWN ARROW key was pressed
    {
		getAttachedModel()->TranslateCamera(0,-1,0);		//Translate camera down
    }
    if (wParam == VK_RETURN)
    {
		getAttachedModel()->CameraHome(0);	//Translate camera down
       
    }
	if (wParam == 0x44) //D
    {
		//Toggle debug flag
		if (!debug)
		{
			debug=true;
		}
		else
		{
			debug=false;
		}  
       
    }
	if (wParam == 0x48) //H
    {
		getAttachedModel()->CameraHome(0);	//Go home   
       
    }
	if (wParam == 0x49) //I
    {
		getAttachedModel()->TranslateCamera(0,1,0);	//Translate camera up
       
    }
	if (wParam == 0x4A) //J
    {
		getAttachedModel()->TranslateCamera(-1,0,0);	//Translate camera left
       
    }
		if (wParam == 0x4B) //K
    {
		getAttachedModel()->TranslateCamera(0,-1,0);	//Translate camera down
       
    }
	if (wParam == 0x4C) //L
    {
		getAttachedModel()->TranslateCamera(1,0,0);	//Translate camera right
       
    }
	if (wParam == 0x4D) //M
    {
		getAttachedModel()->TranslateCamera(0,0,-1);	//Translate camera away screen
       
    }
	if (wParam == 0x4E) //N
    {
		getAttachedModel()->TranslateCamera(0,0,1);	//Translate camera into screen
       
    }
	if (wParam == 0x4F) //O
    {
		getAttachedModel()->AdjustEyeSeparation(-1);	//Decrease Eye Separation 
       
    }
	if (wParam == 0x50) //P
    {
		getAttachedModel()->AdjustEyeSeparation(1);	//Increase Eye Separation
       
    }
	if (wParam == 0x51) //Q
    {
		getAttachedModel()->Zoom(1);	//Increase Eye Separation
       
    }

	if (wParam == 0x59) //Y
    {
		getAttachedModel()->AdjustFocalLength(-1);	//Decrease focal length
       
    }
}

/*****************************************************************************
 GLWindow::StaticWndProc

 Windows message handler
******************************************************************************/
LRESULT CALLBACK GLWindow::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GLWindow* window = NULL;

    //If this is the create message
    if(uMsg == WM_CREATE)
    {
        //Get the pointer we stored during create 
        window = (GLWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;

        //Associate the window pointer with the hwnd for the other events to access
        SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
    }
    else
    {
        //If this is not a creation event, then we should have stored a pointer to the window
        window = (GLWindow*)GetWindowLongPtr(hWnd, GWL_USERDATA);

        if(!window) 
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);    
        }
    }

    //Call our window's member WndProc (allows us to access member variables)
    return window->WndProc(hWnd, uMsg, wParam, lParam);
}

float GLWindow::getElapsedSeconds()
{
    float currentTime = float(GetTickCount()) / 1000.0f;
    float seconds = float(currentTime - m_lastTime);
    m_lastTime = currentTime;
    return seconds;
}
