/*
 * LandscapeGenerator.cpp
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

#include "LandscapeGenerator.hpp"
#include "MatrixGenerator.hpp"
#include "../Chunk/LandscapeChunk.hpp"
#include "../Chunk/MatrixChunk.hpp"
#include "../Chunk/Serializer.hpp"
#include "../Features/Water.hpp"

using namespace Landscape;
using namespace std;

#undef _ExceptionString
#define _ExceptionString "\tLandscapeGenerator: "

LandscapeGenerator::LandscapeGenerator ()
    : m_chunkSize (1000)
    , m_chunkDim (100)
    , m_maxHeight (200)
    , m_maxDiff (400)
    , m_Position (Position (0,0))
    , m_xNumber (1)
    , m_yNumber (1)
    , m_pColorer (NULL)
    , m_pWater (NULL)
    , m_bMoon (false)
    , m_pSerializer (NULL)
{}
LandscapeGenerator::~LandscapeGenerator ()
{}

void LandscapeGenerator::setOneSize ( double in_size )
{
	if (in_size <= 0) {
		throw logic_error (_ExceptionString "Wrong size param.");
	}
	m_chunkSize = in_size;
}
void LandscapeGenerator::setOneDimension ( size_t in_dim )
{
	m_chunkDim = in_dim;
}
void LandscapeGenerator::setMaxHeight ( unsigned int in_maxHeight )
{
	m_maxHeight = in_maxHeight;
}
void LandscapeGenerator::setMaxDifference ( unsigned int in_maxDiff )
{
	m_maxDiff = in_maxDiff;
}
void LandscapeGenerator::setPosition ( const Position &in_Position )
{
	m_Position = in_Position;
}
void LandscapeGenerator::setChunksNumber (size_t x, size_t y)
{
	m_xNumber = x;
	m_yNumber = y;
}
void LandscapeGenerator::setWater ( const Water *in_pWater)
{
	if ( m_pWater != NULL ) {
		delete m_pWater;
	}
	if ( in_pWater != NULL ) {
		m_pWater = in_pWater->clone();
	} else {
		in_pWater = NULL;
	}
}
void LandscapeGenerator::setColorer ( const Colorer *in_pColorer)
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
void LandscapeGenerator::setMoon ( const std::string &path )
{
	m_MoonPath = path;
	m_bMoon = true;
}

void LandscapeGenerator::setSerializer ( Serializer &in_serializer)
{
	m_pSerializer = &in_serializer;
}

Chunk *LandscapeGenerator::generate () const
{
	if ( m_pSerializer != NULL ) {
		Chunk *chank;
		m_pSerializer->serializeChunk (chank);
		return chank;
	}
	MatrixGenerator matrixGen;
	matrixGen.setPosition ( m_Position );
	matrixGen.setSize ( m_chunkSize );
	matrixGen.setDimension ( m_chunkDim );
	matrixGen.setMaxValue( m_maxHeight );

	LandscapeChunk *chunk = new LandscapeChunk();
	if (m_bMoon) {
		chunk->setMoon (m_MoonPath);
	}

	//Немного быдлокода
	if ( m_pColorer != NULL ) {
		if (m_pWater != NULL ) {
			Position waterPos = m_pWater->getPosition();
			waterPos.z = m_pColorer->getWater();
			m_pWater->setSize ( waterPos, Size(m_chunkSize, m_chunkSize) );
			chunk->setWater (m_pWater);
		} else {
			SimpleWater water;
			Position waterPos = m_Position;
			waterPos.z = m_pColorer->getWater();
			water.setSize ( waterPos, Size(m_chunkSize, m_chunkSize));
			chunk->setWater (&water);
		}
		matrixGen.setColorer (m_pColorer);
	} else {
		SimpleColorer colorer;
		colorer.setMaxHeight(m_maxHeight);
		if (m_pWater != NULL ) {
			Position waterPos = m_pWater->getPosition();
			waterPos.z = colorer.getWater();
			m_pWater->setSize ( waterPos, Size(m_chunkSize, m_chunkSize) );
			chunk->setWater (m_pWater);
		} else {
			SimpleWater water;
			Position waterPos = m_Position;
			waterPos.z = colorer.getWater();
			water.setSize ( waterPos, Size(m_chunkSize, m_chunkSize));
			chunk->setWater (&water);
		}
		matrixGen.setColorer (&colorer);
	}

	for (int y = 0; y < (long)m_yNumber; ++y) {
		for (int x = 0; x < (long)m_xNumber; ++x) {
			const MatrixChunk *left = NULL;
			if ( x > 0 ) {
				left = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y) );
			}
			const MatrixChunk *top = NULL;
			if ( y > 0 ) {
				top = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y-1) );
			}
			matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
			matrixGen.setNeighbours(top,NULL,left,NULL);
			chunk->add(matrixGen.generate(),x,y);
		}
	}
	return chunk;
}
double LandscapeGenerator::getMaxHeight() const
{
	return m_maxHeight;
}
void LandscapeGenerator::generate (Chunk* in_chunk, const Position &in_Position) const
{
	LandscapeChunk *chunk = dynamic_cast<LandscapeChunk *> (in_chunk);
	if ( in_chunk == NULL ) {
		return;
	}
	int x,y;
	chunk->chunkFrom(in_Position,x,y);

	MatrixGenerator matrixGen;
	matrixGen.setPosition ( m_Position );
	matrixGen.setSize ( m_chunkSize );
	matrixGen.setDimension ( m_chunkDim );
	matrixGen.setMaxValue( m_maxHeight );

	if ( (*chunk)(x-1,y) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		//Its okay to be NULL
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y+1) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y-1) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x-2,y) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),--x,y);
		++x;
	}
	if ( (*chunk)(x+1,y) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y+1) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y-1) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x+2,y) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),++x,y);
		--x;
	}
	if ( (*chunk)(x,y-1) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y-2) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y-1) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y-1) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),x,--y);
		++y;
	}
	if ( (*chunk)(x,y+1) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y+2) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y+1) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y+1) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),x,++y);
		--y;
	}
	if ( (*chunk)(x-1,y-1) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y-2) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y-1) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x-2,y-1) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),--x,--y);
		++x;
		++y;
	}
	if ( (*chunk)(x+1,y-1) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y-2) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x+2,y-1) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y-1) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),++x,--y);
		--x;
		++y;
	}
	if ( (*chunk)(x-1,y+1) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y+2) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x-1,y) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y+1) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x-2,y+1) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),--x,++y);
	}
	if ( (*chunk)(x+1,y+1) == NULL ) {
		matrixGen.setDifference ( m_maxDiff/2 + (rand() % (m_maxDiff/2)) );
		const MatrixChunk *top = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y+2) );
		const MatrixChunk *botom = dynamic_cast<const MatrixChunk *>( (*chunk)(x+1,y) );
		const MatrixChunk *right = dynamic_cast<const MatrixChunk *>( (*chunk)(x+2,y+1) );
		const MatrixChunk *left = dynamic_cast<const MatrixChunk *>( (*chunk)(x,y+1) );
		matrixGen.setNeighbours(top,botom,left,right);
		chunk->add (matrixGen.generate(),++x,++y);
		--x;
		--y;
	}
}
Generator *LandscapeGenerator::clone () const
{
	return new LandscapeGenerator(*this);
}