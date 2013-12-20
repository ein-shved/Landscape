/*
 * Camera.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 2.11.2012
 */

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "Camera.hpp"
#include "Graphics.hpp"

using namespace Landscape;
using namespace std;

#ifndef PI
# define PI 3.14159265359
#endif //PI

#undef _ExceptionString
#define _ExceptionString "\tCamera: "

Camera::Camera ( double in_dMinHeight, double in_dMaxHeight, const Position &in_pPosition, const Graphics *in_pGraphics )
    : m_pGraphics(in_pGraphics)
    , m_pPosition (in_pPosition)
    , m_dMinHeight (in_dMinHeight)
    , m_dMaxHeight (in_dMaxHeight)
    , m_dAngleZ (PI/2)
    , m_dAngleXY (0)
{
	checkPosition();
}
Camera::Camera ( const Position &in_pPosition, const Graphics *in_pGraphics )
    : m_pGraphics(in_pGraphics)
    , m_pPosition (in_pPosition)
    , m_dMinHeight (_MinCameraHeight)
    , m_dMaxHeight (_MaxCameraHeight)
    , m_dAngleZ (PI/2)
    , m_dAngleXY (0)
{
	checkPosition();
}
void Camera::setGraphics (const Graphics *in_pGraphics)
{
	m_pGraphics = in_pGraphics;
	checkPosition();
}
void Camera::checkPosition()
{
	if (m_pGraphics == NULL ) {
		return;
	}
	
	double height = (*m_pGraphics)( m_pPosition.x, m_pPosition.y );
	const Position &chunkPos = m_pGraphics->getChunk()->getPosition();
	const Size &chunkSize = m_pGraphics->getChunk()->getSize();
	
	if ( m_pPosition.x < chunkPos.x + 100 ) {
		m_pPosition.x = chunkPos.x + 100;
	}
	if ( m_pPosition.y < chunkPos.y + 100 ) {
		m_pPosition.y = chunkPos.y + 100;
	}
	if ( m_pPosition.x > chunkPos.x + chunkSize.length - 100 ) {
		m_pPosition.x = chunkPos.x + chunkSize.length - 100;
	}
	if ( m_pPosition.y > chunkPos.y + chunkSize.width - 100 ) {
		m_pPosition.y = chunkPos.y + chunkSize.width - 100;
	}
	if (m_pPosition.z > m_dMaxHeight) {
		m_pPosition.z = m_dMaxHeight;
	}
	if (m_pPosition.z < height + m_dMinHeight) {
		m_pPosition.z = height + m_dMinHeight;
	}
}

void Camera::move ( const Position &in_pPosition )
{
	m_pPosition = in_pPosition;
	checkPosition();
}
void Camera::move ( double length )
{
	m_pPosition.z += cos (m_dAngleZ)*length;
	double sZ = sin (m_dAngleZ)*length;
	m_pPosition.x += sin (PI - m_dAngleXY)*sZ;
	m_pPosition.y += cos (PI - m_dAngleXY)*sZ;
	checkPosition();
}
void Camera::moveSide ( double length )
{
	m_pPosition.x += cos (m_dAngleXY)*length;
	m_pPosition.y += sin (m_dAngleXY)*length;
	checkPosition();
}
void Camera::moveHeight ( double length )
{
	m_pPosition.z+=length;
	checkPosition();
}
void Camera::rotate ( double z, double xy )
{
	m_dAngleZ += z;
	if ( m_dAngleZ > PI ) {
		m_dAngleZ = PI;
	} else if ( m_dAngleZ < 0 ) {
		m_dAngleZ = 0;
	}

	m_dAngleXY += xy;
	if ( m_dAngleXY > PI ) {
		m_dAngleXY -= 2*PI;
	}
	if ( m_dAngleXY < -PI ) {
		m_dAngleXY = PI + (m_dAngleXY + PI);
	}
}
void Camera::setAngles ( double z, double xy )
{
	m_dAngleZ = z;
	m_dAngleXY = xy;
}

double Camera::getAngleZ () const
{
	return m_dAngleZ;
}
double Camera::getAngleXY() const
{
	return m_dAngleXY;
}
const Position &Camera::getPosition() const
{
	return m_pPosition;
}