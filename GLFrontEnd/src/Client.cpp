/*
 * Client.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */

#include <stdexcept>
#include <iostream>
#include <cstring>

#ifdef __linux__

#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/glx.h>

#elif defined _WIN32 || _WIN64
#include <windows.h>
#include <windowsx.h>
#endif

#include<GL/gl.h>
#include<GL/glu.h>


#include "Client.hpp"

using namespace GLFrontEnd;
using namespace std;


#ifndef _GrabbedPointer_X_Pos
# define _GrabbedPointer_X_Pos 100
#endif //_GrabbedPointer_X_Pos

#ifndef _GrabbedPointer_Y_Pos
# define _GrabbedPointer_Y_Pos 100
#endif //_GrabbedPointer_Y_Pos

#if defined(_WIN32) ||(_WIN64)
Client *Client::s_globalClient = NULL;
#endif

Client::Client ( const std::string &applicationName
#ifdef __linux__
		 , const std::string &displayName
#endif //__linux__
		)
    : _loopFlag(false)
    , _applicationName(applicationName)
#ifdef __linux__
    , _displayName (displayName)
    , _xDisplay(NULL)
#elif defined (_WIN32) || defined(_WIN64)
	, _hRC(NULL)
	, _hDC(NULL)
	, _hWnd(NULL)
	, _hInstance(NULL)
	, _bActive(false)
	, _graphics(NULL)
	, _handler(NULL)
#endif
{}
Client::~Client ()
{
	_loopFlag = false;
#ifdef __linux__
	if ( _xDisplay == NULL ) {
		return;
	}
#elif defined _WIN32 || _WIN64
	if ( s_globalClient == NULL ){
		return;
	}
#endif
	destroy();
}

void Client::start()
{
#ifdef __linux__
	//Checking
	if (_xDisplay != NULL ) {
		throw logic_error ("Incorrect calling start() in Client object");
	}
	//Display
	this->_xDisplay = XOpenDisplay ( _displayName.length() > 0 ? _displayName.c_str() : NULL ); // default display, if displayName length is zero.
	if( _xDisplay == NULL) {
		string explanationString = _displayName.length() > 0 ? "Can not open display: " + _displayName  : "Can not open default display. Check your DISPLAY variable";
		throw logic_error (explanationString);
	}

	//Root window
	this->_xRootWindow = DefaultRootWindow(_xDisplay); //Parant window is default for display

	//Visual info
	GLint attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	this->_xVisualInfo = glXChooseVisual(_xDisplay, 0, attributes);
	if(_xVisualInfo == NULL) {
		throw logic_error ("No appropriate visual found");
	}

	//Color map
	Colormap colorMap;
	colorMap = XCreateColormap(_xDisplay, _xRootWindow, _xVisualInfo->visual, AllocNone);

	//Setting window attributes
	XSetWindowAttributes swa;
	swa.colormap = colorMap;
	swa.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask
			| EnterWindowMask | LeaveWindowMask | VisibilityChangeMask | StructureNotifyMask | FocusChangeMask
			| ButtonPressMask | ButtonReleaseMask;

	//Creating window
	this->_xWindow = XCreateWindow(_xDisplay, _xRootWindow, 10, 10, 600, 600, 0, _xVisualInfo->depth, InputOutput, _xVisualInfo->visual, CWColormap | CWEventMask, &swa);

	//Setting WM_PROTOCOLS property
	Atom wmDelete=XInternAtom(_xDisplay, "WM_DELETE_WINDOW", True); //Get atom of the WM_DELETE_WINDOW potocol
	XSetWMProtocols(_xDisplay, _xWindow, &wmDelete, 1); //Set the WM_PROTOCOLS

	//Setting title
	XStoreName(_xDisplay, _xWindow, _applicationName.c_str());
#elif defined _WIN32 || _WIN64

	if ( s_globalClient != NULL) {
		throw runtime_error ("Only one CLient instance can run on WINDOWS application.");
	}
	s_globalClient = this;

	int bits = 32;
	GLuint    pixelFormat;
	WNDCLASS  wc;
	DWORD    dwExStyle;
	DWORD    dwStyle;

	RECT WindowRect;
	WindowRect.left=(long)0;
	WindowRect.right=(long)600;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)600;
	_winGeometry.x=0;
	_winGeometry.y=0;
	_winGeometry.width=600;
	_winGeometry.height=600;

	_hInstance    = GetModuleHandle(NULL);
	wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc    = (WNDPROC) wndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance    = _hInstance;
	wc.hIcon    = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor    = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground  = NULL;
	wc.lpszMenuName    = NULL;
	wc.lpszClassName  = _applicationName.c_str();

	if( !RegisterClass( &wc ) ) {
		throw runtime_error ("Failed To Register The Window Class.");
	}

	dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle    =   WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );

	if( !( _hWnd = CreateWindowEx(  dwExStyle, _applicationName.c_str(), _applicationName.c_str(),
          WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle, 0, 0, WindowRect.right-WindowRect.left,
		  WindowRect.bottom-WindowRect.top, NULL, NULL, _hInstance, NULL ) ) ) {
			  destroy();
			  throw runtime_error ( "Window Creation Error." );
	}
	static  PIXELFORMATDESCRIPTOR pfd= { sizeof(PIXELFORMATDESCRIPTOR),	1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 32, 0, 0, PFD_MAIN_PLANE,0, 0, 0, 0 };

	if( !( _hDC = GetDC( _hWnd ) ) ){
		destroy();
		throw runtime_error ( "Can't Create A GL Device Context.");
	}
	if( !( pixelFormat = ChoosePixelFormat( _hDC, &pfd ) ) ){
		destroy();
		throw runtime_error ( "Can't Find A Suitable PixelFormat.");
	}
	if( !SetPixelFormat( _hDC, pixelFormat, &pfd ) ){
		destroy();
		throw runtime_error ( "Can't Set The PixelFormat.");
	}
#endif
}
struct EscapeException {
	int key;
};
void Client::run ( GLFrontEnd::Graphics &graphics, GLFrontEnd::ControlEventHandler &handler )
{
	int result = 0;
	//Checking
	if ( _loopFlag ) {
		throw logic_error ("Incorrect calling run() in Client object");
	}
	_loopFlag = true;

#ifdef __linux__

	if ( _xDisplay == NULL) {
		throw logic_error ("Incorrect calling run() in Client object");
	}

	//Create GL context
	_glxContext = glXCreateContext(_xDisplay, _xVisualInfo, NULL, GL_TRUE);
	glXMakeCurrent(_xDisplay, _xWindow, _glxContext );
	glEnable(GL_DEPTH_TEST);
	
	cout<<glGetString(GL_VERSION)<<endl;

	//Safe window geometry
	XWindowAttributes gwa;
	XGetWindowAttributes(_xDisplay, _xWindow, &gwa);
	WindowGeometry winGeometry;
	winGeometry.x = gwa.x;
	winGeometry.y = gwa.y;
	winGeometry.width = gwa.width;
	winGeometry.height = gwa.height;
	
	glViewport(0, 0, gwa.width, gwa.height);
	graphics.init (winGeometry);
					
	//Map window
	XMapWindow(_xDisplay, _xWindow);
	

	//Create empty mouse pointer
	Pixmap bm_no;
        Colormap cmap;
        Cursor no_ptr;
        XColor black, dummy;
        static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};
	cmap = DefaultColormap(_xDisplay, DefaultScreen(_xDisplay));
        XAllocNamedColor(_xDisplay, cmap, "black", &black, &dummy);
        bm_no = XCreateBitmapFromData(_xDisplay, _xWindow, bm_no_data, 8, 8);
        no_ptr = XCreatePixmapCursor(_xDisplay, bm_no, bm_no, &black, &black, 0, 0);

	//Mause magic
	int wrapped = 0;
	bool focused = true;
	if ( ( result = XGrabPointer ( _xDisplay,_xWindow, true,
		PointerMotionMask | EnterWindowMask | LeaveWindowMask,
		GrabModeAsync, GrabModeAsync, _xWindow, None, CurrentTime ) )
		!= GrabSuccess )
	{
		cerr<<"Can not grub pointer with code: "<<result<<endl;
	} else {
		XDefineCursor(_xDisplay, _xWindow, no_ptr);
	}

	//Main cicle
	while( _loopFlag ) {
		XEvent event;
		bool controlEvent = true;
		do {
			if ( !XPending(_xDisplay) ) {
				goto link_ClientDraw;
			}
			XNextEvent(_xDisplay, &event);
			switch (event.type) {

			case VisibilityNotify:
			case MapNotify:
			case Expose:
				{
					XGetWindowAttributes(_xDisplay, _xWindow, &gwa);
					winGeometry.x = gwa.x;
					winGeometry.y = gwa.y;
					winGeometry.width = gwa.width;
					winGeometry.height = gwa.height;

					glViewport(0, 0, gwa.width, gwa.height);
					graphics.init (winGeometry);
					glXSwapBuffers(_xDisplay, _xWindow);
				}
				controlEvent = false;
				break;

			case ClientMessage:
				{
					char* message_type = XGetAtomName( _xDisplay, event.xclient.message_type);
					if ( message_type == NULL )
						break;
					if ( !strcmp (message_type, "WM_PROTOCOLS") ) {
						char* protocol_name = XGetAtomName( _xDisplay,event.xclient.data.l[0] );
						if ( !strcmp ( protocol_name, "WM_DELETE_WINDOW")) {
							_loopFlag = false;
						} else
							cout<<"\tWM send protocol of name `"<<protocol_name<<"'.\n";
						XFree (protocol_name);
					} else {
						cout<<"\tRecive client message of type `"<<message_type<<"'.\n";
					}
					XFree (message_type);
				}
				controlEvent = false;
				break;
			case KeyPress:
				switch (event.xkey.keycode) {
				case 9: //ESCAPE
					_loopFlag = false;
					break;
				case 105: //Right control
					focused = false;
					XUndefineCursor (_xDisplay, _xWindow);
					XUngrabPointer (_xDisplay,CurrentTime);
					break;
				default:
					handler.keyEvent (event.xkey.keycode, true);
					break;
				}
				break;
			case KeyRelease:
				handler.keyEvent (event.xkey.keycode, false);
				break;
			case MotionNotify:
				if (focused) {
					if ( wrapped == 2 ) {
						handler.mouseMoved ( event.xmotion.x-_GrabbedPointer_X_Pos,event.xmotion.y-_GrabbedPointer_Y_Pos );
					}
					if ( wrapped == 1 )
						wrapped = 2;
					if ( event.xmotion.x  != 100 || event.xmotion.y != 100 ) {
						XWarpPointer(_xDisplay,_xWindow,_xWindow,0,0,0,0,_GrabbedPointer_X_Pos,_GrabbedPointer_Y_Pos);
						if (wrapped == 0)
							wrapped = 1;
					}
				}
				break;

			case FocusIn:
				controlEvent = false;
				break;
			case FocusOut:
				focused = false;
				XUndefineCursor (_xDisplay, _xWindow);
				XUngrabPointer (_xDisplay,CurrentTime);
				controlEvent = false;
				break;
			case ButtonPress:
				if (!focused) {
					if ( ( result = XGrabPointer ( _xDisplay,_xWindow, true,
					PointerMotionMask | EnterWindowMask | LeaveWindowMask,
					GrabModeAsync, GrabModeAsync, _xWindow, None, CurrentTime ) )
					!= GrabSuccess )
					{
						cerr<<"Can not grub pointer with code: "<<result<<endl;
					} else {
						XDefineCursor(_xDisplay, _xWindow, no_ptr);
					}
					XWarpPointer(_xDisplay,_xWindow,_xWindow,0,0,0,0,100,100);
					focused = true;
				} else {
					handler.buttonEvent (event.xbutton.button, true);
				}
				break;
			case ButtonRelease:
				if (focused) {
					handler.buttonEvent (event.xbutton.button, false);
				}
			}
		} while ( controlEvent );

	link_ClientDraw:
		if ( handler.step() )
		{
			graphics.step(winGeometry);
			glXSwapBuffers(_xDisplay, _xWindow);
		}
	}

	//free cursor
	if (bm_no != None)
                XFreePixmap(_xDisplay, bm_no);
        XFreeColors(_xDisplay, cmap, &black.pixel, 1, 0);

	//Free context
	glXMakeCurrent(_xDisplay, None, NULL);
	glXDestroyContext(_xDisplay, _glxContext);

	//Unmap window
	XUnmapWindow(_xDisplay, _xWindow);

#elif defined _WIN32 || _WIN64

	if ( s_globalClient == NULL) {
		throw logic_error ("Incorrect calling run() in Client object");
	}
	
	if( !( _hRC = wglCreateContext( _hDC ) ) ) {
		destroy();
		throw runtime_error ( "Can't Create A GL Rendering Context.");
	}
	if( !wglMakeCurrent( _hDC, _hRC ) ) {
		destroy();
		throw runtime_error ( "Can't Activate The GL Rendering Context.");
	}
	
	_mousePos.x = -1;
	_mousePos.y = -1;

	ShowWindow( _hWnd, SW_SHOW );
	SetForegroundWindow( _hWnd );
	SetFocus( _hWnd );

	_graphics = &graphics;
	_handler = &handler;

	MSG  msg;
	_graphics->init (_winGeometry);
	SwapBuffers( _hDC );
	while( _loopFlag ){
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			if( msg.message == WM_QUIT ){
				_loopFlag = false;
			} else {
				TranslateMessage (&msg);
				DispatchMessage (&msg);
			}
		}
		if (_handler->step()) {
			_graphics->step (_winGeometry);
			SwapBuffers( _hDC );
		}
	}
#endif
}
void Client::stop ()
{
#ifdef __linux__
	_loopFlag = false;
#endif
}
void Client::destroy ()
{
#ifdef __linux__
	if (_loopFlag || _xDisplay == NULL ) {
		throw logic_error ("Incorrect calling destroy() in Client object");
	}
	XDestroyWindow(_xDisplay, _xWindow);
	XCloseDisplay(_xDisplay);
	_xDisplay = NULL;
#elif defined _WIN32 || _WIN64
	if (_loopFlag || s_globalClient == NULL ) {
		throw logic_error ("Incorrect calling destroy() in Client object");
	}
	if( _hRC ) {
		if( !wglMakeCurrent( NULL, NULL ) ) {
			throw runtime_error ("Release Of DC And RC Failed.");
		}
		if( !wglDeleteContext( _hRC ) ) {
			throw runtime_error ("Release Rendering Context Failed.");
		}
		_hRC = NULL; 
	}
	if( _hDC && !ReleaseDC( _hWnd, _hDC ) ) {
		throw runtime_error("Release Device Context Failed.");
		_hDC=NULL;
	}
	if(_hWnd && !DestroyWindow(_hWnd)) {
		throw runtime_error( "Could Not Release hWnd.");
		_hWnd = NULL;
	}
	if( !UnregisterClass( _applicationName.c_str(), _hInstance ) ) {
		throw runtime_error( "Could Not Unregister Class.");
		_hInstance = NULL;
	}
	s_globalClient = NULL;
#endif
}

#if defined (_WIN32) || defined(_WIN64)

LRESULT CALLBACK Client::wndProc( HWND  hWnd, UINT  uMsg, WPARAM  wParam, LPARAM  lParam)     
{
	if ( s_globalClient == NULL) {
		throw runtime_error ("Unexpected NULL global client");
	}

	switch (uMsg) {
	case WM_ACTIVATE:
		if( !HIWORD( wParam ) ) {
			s_globalClient->_bActive = true;
		} else {
			s_globalClient->_bActive = false;
		}
		break;
	case WM_CLOSE:
		s_globalClient->_loopFlag = false;
		PostQuitMessage( 0 );
		break;
	case WM_KEYDOWN:
		if ( s_globalClient->_handler == NULL ) {
			break;
		}
		if (wParam == 27) { //esc
			s_globalClient->_loopFlag = false;
			break;
		}
		s_globalClient->_handler->keyEvent (wParam,true);
		break;
	case WM_KEYUP:
		if ( s_globalClient->_handler == NULL ) {
			break;
		}
		s_globalClient->_handler->keyEvent (wParam,false);
		break;
	case WM_MOUSEMOVE:
		{
			TRACKMOUSEEVENT trackMouseEvent = { sizeof(trackMouseEvent), TME_LEAVE, s_globalClient->_hWnd, HOVER_DEFAULT};
			TrackMouseEvent ( &trackMouseEvent);
			int dX = 0, dY = 0;
			if ( s_globalClient->_mousePos.x == -1 || s_globalClient->_mousePos.y == -1 ) {
				dX = dY = 0;
			} else {
				dX = GET_X_LPARAM(lParam) - s_globalClient->_mousePos.x;
				dY = GET_Y_LPARAM(lParam) - s_globalClient->_mousePos.y;
			}
			SetCursorPos ( s_globalClient->_winGeometry.x + _GrabbedPointer_X_Pos, s_globalClient->_winGeometry.y + _GrabbedPointer_Y_Pos);
			s_globalClient->_mousePos.x = _GrabbedPointer_X_Pos;
			s_globalClient->_mousePos.y = _GrabbedPointer_Y_Pos;
			if ( s_globalClient->_handler == NULL ) {
				break;
			}
			s_globalClient->_handler->mouseMoved(dX,dY);
		}
		break;
	case WM_MOUSELEAVE:
		{
			cerr<<"AAAAA\n";
			SetCursorPos ( s_globalClient->_winGeometry.x + _GrabbedPointer_X_Pos, s_globalClient->_winGeometry.y + _GrabbedPointer_Y_Pos);
			s_globalClient->_mousePos.x = _GrabbedPointer_X_Pos;
			s_globalClient->_mousePos.y = _GrabbedPointer_Y_Pos;
		}
		break;
	case WM_SIZE:
		if ( s_globalClient->_graphics== NULL ) {
			break;
		}
		s_globalClient->_winGeometry.width = LOWORD(lParam);
		s_globalClient->_winGeometry.height = HIWORD(lParam);
		cerr<<s_globalClient->_winGeometry.width<<" "<<s_globalClient->_winGeometry.height<<endl;
		s_globalClient->_graphics->init(s_globalClient->_winGeometry);
		break;
	case WM_MOVE:
		s_globalClient->_winGeometry.x = (int)(short) LOWORD(lParam);
		s_globalClient->_winGeometry.y = (int)(short) HIWORD(lParam);
		break;
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	return 0;
}
#endif
