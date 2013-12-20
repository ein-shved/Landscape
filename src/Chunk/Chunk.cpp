/*
 * Chunk.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */

#include "Chunk.hpp"
#include "Serializer.hpp"

using namespace Landscape;
using namespace std;

#ifndef NULL
# define NULL 0
#endif

#ifndef PI
# define PI 3.14
#endif

Chunk::Chunk ( const Position &in_pPosition, const Size &in_sSize )
    : m_pPosition (in_pPosition)
    , m_sSize (in_sSize)
    , drawWater (true)
{}
Chunk::Chunk ( double x, double y, double length, double width )
    : m_pPosition ( x,y,0 )
    , m_sSize ( length,width )
    , drawWater (true)
{}
Chunk::~Chunk ()
{
}
double Chunk::height (const Position &in_Position ) const
{
	return height (in_Position.x, in_Position.y);
}
const Position &Chunk::getPosition () const
{
	return m_pPosition;
}
const Size &Chunk::getSize () const
{
	return m_sSize;
}
void Chunk::setSize ( const Size &in_sSize )
{
	m_sSize = in_sSize;
}
void Chunk::setPosition ( const Position &in_pPosition )
{
	m_pPosition = in_pPosition;
}
void Chunk::setPosition ( double x, double y, double z )
{
	this->setPosition(Position(x,y,z));
}
void Chunk::serialize ( Serializer &serializer)
{
	serializer.serialize (m_pPosition.x);
	serializer.serialize (m_pPosition.y);
	serializer.serialize (m_pPosition.z);

	serializer.serialize (m_sSize.length);
	serializer.serialize (m_sSize.width);
}
Chunk::Sun::Sun()
    : dAngleZ (PI/4)
    , dAngleXY (0)
{}