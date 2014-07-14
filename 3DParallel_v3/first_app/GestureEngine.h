// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// GestureEngine.h
//
// Definition of helper classes for processing of gestures. During this process
// driver sends subsequent finger position information (reference point) and 
// in the some cases other information.

#pragma once
#include <windows.h>
#include "ModelGL.h"		//Declare our ModelGL class
#include "Db.h"				//Declare our Class to connect to Database

				 
// GestureEngine class is abstract class that is responsible for 
// properly decoding information from multi-touch driver. There are
// few pure virtual functions that are responsible for manipulation 
// of the object.
class ModelGL;
class Db;

class GestureEngine
{
public:
    GestureEngine(ModelGL* model, Db* database);
    ~GestureEngine();

    // Process WM_GESTURE messages
    virtual LRESULT WndProc(HWND hWnd, WPARAM wParam, LPARAM lParam);

    // This function is called when press and tap gesture is recognized
    void ProcessPressAndTap();

    // This function is invoked when five finger tap gesture is recognized
    void ProcessFiveFingerTap();

	// This function is invoked when four finger hold is recognized
	void GestureEngine::ProcessFourFingerHold();

	//This function is invoked when two finger tap gesture is recognized
	void ProcessTenFingerHold();

    // This function is called constantly through duration of zoom in/out gesture
    void ProcessZoom(POINT g_CurrentCursorPos);

	// This function is called constantly through duration of translate left/right gesture
    void ProcessTranslate(POINT g_CurrentCursorPos);

	// This function is called constantly through duration of translate left/right gesture
    void ProcessMouseTranslate(POINT g_CurrentCursorPos);

    // This function is called throughout the duration of the panning/inertia gesture
    void ProcessMove(const LONG ldx, const LONG ldy);

    // This function is called throughout the duration of the rotation gesture
    void ProcessRotate(POINT g_CurrentCursorPos);

	    // This function is called throughout the duration of the rotation gesture
    void ProcessMouseRotate(POINT g_CurrentCursorPos);

	//This function handles the key press
	void HandleKeyPress(UINT wParam);
	
	//Set the Current Cursor Position
	void setCursorPosition(POINT g_OrigCursorPos);

	// This function is called to send zoom parameter to the server
    void sendZoom();

	// This function is called to translate to the server
	void sendTranslate();

	// This function is called to rotate to the server
	void sendRotate();

private:
    POINT _ptFirst;				// First significant point of the gesture
    POINT _ptSecond;			// Second significant point of the gesture
	POINT g_LastCursorPos;
    DWORD _dwArguments;			// 4 bytes long argument
	ModelGL*_model;				// A link to the ModelGL program which is a OpenGL Program
	Db* _db;					// Connects to the web interface

	int m_rotate_value_x;		// rotate x direction counter
	int m_rotate_value_y;		// rotate y direction counter
	int m_rotate_value_z;		// rotate z direction counter
	int m_zoom_value;			// zoom value counter
	int m_translate_value_x;	// translate x direction counter
	int m_translate_value_y;	// translate y direction counter
	
	BOOL debug;					// Debug bit
	int session_id;				// Session ID
						
};
