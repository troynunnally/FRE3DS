#ifndef _GLWINDOW_H
#define _GLWINDOW_H

#include <windows.h>
#include <ctime>

//OPENFILENAME structure is located
#include <Commdlg.h>

#include "stdafx.h"
#include <strsafe.h>
#include "resource.h"
#include "NuiApi.h"
//#include "CSkeletonBasics.h"

const int eventCount = 1;

class ModelGL; //Declare our ModelGL class
class Db;
class GLWindow 
{

	static const int        cScreenWidth  = 320;
    static const int        cScreenHeight = 240;
    static const int        cStatusMessageMaxLen = MAX_PATH*2;


public:
    GLWindow(HINSTANCE hInstance); //default constructor

    bool create(int width, int height, int bpp, bool fullscreen);
    void destroy();
    void processEvents();
    void attachModel(ModelGL* model);
	void attachDatabase(Db* database);
    
	bool createKinectWindow(HINSTANCE hInstance);									//Create the Kinect Window
	//CSkeletonBasics* m_Skeleton;
    HANDLE hEvents[eventCount];                     //Array of handle events

    bool isRunning(); //Is the window running?

    void swapBuffers() { SwapBuffers(m_hdc); }

    static LRESULT CALLBACK StaticWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);		//Window Procedure for GLClass
	static LRESULT CALLBACK ToolDlgProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);		//Window Procedure for Kinect

    LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//Handles the command handling when the menu is pressed
    void OnCommand(HWND hwnd, int id, HWND hCtl, UINT codeNotify);
    float getElapsedSeconds();


	    /// <summary>
    /// Handle windows messages for a class instance
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>LRESULT is result of message processing</returns>
    LRESULT CALLBACK        DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	 /// <summary>
    /// Initialize
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="nCmdShow"></param>
	///int  Init();

    /// <summary>
    /// Creates the main window and begins processing
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="nCmdShow"></param>
    int                     Run();

	/// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();
	
	HWND                    m_hWnd;             //Window Handle




private:
    ModelGL* m_model;				//A link to the ModelGL program which is a OpenGL Program
	Db* m_db;	
    bool m_isRunning;				//Is the window still running?
    bool m_isFullscreen; 

	//
    HWND m_hwnd;					//Window handle


	HWND m_hWndKinect;
	RECT m_kWindowRect;				//Kinect Window bounds
	WNDCLASS m_kwindowClass;		//Window Class

    HGLRC m_hglrc;					//Rendering context
    HDC m_hdc;						//Device context
    RECT m_windowRect;				//Window bounds
    HINSTANCE m_hinstance;			//Application instance
    WNDCLASSEX m_windowClass;		//Window Class
    void setupPixelFormat(void);	//Setup Pixel Format
	bool OnCreate(HWND hwnd, LPCREATESTRUCT lpcs); //Callback when window is created

	//UI Elements
	 HWND m_hStatusbar;				//Status Bar window
	 HWND m_hMainToolbar;			//Main Toolbar window
	 HWND m_hPaintToolbar;			//Paint toolbar window
	OPENFILENAME ofn ;				//Open filename structure

	// a another memory buffer to contain the file name
	wchar_t szFile[100] ;

    ModelGL* getAttachedModel() { return m_model; }  //Returns the instance of the ModelGL PpenGL class

    float m_lastTime;

	//Interactions
	void HandleKeyPress(UINT wParam);
	
	//Mouse Interactions
	//POINT Structure - Represents an ordered pair of integer x- and y-coordinates that defines a point in a two-dimensional plane.
	POINT g_OrigCursorPos;			
	
	//Location of the Mouse cursor
	POINT g_LastCursorPos;			//Last Location of the Mouse cursor
	POINT g_OrigWndPos;				//Location fot the application window
	
	int m_rotate_value_x;
	int m_rotate_value_y;
	int m_rotate_value_z;
	int m_zoom_value;
	int session_id;

	bool g_LeftButtonPressed;
	bool g_MiddleButtonPressed;

	bool debug;







	    bool                    m_bSeatedMode;      //Seated Mode Boolean Value
    
	//HANDLE hEvents[1];                     //Array of handle events
	//int eventCount;				//Event Count
    // Current Kinect
    INuiSensor*             m_pNuiSensor;       //References multiple Kinect sensors
    

    // Skeletal drawing
    ID2D1HwndRenderTarget*   m_pRenderTarget;
    ID2D1SolidColorBrush*    m_pBrushJointTracked;      //Brush for the tracked joint
    ID2D1SolidColorBrush*    m_pBrushJointInferred;
    ID2D1SolidColorBrush*    m_pBrushBoneTracked;
    ID2D1SolidColorBrush*    m_pBrushBoneInferred;
    D2D1_POINT_2F            m_Points[NUI_SKELETON_POSITION_COUNT];
	
	D2D1_POINT_2F             m_left_Hand_Point_old;
	D2D1_POINT_2F             m_right_Hand_Point_old;
	D2D1_POINT_2F             m_head_Point_old;

    // Direct2D
    ID2D1Factory*           m_pD2DFactory;                  // a factory pointer - a factory pointer to is used to render a Direct 2D object
    
    HANDLE                  m_pSkeletonStreamHandle;        //Handle to a Skeleton Stream
    HANDLE                  m_hNextSkeletonEvent;           //Handle to the Next Skeleton Event
    


    /// <summary>
    /// Create the first connected Kinect found 
    /// </summary>
    /// <returns>S_OK on success, otherwise failure code</returns>
    HRESULT                 CreateFirstConnected();

    /// <summary>
    /// Handle new skeleton data
    /// </summary>
    void                    ProcessSkeleton();

    /// <summary>
    /// Ensure necessary Direct2d resources are created
    /// </summary>
    /// <returns>S_OK if successful, otherwise an error code</returns>
    HRESULT                 EnsureDirect2DResources( );

    /// <summary>
    /// Dispose Direct2d resources 
    /// </summary>
    void                    DiscardDirect2DResources( );

    /// <summary>
    /// Draws a bone line between two joints
    /// </summary>
    /// <param name="skel">skeleton to draw bones from</param>
    /// <param name="joint0">joint to start drawing from</param>
    /// <param name="joint1">joint to end drawing at</param>
    void                    DrawBone(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX bone0, NUI_SKELETON_POSITION_INDEX bone1);

    /// <summary>
    /// Draws a skeleton
    /// </summary>
    /// <param name="skel">skeleton to draw</param>
    /// <param name="windowWidth">width (in pixels) of output buffer</param>
    /// <param name="windowHeight">height (in pixels) of output buffer</param>
    void                    DrawSkeleton(const NUI_SKELETON_DATA & skel, int windowWidth, int windowHeight);

    /// <summary>
    /// Converts a skeleton point to screen space
    /// </summary>
    /// <param name="skeletonPoint">skeleton point to tranform</param>
    /// <param name="width">width (in pixels) of output buffer</param>
    /// <param name="height">height (in pixels) of output buffer</param>
    /// <returns>point in screen-space</returns>
    D2D1_POINT_2F           SkeletonToScreen(Vector4 skeletonPoint, int width, int height);

	/// <summary>
    /// Shift the left hand
    /// </summary>
    /// <param name="skeletonPoint">skeleton point to tranform</param>
    /// <param name="width">width (in pixels) of output buffer</param>
    /// <param name="height">height (in pixels) of output buffer</param>
    /// <returns>point in screen-space</returns>
    void leftHandMovement(D2D1_POINT_2F leftHandPoint);
	void rightHandMovement(D2D1_POINT_2F rightHandPoint);
	void headMovement(D2D1_POINT_2F headPoint);


    /// <summary>
    /// Set the status bar message
    /// </summary>
    /// <param name="szMessage">message to display</param>
    void                    SetStatusMessage(WCHAR* szMessage);




};

#endif