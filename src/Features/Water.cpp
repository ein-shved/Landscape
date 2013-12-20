/*
 * Water.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 7.11.2012
 */

#include <iostream>

#include "Water.hpp"
#include "../Graphics.hpp"

#undef _ExceptionString
#define _ExceptionString "\tWater: "

using namespace Landscape;
using namespace std;

Water::~Water ()
{}

SimpleWater::~SimpleWater()
{}
void SimpleWater::draw ( const Graphics& in_graphics, Chunk *in_pChunk )
{

	glColor4f(0.0f,0.0f,1.0f,0.5f);
	glBegin (GL_QUADS);
		glVertex3d(m_Position.x, m_Position.z, m_Position.y);
		glVertex3d(m_Position.x + m_Size.length, m_Position.z, m_Position.y);
		glVertex3d(m_Position.x + m_Size.length, m_Position.z, m_Position.y + m_Size.width);
		glVertex3d(m_Position.x, m_Position.z, m_Position.y + m_Size.width);
	glEnd();

}
void SimpleWater::setSize ( const Position &in_Position, const Size &in_Size )
{
	m_Position = in_Position;
	m_Size = in_Size;
}
SimpleWater *SimpleWater::clone () const
{
	return new SimpleWater (*this);
}
const Position &SimpleWater::getPosition () const
{
	return m_Position;
}
void SimpleWater::serialize ( Serialization::Serializer &serializer )
{
	serializer.serialize (m_Position.x);
	serializer.serialize (m_Position.y);
	serializer.serialize (m_Position.z);
	serializer.serialize (m_Size.length);
	serializer.serialize (m_Size.width);
}

ReflectiveWater::~ReflectiveWater ()
{}

void reflectionmatix ( GLfloat reflection_matrix[4][4], const GLfloat plain_point[3], const GLfloat plain_normal[3] ) {
	const GLfloat *p = plain_point, *v = plain_normal;
	float pv = p[0]*v[0]+p[1]*v[1]+p[2]*v[2];
	
	reflection_matrix[0][0] = 1 - 2*v[0]*v[0];
	reflection_matrix[1][0] = -2 * v[0]*v[1];
	reflection_matrix[2][0] = -2 * v[0]*v[2];
	reflection_matrix[3][0] = 2 * pv *v[0];
	
	reflection_matrix[0][1] = -2 * v[0]*v[1];
	reflection_matrix[1][1] = 1 - 2*v[1]*v[1];
	reflection_matrix[2][1] = - 2 * v[1]*v[2];
	reflection_matrix[3][1] = 2 * pv * v[1];
	
	reflection_matrix[0][2] = -2 * v[0] * v[2];
	reflection_matrix[1][2] = -2 * v[1] * v[2];
	reflection_matrix[2][2] = 1 - 2*v[2]*v[2];
	reflection_matrix[3][2] = 2 * pv * v[2];
	
	reflection_matrix[0][3] = 0;
	reflection_matrix[1][3] = 0;
	reflection_matrix[2][3] = 0;
	reflection_matrix[3][3] = 1;
}
void ReflectiveWater::draw ( const Graphics& in_graphics, Chunk *in_pChunk )
{
	if ( in_pChunk == NULL ) {
		return;
	}
	
	glEnable (GL_STENCIL_TEST);
	glStencilFunc (GL_ALWAYS, 1, 0);
	glStencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);
	
	glDisable (GL_DEPTH_TEST);
	glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable (GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	glColor4f(0.0f,0.0f,1.0f,0.5f);
	glBegin (GL_QUADS);
		glVertex3d(m_Position.x + m_Size.length + in_graphics.getViewLength(), m_Position.z, m_Position.y + m_Size.width + in_graphics.getViewLength());
		glVertex3d(m_Position.x - in_graphics.getViewLength(), m_Position.z, m_Position.y + m_Size.width + in_graphics.getViewLength());
		glVertex3d(m_Position.x - in_graphics.getViewLength(), m_Position.z, m_Position.y- in_graphics.getViewLength());
		glVertex3d(m_Position.x + m_Size.length + in_graphics.getViewLength(), m_Position.z, m_Position.y - in_graphics.getViewLength());
	glEnd();
	
	glDisable (GL_CULL_FACE);
	
	glEnable (GL_DEPTH_TEST);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	
	glStencilFunc (GL_EQUAL, 0x1, 0xFFFFFFFF );
	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	GLfloat vertex[3] = {(GLfloat)m_Position.x, (GLfloat)m_Position.z, (GLfloat)m_Position.y};
	GLfloat normal[3] = {0,1,0};
	GLfloat reflection_matrix[4][4];
	reflectionmatix (reflection_matrix,vertex,normal);
	
	glPushMatrix();
// 	glTranslatef(0.0, 2*m_Position.z, 0.0);
// 	glScalef(1.0, -1.0, 1.0);
	glMultMatrixf ( (float*)reflection_matrix);

	glEnable(GL_CLIP_PLANE0);
	GLdouble plane[] = {0,1,0,-m_Position.z};
	glClipPlane (GL_CLIP_PLANE0,plane);

	in_pChunk->drawWater = false;
	in_pChunk->draw(in_graphics);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDisable(GL_CLIP_PLANE0);

	glPopMatrix();
	
// 	glDisable (GL_FOG);
	glColor4f(100.0/255.0,133.0/255.0,204.0/255.0,0.5f);
	glBegin (GL_QUADS);
		glVertex3d(m_Position.x + m_Size.length + in_graphics.getViewLength(), m_Position.z, m_Position.y + m_Size.width + in_graphics.getViewLength());
		glVertex3d(m_Position.x - in_graphics.getViewLength(), m_Position.z, m_Position.y + m_Size.width + in_graphics.getViewLength());
		glVertex3d(m_Position.x - in_graphics.getViewLength(), m_Position.z, m_Position.y- in_graphics.getViewLength());
		glVertex3d(m_Position.x + m_Size.length + in_graphics.getViewLength(), m_Position.z, m_Position.y - in_graphics.getViewLength());
	glEnd();
	
// 	glEnable (GL_FOG);
	
	glDisable (GL_BLEND);
	glDisable (GL_STENCIL_TEST);


}
ReflectiveWater *ReflectiveWater::clone () const
{
	return new ReflectiveWater(*this);
}
