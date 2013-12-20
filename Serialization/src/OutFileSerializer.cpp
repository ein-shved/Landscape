/*
 * OutFileSerializer.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 11.11.2012
 */

#include <iostream>
#include <cstring>
#include "OutFileSerializer.hpp"

using namespace Serialization;
using namespace std;

OutFileSerializer::OutFileSerializer()
{
	m_ofStream.exceptions (ios_base::badbit | ios_base::eofbit | ios_base::failbit );
}
OutFileSerializer::~OutFileSerializer()
{
	if (m_ofStream.is_open()) {
		m_ofStream.flush();
		m_ofStream.close();
	}
}
void OutFileSerializer::serialize ( long &data )
{
	m_ofStream<<data<<" ";
	m_ofStream.flush();
}
void OutFileSerializer::serialize ( double &data )
{
	m_ofStream<<data<<" ";
	m_ofStream.flush();
}
void OutFileSerializer::serialize ( std::string &data )
{
	m_ofStream<<data<<" ";
	m_ofStream.flush();
}

OutBinaryFileSerializer::~OutBinaryFileSerializer()
{}
void OutBinaryFileSerializer::open ( const std::string &filename )
{
	m_ofStream.open( filename.c_str(), ios::out | ios::binary | ios::trunc );
	m_ofStream.flush();
}

void OutBinaryFileSerializer::serialize ( long &data )
{
	m_ofStream.write ((const char*)&data, sizeof(data)/sizeof(const char));
	m_ofStream.flush();
}
void OutBinaryFileSerializer::serialize ( double &data )
{
	m_ofStream.write ((const char*)&data, sizeof(data)/sizeof(const char));
	m_ofStream.flush();
}
void OutBinaryFileSerializer::serialize ( std::string &data )
{
	size_t size = strlen(data.c_str());
	m_ofStream.write ((const char*)&size, sizeof(size)/sizeof(const char));
	m_ofStream.write ((const char*)data.c_str(), size*sizeof(const char)/sizeof(const char));
	m_ofStream.flush();
}

OutStringFileSerializer::OutStringFileSerializer()
    :m_Indent(0)
{}
OutStringFileSerializer::~OutStringFileSerializer()
{}
void OutStringFileSerializer::open ( const std::string &filename )
{
	m_ofStream.open( filename.c_str(), ios::out | ios::trunc );
}