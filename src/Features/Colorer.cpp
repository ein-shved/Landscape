/*
 * Colorer.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */


#include "Colorer.hpp"

#undef _ExceptionString
#define _ExceptionString "\tColorer: "

using namespace Landscape;
using namespace std;

Colorer::~Colorer ()
{}

SimpleColorer::SimpleColorer( double height )
    : m_MaxHeight (height)
{
	calcLevels();
}
SimpleColorer::~SimpleColorer ()
{}
Colorer::Color3f SimpleColorer::getColor ( double height ) const
{
	Colorer::Color3f result;
	if ( height <= m_Sand ) {
		result.r = 238.0/255.0;
		result.g = 214.0/255.0;
		result.b = 175.0/255.0; //Desert sand
		return result;
	}
	if ( height <= m_Grass ) {
		result.r =  77.0/255.0;
		result.g = 189.0/255.0;
		result.b = 51.0/255.0; //Grass
		return result;
	}
	if ( height <= m_Stone ) {
		result.r = 139.0/255.0;
		result.g = 137.0/255.0;
		result.b = 132.0/255.0; //Grey
		return result;
	}
	result.r =  1.0;
	result.g =  1.0;
	result.b =  1.0; //white
	return result;
}
SimpleColorer *SimpleColorer::clone () const
{
	return new SimpleColorer (*this);
}
void SimpleColorer::setMaxHeight( double height )
{
	m_MaxHeight = height;
	calcLevels();
}
void SimpleColorer::calcLevels()
{
	m_Snow = m_MaxHeight;
	double part = m_MaxHeight / 4;
	m_Sand = part;
	m_Grass = 2*part;
	m_Stone = 3*part;
}
bool SimpleColorer::grass ( double height ) const
{
	return ( height > m_Sand ) && ( height <= m_Grass );
}
bool SimpleColorer::mountain ( double height ) const
{
	return false; //height > m_Grass;
}
bool SimpleColorer::peak ( double height ) const
{
	return false; //height > m_Stone;
}
double SimpleColorer::getWater () const
{
	return m_Sand - m_MaxHeight / 40;
}

void SimpleColorer::serialize ( Serialization::Serializer &serializer )
{
	serializer.serialize(m_MaxHeight);
	calcLevels();
}

Colorer::Color3f::Color3f ()
    : r(0)
    , g(0)
    , b(0)
{}
void Colorer::Color3f::shade(float shadow)
{
	r -= shadow;
	g -= shadow;
	b -= shadow;
	if ( r<0 ) {
		r = 0;
	} else if (r >1 ) {
		r = 1;
	}
	if ( g<0 ) {
		g = 0;
	} else if (g >1 ) {
		g = 1;
	}
	if ( b<0 ) {
		b = 0;
	} else if (b >1 ) {
		b = 1;
	}
}