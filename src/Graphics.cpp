/*
 * Graphics.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */

#include<GL/gl.h>
#include<GL/glu.h>
#include <iostream>

#include "Chunk/Serializer.hpp"
#include "Graphics.hpp"
#include "Generator/Generator.hpp"
#include "Camera.hpp"
#include "Features/Fog.hpp"

using namespace Landscape ;
using namespace std;


#ifndef PI
# define PI 3.14159265359
#endif //PI

Graphics::Graphics(const Generator &in_lGenerator, const Camera &in_lCamera, double in_viewLength)
    : m_viewLength (in_viewLength)

    , m_pChunk (NULL)
    , m_lCamera (in_lCamera)
    , m_pGenerator (in_lGenerator.clone())

    ,m_pFog (new SimpleFog())
{
	m_pChunk = m_pGenerator->generate();
}
Graphics::~Graphics()
{
	if ( m_pChunk != NULL ) {
		delete m_pChunk;
	}
	if ( m_pGenerator != NULL ) {
		delete m_pGenerator;
	}
	if ( m_pFog != NULL ) {
		delete m_pFog;
	}
}
void Graphics::init ( const GLFrontEnd::WindowGeometry &in_lWindow )
{
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective ( 60, in_lWindow.width/in_lWindow.height,m_viewLength/1000,m_viewLength );

	//Свет ..озарил мою больную душу
	GLfloat mLightAmbient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mLightDiffuse[] = { 100.0/255.0, 133.0/255.0, 204.0/255.0, 1.0 };
	GLfloat mLightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv (GL_LIGHT0, GL_AMBIENT, mLightAmbient);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, mLightDiffuse);
	glLightfv (GL_LIGHT0, GL_SPECULAR, mLightSpecular);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	//Color material
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	step (in_lWindow);
}
void Graphics::step ( const GLFrontEnd::WindowGeometry &in_lWindow )
{
	glViewport(0,0,in_lWindow.width,in_lWindow.height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	const Position &pos = m_lCamera.getPosition();
	//m_pGenerator->generate(m_pChunk, pos);
	glRotated ( (m_lCamera.getAngleZ() - PI/2)*180/PI, 1,0,0);
	glRotated ( m_lCamera.getAngleXY()*180/PI, 0,1,0);
	glTranslated ( -pos.x, -pos.z, -pos.y);

	glClearColor(100.0/255.0, 133.0/255.0, 204.0/255.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (m_pFog != NULL ) {
		m_pFog->draw(*this);
	}
	if ( m_pChunk != NULL ) {
		m_pChunk->drawWater = true;
		m_pChunk->draw(*this);
	}
}
void Graphics::drawFog () const
{
	if (m_pFog != NULL ) {
		m_pFog->draw(*this);
	}
}
double Graphics::operator() (double x, double y) const
{
	//const Size &chunkSize = m_pChunk->getSize();
	const Position &chunkPos = m_pChunk->getPosition();

	return m_pChunk->height( x, y ) + chunkPos.z;
}

double Graphics::getViewLength() const
{
	return m_viewLength;
}
const Camera &Graphics::getCamera() const
{
	return m_lCamera;
}
const Chunk *Graphics::getChunk() const
{
	return m_pChunk;
}
double Graphics::getMaxHeight() const
{
	return m_pGenerator->getMaxHeight();
}

void Graphics::serialize (Serializer &serializer)
{
	serializer.serializeChunk(m_pChunk);
}