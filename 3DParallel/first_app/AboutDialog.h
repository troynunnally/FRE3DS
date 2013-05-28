<<<<<<< HEAD
/* AboutDialog.h
*
*Creates an information dialog for P3D
*/
 
#include <windows.h>
#include "resource.h"
 
// Class: AboutDialog
// Draws the About Dialog
class AboutDialog
{
	public:
		AboutDialog();
		~AboutDialog();
		static BOOL CALLBACK DialogProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int Run(HINSTANCE hInstance, HWND hParent);
		 
	private:
		HWND m_hwnd;
};

=======
/* AboutDialog.h
*
*Creates an information dialog for P3D
*/
 
#include <windows.h>
#include "resource.h"
 
// Class: AboutDialog
// Draws the About Dialog
class AboutDialog
{
	public:
		AboutDialog();
		~AboutDialog();
		static BOOL CALLBACK DialogProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int Run(HINSTANCE hInstance, HWND hParent);
		 
	private:
		HWND m_hwnd;
};

>>>>>>> Fix some fragmentation bugs
