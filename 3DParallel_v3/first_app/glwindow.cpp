#include <ctime>
#include <iostream>
#include <windows.h>
//Contains many macros designed to make coding easier.
//Use to handle the menu
#include <windowsx.h>
#define _WIN32_IE 0x0500 // To support INITCOMMONCONTROLSEX

//Contains the function for special controls 
#include <commctrl.h>

#include <GL/gl.h>
#include "wglext.h"
#include "glwindow.h"
#include "ModelGL.h"
//File with the icons, GUI elements, Menu, Version
#include "resource.h"
//About dialog bix
#include "AboutDialog.h"
#include "Db.h"
#include "GestureWorksCore.h"
#include <cmath>


//#include "CSkeletonBasics.h"

static const float g_JointThickness = 3.0f;
static const float g_TrackedBoneThickness = 6.0f;
static const float g_InferredBoneThickness = 1.0f;


typedef HGLRC (APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

//Constructor
GLWindow::GLWindow(HINSTANCE hInstance):
	m_isRunning(false),		  //application is running
	m_isIntersec(false),
	m_isKinect(false),
	m_model(NULL),			  // Initialize model to NULL
	m_db(NULL),
	m_hinstance(hInstance),   // application instance
	m_lastTime(0),
	//m_rotate_value_x(0),
	//m_rotate_value_y(0),
	//m_rotate_value_z(0),
	//m_translate_value_x(0),
	//m_translate_value_y(0),
	//m_zoom_value(0),
	//session_id(1),F
    m_pD2DFactory(NULL),
    m_hNextSkeletonEvent(INVALID_HANDLE_VALUE),
    m_pSkeletonStreamHandle(INVALID_HANDLE_VALUE),
    m_bSeatedMode(false),
    m_pRenderTarget(NULL),
    m_pBrushJointTracked(NULL),                 //Brush for the tracked joint
    m_pBrushJointInferred(NULL),
    m_pBrushBoneTracked(NULL),
    m_pBrushBoneInferred(NULL),
    m_pNuiSensor(NULL)
{
  // eventCount = 1;								//Event Count
	ZeroMemory(m_Points,sizeof(m_Points));      //Fills a block of memory, m_Points, with zeros.
	//m_left_Hand_Point_old = NULL;
    g_LeftButtonPressed = false;	//Left Button Pressed
	g_RightButtonPressed = false;
	g_MiddleButtonPressed = false;	//Middle Button is Pressed
	debug = true;					//Debug is false
	//debug = true;
	m_hMainToolbar = NULL; //Init Main Toolbar window to NULL
	m_hPaintToolbar = NULL;//Init Paint toolbar window to NULL
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

    // *******Fill out the window class structure WNDCLASSEX******
	//WNDCLASSEX provides all the information necessary to perform 
	//windows stuff like icon, cursor, menu, calling the callback,and 
	//receive messages
    m_windowClass.cbSize          = sizeof(WNDCLASSEX);// Must always be sizeof(WNDCLASSEX)
    m_windowClass.style           = CS_HREDRAW | CS_VREDRAW;// Class styles
    m_windowClass.lpfnWndProc     = GLWindow::StaticWndProc; //We set our static method as the pointer to callback or event handler
    m_windowClass.cbClsExtra      = 0;// Extra bytes to allocate following the wndclassex structure
    m_windowClass.cbWndExtra      = 0;// Extra bytes to allocate following an instance of the structure
    m_windowClass.hInstance       = m_hinstance;//Instance of the application
    m_windowClass.hIcon           = LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_APP_ICON)); // Class Icon  // default icon
    m_windowClass.hCursor         = LoadCursor(NULL, IDC_ARROW);      // default arrow
    m_windowClass.hbrBackground   = NULL;                             // don't need background brush
    m_windowClass.lpszMenuName    = MAKEINTRESOURCE(IDM_MAINMENU); // Menu Resource in Resource.h                            // no menu
    m_windowClass.lpszClassName   = L"GLClass";						  //Name of this class
    m_windowClass.hIconSm         = LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_APP_ICON)); // windows logo small icon

    // Register the windows class
    if (!RegisterClassEx(&m_windowClass))
    {
        return false;
    }

	 // Initialize Common controls
	INITCOMMONCONTROLSEX icx;
	// Ensure common control DLL is loaded
	icx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icx.dwICC = ICC_BAR_CLASSES; // Specify BAR classes
	InitCommonControlsEx(&icx); // Load the common control DLL

    if (m_isFullscreen) // If we are fullscreen, we need to change the display mode                             
    {
        DEVMODE dmScreenSettings;                   // Device mode
        
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));		//Clear the dmScreenSettings memory
        dmScreenSettings.dmSize = sizeof(dmScreenSettings); 

        dmScreenSettings.dmPelsWidth = width;			// screen width
        dmScreenSettings.dmPelsHeight = height;         // screen height
        dmScreenSettings.dmBitsPerPel = bpp;            // bits per pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // setting display mode failed, switch to windowed
            MessageBoxW(NULL, L"Display mode failed", NULL, MB_OK);
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
        L"GLClass",							    // class name
        L"3D Parallel Coordinates",				// Window Title
        dwStyle | WS_CLIPCHILDREN |				//Window Style
        WS_CLIPSIBLINGS,
        900, 0,									// x,y position of window
        m_windowRect.right - m_windowRect.left,	//Width of Window
        m_windowRect.bottom - m_windowRect.top, // Height of the window
        NULL,									// handle to parent
        NULL,									// handle to menu
        m_hinstance,						    // handle to application instance
        this);									// we pass a pointer to the GLWindow here

    // check if window creation failed (hwnd would equal NULL)
    if (!m_hwnd)
        return 0;
	
	
    m_hdc = GetDC(m_hwnd);				  //Get the device context of the window

	//Prepare windwo for touch
	/*
	if (!RegisterTouchWindow(m_hwnd, 0))
	{
		OutputDebugStringW(L"Registered for touch... \n");
	}
	*/

	/*Setup InterSec if enabled*/
	if(m_isIntersec){
		Setup_GestureWorks(m_hwnd);
	}



    ShowWindow(m_hwnd, SW_SHOW);          // display the window
    UpdateWindow(m_hwnd);                 // update the window

	/*START TIMER*/
	   aTimer = new Timer(m_db); 
       aTimer->start();
	   Sleep(1000);
	   //aTimer->stop();
	   //aTimer->uploadTime();
		//m_lastTime = GetTickCount() / 1000.0f; //Initialize the time

    return true;
}



/*****************************************************************************
GLWindow::createVisualizationWindow()
Creates the visualization window
*****************************************************************************/
bool GLWindow::setupVisualizationWindow(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

	    m_hdc = GetDC(hWnd);		//Get the device context handle
        setupPixelFormat();			//Set up the pixel format

		//Create the UI Elements
		if (!GLWindow::OnCreate(hWnd, (LPCREATESTRUCT)lParam))
        {
			std::cerr << "Status Bar did not create" << std::endl;
  
        }
        //Set the version that we want, in this case 3.0
        int attribs[] = {
	        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        0}; //zero indicates the end of the array

        //Create temporary context so we can get a pointer to the function
        HGLRC tmpContext = wglCreateContext(m_hdc);
        //Make it current context
        wglMakeCurrent(m_hdc, tmpContext);

        //Get the function pointer for opengl 3.0 context
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

		
    return true;

}

/*****************************************************************************
GLWindow::createKinectWindow()
Creates the Kinect Dialog Box
*****************************************************************************/
bool GLWindow::createKinectWindow(HINSTANCE hInstance) 
{
   
	DWORD      dwExStyle;			        // Window Extended Style
    DWORD      dwStyle;					    // Window Style

    // *******Fill out the window class structure WNDCLASSEX******
	//WNDCLASSEX provides all the information necessary to perform 
	//windows stuff like icon, cursor, menu, calling the callback,and 
	//receive messages
	MSG       msg = {0};                                //Windows message
	WNDCLASS  wc  = {0};                                //Windows class structure

    // Dialog custom window class
    wc.style         = CS_HREDRAW | CS_VREDRAW;         //window class styles 
    wc.cbWndExtra    = DLGWINDOWEXTRA;                  //number of extra bytes to allocate following the window instance
    wc.hInstance     = hInstance;                       //A handle to the instance that contains the window procedure for the class.
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
    wc.lpfnWndProc   = DefDlgProcW;                     //pointer to the window procedure.
    wc.lpszClassName = L"SkeletonBasicsAppDlgWndClass";

    
   //CS_HREDRAW -  Redraws the entire window if a movement or size adjustment changes the width of the client area
   //CS_VREDRAW - Redraws the entire window if a movement or size adjustment changes the height of the client area.
    
    
    if (!RegisterClassW(&wc))                       // Register a Unicode window class
    {

		//Display Error
			DWORD lastError = GetLastError();
			wchar_t  buf[2048];
			wsprintf(buf,L"Error Loading Window: Code %d\n", lastError);
			OutputDebugStringW(buf);

        return 0;
    }

	// Create main application window
    HWND m_hWndKinect = CreateDialogParamW(
        m_hinstance,                                   // Handle to the application instance
        MAKEINTRESOURCE(IDD_KINECT),                   // An integer value that specifies the resource identifier 
        NULL,                                          // A handle to the window that owns the dialog box.
        (DLGPROC)GLWindow::ToolDlgProc,				   // Casted Dialog Box Procedure
        reinterpret_cast<LPARAM>(this));   // A pointer to the dialog box procedure - this Skeleton class 

		
    // check if window creation failed (hwnd would equal NULL)      

		if(m_hWndKinect != NULL)
        {
            ShowWindow(m_hWndKinect, SW_SHOW);
        }
        else
        {
			//Display Error
			DWORD lastError = GetLastError();
			wchar_t  buf[2048];
			wsprintf(buf,L"Error Loading Window: Code %d\n", lastError);
			OutputDebugStringW(buf);
	
            MessageBoxW(m_hwnd, L"CreateDialog returned NULL", L"Warning!",  
                MB_OK | MB_ICONINFORMATION);
        }

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
 GLWindow::database(Example* example)
Sets the Db class and attach it to a new variable called m_example
*****************************************************************************/
void GLWindow::attachDatabase(Db* database)
{
    m_db = database;
}


/*****************************************************************************
 GLWindow::attachGestureEngine
Sets the Gesture Engine and attach it
*****************************************************************************/
void GLWindow::attachGestureEngine(GestureEngine* gestureEngine)
{
    m_gestureEngine = gestureEngine;
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
    MSG msg;	//The message from OS to application

	// Check for Kinect event (hEvents)
	hEvents[0] = m_hNextSkeletonEvent;          //Next Skeleton Events

	//Update a gesture if intersec is enabled
		if(m_isIntersec){
			update_gesture();
		}

		// Check to see if we have either a message (by passing in QS_ALLEVENTS)
        // Or a Kinect event (hEvents)
        // Update() will check for Kinect events individually, in case more than one are signalled
        // the return value indicates the event that caused the function to return
        DWORD dwEvent = MsgWaitForMultipleObjects(eventCount,           //number of object handles in the array pointed to by hEvents
                                                  hEvents,              //An array of object handles.
                                                  FALSE,                //returns when the state of any one of the objects is set to signaled
                                                  INFINITE,             //return only when the specified objects are signaled
                                                  QS_ALLINPUT           //Any message is in the queue.
                                                  );
		
		// Check if this is an event we're waiting on (Kinect Event) and not a timeout or message
        if (WAIT_OBJECT_0 == dwEvent)               //The state of the specified object as a Kinect Signal
        {
            
			Update();                               // Main processing function
			// no stereo pixel formats available
			//OutputDebugStringW(L"K \n");
        }

    //While there are messages in the queue, store them in msg
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        //Process the messages one-by-one
        TranslateMessage(&msg); // translates virtual key messages to character messages.
        DispatchMessage(&msg); // dispatche the message to a window procedure.
    }
}


/*****************************************************************************
 GLWindow::setupPixelFormat
Sets the pixel format in the device context
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
		OutputDebugStringW(L"No Stereo is available \n");
	}
	else{
		OutputDebugStringW(L"Stereo formats is available \n");
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
		
		GLWindow::setupVisualizationWindow(hWnd, wParam, lParam);

		if(m_isKinect){
			GLWindow::createKinectWindow(m_hinstance);		//Create Dialog
		}
		   //Start an instance of the CSkeletonBasics Class
    }
    break;
    case WM_DESTROY: // window destroy
    case WM_CLOSE: // windows is closing
        wglMakeCurrent(m_hdc, NULL); //Make current device context to be null
        wglDeleteContext(m_hglrc);	//Delete the current rendering context 
        m_isRunning = false; //Stop the main loop
        PostQuitMessage(0); //Send a WM_QUIT message
        return 0;
    break;
	//When the window is resized
    case WM_SIZE:
    {
        int height = HIWORD(lParam);        // retrieve width and height
        int width = LOWORD(lParam);
        getAttachedModel()->onResize(width, height); //Call the example's resize method

		// Resize Status Bar Window by sending message statusbar window
		SendMessage(GLWindow::m_hStatusbar,uMsg,wParam,lParam);

		// Resize Toolbar Window by sending message statusbar window
		SendMessage(GLWindow::m_hMainToolbar,uMsg,wParam,lParam);
    }
    break;
	case WM_KEYDOWN:             
        {
            m_gestureEngine->HandleKeyPress(wParam);    // KeyPress function acts on the key passed in
            break;
        }
	
	case WM_LBUTTONDOWN:
		if(!m_isIntersec){
			g_LeftButtonPressed = true;					// Set left button as pressed		
			m_gestureEngine->setCursorPosition(g_OrigCursorPos);
		}
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();								// Releases the mouse capture from a window in the current thread and restores normal mouse input processing.
		g_LeftButtonPressed = false;					// Set Left Button as not pressed
		m_gestureEngine->sendTranslate();   

		return 0;
	
   	case WM_RBUTTONDOWN:
		if (GetCursorPos(&g_OrigCursorPos))	//Retrieves the cursor's position, in screen coordinates.
		{
			g_RightButtonPressed = true;	
			m_gestureEngine->setCursorPosition(g_OrigCursorPos);
		}
		return 0;
	case WM_RBUTTONUP:
		ReleaseCapture();						//Releases the mouse capture from a window in the current thread and restores normal mouse input processing.
		g_RightButtonPressed = false;
		m_gestureEngine->sendRotate();			//When the rotate button is release send it to the server
		
		return 0;
	case WM_MBUTTONDOWN:
		if (GetCursorPos(&g_OrigCursorPos))	//Retrieves the cursor's position, in screen coordinates.
		{
			g_MiddleButtonPressed = true;
			m_gestureEngine->setCursorPosition(g_OrigCursorPos);
		}
		return 0;
	case WM_MBUTTONUP:
		g_MiddleButtonPressed = false;
		m_gestureEngine->sendZoom();
		ReleaseCapture();						//Releases the mouse capture from a window in the current thread and restores normal mouse input processing.
	return 0;
	case WM_CAPTURECHANGED:						//Sent to the window that is losing the mouse capture.
		g_LeftButtonPressed = (HWND)lParam == hWnd;
		g_RightButtonPressed = (HWND)lParam == hWnd;	//A handle to the window gaining the mouse capture
		g_MiddleButtonPressed = (HWND)lParam == hWnd;	
	return 0;

	case WM_MOUSEMOVE:
		if (g_RightButtonPressed)
		{
			POINT pt;
			if (GetCursorPos(&pt))
			{
				if(m_isIntersec){
					m_gestureEngine->ProcessRotate(pt);		//Use Gesture Engine to process translate
				}else
				{
					m_gestureEngine->ProcessMouseRotate(pt);		//Use Gesture Engine to process rotate
				}
			}
		}
		if (g_LeftButtonPressed)
		{
			POINT pt;
			if (GetCursorPos(&pt))
			{
				//IF Intersec is running run a translate command
				//If not, run a translate command for the mouse
				if(m_isIntersec){
					m_gestureEngine->ProcessTranslate(pt);		//Use Gesture Engine to process translate
				}else
				{
					m_gestureEngine->ProcessMouseTranslate(pt);		//Use Gesture Engine to process translate
				}

			}
		}
		if (g_MiddleButtonPressed)
		{
			POINT pt;
			if (GetCursorPos(&pt))
			{
				m_gestureEngine->ProcessZoom(pt);		//Use Gesture Engine to process zoom
			}
		}
	return 0;
	//Handles MENU presses adn set the callback
	case WM_COMMAND:
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, OnCommand);
    default:
        break;
    }
	//All messages that you don't want to handle should be passed to the DefWindowProc()
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


std::map<int,gwc::Point> active_points;

void GLWindow::Setup_GestureWorks(HWND hWnd) {


	active_points = std::map<int,gwc::Point>();
	
	wchar_t  WindowName[256];

	GetWindowText(hWnd, WindowName, 256);
	std::wstring str(WindowName);

	//Convert from wstring to string
	int len;
    int slength = (int)str.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, 0, 0, 0, 0); 
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, buf, len, 0, 0); 
    std::string string_window_name(buf);
    delete[] buf;

	std::string strLibrary("C:\\GestureWorks\\GestureworksCore32.dll");
	std::string strGML("C:\\GestureWorks\\basic_manipulation.gml");

	if(loadGestureWorks("C:\\GestureWorks\\GestureworksCore32.dll")) { 
		OutputDebugStringW(L"Error loading gestureworks dll");
	}
	
	if(!loadGML("C:\\GestureWorks\\basic_manipulation.gml")) {
		OutputDebugStringW(L"Could not find gml file");
	}
	
	initializeGestureWorks(1920,1080);

	if( !registerWindowForTouchByName(  string_window_name ) ) { 
		OutputDebugStringW(L"Could not register target window for touch.");
	}

	registerTouchObject("p3d_window");

	addGesture("p3d_window","n-drag");
	addGesture("p3d_window","n-rotate");
	addGesture("p3d_window","n-scale");
	addGesture("p3d_window","n-flick");
	addGesture("p3d_window","n-hold");
	addGesture("p3d_window","5-tap");
	//addGesture("p3d_window","n-tap");
	

}



void GLWindow::update_gesture() {

	wchar_t buf[2048];

	processFrame();
	
	std::vector<gwc::PointEvent> point_events = consumePointEvents();

	for(std::vector<gwc::PointEvent>::iterator event_it = point_events.begin(); event_it != point_events.end(); event_it++) {
		switch(event_it->status) {
			case gwc::TOUCHADDED:
				//OutputDebugStringW(L"touch added..");
				assignTouchPoint("p3d_window",event_it->point_id);
			
			case gwc::TOUCHUPDATE:
				//OutputDebugStringW(L"touch updated..");
				//If the point is being added, this will place it in our point map; the same line of code will update the point's
				//position if it's already present, so we can use this command to handle new points as well as point updates
				active_points[event_it->point_id] = gwc::Point(event_it->position.getX(),event_it->position.getY());
				break;
			case gwc::TOUCHREMOVED:
				//Remove the point from the map
				active_points.erase(event_it->point_id);
				break;
		}
	}


	//Interpret gesture events
	std::vector<gwc::GestureEvent> gesture_events = consumeGestureEvents();

	//wsprintf(buf,L"Gesture events = (%d) \n",gesture_events.size());
		//		OutputDebugStringW(buf);
	for(std::vector<gwc::GestureEvent>::iterator gesture_it = gesture_events.begin(); gesture_it != gesture_events.end(); gesture_it++) {
	
			if(gesture_it->gesture_id == "n-drag") {
				float dx = gesture_it->values["drag_dx"];
				float dy = gesture_it->values["drag_dy"];

				if(debug){
					swprintf(buf,L"drag = (%f, %f) \n",dx, dy);
					OutputDebugStringW(buf);
				}

				//if (dx < 0)      dx = 1;//Mouse moved to the left
				//else if (dx > 0) dx = -1;//Mouse moved to the right
				//if (dy < 0)      dy = -1;//Mouse moved up
				//else if (dy > 0) dy =  1;//Mouse moved down
				
				dx = (int)(dx+0.5);			//round dx
				dy = (int)(dy+0.5);			//round dy

				getAttachedModel()->TranslateCamera(-dx,dy,0);

			}
			else if(gesture_it->gesture_id == "n-rotate") {
				//Rotation is about a specific point, so we need to do a coordinate transform and adjust
				//not only the object's rotation, but it's x and y values as well
				//float rotation_angle = degreesToRads(gesture_it->values["rotate_dtheta"]);
				float rotation_angle = degreesToRads(gesture_it->values["rotate_dtheta"]);
					
				rotation_angle = (int)(rotation_angle*250);			//round dx
				getAttachedModel()->RotateCamera(rotation_angle,0,0);


				//swprintf(buf,L"rotation_angle = (%f) \n",rotation_angle);
				//(buf,L"(x,y) = (%f, %f) \n", gesture_it->x, gesture_it->y);
				//OutputDebugStringW(buf);
					//If we have points down, move the box; if there are no points, this is from gesture inertia and there is no
					//center about which to rotate
					/*if(gesture_it->n != 0) {
						float temp_x = rotateAboutCenterX( logo1_dimensions.x, logo1_dimensions.y, gesture_it->x, gesture_it->y, rotation_angle);
						float temp_y = rotateAboutCenterY( logo1_dimensions.x, logo1_dimensions.y, gesture_it->x, gesture_it->y, rotation_angle);
					}*/


				//logo1_dimensions.rotation = logo1_dimensions.rotation + rotation_angle;
			}
			else if(gesture_it->gesture_id == "n-scale") {
				float dz = gesture_it->values["scale_dsx"];

				dz = (int)(dz*250);			//round dx
				getAttachedModel()->Zoom(dz);

				if(debug){
					//swprintf(buf,L"zoom = (%f) \n",dz);
					//OutputDebugStringW(buf);
				}
				
			}
			else if(gesture_it->gesture_id == "n-flick") {
				float swipe_dx = gesture_it->values["flick_dx"];
				float swipe_dy = gesture_it->values["flick_dy"];

				if(debug){
					swprintf(buf,L"flick = (%f,%f) \n",swipe_dx,swipe_dy);
				
					OutputDebugStringW(buf);
				}
				
			}
			else if(gesture_it->gesture_id == "n-hold") {

				
				float numberFingers = gesture_it->values["hold_n"];

				if(numberFingers == 10){
					//Run Wireshark
					m_gestureEngine->ProcessTenFingerHold();
				}
				if(numberFingers == 4){
					//Run Wireshark
					m_gestureEngine->ProcessFourFingerHold();
				}

				if(debug){
					swprintf(buf,L"n finger hold = (%f,%f) and n = %f \n", gesture_it->values["hold_x"],gesture_it->values["hold_y"],gesture_it->values["hold_n"]);
					OutputDebugStringW(buf);
				}
			}
			else if(gesture_it->gesture_id == "5-tap") {

				//Add left plane
				m_gestureEngine->ProcessFiveFingerTap();

				if(debug){
					swprintf(buf,L"5 finger tap");
					OutputDebugStringW(buf);
				}
				
			}
			

	}

}

/*****************************************************************************
 GLWindow::degreesToRads

Convert degrees to radians
******************************************************************************/
float GLWindow::degreesToRads(float degrees){
	float pi = 3.141592653;
	float radians = degrees*(pi/180.0);
	return radians;
}





/*****************************************************************************
 GLWindow::StaticWndProc

 Windows message handler
******************************************************************************/
LRESULT CALLBACK GLWindow::StaticWndProc(HWND hWnd, // // Handle of window which received this message
										 UINT uMsg,	//The message 
										 WPARAM wParam, //Pass extra info about the message
										 LPARAM lParam)//Pass extra info about the message
{
    //Initialize the window
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
            //All messages that you don't want to handle should be passed to the DefWindowProc()
			return DefWindowProc(hWnd, uMsg, wParam, lParam);    
        }
    }

    //Call our window's member WndProc (allows us to access member variables)
    return window->WndProc(hWnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK GLWindow::ToolDlgProc(
												HWND hWnd,              //Window Instance
                                                UINT uMsg,              //Message
                                                WPARAM wParam,          //Parameter
                                                LPARAM lParam           //Additional Parameter
                                                )
{

	 GLWindow* pThis = NULL;  //Pointer to the CSkeletonBasics

  //If the Dialog box intitalizes
    if (WM_INITDIALOG == uMsg)      //Sent to the dialog box procedure immediately before a dialog box is displayed.
    {

        //Current State of CSkeletonBasics
        pThis = reinterpret_cast<GLWindow*>(lParam);                 //Allows any lParam pointer to be converted into CSkeletonBasics
        
        //Changes an attribute of the specified window.
        SetWindowLongPtr(
                         hWnd,                                              //A handle to the window 
                         GWLP_USERDATA,                                     //Sets the user data associated with the window. 
                         reinterpret_cast<LONG_PTR>(pThis)                  //The replacement value.
                         );
        //the return value is the previous value of the specified offset.
        //unction also sets a value at the specified offset in the extra window memory

    }
    else
    {
        //The function also retrieves the value of the current class pointer passed through 
        pThis = reinterpret_cast<GLWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));//Retrieves the user data associated with the window.
    
        
    }

    if (pThis)
    {
	  

        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);  //Dialog Process
    }

	return 0;
}

/// <summary>
/// Handle windows messages for the class instance. In particular, the dialog box procedure. 
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK GLWindow::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {




			// setting display mode failed, switch to windowed
            OutputDebugStringW(L"Init Kinect Dialog \n");

			wchar_t szBuff[64];
			wsprintf(szBuff, L"%p", hWnd);
			OutputDebugStringW(szBuff);

            // Bind application window handle
             m_hWnd = hWnd;

			
            // Init Direct2D
            //The ID2D1Factory interface provides the starting point for Direct2D.
            //Creates a factory object that can be used to create Direct2D resources.
            D2D1CreateFactory(
                              D2D1_FACTORY_TYPE_SINGLE_THREADED,            //threading model of the factory and the resources it creates
                              &m_pD2DFactory                                //a factory pointer
                              );

			OutputDebugStringW(L"Created factory... \n");

            // Look for a connected Kinect, and create it if found

            CreateFirstConnected();
        }
        break;

        // If the titlebar X is clicked, destroy app
    case WM_CLOSE:
		{
        DestroyWindow(hWnd);            //Destroys the hWnd window.
		}
		break;

    case WM_DESTROY:
        // Quit the main message pump
        PostQuitMessage(0);
        break;

        // Handle button press
    case WM_COMMAND:
        // If it was for the near mode control and a clicked event, change near mode
        if (IDC_CHECK_SEATED == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
        {
            // Toggle out internal state for near mode
            m_bSeatedMode = !m_bSeatedMode;

            if (NULL != m_pNuiSensor)
            {
                // Set near mode for sensor based on our internal state
                m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, m_bSeatedMode ? NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT : 0);
			}
			else
			{
				MessageBoxW(NULL, L"CreateDialog returned NULL", L"Warning!",  
                MB_OK | MB_ICONINFORMATION);
			}
        }
        break;
    }

    return FALSE;
}



// Function: OnCommand
// Handles WM_COMMAND messages (Menu, toolbar, etc)
void GLWindow::OnCommand(HWND hwnd, int id, HWND hCtl, UINT codeNotify)
{
	switch(id)
	{
		case IDM_FILE_EXIT:                                        
			wglMakeCurrent(m_hdc, NULL); //Make current device context to be null
			wglDeleteContext(m_hglrc);	//Delete the current rendering context 
			m_isRunning = false; //Stop the main loop
		break;
		case IDM_FILE_OPEN:
			 // open a file name
			ZeroMemory( &ofn , sizeof( ofn));
			ofn.lStructSize = sizeof ( ofn );
			ofn.hwndOwner = NULL ;
			ofn.lpstrFile = szFile ;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof( szFile );
			ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex =1;
			ofn.lpstrFileTitle = NULL ;
			ofn.nMaxFileTitle = 0 ;
			ofn.lpstrInitialDir=NULL ;
			ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
			GetOpenFileName( &ofn );
			// Now simpley display the file name
			

			if((char*)ofn.lpstrFile[0] != 0){
				wchar_t  buf[2048];
				wsprintf(buf,L"Loading the pcap file '%s'\n", ofn.lpstrFile);
				OutputDebugStringW(buf);

				//MessageBoxW ( NULL , ofn.lpstrFile , L"File Name" , MB_OK);

				//Clear the opengl screen
				getAttachedModel()->clearScreen();	//Clear the screen


				//getAttachedModel()->loadPcapFile((char*)ofn.lpstrFile);			
				

			}
			else
			{
				wchar_t  buf[2048];
				wsprintf(buf,L"No pcap file was loaded \n");
				OutputDebugStringW(buf);
			}


		break;
		case IDM_HELP_ABOUT:
			AboutDialog* dlg = new AboutDialog();	//Create an instance of a about Dialog
			dlg->Run(m_hinstance, hwnd);			//Show the window
			delete dlg; 				//Delete the instance to prevent memory leaks
			dlg = NULL;	
		break;

	}
}

// Creates the toolbars and statusbar
// Parameters:
// cs - Contains initialization parameters
// Returns:
// void
bool GLWindow::OnCreate(HWND hwnd, LPCREATESTRUCT lpcs)
{
	//Number of buttons in the toolbar
	const int numbuttons1 = 1;

	TBBUTTON buttons[numbuttons1];
	// Create Statusbar
	GLWindow::m_hStatusbar = CreateStatusWindowW(WS_CHILD|WS_VISIBLE, L"Ready", hwnd, IDC_STATUSBAR);
	
	/* Create image list from a custom bitmap */
	HIMAGELIST himlToolbar = ImageList_LoadImage(
	GLWindow::m_hinstance,			/* App instance handle */
	(LPCTSTR)IDB_STOP_ICON,/* ID of your custom bitmap in resources */
	16,					/* Width of each button image within the bitmap */
	1,					/* By how much toolbar can grow. I don't use this */
	RGB(255, 0, 255),	/* Transparent color used in button images. I use magenta */
	IMAGE_BITMAP,
	LR_CREATEDIBSECTION);

	if(!himlToolbar)
	{
		wchar_t  buf[2048];
		wsprintf(buf,L"No toolbar img list was loaded \n");
		OutputDebugStringW(buf);
		return 1;
	}

GLWindow::m_hMainToolbar
	= CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 1, 1, hwnd, NULL, GLWindow::m_hinstance, NULL);

if(!GLWindow::m_hMainToolbar)
{
		wchar_t  buf[2048];
		wsprintf(buf,L"No toolbar was loaded \n");
		OutputDebugStringW(buf);
		return 1;
}

/* Attach image list to the toolbar */

SendMessage(GLWindow::m_hMainToolbar, TB_SETIMAGELIST, 0, (LPARAM)himlToolbar);

/* Set common properties for all buttons */

for(int n = 0; n < numbuttons1; n++)
{
	buttons[n].iBitmap = n;
	buttons[n].fsState = TBSTATE_ENABLED;
	buttons[n].fsStyle = TBSTYLE_BUTTON;
	buttons[n].dwData = 0;
	buttons[n].iString = 0;
}

/* Set properties specific to each button */
buttons[0].idCommand = ID_EDITOR_STOP;


/* Tell toolbar the size of button structure */

SendMessage(GLWindow::m_hMainToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

/* Add 6 buttons to the toolbar that we initialized earlier */

SendMessage(GLWindow::m_hMainToolbar, TB_ADDBUTTONS, (WPARAM)6, (LPARAM)&buttons);

/* Auto-size the toolbar */

SendMessage(GLWindow::m_hMainToolbar, TB_AUTOSIZE, 0, 0);

	/*
		 // Create Main Toolbar
	GLWindow::m_hMainToolbar = CreateWindowEx(
		0, 
		TOOLBARCLASSNAME, 
		NULL,
		WS_CHILD | TBSTYLE_FLAT,
		0, 
		0, 
		0, 
		0,
		hwnd, 
		NULL, 
		GLWindow::m_hinstance, 
		NULL);

	 HIMAGELIST hImageList1 = ImageList_Create(
		16,	
		16, // 16x16 button size
		ILC_COLOR16 | ILC_MASK, // ILC_MASK ensures transparent background
		numbuttons1,
		0);



	 // Set the image list.
	SendMessage(
		GLWindow::m_hMainToolbar, 
		TB_SETIMAGELIST, 
		(WPARAM)0,
		(LPARAM)hImageList1);

	 // Load the button images.
	SendMessage(
		GLWindow::m_hMainToolbar, 
		TB_LOADIMAGES,
		(WPARAM)IDB_STD_SMALL_COLOR, 
		(LPARAM)HINST_COMMCTRL);
	
	//Create an array of buttons
	 TBBUTTON tbButtons1[numbuttons1] = {
		 {MAKELONG(STD_FILENEW, 0), 
			IDM_FILE_NEW, 
			TBSTATE_ENABLED,
			BTNS_AUTOSIZE, 
			{0}, 
			0, 
			0}
	};

	// Add buttons to toolbar
	SendMessage(GLWindow::m_hMainToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(GLWindow::m_hMainToolbar, TB_ADDBUTTONS, (WPARAM)numbuttons1, (LPARAM)&tbButtons1);
 
	// Show toolbar
	SendMessage(GLWindow::m_hMainToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(GLWindow::m_hMainToolbar, TRUE);
	*/

	return true;
}


float GLWindow::getElapsedSeconds()
{
    float currentTime = float(GetTickCount()) / 1000.0f;
    float seconds = float(currentTime - m_lastTime);
    m_lastTime = currentTime;
    return seconds;
}






















/// <summary>
/// Main processing function
/// </summary>
void GLWindow::Update()
{
    if (NULL == m_pNuiSensor)       //If Sensor does not exist
    {
        return;
    }

    // Wait for 0ms, just quickly test if it is time to process a skeleton
    if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0) )
    {
        ProcessSkeleton();          //Process the new skeleton
    }
}





/// <summary>
/// Create the first connected Kinect found 
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT GLWindow::CreateFirstConnected()
{

	OutputDebugStringW(L"Create the first connected Kinect found \n");
    INuiSensor * pNuiSensor;                            //References multiple Kinect sensors. 

    int iSensorCount = 0;
    HRESULT hr = NuiGetSensorCount(&iSensorCount);      //Gets the number of Kinect sensors on the machine, including disconnected ones.
    if (FAILED(hr))
    {
        return hr;
    }

    // Look at each Kinect sensor
    for (int i = 0; i < iSensorCount; ++i)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);        //Return number of sensors in pNuiSensor
        if (FAILED(hr))                                     //Check if there is a failure
        {
            continue;
        }

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();           //Returns S_OK if the device is valid and properly connected.
        if (S_OK == hr)
        {
            m_pNuiSensor = pNuiSensor;          //Set the new sensor as the connected sensor
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }

    if (NULL != m_pNuiSensor)
    {
        // Initialize the Kinect and specify that we'll be using skeleton
        hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);            //Initializes the Kinect and Initialize the sensor to provide skeleton data.
        if (SUCCEEDED(hr))
        {
            // Create an event that will be signaled when skeleton data is available
            m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);               //Creates or opens a named or unnamed event object

            // Open a skeleton stream to receive skeleton data
            hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);      //Enables skeleton tracking.
        }
    }

    if (NULL == m_pNuiSensor || FAILED(hr))
    {
        SetStatusMessage(L"No ready Kinect found!");                //If No sensor, display status in the status bar
        return E_FAIL;
    }

    return hr;
}



/// <summary>
/// Handle new skeleton data
/// </summary>
void GLWindow::ProcessSkeleton()
{
    NUI_SKELETON_FRAME skeletonFrame = {0};                                     //structure that contains the next image frame in the skeleton stream

    HRESULT hr = m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);      //Gets the next frame of data from the skeleton stream.
    if ( FAILED(hr) )
    {
        return;
    }

    // smooth out the skeleton data
    m_pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);                     //Filters skeleton positions to reduce jitter between frames.

    // Ensure Direct2D is ready to draw
    // Sets color for the bone and joint
    hr = EnsureDirect2DResources( );
    if ( FAILED(hr) )
    {
        return;
    }

    //
    m_pRenderTarget->BeginDraw();                                   //Initiates drawing on this render target.
    m_pRenderTarget->Clear( );                                      //Clears the drawing area to the specified color - Black. 

    RECT rct;                                                       //Rectangle Structure
    GetClientRect( GetDlgItem( m_hWnd, IDC_VIDEOVIEW ), &rct);      //Get Dialog box dimensions
    int width = rct.right;                                          //Dialog box width
    int height = rct.bottom;                                        //Dialog box width

    for (int i = 0 ; i < NUI_SKELETON_COUNT; ++i)                   //number of tracked skeletons 
    {
        NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;   //Options for tracking skeleton data.

        if (NUI_SKELETON_TRACKED == trackingState)                              //All joint positions are being tracked.
        {
            // We're tracking the skeleton, draw it
            DrawSkeleton(skeletonFrame.SkeletonData[i], width, height);         // Draws a skeleton
        }
        else if (NUI_SKELETON_POSITION_ONLY == trackingState)                   //The overall position is being tracked but not the individual joint positions.
        {
            // we've only received the center point of the skeleton, draw that
            D2D1_ELLIPSE ellipse = D2D1::Ellipse(
                SkeletonToScreen(skeletonFrame.SkeletonData[i].Position, width, height),    //center point of the ellipse
                g_JointThickness,                                                           //radiusX
                g_JointThickness                                                            //radiusY
                );

            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointTracked);                    //Draws the outline of an ellipse with the specified dimensions and stroke.
        }
    }

    hr = m_pRenderTarget->EndDraw();                                                        //Ends drawing operations on the render target 

    // Device lost, need to recreate the render target
    // We'll dispose it now and retry drawing
    if (D2DERR_RECREATE_TARGET == hr)
    {
        hr = S_OK;                              //Set error code to S_OK
        DiscardDirect2DResources();             // discard a resource by simply release the interface for that resource
    }
}

/// <summary>
/// Draws a skeleton
/// </summary>
/// <param name="skel">skeleton to draw</param>
/// <param name="windowWidth">width (in pixels) of output buffer</param>
/// <param name="windowHeight">height (in pixels) of output buffer</param>
void GLWindow::DrawSkeleton(const NUI_SKELETON_DATA & skel, int windowWidth, int windowHeight)
{      
    int i;

    for (i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i)
    {
        m_Points[i] = SkeletonToScreen(skel.SkeletonPositions[i], windowWidth, windowHeight);           //Screen Points from  single joint position (x, y, z) for the skeleton.
    }

    // Render Torso
    DrawBone(skel, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
    DrawBone(skel, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);


		headMovement(m_Points[NUI_SKELETON_POSITION_HEAD]);

    // Left Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);
	
	leftHandMovement(m_Points[NUI_SKELETON_POSITION_HAND_LEFT]);
	
	

    // Right Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

	rightHandMovement(m_Points[NUI_SKELETON_POSITION_HAND_RIGHT]);

    // Left Leg
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

    // Right Leg
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

    // Draw the joints in a different color
    for (i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i)
    {
        D2D1_ELLIPSE ellipse = D2D1::Ellipse( m_Points[i], g_JointThickness, g_JointThickness );

        if ( skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_INFERRED )
        {
            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointInferred);
        }
        else if ( skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_TRACKED )
        {
            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointTracked);            //Brush for the tracked joint
        }
    }
}


/// <summary>
/// Shift the left hand
/// </summary>
/// <param name="skeletonPoint">skeleton point to tranform</param>
/// <param name="width">width (in pixels) of output buffer</param>
/// <param name="height">height (in pixels) of output buffer</param>
/// <returns>point in screen-space</returns>
void GLWindow::leftHandMovement(D2D1_POINT_2F leftHandPoint){
	
	//Translate left
	if(leftHandPoint.x > m_left_Hand_Point_old.x){
		getAttachedModel()->TranslateCamera(-1,0,0);		// Translate camera left
	}

	//Translate right
	if(leftHandPoint.x<m_left_Hand_Point_old.x){
		getAttachedModel()->TranslateCamera(1,0,0);		// Translate camera right
	}

		//Translate left
	if(leftHandPoint.y > m_left_Hand_Point_old.y){
		getAttachedModel()->TranslateCamera(0,1,0);		// Translate camera up
	}

	//Translate right
	if(leftHandPoint.y<m_left_Hand_Point_old.y){
		getAttachedModel()->TranslateCamera(0,-1,0);		// Translate camera up
	}
	
	m_left_Hand_Point_old = leftHandPoint;



}



void GLWindow::rightHandMovement(D2D1_POINT_2F rightHandPoint){
	

	//Translate right
	/*if(rightHandPoint.x > m_right_Hand_Point_old.x){
		getAttachedModel()->Zoom(-1);		// Translate camera left
	}

	//Translate right
	if(rightHandPoint.x<m_right_Hand_Point_old.x){
		getAttachedModel()->TranslateCamera(1,0,0);		// Translate camera right
	}*/

		//Translate left
	if(rightHandPoint.y > m_right_Hand_Point_old.y){
		getAttachedModel()->Zoom(-1);		// Translate camera up

	}

	//Translate right
	if(rightHandPoint.y<m_right_Hand_Point_old.y){
		getAttachedModel()->Zoom(1);		// Translate camera up
	}
	
	m_right_Hand_Point_old = rightHandPoint;



}


/// <summary>
/// Shift the left hand
/// </summary>
/// <param name="skeletonPoint">skeleton point to tranform</param>
/// <param name="width">width (in pixels) of output buffer</param>
/// <param name="height">height (in pixels) of output buffer</param>
/// <returns>point in screen-space</returns>
void GLWindow::headMovement(D2D1_POINT_2F headPoint){
	
	//Translate left
	if(headPoint.x > m_head_Point_old.x){
		getAttachedModel()->RotateCamera(-5,0,0);		// Rotate camera left
	}

	//Translate right
	if(headPoint.x<m_head_Point_old.x){
		getAttachedModel()->RotateCamera(5,0,0);
	}

		//Translate left
	if(headPoint.y > m_head_Point_old.y){
		getAttachedModel()->RotateCamera(0,5,0);
	}

	//Translate right
	if(headPoint.y<m_head_Point_old.y){
		getAttachedModel()->RotateCamera(0,-5,0);
	}
	
	m_head_Point_old = headPoint;



}




/// <summary>
/// Draws a bone line between two joints
/// </summary>
/// <param name="skel">skeleton to draw bones from</param>
/// <param name="joint0">joint to start drawing from</param>
/// <param name="joint1">joint to end drawing at</param>
void GLWindow::DrawBone(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1)
{
    NUI_SKELETON_POSITION_TRACKING_STATE joint0State = skel.eSkeletonPositionTrackingState[joint0];
    NUI_SKELETON_POSITION_TRACKING_STATE joint1State = skel.eSkeletonPositionTrackingState[joint1];

    // If we can't find either of these joints, exit
    if (joint0State == NUI_SKELETON_POSITION_NOT_TRACKED || joint1State == NUI_SKELETON_POSITION_NOT_TRACKED)
    {
        return;
    }

    // Don't draw if both points are inferred from previous frame data, 
    if (joint0State == NUI_SKELETON_POSITION_INFERRED && joint1State == NUI_SKELETON_POSITION_INFERRED)     
    {
        return;
    }

    // We assume all drawn bones are inferred unless BOTH joints are tracked based on the current frame data.
    if (joint0State == NUI_SKELETON_POSITION_TRACKED && joint1State == NUI_SKELETON_POSITION_TRACKED)
    {
        m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneTracked, g_TrackedBoneThickness);
    }
    else
    {
        m_pRenderTarget->DrawLine(m_Points[joint0], m_Points[joint1], m_pBrushBoneInferred, g_InferredBoneThickness);
    }
}

/// <summary>
/// Converts a skeleton point to screen space
/// </summary>
/// <param name="skeletonPoint">skeleton point to tranform</param>
/// <param name="width">width (in pixels) of output buffer</param>
/// <param name="height">height (in pixels) of output buffer</param>
/// <returns>point in screen-space</returns>
D2D1_POINT_2F GLWindow::SkeletonToScreen(Vector4 skeletonPoint, int width, int height)
{
    LONG x, y;
    USHORT depth;

    // Calculate the skeleton's position on the screen
    // NuiTransformSkeletonToDepthImage returns coordinates in NUI_IMAGE_RESOLUTION_320x240 space
    NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);

    float screenPointX = static_cast<float>(x * width) / cScreenWidth;          //Create Screen X Point
    float screenPointY = static_cast<float>(y * height) / cScreenHeight;        //Create Screen Y Point

    return D2D1::Point2F(screenPointX, screenPointY);                           //Direct 2D point
}

/// <summary>
/// Ensure necessary Direct2d resources are created
/// </summary>
/// <returns>S_OK if successful, otherwise an error code</returns>
HRESULT GLWindow::EnsureDirect2DResources()
{
    HRESULT hr = S_OK;

    // If there isn't currently a render target, we need to create one
    if (NULL == m_pRenderTarget)
    {
        RECT rc;                                                        //rectangle structure
        GetWindowRect( GetDlgItem( m_hWnd, IDC_VIDEOVIEW ), &rc );      //Stores the dimensions of the bounding rectangle of the specified window to rc

        int width = rc.right - rc.left;                                 //Width of dialog window
        int height = rc.bottom - rc.top;                                //height of dialog window
        D2D1_SIZE_U size = D2D1::SizeU( width, height );                //Stores an ordered pair of width and height of a rectangle.
        D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(); //Contains rendering options (hardware or software), pixel format, DPI information, remoting options, and Direct3D support requirements for a render target.
        rtProps.pixelFormat = D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);   //Set pixel format 8 bits for each color channel, 8-bit alpha, and blending
        rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;        //The render target can be used efficiently with GDI.

        // Create a Hwnd render target, in order to render to the window set in initialize
        hr = m_pD2DFactory->CreateHwndRenderTarget(
            rtProps,                                                    //The rendering mode, pixel format, remoting options, DPI information,
            D2D1::HwndRenderTargetProperties(GetDlgItem( m_hWnd, IDC_VIDEOVIEW), size), //The window handle, initial size (in pixels), and present options.
            &m_pRenderTarget                                            //ddress of the pointer to the ID2D1HwndRenderTarget object created by this method.
            );
        if ( FAILED(hr) )
        {
            SetStatusMessage(L"Couldn't create Direct2D render target!");
            return hr;
        }

        //light green
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.27f, 0.75f, 0.27f), &m_pBrushJointTracked);           //Brush for the tracked joint

        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &m_pBrushJointInferred);   //Joint
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &m_pBrushBoneTracked);      //Bone
        m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &m_pBrushBoneInferred);      //Inferred bone
    }

    return hr;
}

/// <summary>
/// Dispose Direct2d resources 
/// </summary>
void GLWindow::DiscardDirect2DResources( )
{
    SafeRelease(m_pRenderTarget);                   //release the rendering target
    SafeRelease(m_pBrushJointTracked);              //release the tracked brush joint
    SafeRelease(m_pBrushJointInferred);             //release the inferred brush joint
    SafeRelease(m_pBrushBoneTracked);               //release the tracked brush bone
    SafeRelease(m_pBrushBoneInferred);              //release the inferred brush bone
}

/// <summary>
/// Set the status bar message
/// </summary>
/// <param name="szMessage">message to display</param>
void GLWindow::SetStatusMessage(WCHAR * szMessage)
{
    SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, (LPARAM)szMessage);
}