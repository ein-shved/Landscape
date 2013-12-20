/*
 * InFileSerializer.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 11.11.2012
 */

#include "InFileSerializer.hpp"
#include <iostream>

using namespace Serialization;
using namespace std;

InFileSerializer::InFileSerializer()
{
	m_ifStream.exceptions (ios_base::badbit | ios_base::eofbit | ios_base::failbit );
}
InFileSerializer::~InFileSerializer()
{
	if (m_ifStream.is_open()) {
		m_ifStream.close();
	}
}
void InFileSerializer::serialize ( long &data )
{
	m_ifStream>>data;
}
void InFileSerializer::serialize ( double &data )
{
	m_ifStream>>data;
}
void InFileSerializer::serialize ( std::string &data )
{
	m_ifStream>>data;
}

InBinaryFileSerializer::~InBinaryFileSerializer()
{}
void InBinaryFileSerializer::open ( const std::string &filename )
{
	m_ifStream.open( filename.c_str(), ios_base::in | ios_base::binary );
}

void InBinaryFileSerializer::serialize ( long &data )
{
	m_ifStream.read ((char*)&data, sizeof(data)/sizeof(const char));
}
void InBinaryFileSerializer::serialize ( double &data )
{
	m_ifStream.read ((char*)&data, sizeof(data)/sizeof(const char));
}
void InBinaryFileSerializer::serialize ( std::string &data )
{
	size_t size;
	m_ifStream.read ((char*)&size, sizeof(size)/sizeof(const char));
	data.resize(size,'\0');
	m_ifStream.read ((char*)&data[0], (size)*sizeof(const char)/sizeof(const char));
}

InStringFileSerializer::~InStringFileSerializer()
{}
void InStringFileSerializer::open ( const std::string &filename )
{
	m_ifStream.open( filename.c_str(), ios_base::in);
}