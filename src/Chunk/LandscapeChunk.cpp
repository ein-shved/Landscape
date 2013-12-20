/*
 * LandscapeChunk.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 5.11.2012
 */


#include <sstream>
#include <stdexcept>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "LandscapeChunk.hpp"
#include "../Features/Water.hpp"
#include "../Graphics.hpp"
#include "Serializer.hpp"

using namespace Landscape;
using namespace std;

#undef _ExceptionString
#define _ExceptionString "\tLandscapeChunk: "

typedef vector < vector<Chunk *> > ChunkMatrix;
typedef vector<Chunk *> ChunkVector;

LandscapeChunk::LandscapeChunk ( const Position &in_pPosition, const Size &in_sSize )
    : Chunk (in_pPosition, in_sSize)
    , m_pWater(NULL)
    , m_pMoon(NULL)
{}
LandscapeChunk::LandscapeChunk ( double x, double y, double length, double width)
    : Chunk (x,y,length,width)
    , m_pWater(NULL)
    , m_pMoon(NULL)
{}
LandscapeChunk::LandscapeChunk ( const LandscapeChunk &in_other )
    : Chunk (in_other)
    , m_pWater(in_other.m_pWater->clone())
    , m_pMoon(NULL)
{
	m_mMatrix.resize (in_other.m_mMatrix.size());
	for ( size_t x = 0; x < m_mMatrix.size(); ++x ) {
		m_mMatrix[x].resize (in_other.m_mMatrix[x].size());
		for ( size_t y = 0; y < m_mMatrix[x].size(); ++y ) {
			if ( in_other.m_mMatrix[x][y] != NULL ) {
				m_mMatrix[x][y] = in_other.m_mMatrix[x][y]->clone();
			} else {
				m_mMatrix[x][y] = NULL;
			}
		}
	}
}
LandscapeChunk::~LandscapeChunk()
{
	for ( size_t x = 0; x < m_mMatrix.size(); ++x ) {
		for ( size_t y = 0; y < m_mMatrix[x].size(); ++y ) {
			if ( m_mMatrix[x][y] != NULL ) {
				delete m_mMatrix[x][y];
			}
		}
	}
	if ( m_pWater != NULL ) {
		delete m_pWater;
	}
	if (m_pMoon != NULL ) {
		delete m_pMoon;
	}
}

LandscapeChunk* LandscapeChunk::clone() const
{
	return new LandscapeChunk(*this);
}
void LandscapeChunk::draw ( const Graphics &in_lGraphics )
{
	const Camera &lCamera = in_lGraphics.getCamera();
	double vL = in_lGraphics.getViewLength();
	GLfloat mLightPosition[] = { 0, 100,(float)(lCamera.getPosition().y - vL/1.5), 1.0 };
	glLightfv (GL_LIGHT0, GL_POSITION, mLightPosition);
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	if ( in_lGraphics.getCamera().getPosition().z > m_pWater->getPosition().z) {
		if ( m_pWater != NULL && drawWater ) {
			m_pWater->draw(in_lGraphics, this);
		}
	}
	glLightfv (GL_LIGHT0, GL_POSITION, mLightPosition);
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	for ( size_t x = 0; x < m_mMatrix.size(); ++x ) {
		for ( size_t y = 0; y < m_mMatrix[x].size(); ++y ) {
			if ( m_mMatrix[x][y] != NULL ) {
				m_mMatrix[x][y]->draw(in_lGraphics);
			}
		}
	}
	if ( (!drawWater && in_lGraphics.getCamera().getPosition().z > m_pWater->getPosition().z) ||
		drawWater )
	{
		if (m_pMoon != NULL ) {
			m_pMoon->draw(in_lGraphics);
		}
	}
	glLightfv (GL_LIGHT0, GL_POSITION, mLightPosition);
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	if ( in_lGraphics.getCamera().getPosition().z <= m_pWater->getPosition().z) {
		if ( m_pWater != NULL && drawWater ) {
			m_pWater->draw(in_lGraphics, this);
		}
	}
// 	if ( m_pWater != NULL && drawWater ) {
// 			m_pWater->draw(in_lGraphics, this);
// 		}
}
double LandscapeChunk::height (double x, double y) const
{
	const Chunk *pChunk = chunkFrom(x,y);
	if ( pChunk == NULL ) {
		return m_pPosition.z;
	}
	return pChunk->height(x,y);
}
double LandscapeChunk::absoluteHeight (double x, double y) const
{
	x = x/oneChunkSize.length;
	y = y/oneChunkSize.width;
	if ( x<0 || y<0 || x>=(long)m_mMatrix.size() || y>=(long)m_mMatrix[x].size() ) {
		return m_pPosition.z;
	}
	const Position &pos = m_mMatrix[x][y]->getPosition();
	return m_mMatrix[x][y]->absoluteHeight(x - pos.x,y-pos.y);
}

void LandscapeChunk::resize ()
{
	if ( m_mMatrix.size() == 0 ) {
		return;
	}
	if ( m_pWater != NULL )
	{
		Position waterPos = m_pWater->getPosition();
		m_pWater->setSize ( waterPos, m_sSize );
	}
	m_sSize.length = oneChunkSize.length*m_mMatrix.size();
	size_t max = m_mMatrix[0].size();
	for ( size_t x=1; x<m_mMatrix.size(); ++x ) {
		if ( m_mMatrix[x].size() > max ) {
			max = m_mMatrix[x].size();
		}
	}
	m_sSize.width = oneChunkSize.width*max;
}

void LandscapeChunk::add ( Chunk *in_chunk, int &in_x, int &in_y)
{
	Position oldPosition = m_pPosition;
	if (in_chunk == NULL) {
		throw logic_error ( _ExceptionString "Trying to add NULL chunk" );
	}
	if ( m_mMatrix.empty() ) {
		oneChunkSize = in_chunk->getSize();
	}

	if ( in_chunk->getSize() != oneChunkSize ) {
		throw logic_error ( _ExceptionString "Wrong new chunk size" );
	}

	if ( in_x < -1 || in_y < -1 || in_x > (int)m_mMatrix.size() ) {
		stringstream strs;
		strs << _ExceptionString "Wrong indexing: (" <<in_x<<","<<in_y<<")";
		throw logic_error ( strs.str() );
	}

	Position chunkPos = Position (m_pPosition.x + oneChunkSize.length*in_x,
				      m_pPosition.y + oneChunkSize.width*in_y);

	in_chunk->setPosition(chunkPos);

	if ( in_x == -1 ) {
		m_mMatrix.insert ( m_mMatrix.begin(), vector<Chunk *>((size_t)(in_y+1),NULL) );
		++in_x;
	}
	if ( in_y == -1 ) {
		for ( int i = 0; i < (int)m_mMatrix.size(); ++i ) {
			m_mMatrix[i].insert(m_mMatrix[i].begin(), NULL);
		}
		++in_y;
	}
	if (in_x == 0 && in_y == 0) {
		m_pPosition = chunkPos;
	}
	if ( in_x == (int)m_mMatrix.size() ) {
		m_mMatrix.push_back ( ChunkVector((size_t)(in_y+1),NULL) );
	}
	if ( in_y > (int)m_mMatrix[in_x].size() ) {
		stringstream strs;
		strs << _ExceptionString "Wrong indexing: (" <<in_x<<","<<in_y<<")";
		throw logic_error ( strs.str() );
	}
	if ( in_y == (int)m_mMatrix[in_x].size() ) {
		m_mMatrix[in_x].push_back ( NULL );
	}
	if ( m_mMatrix[in_x][in_y] != NULL ) {
		delete m_mMatrix[in_x][in_y];
	}
	m_mMatrix[in_x][in_y] = in_chunk;
	resize();
	if ( m_pWater != NULL ) {
		double height = m_pPosition.z - oldPosition.z;
		oldPosition = m_pWater->getPosition();
		oldPosition.x = m_pPosition.x;
		oldPosition.y = m_pPosition.y;
		oldPosition.z += height;
		m_pWater->setSize ( oldPosition, m_sSize );
	}
}
void LandscapeChunk::setWater ( const Water *in_pWater )
{
	if ( m_pWater != NULL ) {
		delete m_pWater;
	}
	if ( in_pWater != NULL ) {
		m_pWater = in_pWater->clone();
		Position waterPos = m_pWater->getPosition();
		waterPos.x = m_pPosition.x;
		waterPos.y = m_pPosition.y;
		m_pWater->setSize ( waterPos, m_sSize );
	} else {
		in_pWater = NULL;
	}
}
void LandscapeChunk::setMoon ( const std::string &path)
{
	if (m_pMoon != NULL) {
		delete m_pMoon;
	}
	m_pMoon = new Moon (path);
}

void LandscapeChunk::chunkFrom ( const Position &in_pos, int &x, int &y) const
{
	x = (in_pos.x - m_pPosition.x)/oneChunkSize.length;
	y = (in_pos.y - m_pPosition.y)/oneChunkSize.width;
}
const Chunk *LandscapeChunk::chunkFrom ( double dX, double dY ) const
{
	int x=0, y=0;
	chunkFrom (Position(dX,dY),x,y);
	if ( x<0 || y<0 || x>=(long)m_mMatrix.size() || y>=(long)m_mMatrix[x].size() ) {
		return NULL;
	}
	return m_mMatrix[x][y];
}
const Chunk *LandscapeChunk::chunkFrom ( const Position &in_pos ) const
{
	return chunkFrom(in_pos.x, in_pos.y);
}
const Chunk *LandscapeChunk::operator () ( size_t x, size_t y) const
{
	if ( x < 0 || y < 0 || x >= m_mMatrix.size() || y >= m_mMatrix[x].size()) {
		return NULL;
	}
	return m_mMatrix[x][y];
}

void LandscapeChunk::serialize ( Serializer &serializer)
{
	Chunk::serialize (serializer);
	serializer.serialize (oneChunkSize.length);
	serializer.serialize (oneChunkSize.width);
	serializer.serializeWater (m_pWater);
	long length = m_mMatrix.size();
	serializer.serialize ( length );
	m_mMatrix.resize (length);
	for ( long x = 0; x<length; ++x ) {
		long width = m_mMatrix[x].size();
		serializer.serialize (width);
		m_mMatrix[x].resize (width);
		for ( long y =0; y < width; ++y ) {
			serializer.serializeChunk ( m_mMatrix[x][y] );
		}
	}

	long bMoon = 0;
	if ( m_pMoon != NULL ) {
		bMoon = 1;
	}
	serializer.serialize(bMoon);
	if ( bMoon == 1 ) {
		if ( m_pMoon == NULL ) {
			m_pMoon = new Moon();
			serializer.serialize(m_pMoon->m_path);
		} else {
			serializer.serialize(m_pMoon->m_path);
		}
	} else {
		if ( m_pMoon != NULL ) {
			delete m_pMoon;
			m_pMoon = NULL;
		}
	}
}

LandscapeChunk::Moon::Moon (const std::string &path)
    : m_Texture(0)
    , m_path (path)
    , m_bLoaded (false)
{}
LandscapeChunk::Moon::Moon ()
    : m_Texture(0)
    , m_bLoaded (false)
{}
void LandscapeChunk::Moon::loadTexture()
{
	SDL_Surface *texture = SDL_LoadBMP (m_path.c_str());
	if ( texture == NULL ) {
		throw logic_error (_ExceptionString "Can not load moot texture." );
	}

	GLenum texture_format;
	GLint nOfColors = texture->format->BytesPerPixel;
        if (nOfColors == 4)
        {
                if (texture->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)
        {
                if (texture->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
		SDL_FreeSurface ( texture);
		throw logic_error (_ExceptionString "Can not load moot texture." );
        }

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	SDL_LockSurface (texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->h, texture->w, 0, texture_format, GL_UNSIGNED_BYTE, texture->pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SDL_UnlockSurface (texture);
	SDL_FreeSurface ( texture);
	m_bLoaded = true;
	glBindTexture(GL_TEXTURE_2D, 0);
}
#ifndef PI
# define PI 3.14159265359
#endif //PI
void LandscapeChunk::Moon::draw (const Graphics &in_lGraphics)
{
	if (!m_bLoaded) {
		loadTexture();
	}
	
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	const Camera &lCamera = in_lGraphics.getCamera();
	double vL = in_lGraphics.getViewLength();

	glDisable (GL_LIGHTING);
	
	glFogf(GL_FOG_DENSITY, 1);
	glFogf(GL_FOG_START, in_lGraphics.getViewLength()/10);
	glFogf(GL_FOG_END, in_lGraphics.getViewLength());
	
	glTranslated (lCamera.getPosition().x,lCamera.getPosition().z,lCamera.getPosition().y - vL/1.5);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glColor3f(1,1,1);

	glBegin(GL_QUADS);

	glTexCoord2i(1, 0); glVertex2f(  1000.0f,  1000.0f );
	glTexCoord2i(0, 0); glVertex2f( -1000.0f,  1000.0f );
	glTexCoord2i(0, 1); glVertex2f( -1000.0f, -1000.0f );
	glTexCoord2i(1, 1); glVertex2f(  1000.0f, -1000.0f );

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable (GL_LIGHTING);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	in_lGraphics.drawFog();
	
}
