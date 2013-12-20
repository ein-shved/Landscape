/*
 * MatrixChunk.cpp
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
#include <sstream>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cmath>

#include "MatrixChunk.hpp"
#include "../Graphics.hpp"
#include "../Features/Colorer.hpp"
#include "../Features/Objects.hpp"
#include "Serializer.hpp"

#undef _ExceptionString
#define _ExceptionString "\tMatrixChunk: "

using namespace Landscape;
using namespace std;

static bool loadedARB = false;

#if defined (_WIN32) || defined(_WIN64)
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);
#endif

PFNGLGENBUFFERSARBPROC		glGenBuffersARB;
PFNGLBINDBUFFERARBPROC		glBindBufferARB;
PFNGLBUFFERDATAARBPROC		glBufferDataARB;
PFNGLDELETEBUFFERSARBPROC	glDeleteBuffersARB;

static void loadARBFunctions () {
	if ( !loadedARB ) {
#	ifdef __linux__
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) glXGetProcAddress((const GLubyte*)"glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) glXGetProcAddress((const GLubyte*)"glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) glXGetProcAddress((const GLubyte*)"glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) glXGetProcAddress((const GLubyte*)"glDeleteBuffersARB");
#	elif defined _WIN32 || _WIN64
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress((LPCSTR)"glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress((LPCSTR)"glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress((LPCSTR)"glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress((LPCSTR)"glDeleteBuffersARB");

#		endif
		loadedARB = true;
	}
}

MatrixChunk::MatrixChunk ( const Position &in_pPosition, double in_size )
    : Chunk ( in_pPosition, Size(in_size,in_size) )
    , lDelta (1)
    , wDelta (1)
    , m_pColorer (NULL)
    , m_viewLength (0)
    , m_bigViewLength (0)
    , m_Viewable(true)
    , m_vboId (0)
{
	loadARBFunctions ();
}
MatrixChunk::MatrixChunk ( double x, double y, double in_size )
    : Chunk ( x,y, in_size,in_size )
    , lDelta (1)
    , wDelta (1)
    , m_pColorer (NULL)
    , m_viewLength (0)
    , m_bigViewLength (0)
    , m_Viewable(true)
    , m_vboId (0)
{
	loadARBFunctions ();
}
MatrixChunk::MatrixChunk ( const MatrixChunk &in_other)
    : Chunk ( in_other )
    , m_mMatrix ( in_other.m_mMatrix)
    , lDelta (in_other.lDelta)
    , wDelta (in_other.wDelta)
    , m_pColorer (in_other.m_pColorer->clone())
    , m_viewLength (0)
    , m_bigViewLength (0)
    , m_Viewable(true)
    , m_vboId (0)
{
	loadARBFunctions ();
}
MatrixChunk::~MatrixChunk ()
{
	if ( m_pColorer != NULL ) {
		delete m_pColorer;
	}
	for (size_t i = 0; i< m_mObjects.size(); ++i ) {
		delete m_mObjects[i];
	}
	deallocVBO();
}

MatrixChunk *MatrixChunk::clone () const
{
	return new MatrixChunk(*this);
}
void MatrixChunk::draw ( const Graphics &in_lGraphics  )
{
	size_t length = m_mMatrix.size();
	if ( length == 0 ) return;
	size_t width = m_mMatrix[0].size();

	double viewLength = in_lGraphics.getViewLength();
	if ( m_viewLength != viewLength ) {
		m_viewLength = viewLength;
		calcBigView();
	}
	m_Viewable = viewable(in_lGraphics.getCamera().getPosition() ); 

	if ( !m_Viewable ) {
		return;
	}
	allocVBO(in_lGraphics);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboIdColors);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboIdNormals);
	glNormalPointer(GL_FLOAT, 0, 0);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0,(length + 1)*(width - 1)*2);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	size_t objectMount = m_mObjects.size();
	for (size_t i =0; i< objectMount; ++i ){
		m_mObjects[i]->draw(in_lGraphics);
	}
}

double MatrixChunk::height (double in_dX, double in_dY) const
{
	return absoluteHeight (in_dX - m_pPosition.x, in_dY - m_pPosition.y);
}
double MatrixChunk::absoluteHeight (double dX, double dY) const
{
	dX/=lDelta;
	dY/=wDelta;
	size_t uX = dX, uY = dY;
	if ( uX < 0 ) { uX = 0; dX = 0; }
	else if (uX >= m_mMatrix.size() ) {
		uX = m_mMatrix.size() -1;
		dX = uX;
	}
	if (uY < 0 ) { uY = 0; dY = 0; }
	else if ( uY >= m_mMatrix[0].size() ) {
		uY = m_mMatrix[0].size() -1;
		dY = uY;
	}

	double dF00 = m_mMatrix[uX][uY];
	double dF01 = (uX + 1 >=  m_mMatrix.size()) ? dF00 : m_mMatrix[uX + 1][uY];
	double dF10 = (uY + 1 >=  m_mMatrix[0].size()) ? dF00 : m_mMatrix[uX][uY + 1];
	double dF11 = (uX + 1 >=  m_mMatrix.size()) ? dF10 : m_mMatrix[uX + 1][uY + 1];

	return 	((double)uY + 1 - dY) * ( ((double)uX + 1 - dX) * dF00 + (dX - (double)uX) * dF10 ) +
		(dY - (double)uY) * ( ((double)uX + 1 - dX) * dF01 + (dX - (double)uX) * dF11 );
}

void MatrixChunk::setMatrix ( size_t in_dimension )
{
	deallocVBO();
	m_mMatrix.resize (in_dimension);
	for ( size_t i = 0; i < in_dimension; ++i ) {
		m_mMatrix[i].resize (in_dimension);
	}
	lDelta = m_sSize.length/(in_dimension -1);
	wDelta = m_sSize.width/(in_dimension -1);
}
void MatrixChunk::setMatrix ( size_t in_dimension, double in_default )
{
	deallocVBO();
	m_mMatrix.clear ();
	m_mMatrix.resize (in_dimension);
	for ( size_t i = 0; i < in_dimension; ++i ) {
		m_mMatrix[i].resize (in_dimension, in_default);
	}
	lDelta = m_sSize.length/(in_dimension-1);
	wDelta = m_sSize.width/(in_dimension-1);
}
void MatrixChunk::setMatrix ( const DoubleMatrix &in_mMatrix )
{
	deallocVBO();
	m_mMatrix = in_mMatrix;

	size_t length = m_mMatrix.size();
	lDelta = m_sSize.length/(length-1);
	
	if ( length == 0 ) {
		wDelta = 1;
		return;
	}
	size_t width = m_mMatrix[0].size();
	wDelta = m_sSize.width/(width-1);
	
}
void MatrixChunk::setColorer ( const Colorer *in_pColorer)
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

size_t MatrixChunk::getDimention () const
{
	return m_mMatrix.size();
}
const double &MatrixChunk::operator () (size_t x, size_t y) const
{
	return m_mMatrix[x][y];
}
double &MatrixChunk::operator () (size_t x, size_t y)
{
	deallocVBO();
	return m_mMatrix[x][y];
}

void MatrixChunk::addObject ( const Object &in_Object, int x, int y)
{
	m_mObjects.push_back(in_Object.clone());
	if ( x > 0 || y > 0 ) {
		m_mObjects.back()->setPosition(Position( m_pPosition.x + x*lDelta,
							 m_pPosition.y + y*wDelta,
							 m_pPosition.z + (*this)(x,y) - 1) );
	}
}

void MatrixChunk::setSize ( const Size &in_sSize )
{
	deallocVBO();
	Chunk::setSize (in_sSize);
	
	size_t length = m_mMatrix.size();
	lDelta = m_sSize.length/(length-1);
	if ( length == 0 ) {
		wDelta = 1;
		return;
	}
	size_t width = m_mMatrix[0].size();
	wDelta = m_sSize.width/(width-1);
}
void MatrixChunk::setPosition ( const Position &in_pPosition )
{
	deallocVBO();
	double dX = in_pPosition.x - m_pPosition.x;
	double dY = in_pPosition.y - m_pPosition.y;
	double dZ = in_pPosition.z - m_pPosition.z;
	size_t objectMount = m_mObjects.size();
	for (size_t i =0; i< objectMount; ++i ){
		Position pos = m_mObjects[i]->getPosition();
		pos.x += dX;
		pos.y += dY;
		pos.z += dZ;
		m_mObjects[i]->setPosition(pos);
	}
	m_pPosition = in_pPosition;
}

static void normal ( float x00, float y00, float z00,
		     float x01, float y01, float z01,
		     float x10, float y10, float z10,
		     float x11, float y11, float z11,
		     float &out_x, float &out_y, float &out_z )
{
	x01 -= x00; y01 -= y00; z01 -= z00;
	x11 -= x10; y11 -= y10; z11 -= z10;
	out_x = y01*z11 - z01*y11;
	out_y = z01*x11 - x01*z11;
	out_z = x01*y11 - y01*x11;

	float l = sqrt ( out_x*out_x + out_y*out_y + out_z*out_z);
	out_x/=l;
	out_y/=l;
	out_z/=l;
}
void MatrixChunk::allocVBO ( const Graphics &in_lGraphics ) const
{
	if (m_vboId != 0) {
		return;
	}
	if ( m_mMatrix.size() == 0) {
		return;
	}
	size_t length = m_mMatrix.size();
	if ( length == 0 ) return;
	size_t width = m_mMatrix[0].size();

	vector < vector<bool> > mShadows( length, vector<bool>(width,false) );
	/* for ( size_t y = 0; y < width; ++y ) {
		for ( size_t x = 0; x < length; ++x ) {
			mShadows[x][y] = shadowed (x,y,m_mMatrix[x][y], in_lGraphics.getChunk(), in_lGraphics.getMaxHeight());
		}
	}*/

	GLuint *pVboId = const_cast<GLuint *> (&m_vboId);
	GLuint *pVboIdColors = const_cast<GLuint *> (&m_vboIdColors);
	GLuint *pVboIdNormals = const_cast<GLuint *> (&m_vboIdNormals);
	if ( pVboId == NULL || pVboIdColors == NULL || pVboIdNormals == NULL ) {
		throw logic_error ( _ExceptionString "Can not alloc VBO" );
	}
	glGenBuffersARB(1, pVboId);
	glGenBuffersARB(1, pVboIdColors);
	glGenBuffersARB(1, pVboIdNormals);

	//initialise vertexes and collors
	size_t vertexNumber = (length + 1)*(width - 1);
	vector<float>vVertexes(2*3*vertexNumber,0.0);
	vector<float>vColors(2*3*vertexNumber,0.0);
	vector<float>vNormals(2*3*vertexNumber,0.0);

	std::vector< std::vector <Normal> > mNormals;
	std::vector< std::vector <Normal> > &lmNormals = const_cast <std::vector< std::vector <Normal> > &> (mNormals);
	lmNormals.clear();
	lmNormals.resize (width, vector<Normal>(length));
	
	for ( size_t y = 0; y < width - 1; ++y ) {
		for ( size_t _x = 0; _x < length; ++_x ) {
			size_t x = _x;
			if ( y % 2 == 1 ) {
				x = length - 1 - _x;
			}
			double val = m_mMatrix[x][y];
			if ( val != val ) {
				stringstream strs;
				strs << _ExceptionString "NaN specified on (" <<x<<","<<y<<")";
				throw logic_error ( strs.str() );
			}
			float dX = x*lDelta + m_pPosition.x;
			float dY = y*wDelta + m_pPosition.y;

			float z0 = m_mMatrix[x][y];
			float z1 = m_mMatrix[x][y+1];
			
			float z00, z01, z10, z11;
			float z20, z21, z30, z31;

			z01 = m_mMatrix[x][y+1];
			if ( y > 0 ) {
				z00 = m_mMatrix[x][y-1];
			} else {
				z00 = 2*z0 - z01;//in_lGraphics.getChunk()->height (dX, dY - wDelta);
			}
			z20 = m_mMatrix[x][y];
			if ( y < length - 2 ) {
				z21 = m_mMatrix[x][y+2];
			} else {
				z21 = 2*z1 - z20;//in_lGraphics.getChunk()->height (dX, dY + 2*wDelta);
			}
			if ( x > 0 ) {
				z10 = m_mMatrix[x-1][y];
				z30 = m_mMatrix[x-1][y+1];
			} else {
				z10 = 2*z0 - m_mMatrix[x+1][y];//in_lGraphics.getChunk()->height (dX - lDelta, dY);
				z30 = 2*z1 - m_mMatrix[x+1][y+1];//in_lGraphics.getChunk()->height (dX - lDelta, dY + wDelta);
			}
			if ( x < width - 1 ) {
				z11 = m_mMatrix[x+1][y];
				z31 = m_mMatrix[x+1][y+1];
			} else {
				z11 = 2*z0 - z10;//in_lGraphics.getChunk()->height (dX + lDelta, dY);
				z31 = 2*z1 - z30;//in_lGraphics.getChunk()->height (dX + lDelta, dY + wDelta);
			}

			Colorer::Color3f color;
			if (m_pColorer != NULL ) {
				color = m_pColorer->getColor(z1);
			}
			if ( mShadows[x][y+1] ) {
				color.shade(0.1);
			}
			vColors[(y*(length + 1) + _x)*6] = color.r;
			vColors[(y*(length + 1) + _x)*6 + 1] = color.g;
			vColors[(y*(length + 1) + _x)*6 + 2] = color.b;

			vVertexes [(y*(length + 1) + _x)*6] = dX;
			vVertexes [(y*(length + 1) + _x)*6 + 1] = z1;
			vVertexes [(y*(length + 1) + _x)*6 + 2] = dY + wDelta;

			normal ( dX, dY+2*wDelta, z21,
				 dX, dY, z20,
				 dX - lDelta, dY + wDelta, z30,
				 dX + lDelta, dY + wDelta, z31,
				 lmNormals[x][y+1].x, lmNormals[x][y+1].y, lmNormals[x][y+1].z);
			vNormals [(y*(length + 1) + _x)*6] = lmNormals[x][y+1].x;
			vNormals [(y*(length + 1) + _x)*6 + 1] = lmNormals[x][y+1].z;
			vNormals [(y*(length + 1) + _x)*6 + 2] = lmNormals[x][y+1].y;

			if ( _x == length - 1){
				vColors[(y*(length + 1) + _x)*6 + 6] = color.r;
				vColors[(y*(length + 1) + _x)*6 + 7] = color.g;
				vColors[(y*(length + 1) + _x)*6 + 8] = color.b;

				vVertexes [(y*(length + 1) + _x)*6 + 6] = dX;
				vVertexes [(y*(length + 1) + _x)*6 + 7] = z1;
				vVertexes [(y*(length + 1) + _x)*6 + 8] = dY + wDelta;

				vNormals [(y*(length + 1) + _x)*6 + 6] = lmNormals[x][y+1].x;
				vNormals [(y*(length + 1) + _x)*6 + 7] = lmNormals[x][y+1].z;
				vNormals [(y*(length + 1) + _x)*6 + 8] = lmNormals[x][y+1].y;

				vColors[(y*(length + 1) + _x)*6 + 9] = color.r;
				vColors[(y*(length + 1) + _x)*6 + 10] = color.g;
				vColors[(y*(length + 1) + _x)*6 + 11] = color.b;

				vVertexes [(y*(length + 1) + _x)*6 + 9] = dX;
				vVertexes [(y*(length + 1) + _x)*6 + 10] = z1;
				vVertexes [(y*(length + 1) + _x)*6 + 11] = dY + wDelta;

				vNormals [(y*(length + 1) + _x)*6 + 9] = lmNormals[x][y+1].x;
				vNormals [(y*(length + 1) + _x)*6 + 10] = lmNormals[x][y+1].z;
				vNormals [(y*(length + 1) + _x)*6 + 11] = lmNormals[x][y+1].y;
			}
			
			if (m_pColorer != NULL ) {
				color = m_pColorer->getColor(z0);
			}
			if ( mShadows[x][y]) {
				color.shade(0.1);
			}

			vColors[(y*(length + 1) + _x)*6 + 3] = color.r;
			vColors[(y*(length + 1) + _x)*6 + 4] = color.g;
			vColors[(y*(length + 1) + _x)*6 + 5] = color.b;
		

			vVertexes [(y*(length + 1) + _x)*6 + 3] = dX;
			vVertexes [(y*(length + 1) + _x)*6 + 4] = z0;
			vVertexes [(y*(length + 1) + _x)*6 + 5] = dY;

			if ( y == 0 ) {
				normal ( dX, dY + wDelta, z01,
					 dX, dY - wDelta, z00,
					 dX - lDelta, dY, z10,
					 dX + lDelta, dY, z11,
					 lmNormals[x][y].x, lmNormals[x][y].y, lmNormals[x][y].z);
			}
			vNormals [(y*(length + 1) + _x)*6 + 3] = lmNormals[x][y].x;
			vNormals [(y*(length + 1) + _x)*6 + 4] = lmNormals[x][y].z;
			vNormals [(y*(length + 1) + _x)*6 + 5] = lmNormals[x][y].y;
		}
	}

	//load vertexes
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*3*vertexNumber*sizeof(float), &(vVertexes[0]), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	//load colors
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboIdColors);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*3*vertexNumber*sizeof(float), &(vColors[0]), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	//load normals
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboIdNormals);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2*3*vertexNumber*sizeof(float), &(vNormals[0]), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}
void MatrixChunk::deallocVBO()
{
	if (m_vboId == 0) {
		return;
	}
	glDeleteBuffersARB(1, &m_vboId);
	glDeleteBuffersARB(1, &m_vboIdColors);
	glDeleteBuffersARB(1, &m_vboIdNormals);
}

void MatrixChunk::calcBigView()
{
	double size = m_sSize.width/2; //==length;
	m_bigViewLength = m_viewLength/cos(acos(size/m_viewLength));
}
bool MatrixChunk::viewable (const Position &in_Pos) const
{
	//(x0,y0)
	double x = m_pPosition.x - in_Pos.x;
	double y = m_pPosition.y - in_Pos.y;
	x*=x;
	y*=y;
	double length = sqrt(x+y);
	if ( m_bigViewLength > length ) {
		return true;
	}

	//(x1,y0)
	x = m_pPosition.x + m_sSize.length - in_Pos.x; x*=x;
	y = m_pPosition.y - in_Pos.y; y*=y;
	length = sqrt(x+y);
	if ( m_bigViewLength > length ) {
		return true;
	}

	//(x0,y1)
	x = m_pPosition.x - in_Pos.x; x*=x;
	y = m_pPosition.y + m_sSize.width - in_Pos.y; y*=y;
	length = sqrt(x+y);
	if ( m_bigViewLength > length ) {
		return true;
	}

	//(x1,y1)
	x = m_pPosition.x + m_sSize.length - in_Pos.x; x*=x;
	y = m_pPosition.y + m_sSize.width - in_Pos.y; y*=y;
	length = sqrt(x+y);
	if ( m_bigViewLength > length ) {
		return true;
	}

	return false;
}

bool MatrixChunk::shadowed (double in_x, double in_y,
			    double in_z, const Chunk *chunk, double maxHeight ) const
{
#	ifndef _ShadowStep
#	 define _ShadowStep 1
#	endif //_ShadowStep
	if (chunk == NULL ) {
		return false;
	}
	double zStep = cos (m_Sun.dAngleZ)*_ShadowStep;
	double sinZ = sin (m_Sun.dAngleZ)*_ShadowStep;
	double xStep = cos (m_Sun.dAngleXY)*sinZ;
	double yStep = sin (m_Sun.dAngleXY)*sinZ;
	const Size &size = chunk->getSize();
	in_x+=xStep;
	in_y+=yStep;
	in_z+=zStep;
	while ( ( in_x < size.width ) &&
		( in_y < size.length ) &&
		( in_x > 0 ) &&
		( in_y > 0 ) &&
		( in_z < maxHeight) )
	{
		if ( chunk->height(in_x + m_pPosition.x,in_y + m_pPosition.y) > in_z ) {
			return true;
		}
		in_x+=xStep;
		in_y+=yStep;
		in_z+=zStep;
	}
	return false;
}

void MatrixChunk::serialize ( Serializer &serializer)
{
	Chunk::serialize (serializer);
	serializer.serializeColorer ( m_pColorer );

	long objectNuber = m_mObjects.size();
	serializer.serialize (objectNuber);
	m_mObjects.resize (objectNuber, NULL);
	for ( long i =0; i < objectNuber; ++i ) {
		serializer.serializeObject (m_mObjects[i]);
	}

	long length = m_mMatrix.size();
	serializer.serialize ( length );
	m_mMatrix.resize(length);
	long width;
	for ( long x = 0; x < length; ++x ) {
		width = m_mMatrix[x].size();
		serializer.serialize ( width );
		m_mMatrix[x].resize(width,0);
		for ( long y = 0; y < width; ++y) {
			//cerr<<x<<" "<<y<<": "<<length<<" "<<width<< endl;
			serializer.serialize(m_mMatrix[x][y]);
		}
	}

	lDelta = m_sSize.length/(length-1);
	wDelta = m_sSize.width/(width-1);
}
