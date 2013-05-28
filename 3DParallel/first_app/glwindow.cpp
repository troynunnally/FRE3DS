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
	//debug = false;					//Debug is false
	debug = true;
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
    m_windowClass.lpszClassName   = "GLClass";						  //Name of this class
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
        "3D Parallel Coordinates",				// Window Title
        dwStyle | WS_CLIPCHILDREN |				//Window Style
        WS_CLIPSIBLINGS,
        0, 0,									// x,y position of window
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
    MSG msg;	//The message from OS to application

    //While there are messages in the queue, store them in msg
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        //Process the messages one-by-one
        TranslateMessage(&msg); // translates virtual key messages to character messages.
        DispatchMessage(&msg); // dispatches the message to a window procedure.
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
			OutputDebugString("Middle Button Pressed. \n");
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
	//Handles MENU presses adn set the callback
	case WM_COMMAND:
		HANDLE_WM_COMMAND(hWnd, wParam, lParam, OnCommand);
    default:
        break;
    }
	//All messages that you don't want to handle should be passed to the DefWindowProc()
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
		getAttachedModel()->Zoom(1);	//Zoom
       
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
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex =1;
			ofn.lpstrFileTitle = NULL ;
			ofn.nMaxFileTitle = 0 ;
			ofn.lpstrInitialDir=NULL ;
			ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
			GetOpenFileName( &ofn );
			// Now simpley display the file name
			MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
			//Clear the opengl screen
			getAttachedModel()->clearScreen();	//Clear the screen
			
			char buf[2048];
			sprintf(buf,"Loading the pcap file %s\n", ofn.lpstrFile);
			OutputDebugString(buf);
			getAttachedModel()->loadPcapFile(ofn.lpstrFile);

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
	const int numbuttons1 = 4;

	// Create Statusbar
	GLWindow::m_hStatusbar = CreateStatusWindow(WS_CHILD|WS_VISIBLE, "Ready", hwnd, IDC_STATUSBAR);
	
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
			0},
		{MAKELONG(STD_FILEOPEN, 0), 
			IDM_FILE_OPEN, 
			TBSTATE_ENABLED,
			BTNS_AUTOSIZE, 
			{0}, 
			0, 
			0},
		{MAKELONG(STD_FILESAVE, 0), 
			IDM_FILE_SAVE, 
			TBSTATE_ENABLED,
			BTNS_AUTOSIZE, 
			{0}, 
			0, 
			0},
		{MAKELONG(STD_FILESAVE, 0), 
			0, 
			TBSTATE_ENABLED,
			TBSTYLE_SEP, 
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

	return true;
}


float GLWindow::getElapsedSeconds()
{
    float currentTime = float(GetTickCount()) / 1000.0f;
    float seconds = float(currentTime - m_lastTime);
    m_lastTime = currentTime;
    return seconds;
}
