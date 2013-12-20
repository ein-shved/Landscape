/*
 * ControlEventHandler.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */

#include "ControlEventHandler.hpp"
#include "Camera.hpp"

#include <iostream>

#define DEF_NANO (1000000000)

using namespace Landscape;
using namespace std;

ControlEventHandler::ControlEventHandler ( Landscape::Camera &in_lCamera ):
	m_vKeys (0xFF, false),
	m_vButtons (0x8, false),
	m_lCamera (in_lCamera),
	m_cTime ()
{
	clock_gettime(CLOCK_REALTIME, &m_cTime);
}

ControlEventHandler::XWindowPosition::XWindowPosition ():
	x(0),
	y(0)
{}
ControlEventHandler::XWindowPosition::XWindowPosition ( const Position &position):
	x(position.x),
	y(position.y)
{}

void ControlEventHandler::keyEvent ( size_t key, bool pressed )
{
	m_vKeys[key] = pressed;
}
void ControlEventHandler::buttonEvent ( size_t button, bool pressed )
{
	m_vButtons[button] = pressed;
}
void ControlEventHandler::mouseMoved ( int x, int y)
{
	_mousePosition.x = x;
	_mousePosition.y = y;
}

ControlEventHandler::XWindowPosition::operator Position ()
{
	return Position (x,y,0);
}

bool ControlEventHandler::step ()
{
	struct timespec cTime;
	clock_gettime(CLOCK_REALTIME, &cTime );
	double systemSpeed = (cTime.tv_sec - m_cTime.tv_sec)*DEF_NANO + 
				(double)(cTime.tv_nsec - m_cTime.tv_nsec);
	m_cTime = cTime;
	if (systemSpeed == 0 ) {
		systemSpeed = 1;
	}
	bool moved = (_mousePosition.y != 0) || ( _mousePosition.x != 0 );
	m_lCamera.rotate ( ((double)_mousePosition.y)/360, ((double)_mousePosition.x)/720 );
	_mousePosition.x = 0;
	_mousePosition.y = 0;
	#define ifKey(c) if (m_vKeys[(c)])
	double delta = 1000*systemSpeed/DEF_NANO;

	ifKey(25) { //w
		moved =true;
		m_lCamera.move (delta);
	}
	ifKey(39) { //s
		moved =true;
		m_lCamera.move (-delta);
	}
	ifKey(38) { //a
		m_lCamera.moveSide(-delta);
		moved = true;
	}
	ifKey(40) { //d
		m_lCamera.moveSide(delta);
		moved = true;
	}
	ifKey(65) { //space
		m_lCamera.moveHeight(delta);
		moved = true;
	}
	ifKey(37) { //ctrl
		m_lCamera.moveHeight(-delta);
		moved = true;
	}
	return moved;
}




