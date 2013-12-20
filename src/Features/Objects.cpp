/*
 * Objects.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 9.11.2012
 */

#include "Objects.hpp"
#include "../Graphics.hpp"
#include <iostream>
#include <cmath>

#undef _ExceptionString
#define _ExceptionString "\tObjects: "

using namespace Landscape;
using namespace std;

Object::Object ( const Position &in_Position, const Size &in_Size)
    : m_Position (in_Position)
    , m_Size (in_Size)
{}
Object::~Object ()
{}
const Size &Object::getSize () const
{
	return m_Size;
}
const Position &Object::getPosition () const
{
	return m_Position;
}
void Object::setPosition ( const Position &in_Position )
{
	m_Position = in_Position;
}
double Object::height ( const Position &in_Position ) const
{
	return height(in_Position.x,in_Position.y);
}
void Object::serialize ( Serialization::Serializer &serializer )
{
	serializer.serialize (m_Position.x);
	serializer.serialize (m_Position.y);
	serializer.serialize (m_Position.z);
	serializer.serialize (m_Size.length);
	serializer.serialize (m_Size.width);
}

Tree::Tree ( const Position &in_Position, double in_Height)
    : Object (in_Position, Size(1,1))
    , m_Height(in_Height)
{}
Tree::~Tree ()
{}

void Tree::draw ( const Graphics& in_graphics)
{
	glBegin(GL_TRIANGLES);
	
		//Листва
		glColor3f(69.0/255.0,139.0/255.0,0.0);
		glNormal3f ( -0.5,0,0.5);
		glVertex3f(m_Position.x - m_Height/10,m_Position.z, m_Position.y - m_Height/10);
		glNormal3f ( -0.5,0,0.5);
		glVertex3f(m_Position.x, m_Position.z + m_Height, m_Position.y);
		glNormal3f ( -0.5,0,0.5);
		glVertex3f(m_Position.x + m_Height/10,m_Position.z, m_Position.y + m_Height/10);

		glNormal3f ( -0.5,0,-0.5);
		glVertex3f(m_Position.x + m_Height/10,m_Position.z, m_Position.y - m_Height/10);
		glNormal3f ( -0.5,0,-0.5);
		glVertex3f(m_Position.x, m_Position.z + m_Height, m_Position.y);
		glNormal3f ( -0.5,0,-0.5);
		glVertex3f(m_Position.x - m_Height/10,m_Position.z, m_Position.y + m_Height/10);

		//Ствол
		glColor3f(133.0/256.0,94.0/256.0,66.0/256.0);
		glVertex3f(m_Position.x + m_Height/30,m_Position.z, m_Position.y + m_Height/30);
		glVertex3f(m_Position.x, m_Position.z + m_Height/1.5, m_Position.y);
		glVertex3f(m_Position.x - m_Height/30,m_Position.z, m_Position.y + m_Height/30);

		glVertex3f(m_Position.x + m_Height/30,m_Position.z, m_Position.y + m_Height/30);
		glVertex3f(m_Position.x, m_Position.z + m_Height/1.5, m_Position.y);
		glVertex3f(m_Position.x + m_Height/30,m_Position.z, m_Position.y - m_Height/30);

		glVertex3f(m_Position.x - m_Height/30,m_Position.z, m_Position.y - m_Height/30);
		glVertex3f(m_Position.x, m_Position.z + m_Height/1.5, m_Position.y);
		glVertex3f(m_Position.x - m_Height/30,m_Position.z, m_Position.y + m_Height/30);

		glVertex3f(m_Position.x - m_Height/30,m_Position.z, m_Position.y - m_Height/30);
		glVertex3f(m_Position.x, m_Position.z + m_Height/1.5, m_Position.y);
		glVertex3f(m_Position.x + m_Height/30,m_Position.z, m_Position.y - m_Height/30);
		
	glEnd();
}
double Tree::height ( double in_dX, double in_dY  ) const
{
	return m_Height;
}
Tree *Tree::clone () const
{
	return new Tree(*this);
}

void Tree::serialize ( Serialization::Serializer &serializer )
{
	Object::serialize (serializer);
	serializer.serialize (m_Height);
}