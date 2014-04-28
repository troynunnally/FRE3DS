//------------------------------------------------------------------------------
// <copyright file="SkeletonBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

//#include "stdafx.h"
//#include <strsafe.h>
#include "CSkeletonBasics.h"
#include "resource.h"

static const float g_JointThickness = 3.0f;
static const float g_TrackedBoneThickness = 6.0f;
static const float g_InferredBoneThickness = 1.0f;


/// <summary>
/// Constructor
/// </summary>
int CSkeletonBasics::Init() 

{
  // eventCount = 1;								//Event Count
	m_pD2DFactory == 0;
    m_hNextSkeletonEvent == INVALID_HANDLE_VALUE;
    m_pSkeletonStreamHandle == INVALID_HANDLE_VALUE;
    m_bSeatedMode == false;
    m_pRenderTarget == NULL;

	/*
    m_pBrushJointTracked(NULL),                 //Brush for the tracked joint
    m_pBrushJointInferred(NULL),
    m_pBrushBoneTracked(NULL),
    m_pBrushBoneInferred(NULL),
    m_pNuiSensor(NULL)*/
//	ZeroMemory(m_Points,sizeof(m_Points));      //Fills a block of memory, m_Points, with zeros.

	return 0;
}

/// <summary>
/// Constructor
/// </summary>
CSkeletonBasics::CSkeletonBasics() :
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
}

/// <summary>
/// Destructor
/// </summary>
CSkeletonBasics::~CSkeletonBasics()
{
    if (m_pNuiSensor)                           //References multiple Kinect sensors
    {
        m_pNuiSensor->NuiShutdown();            //Turns the sensor off. No action is taken if the sensor is already off.
    }

    if (m_hNextSkeletonEvent && (m_hNextSkeletonEvent != INVALID_HANDLE_VALUE))
    {
        CloseHandle(m_hNextSkeletonEvent);
    }

    // clean up Direct2D objects
    DiscardDirect2DResources();

    // clean up Direct2D
    SafeRelease(m_pD2DFactory);

    SafeRelease(m_pNuiSensor);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int CSkeletonBasics::Run()
{
   // MSG       msg = {0};                                //Windows message
   /* WNDCLASS  wc  = {0};                                //Windows class structure

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
        return 0;
    }
	*/
    // Create main application window
   /* HWND hWndApp = CreateDialogParamW(
        hInstance,                                  //handle to the application instance
        MAKEINTRESOURCE(IDD_APP),                   //an integer value that specifies the resource identifier 
        hWndMain,                                   //A handle to the window that owns the dialog box.
        (DLGPROC)CSkeletonBasics::MessageRouter,    //Casted Dialog Box Procedure
        reinterpret_cast<LPARAM>(this));            //A pointer to the dialog box procedure - this Skeleton class 

    // Show window
    ShowWindow(hWndApp, nCmdShow);                  //Show Window


	*/
    // Main message loop

    //while (WM_QUIT != msg.message)
    //{
	    const int eventCount = 1;
		HANDLE hEvents[eventCount];                     //Array of handle events   
		hEvents[0] = m_hNextSkeletonEvent;          //Next Skeleton Events

        // Check to see if we have either a message (by passing in QS_ALLEVENTS)
        // Or a Kinect event (hEvents)
        // Update() will check for Kinect events individually, in case more than one are signalled
        // the return value indicates the event that caused the function to return
   /*     DWORD dwEvent = MsgWaitForMultipleObjects(eventCount,           //number of object handles in the array pointed to by hEvents
                                                  hEvents,              //An array of object handles.
                                                  FALSE,                //returns when the state of any one of the objects is set to signaled
                                                  INFINITE,             //return only when the specified objects are signaled
                                                  QS_ALLINPUT           //Any message is in the queue.
                                                  );
												  
        // Check if this is an event we're waiting on (Kinect Event) and not a timeout or message
        if (WAIT_OBJECT_0 == dwEvent)               //The state of the specified object as a Kinect Signal
        {
            Update();                               // Main processing function
        }
		*/
  
		/*
        if (PeekMessageW(&msg,          //Pointer to a MSG structure that receives message data
                         NULL,          //Handle to the window - method obtains messages for any window that belongs to the calling thread
                         0,             //Specifies the lowest message value to obtain. - 0 no min
                         0,             ////Specifies the higest message value to obtain. - 0 no max
                         PM_REMOVE      //Messages are removed from the queue after processing
                         ))
        {
            // If a dialog message will be taken care of by the dialog proc
            if ((hWndApp != NULL) && IsDialogMessageW(hWndApp, &msg)) //Determines whether a message is intended for the specified dialog box
            {
                continue;
            }

            TranslateMessage(&msg);     //Translates virtual-key messages into character messages and posted to the calling thread's message queue
            DispatchMessageW(&msg);     //spatches a message to a window procedure.
        }
    }
	*/
    return 0;
	
}

/// <summary>
/// Main processing function
/// </summary>
void CSkeletonBasics::Update()
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
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CSkeletonBasics::MessageRouter(
                                                HWND hWnd,              //Window Instance
                                                UINT uMsg,              //Message
                                                WPARAM wParam,          //Parameter
                                                LPARAM lParam           //Additional Parameter
                                                )
{
    CSkeletonBasics* pThis = NULL;  //Pointer to the CSkeletonBasics

    //If the Dialog box intitalizes
    if (WM_INITDIALOG == uMsg)      //Sent to the dialog box procedure immediately before a dialog box is displayed.
    {
        //Current State of CSkeletonBasics
        pThis = reinterpret_cast<CSkeletonBasics*>(lParam);                 //Allows any lParam pointer to be converted into CSkeletonBasics
        
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
        pThis = reinterpret_cast<CSkeletonBasics*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));//Retrieves the user data associated with the window.
    
        
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
LRESULT CALLBACK CSkeletonBasics::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

/// <summary>
/// Create the first connected Kinect found 
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CSkeletonBasics::CreateFirstConnected()
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
void CSkeletonBasics::ProcessSkeleton()
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
void CSkeletonBasics::DrawSkeleton(const NUI_SKELETON_DATA & skel, int windowWidth, int windowHeight)
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

    // Left Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

    // Right Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

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
/// Draws a bone line between two joints
/// </summary>
/// <param name="skel">skeleton to draw bones from</param>
/// <param name="joint0">joint to start drawing from</param>
/// <param name="joint1">joint to end drawing at</param>
void CSkeletonBasics::DrawBone(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1)
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
D2D1_POINT_2F CSkeletonBasics::SkeletonToScreen(Vector4 skeletonPoint, int width, int height)
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
HRESULT CSkeletonBasics::EnsureDirect2DResources()
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
void CSkeletonBasics::DiscardDirect2DResources( )
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
void CSkeletonBasics::SetStatusMessage(WCHAR * szMessage)
{
    SendDlgItemMessageW(m_hWnd, IDC_STATUS, WM_SETTEXT, 0, (LPARAM)szMessage);
}