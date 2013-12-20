/*
 * Fog.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 7.11.2012
 */

#include "Fog.hpp"
#include "../Graphics.hpp"

#undef _ExceptionString
#define _ExceptionString "\tFog: "

using namespace Landscape;
using namespace std;

Fog::~Fog ()
{}

SimpleFog::SimpleFog()
{
	m_fogColor [0] = 100.0/255.0;
	m_fogColor [1] = 133.0/255.0;
	m_fogColor [2] = 204.0/255.0;
	m_fogColor [3] = 0.5f;
}
SimpleFog::~SimpleFog()
{}

void SimpleFog::draw ( const Graphics& in_graphics )
{
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, m_fogColor);
	glFogf(GL_FOG_DENSITY, 1);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, in_graphics.getViewLength()/10);
	glFogf(GL_FOG_END, in_graphics.getViewLength()/2);
}
SimpleFog *SimpleFog::clone ()
{
	return new SimpleFog(*this);
}
void SimpleFog::serialize ( Serialization::Serializer &serializer )
{
	double d = m_fogColor[0];
	serializer.serialize ( d );
	m_fogColor[0] = d;

	d = m_fogColor[1];
	serializer.serialize ( d );
	m_fogColor[1] = d;

	d = m_fogColor[2];
	serializer.serialize ( d );
	m_fogColor[2] = d;

	d = m_fogColor[3];
	serializer.serialize ( d );
	m_fogColor[3] = d;
}