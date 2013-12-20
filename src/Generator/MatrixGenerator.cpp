/*
 * MatrixGenerator.hpp
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
#include <cmath>
#include <limits>
#include <iostream>
#include<GL/gl.h>

#include "MatrixGenerator.hpp"
#include "../Chunk/MatrixChunk.hpp"
#include "../Features/Colorer.hpp"
#include "../Features/Objects.hpp"

using namespace Landscape;
using namespace std;

#undef _ExceptionString
#define _ExceptionString "\tMatrixGenerator: "

MatrixGenerator::MatrixGenerator ()
    : m_Position (0,0)
    , m_Size (1)
    , m_Dimension (0)
    , m_MaxValue (100)
    , m_Difference (100)
    , m_pColorer (NULL)
    , m_top (NULL), m_botoom(NULL), m_left(NULL), m_right(NULL)
{}
MatrixGenerator::~MatrixGenerator ()
{}

void MatrixGenerator::setPosition ( const Position &in_Position )
{
	m_Position = in_Position;
}
void MatrixGenerator::setSize ( double in_Size )
{
	m_Size = in_Size;
}
void MatrixGenerator::setDimension ( size_t in_Dimension )
{
	m_Dimension = in_Dimension;
}
void MatrixGenerator::setMaxValue ( size_t in_MaxValue )
{
	m_MaxValue = in_MaxValue;
}
void MatrixGenerator::setDifference (size_t in_Difference )
{
	m_Difference = in_Difference;
}
void MatrixGenerator::setColorer ( const Colorer *in_pColorer)
{
	if ( m_pColorer != NULL ) {
		delete m_pColorer;
	}
	if ( in_pColorer != NULL ) {
		m_pColorer = in_pColorer->clone();
	} else {
		in_pColorer = NULL;
	}
}
void MatrixGenerator::setNeighbours ( const MatrixChunk *in_top,
				      const MatrixChunk *in_botoom,
				      const MatrixChunk *in_left,
				      const MatrixChunk *in_right )
{
	m_top = in_top;
	m_botoom = in_botoom;
	m_left = in_left;
	m_right = in_right;
}
double MatrixGenerator::getMaxHeight() const
{
	return m_MaxValue;
}
void MatrixGenerator::generate (Chunk* chunk, const Position &in_Position) const
{}
Chunk *MatrixGenerator::generate () const
{
	MatrixChunk *chunk = new MatrixChunk(m_Position,m_Size);
	chunk->setMatrix ( m_Dimension, numeric_limits<double>::quiet_NaN() );

	SimpleColorer colorer;
	const Colorer *pColorer;
	if ( m_pColorer == NULL ) {
		colorer.setMaxHeight (m_MaxValue);
		pColorer = &colorer;
	} else {
		pColorer = m_pColorer;
	}
	chunk->setColorer (pColorer);

	if ( m_Dimension == 0 ) {
		return chunk;
	}
	
	double z00, z01,z10,z11;
	int x00 = 0, y00 = 0, x11 =  m_Dimension - 1, y11 = m_Dimension - 1;
	
	if (m_top == NULL ) {
		if (m_left == NULL) {
			if ( (m_botoom == NULL) && (m_right == NULL ) ) {
				z00 = 0;
			} else {
				z00 = ((double)(rand() % ((long)m_MaxValue*100)))/100;
			}
			accept ( chunk, x00, y00, z00 );
		} else {
			z00 = (*m_left)(0, m_Dimension - 1);
		}
	} else {
		if ( m_top->getDimention() != m_Dimension ) {
			throw logic_error ( _ExceptionString "Unsupported top chunk.");
		}
		for ( size_t x = 0 ; x < m_Dimension; ++x ) {
			double Z = (*m_top)(x,m_Dimension - 1);
			(*chunk)(x,0) = Z;
			(*chunk)(x,1) = 2*Z - (*m_top)(x,m_Dimension - 2);
		}
		z00 = (*m_top)(0,m_Dimension - 1);
	}
	
	if (m_right == NULL) {
		if ( m_top == NULL ) {
			z10 = ((double)(rand() % ((long)m_MaxValue*100)))/100;
			accept ( chunk, x11, y00, z10 );
		} else {
			z10 = (*m_top)(m_Dimension - 1,m_Dimension - 1);
		}
	} else {
		if ( m_right->getDimention() != m_Dimension ) {
			throw logic_error ( _ExceptionString "Unsupported right chunk.");
		}
		for ( size_t y = 0 ; y < m_Dimension; ++y ) {
			double Z = (*m_right)(0,y);
			(*chunk)(m_Dimension - 1,y) = Z;
			(*chunk)(m_Dimension - 2,y) = 2*Z - (*m_right)(1,y);
		}
		z10 = (*m_right)(0,0);
	}
	
	if (m_botoom == NULL) {
		if ( m_right == NULL ) {
			z11 = ((double)(rand() % ((long)m_MaxValue*100)))/100;
			accept ( chunk, x11, y11, z11 );
		} else {
			z11 = (*m_right)(0,m_Dimension -1 );
		}
	} else {
		if ( m_botoom->getDimention() != m_Dimension ) {
			throw logic_error ( _ExceptionString "Unsupported bottom chunk.");
		}
		for ( size_t x = 0 ; x < m_Dimension; ++x ) {
			double Z = (*m_botoom)(x,0);
			(*chunk)(x,m_Dimension - 1) = Z;
			(*chunk)(x,m_Dimension - 2) = Z*2 - (*m_botoom)(x,1);
		}
		z11 = (*m_botoom)(m_Dimension - 1, 0 );
	}
	
	if (m_left == NULL) {
		if ( m_botoom == NULL ) {
			z01 = ((double)(rand() % ((long)m_MaxValue*100)))/100;
			accept ( chunk, x00, y11, z01 );
		} else {
			z01 = (*m_botoom)( 0, 0 );
		}
	} else {
		if ( m_left->getDimention() != m_Dimension ) {
			throw logic_error ( _ExceptionString "Unsupported left chunk.");
		}
		for ( size_t y = 0 ; y < m_Dimension; ++y ) {
			double Z = (*m_left)(m_Dimension - 1,y);
			(*chunk)(0,y) = Z;
			(*chunk)(1,y) = 2*Z - (*m_left)(m_Dimension - 2,y);
		}
		z01 = (*m_left)(m_Dimension - 1, m_Dimension - 1);
	}
	
	generate ( chunk,
		   x00, y00,
		   x11, y11,
		   *pColorer );
	return chunk;
}
void MatrixGenerator::generate ( MatrixChunk* chunk,
				 int x00, int y00,
				 int x11, int y11,
				 const Colorer &in_lcolorer ) const
{
	int dX = x11 - x00;
	int dY = y11 - y00;
	if ( (dX <= 1) && (dY <= 1) ) {
		return;
	}
	if ( dX < 0 || dY < 0 ) {
		throw runtime_error ( _ExceptionString "FATAL ERROR in generating");
	}
	double z00 = get (chunk, x00,y00);
	double z01 = get (chunk, x00,y11);
	double z10 = get (chunk, x11,y00);
	double z11 = get (chunk, x11,y11);

	double topRand = get ( chunk, x00 + dX/2, y00);
	if ( topRand != topRand ) { //is NAN
		int devider = ((size_t)fabs(z00-z10) ) + dX*m_Difference;
		double min = (z00 + z10)/2;
		if ( in_lcolorer.grass(min) ) {
			devider /= 3;
		}
		if ( in_lcolorer.mountain(min) ) {
			devider*=2;
			if ( in_lcolorer.peak(min) ) {
				devider*=2;
			}
		}
		if (devider == 0) {
			topRand = min;
		} else {
			topRand = (rand() % devider - devider/2)/100 + min;
		}
		if ( topRand > m_MaxValue ) topRand = m_MaxValue;
		else if ( topRand < 0 ) topRand = 0;
	}

	double bottomRand = get ( chunk, x00 + dX/2, y11);
	if ( bottomRand != bottomRand ) { //is NAN
		int devider = ((size_t)fabs(z01-z11)) + dX*m_Difference;
		double min = (z01 + z11)/2;
		if ( in_lcolorer.grass(min) ) {
			devider /= 3;
		}
		if ( in_lcolorer.mountain(min) ) {
			devider*=2;
			if ( in_lcolorer.peak(min) ) {
				devider*=2;
			}
		}
		if (devider == 0) {
			bottomRand = min;
		} else {
			bottomRand = (rand() % devider - devider/2)/100 + min;
		}
		if ( bottomRand > m_MaxValue ) bottomRand = m_MaxValue;
		else if ( bottomRand < 0 ) bottomRand = 0;
	}

	double leftRand = get ( chunk, x00, y00 + dY/2);
	if ( leftRand != leftRand ) { //is NAN
		int devider = ((size_t)fabs(z00-z01)) + dY*m_Difference;
		double min = (z00 + z01)/2;
		if ( in_lcolorer.grass( min) ) {
			devider /= 3;
		}
		if ( in_lcolorer.mountain(min) ) {
			devider*=2;
			if ( in_lcolorer.peak(min) ) {
				devider*=2;
			}
		}
		if (devider == 0) {
			leftRand = min;
		} else {
			leftRand = (rand() % devider - devider/2)/100 + min;
		}
		if ( leftRand > m_MaxValue ) leftRand = m_MaxValue;
		else if ( leftRand < 0 ) leftRand = 0;
	}

	double rightRand = get ( chunk, x11, y00 + dY/2);
	if ( rightRand != rightRand ) {
		int devider = ((size_t)fabs(z10-z11)) + dY*m_Difference;
		double min = (z10 + z11)/2;
		if ( in_lcolorer.grass(min) ) {
			devider /= 3;
		}
		if ( in_lcolorer.mountain(min) ) {
			devider*=2;
			if ( in_lcolorer.peak(min) ) {
				devider*=2;
			}
		}
		if (devider == 0) {
			rightRand = min;
		} else {
			rightRand = (rand() % devider - devider/2)/100 + min;
		}
		if ( rightRand > m_MaxValue ) rightRand = m_MaxValue;
		else if ( rightRand < 0 ) rightRand = 0;
	}
	
	int dL;
	double min, delta;
	delta = fabs( (topRand - bottomRand) + (leftRand - rightRand) )/2;
	dL = (dX + dY)/2;
	min = ( topRand + bottomRand + leftRand + rightRand )/4;

	double centrRand = get ( chunk, x00 + dX/2, y00 + dY/2);
	if ( centrRand != centrRand ) {
		int devider = ((size_t)delta) + dL*m_Difference;
		if ( in_lcolorer.grass(min) ) {
			devider /= 3;
		}
		if ( in_lcolorer.mountain(min) ) {
			devider*=2;
			if ( in_lcolorer.peak(min) ) {
				devider*=2;
			}
		}
		if (devider == 0) {
			centrRand = min;
		} else {
			centrRand = (rand() % devider - devider/2)/100 + min;
		}
		if ( centrRand > m_MaxValue ) centrRand = m_MaxValue;
		else if ( centrRand < 0 ) centrRand = 0;
	}
	accept (chunk, x00 + dX/2, y00, topRand);
	accept (chunk, x00 + dX/2, y11, bottomRand);
	accept (chunk, x00, y00 + dY/2, leftRand);
	accept (chunk, x11, y00 + dY/2, rightRand);
	accept (chunk, x00 + dX/2, y00 + dY/2, centrRand);

	if ( in_lcolorer.grass(centrRand)) {
		if ( rand()%10000 < 10 )
		{
			chunk->addObject (Tree(Position(), rand()%50 + 50),x00+dX/2, y00+dY/2);
		}
	}
	
	generate ( chunk,
		   x00, y00,
		   x00 + dX/2, y00 + dY/2,
		   in_lcolorer); //top left
	generate ( chunk,
		   x00 + dX/2, y00,
		   x11, y00 + dY/2,
		   in_lcolorer); //top right
	generate ( chunk,
		   x00, y00 + dY/2,
		   x00 + dX/2, y11,
		   in_lcolorer); //bottom left
	generate ( chunk,
		   x00 + dX/2, y00 + dY/2,
		   x11, y11,
		   in_lcolorer); //top right
	
}
double MatrixGenerator::get ( const MatrixChunk* chunk, int x, int y ) const
{
	if ( x < 0 && y < 0 ) {
		if ( m_left != NULL ) {
			return (*m_left)(m_Dimension - 1, 0);
		} else {
			return (*m_top)(0, m_Dimension - 1);
		}
	}
	if ( x >= (long)m_Dimension && y >= (long)m_Dimension ) {
		if ( m_right != NULL )
		return (*m_right)(0, m_Dimension - 1);
	}
	if ( x < 0 ) {
		return (*m_left) (m_Dimension - 1, y);
	}
	if ( x >= (long)m_Dimension ) {
		return (*m_right) (0, y);
	}
	if ( y < 0 ) {
		return (*m_top) (x,m_Dimension - 1);
	}
	if ( y >= (long)m_Dimension ) {
		return (*m_botoom) (x,0);
	}
	return (*chunk)(x,y);
}
void MatrixGenerator::accept ( MatrixChunk* chunk, int x, int y, double z) const
{
	if ( (x >= 0) && (y >= 0) && (x < (long)m_Dimension) && (y < (long)m_Dimension) ) {
		(*chunk)(x,y) = z;
	}
}
Generator *MatrixGenerator::clone () const
{
	return new MatrixGenerator(*this);
}
