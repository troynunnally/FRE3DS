// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// GestureEngine.cpp: implementation of GestureEngine class

// C RunTime header files
#include <assert.h>
#include <process.h>				//for the system command
#define ASSERT assert

#define _USE_MATH_DEFINES // has to be defined to activate definition of M_PI
#include <math.h>

#include "GestureEngine.h" // contains definition of this class

// Default constructor of the class.
GestureEngine::GestureEngine(ModelGL* model, Db* database)
: //  _dwArguments(0),
	_model(model),					// Initialize model to NULL
	_db(database),					// Init Db
	debug(true), 
	session_id(1)					// Init Session ID value
{

	m_rotate_value_x	= 0;			// Init Rotate X value
	m_rotate_value_y	= 0;			// Init Rotate Y value
	m_rotate_value_z	= 0;			// Init Rotate Z value
	m_zoom_value		= 0;			// Init Zoom value
	m_translate_value_x = 0;			// Init Translate X value
	m_translate_value_y	= 0;			// Init Translate Y value				

}

// Destructor
GestureEngine::~GestureEngine()
{
}

// Main function of this class decodes gesture information
// in:
//      hWnd        window handle
//      wParam      message parameter (message-specific)
//      lParam      message parameter (message-specific)
LRESULT GestureEngine::WndProc(HWND hWnd, WPARAM /* wParam */, LPARAM lParam)
{
				
	return TRUE;
}

// Press and tap command
void GestureEngine::ProcessPressAndTap()
{
		
	OutputDebugStringW(L"ProcessPressAndTap");
}

// Two finger tap command
void GestureEngine::ProcessFiveFingerTap()
{
	_model->AddLeftPlane(1);			//Add left plane
	_model->AddAvgYTotalPacketsZ(1);	//Total Avg Packets Y  vs. TotalPackets Z  left plane
}

// Zoom command
// in: 
//      dZoomFactor - scaling factor of zoom in/out
//      lZx         - x-coordinate of zoom center
//      lZy         - y-coordinate of zoom center
void GestureEngine::ProcessZoom(POINT g_CurrentCursorPos)
{
	
	int dy;
				
	dy = g_CurrentCursorPos.y - g_LastCursorPos.y;		//Change in y direction

	if (dy < 0)      dy = -1;	//Mouse moved up
	else if (dy > 0) dy =  1;	//Mouse moved down
				
	_model->Zoom(-dy);			//perform Zoom in OpenGL

	m_zoom_value	= m_zoom_value+dy;
				
	//Set Last Cursor Position of the mouse
	g_LastCursorPos.x=g_CurrentCursorPos.x;
	g_LastCursorPos.y=g_CurrentCursorPos.y;

	if (debug){
		wchar_t buf[2048];
		wsprintf(buf,L"g_OrigCursorPos = (%d, %d) \n",g_LastCursorPos.x, g_LastCursorPos.y);
		OutputDebugStringW(buf);

		wsprintf(buf,L"MovedCursorPos = (%d, %d) \n",g_CurrentCursorPos.x, g_CurrentCursorPos.y);
		OutputDebugStringW(buf);
	}



}


void GestureEngine::ProcessTenFingerHold()
{
		system("wireshark.exe -r pcap/ftp_disguise.pcap");
}



void GestureEngine::ProcessFourFingerHold()
{
		system("wireshark.exe -r pcap/ftp_disguise.pcap -d \"ip.src==57.25.6.30&&tcp.srcport>50331&&tcp.srcport<50342\"");
}



// Translate command
// in:
//      dAngle  - angle of rotation
//      g_CurrentCursorPos     - x-coordinate of the center of rotation
//      g_LastCursorPos     - y-coordinate of the center of rotation
void GestureEngine::ProcessTranslate(POINT g_CurrentCursorPos)
{
     OutputDebugStringW(L"ProcessTranslate");
	 
	 int dx,dy;
				
	dx = g_CurrentCursorPos.x  - g_LastCursorPos.x;		//Change in x direction
	dy = g_CurrentCursorPos.y - g_LastCursorPos.y;		//Change in y direction

	if (dx < 0)      dx = 1;	//Mouse moved to the left
	else if (dx > 0) dx =  -1;	//Mouse moved to the right
	if (dy < 0)      dy = -1;	//Mouse moved up
	else if (dy > 0) dy =  1;	//Mouse moved down
		
	//dx = dx/2000;
	//dy = dy/2000;
	_model->TranslateCamera(dx,dy,0);			//perform Translate in OpenGL

	m_translate_value_x	= m_translate_value_x+dx;	//
	m_translate_value_y	= m_translate_value_y+dy;	//
				

	//Set Last Cursor Position of the mouse
	g_LastCursorPos.x=g_CurrentCursorPos.x;
	g_LastCursorPos.y=g_CurrentCursorPos.y;

	if (debug){
		wchar_t buf[2048];
		wsprintf(buf,L"dx, dy = (%d,%d) \n",dx,dy);
		OutputDebugStringW(buf);

		//wsprintf(buf,L"MovedCursorPos = (%d, %d) \n",g_CurrentCursorPos.x, g_CurrentCursorPos.y);
		//OutputDebugStringW(buf);
	}




}



// Translate command for mouse
// in:
//      dAngle  - angle of rotation
//      g_CurrentCursorPos     - x-coordinate of the center of rotation
//      g_LastCursorPos     - y-coordinate of the center of rotation

void GestureEngine::ProcessMouseTranslate(POINT g_CurrentCursorPos)
{
     OutputDebugStringW(L"ProcessTranslate");
	 
	 int dx,dy;
				
	dx = g_CurrentCursorPos.x  - g_LastCursorPos.x;		//Change in x direction
	dy = g_CurrentCursorPos.y - g_LastCursorPos.y;		//Change in y direction

	if (dx < 0)      dx = 10;	//Mouse moved to the left
	else if (dx > 0) dx =  -10;	//Mouse moved to the right
	if (dy < 0)      dy = -10;	//Mouse moved up
	else if (dy > 0) dy =  10;	//Mouse moved down
		
	//dx = dx/2000;
	//dy = dy/2000;
	_model->TranslateCamera(dx,dy,0);			//perform Translate in OpenGL

	m_translate_value_x	= m_translate_value_x+dx;	//
	m_translate_value_y	= m_translate_value_y+dy;	//
				

	//Set Last Cursor Position of the mouse
	g_LastCursorPos.x=g_CurrentCursorPos.x;
	g_LastCursorPos.y=g_CurrentCursorPos.y;

	if (debug){
		wchar_t buf[2048];
		wsprintf(buf,L"dx, dy = (%d,%d) \n",dx,dy);
		OutputDebugStringW(buf);

		//wsprintf(buf,L"MovedCursorPos = (%d, %d) \n",g_CurrentCursorPos.x, g_CurrentCursorPos.y);
		//OutputDebugStringW(buf);
	}




}





// Pan/Inertia command
// in:
//      ldx - increment/decrement in x direction
//      ldy - increment/decrement in y direction
void GestureEngine::ProcessMove(const LONG ldx, const LONG ldy)
{
   OutputDebugStringW(L"ProcessMove");
}



// Rotate command for mouse
// in:
//      dAngle  - angle of rotation
//      lOx     - x-coordinate of the center of rotation
//      lOy     - y-coordinate of the center of rotation
void GestureEngine::ProcessMouseRotate(POINT g_CurrentCursorPos)
{
     OutputDebugStringW(L"ProcessRotate");
	 int dx,dy;
				
	dx = g_CurrentCursorPos.x - g_LastCursorPos.x;
	dy = g_CurrentCursorPos.y - g_LastCursorPos.y;

	if (dx < 0)      dx = -1;//Mouse moved to the left
	else if (dx > 0) dx =  1;//Mouse moved to the right
	if (dy < 0)      dy = -1;//Mouse moved up
	else if (dy > 0) dy =  1;//Mouse moved down
	
	_model->RotateCamera(-dx,dy,0);

	m_rotate_value_x	= m_rotate_value_x+dx;
	m_rotate_value_y	= m_rotate_value_y+dy;
				
	//Set Last Cursor Position of the mouse
	g_LastCursorPos.x=g_CurrentCursorPos.x;
	g_LastCursorPos.y=g_CurrentCursorPos.y;

	if (debug){
		wchar_t  buf[2048];
		wsprintf(buf,L"g_OrigCursorPos = (%d, %d) \n",g_LastCursorPos.x, g_LastCursorPos.y);
		OutputDebugStringW(buf);

		wsprintf(buf,L"MovedCursorPos = (%d, %d) \n",g_CurrentCursorPos.x, g_CurrentCursorPos.y);
		OutputDebugStringW(buf);
	}
}


// Rotate command
// in:
//      dAngle  - angle of rotation
//      lOx     - x-coordinate of the center of rotation
//      lOy     - y-coordinate of the center of rotation
void GestureEngine::ProcessRotate(POINT g_CurrentCursorPos)
{
     OutputDebugStringW(L"ProcessRotate");
	 int dx,dy;
				
	dx = g_CurrentCursorPos.x - g_LastCursorPos.x;
	dy = g_CurrentCursorPos.x - g_LastCursorPos.y;

	if (dx < 0)      dx = -1;//Mouse moved to the left
	else if (dx > 0) dx =  1;//Mouse moved to the right
	if (dy < 0)      dy = -1;//Mouse moved up
	else if (dy > 0) dy =  1;//Mouse moved down
	
	dy = 0;
	_model->RotateCamera(-dx,dy,0);

	m_rotate_value_x	= m_rotate_value_x+dx;
	m_rotate_value_y	= m_rotate_value_y+dy;
				
	//Set Last Cursor Position of the mouse
	g_LastCursorPos.x=g_CurrentCursorPos.x;
	g_LastCursorPos.y=g_CurrentCursorPos.y;

	if (debug){
		wchar_t  buf[2048];
		wsprintf(buf,L"g_OrigCursorPos = (%d, %d) \n",g_LastCursorPos.x, g_LastCursorPos.y);
		OutputDebugStringW(buf);

		wsprintf(buf,L"MovedCursorPos = (%d, %d) \n",g_CurrentCursorPos.x, g_CurrentCursorPos.y);
		OutputDebugStringW(buf);
	}
}

void GestureEngine::setCursorPosition(POINT g_OrigCursorPos)
{
	//Set Last Cursor Position of the mouse
	g_LastCursorPos.x=g_OrigCursorPos.x;
	g_LastCursorPos.y=g_OrigCursorPos.y;
}

/*****************************************************************************
 GestureEngine::HandleKeyPress

 Windows message handler
******************************************************************************/
void GestureEngine::HandleKeyPress(UINT wParam)
{
    if (wParam == VK_ESCAPE) //If the escape key was pressed
    {
        //DestroyWindow(m_hwnd); //Send a WM_DESTROY message
    }
	if (wParam == VK_LEFT) //If the LEFT ARROW key was pressed
    {
         _model->TranslateCamera(-1,0,0);		// Translate camera left
    }
	if (wParam == VK_RIGHT) //If the RIGHT ARROW key was pressed
    {
        _model->TranslateCamera(1,0,0);		//Translate camera right
    }
	if (wParam == VK_UP) //If the UP ARROW key was pressed
    {
        _model->TranslateCamera(0,1,0);		// Translate camera up
    }
	if (wParam == VK_DOWN) //If the DOWN ARROW key was pressed
    {
		_model->TranslateCamera(0,-1,0);		//Translate camera down
    }
    if (wParam == VK_RETURN)
    {
		_model->CameraHome(0);	//Translate camera down
       
    }
	if (wParam == VK_NUMPAD0)
    {
		_model->AddLeftPlane(1);	//Add left plane
		_db->getrequest(session_id,"1");		//Perform a Get Request to NAVSEC (sid, iid)
       
    }
	if (wParam == VK_NUMPAD1)
    {
		_model->AddAvgYTotalPacketsZ(1);	//Add left plane
		_db->getrequest(session_id,"20");		//Perform a Get Request to NAVSEC (sid, iid)
		_db->getrequest(session_id,"21");		//Perform a Get Request to NAVSEC (sid, iid)
		_db->getrequest(session_id,"22");		//Perform a Get Request to NAVSEC (sid, iid)
       
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
		 if (_model&&_db)
		{
			_model->CameraHome(0);	//Go home   
			_db->getrequest(session_id,"3");		//Perform a Get Request to NAVSEC (sid, iid)
		
		 }
       
    }
	if (wParam == 0x49) //I
    {
		if (_model)
		{
		_model->TranslateCamera(0,1,0);	//Translate camera up
		}
    }
	if (wParam == 0x4A) //J
    {
		if (_model)
		{
		_model->TranslateCamera(-1,0,0);	//Translate camera left
		}
    }
		if (wParam == 0x4B) //K
    {
		if (_model)
		{
			_model->TranslateCamera(0,-1,0);	//Translate camera down
		}
    }
	if (wParam == 0x4C) //L
    {
		if (_model)
		{
		_model->TranslateCamera(1,0,0);	//Translate camera right
		}
    }
	if (wParam == 0x4D) //M
    {
		if (_model)
		{
		_model->TranslateCamera(0,0,-1);	//Translate camera away screen
		}
    }
	if (wParam == 0x4E) //N
    {
		if (_model)
		{
		_model->TranslateCamera(0,0,1);	//Translate camera into screen
		}
    }
	if (wParam == 0x4F) //O
    {
		if (_model)
		{
		_model->AdjustEyeSeparation(-1);	//Decrease Eye Separation 
		}
    }
	if (wParam == 0x50) //P
    {
		if (_model)
		{
		_model->AdjustEyeSeparation(1);	//Increase Eye Separation
		}
    }
	if (wParam == 0x51) //Q
    {
		if (_model)
		{
		_model->Zoom(1);	//Zoom
		}
    }

	if (wParam == 0x59) //Y
    {
		if (_model)
		{
		_model->AdjustFocalLength(-1);	//Decrease focal length
		}
    }
}





/*****************
* NAVSEC API Functions
*
*************************************/
// Zoom command
// in: 
//      dZoomFactor - scaling factor of zoom in/out
//      lZx         - x-coordinate of zoom center
//      lZy         - y-coordinate of zoom center


void GestureEngine::sendZoom(){
	
		if(m_zoom_value>=0 && m_zoom_value<10){
			//Db database;						//Database Class
			_db->getrequest(session_id,"10");		//Perform a Get Request to NAVSEC (sid, iid)

			if (debug){
				OutputDebugStringW(L"Zoom Out 10. \n");
			}

		}
		else if (m_zoom_value>=10 && m_zoom_value<20){
			//Db database;						//Database Class
			_db->getrequest(session_id,"11");		//Perform a Get Request to NAVSEC (sid, iid)
			
			if (debug){
				OutputDebugStringW(L"Zoom Out 20. \n");
			}
		}
		else if (m_zoom_value>=20 && m_zoom_value<30){
			//Db database;						//Database Class
			_db->getrequest(session_id,"12");		//Perform a Get Request to NAVSEC (sid, iid)
			OutputDebugStringW(L"Zoom Out 30. \n");
		}
		else if (m_zoom_value>=30 && m_zoom_value<40){
			//Db database;						//Database Class
			_db->getrequest(session_id,"13");		//Perform a Get Request to NAVSEC (sid, iid)
			if (debug){
				OutputDebugStringW(L"Zoom Out 40. \n");
			}
		}
		else if (m_zoom_value>40){
			//Db database;						//Database Class
			_db->getrequest(session_id,"14");		//Perform a Get Request to NAVSEC (sid, iid)
			if (debug){
				OutputDebugStringW(L"Zoom Out 50. \n");
			}
		}
		else if (m_zoom_value>-10 && m_zoom_value<0){
			//Db database;						//Database Class
			_db->getrequest(session_id,"15");		//Perform a Get Request to NAVSEC (sid, iid)
			
			if (debug){
				OutputDebugStringW(L"Zoom In 10. \n");
			}
		}
		else if (m_zoom_value>=-20 && m_zoom_value<-10){
			//Db database;						//Database Class
			_db->getrequest(session_id,"16");		//Perform a Get Request to NAVSEC (sid, iid)
			
			if (debug){
				OutputDebugStringW(L"Zoom In 20. \n");
			}
		}
		else if (m_zoom_value>=-30 && m_zoom_value<-20){
			//Db database;						//Database Class
			_db->getrequest(session_id,"17");		//Perform a Get Request to NAVSEC (sid, iid)
			OutputDebugStringW(L"Zoom In 30. \n");
		}
		else if (m_zoom_value>=-40 && m_zoom_value<-30){
			//Db database;						//Database Class
			_db->getrequest(session_id,"18");		//Perform a Get Request to NAVSEC (sid, iid)
			if (debug){
				OutputDebugStringW(L"Zoom In 40. \n");
			}
		}
		else if (m_zoom_value<-40){
			//Db database;						//Database Class
			_db->getrequest(session_id,"19");		//Perform a Get Request to NAVSEC (sid, iid)
			if (debug){
				OutputDebugStringW(L"Zoom In 50. \n");
			}
		}
		
		m_zoom_value = 0;					//Clear the Zoom value
}



void GestureEngine::sendRotate(){
								
		if(m_rotate_value_x<15){
			//Db database;						//Database Class
			_db->getrequest(session_id,"4");		//Perform a Get Request to NAVSEC (sid, iid)

			if (debug){
				OutputDebugStringW(L"Rotate 15. \n");
			}

		}
		else if (m_rotate_value_x>=15 && m_rotate_value_x<30){
			//Db database;						//Database Class
			_db->getrequest(session_id,"5");		//Perform a Get Request to NAVSEC (sid, iid)
			
			if (debug){
				OutputDebugStringW(L"Rotate 30. \n");
			}
		}
		else if (m_rotate_value_x>=30 && m_rotate_value_x<45){
			//Db database;						//Database Class
			_db->getrequest(session_id,"6");		//Perform a Get Request to NAVSEC (sid, iid)
			OutputDebugStringW(L"Rotate 45. \n");
		}
		else if (m_rotate_value_x>=45 && m_rotate_value_x<60){
			//Db database;						//Database Class
			_db->getrequest(session_id,"7");		//Perform a Get Request to NAVSEC (sid, iid)
			if (debug){
				OutputDebugStringW(L"Rotate 60. \n");
			}
		}
		else if (m_rotate_value_x>=60 && m_rotate_value_x<75){
			//Db database;						//Database Class
			_db->getrequest(session_id,"8");		//Perform a Get Request to NAVSEC (sid, iid)
			if (debug){
				OutputDebugStringW(L"Rotate 75. \n");
			}
		}
		else if (m_rotate_value_x>=75 && m_rotate_value_x<90){
			//Db database;						//Database Class
			_db->getrequest(session_id,"9");		//Perform a Get Request to NAVSEC (sid, iid)
			if (debug){
				OutputDebugStringW(L"Rotate 90. \n");
			}
		}

		m_rotate_value_x = 0;					//Clear the rotate X value
		m_rotate_value_y = 0;					//Clear the rotate Y value
}



void GestureEngine::sendTranslate(){


	//Clear Translate Value
	m_translate_value_x = 0;	// translate x direction counter
	m_translate_value_y = 0;	// translate y direction counter = 0;
}