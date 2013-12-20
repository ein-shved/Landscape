/*
 * Serializer.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 11.11.2012
 */

#include <ios>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cstring>

#include "Serializer.hpp"
#include "Chunk.hpp"
#include "MatrixChunk.hpp"
#include "LandscapeChunk.hpp"

#include "../Features/Fog.hpp"
#include "../Features/Objects.hpp"
#include "../Features/Colorer.hpp"
#include "../Features/Water.hpp"

using namespace Landscape;
using namespace std;

void OutBinaryFileSerializer::serializeChunk ( Chunk *&data )
{
	if ( data == NULL ) {
		string str ="NULL";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	serialize ( outStr );
	data->serialize(*this);
}
void OutBinaryFileSerializer::serializeWater ( Water *&data )
{
	if ( data == NULL ) {
		string str ="NULL";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	serialize ( outStr );
	data->serialize(*this);
}
void OutBinaryFileSerializer::serializeFog ( Fog *&data )
{
	if ( data == NULL ) {
		string str ="NULL";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	serialize ( outStr );
	data->serialize(*this);
}
void OutBinaryFileSerializer::serializeObject ( Object *&data )
{
	if ( data == NULL ) {
		string str ="NULL";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	serialize ( outStr );
	data->serialize(*this);
}
void OutBinaryFileSerializer::serializeColorer ( Colorer *&data )
{
	if ( data == NULL ) {
		string str ="NULL";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	serialize ( outStr );
	data->serialize(*this);
}



void OutStringFileSerializer::serializeChunk ( Chunk *&data )
{
	string indention = "\n";
	for ( int i=0; i<m_Indent; ++i) {
		indention+='\t';
	}
	if ( data == NULL ) {
		string str = indention+"<Chunk = NULL/>\n";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	outStr=indention+"<Chunk = "+ outStr + ">\n";
	serialize ( outStr );
	++m_Indent;
	data->serialize(*this);
	--m_Indent;
	outStr=indention+"</Chunk>\n";
	serialize(outStr);
}
void OutStringFileSerializer::serializeWater ( Water *&data )
{
	string indention = "\n";
	for ( int i=0; i<m_Indent; ++i) {
		indention+='\t';
	}
	if ( data == NULL ) {
		string str =indention+"<Water = NULL/>\n";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	outStr=indention+"<Water = "+ outStr + ">\n";
	serialize ( outStr );
	++m_Indent;
	data->serialize(*this);
	--m_Indent;
	outStr=indention+"</Water>\n";
	serialize(outStr);
}
void OutStringFileSerializer::serializeFog ( Fog *&data )
{
	string indention = "\n";
	for ( int i=0; i<m_Indent; ++i) {
		indention+='\t';
	}
	if ( data == NULL ) {
		string str =indention+"<Fog = NULL/>\n";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	outStr=indention+"<Fog = "+ outStr + ">\n";
	serialize ( outStr );
	++m_Indent;
	data->serialize(*this);
	--m_Indent;
	outStr=indention+"</Fog>\n";
	serialize(outStr);
}
void OutStringFileSerializer::serializeObject ( Object *&data )
{
	string indention = "\n";
	for ( int i=0; i<m_Indent; ++i) {
		indention+='\t';
	}
	if ( data == NULL ) {
		string str =indention+"<Object = NULL/>\n";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	outStr=indention+"<Object = "+ outStr + ">\n";
	serialize ( outStr );
	++m_Indent;
	data->serialize(*this);
	--m_Indent;
	outStr=indention+"<Object/>\n";
	serialize(outStr);
}
void OutStringFileSerializer::serializeColorer ( Colorer *&data )
{
	string indention = "\n";
	for ( int i=0; i<m_Indent; ++i) {
		indention+='\t';
	}
	if ( data == NULL ) {
		string str =indention+"<Colorer = NULL/>\n";
		serialize ( str );
		return;
	}
	string outStr (typeid(*data).name());
	outStr=indention+"<Colorer = "+ outStr + ">\n";
	serialize ( outStr );
	++m_Indent;
	data->serialize(*this);
	--m_Indent;
	outStr=indention+"</Colorer>\n";
	serialize(outStr);
}



void InBinaryFileSerializer::serializeChunk ( Chunk *&data )
{
	string str;
	serialize (str);
	if ( !strcmp(str.c_str(), typeid(MatrixChunk).name()) ) {
		data = new MatrixChunk();
		data->serialize (*this);
		return;
	}
	if ( !strcmp(str.c_str(), typeid(LandscapeChunk).name()) ) {
		data = new LandscapeChunk();
		data->serialize (*this);
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		return;
	}
	throw ios_base::failure ("Can not read chunk type: '" + str+"'");
}
void InBinaryFileSerializer::serializeWater ( Water *&data )
{
	string str;
	serialize (str);
	if ( str == typeid(SimpleWater).name() ) {
		data = new SimpleWater();
		data->serialize (*this);
		return;
	}
	if ( str == typeid(ReflectiveWater).name() ) {
		data = new ReflectiveWater();
		data->serialize (*this);
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		return;
	}
	throw ios_base::failure ("Can not read water type: '" + str+"'");
}
void InBinaryFileSerializer::serializeFog ( Fog *&data )
{
	string str;
	serialize (str);
	if ( str == typeid(SimpleFog).name() ) {
		data = new SimpleFog();
		data->serialize (*this);
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		return;
	}
	throw ios_base::failure ("Can not read fog type: '" + str+"'");
}
void InBinaryFileSerializer::serializeObject ( Object *&data )
{
	string str;
	serialize (str);
	if ( str == typeid(Tree).name() ) {
		data = new Tree();
		data->serialize (*this);
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		return;
	}
	throw ios_base::failure ("Can not read fog type: '" + str+"'");
}
void InBinaryFileSerializer::serializeColorer ( Colorer *&data )
{
	string str;
	serialize (str);
	if ( str == typeid(SimpleColorer).name() ) {
		data = new SimpleColorer();
		data->serialize (*this);
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		return;
	}
	throw ios_base::failure ("Can not read fog type: '" + str+"'");
}



void InStringFileSerializer::serializeChunk ( Chunk *&data )
{
	char oneGetted =' ';
	while ( isspace(oneGetted) ) {
		m_ifStream.get(oneGetted);
	}
	m_ifStream.putback(oneGetted);
	
	stringbuf strBuffer;
	m_ifStream.get (strBuffer, '>');
	string str = strBuffer.str();
	m_ifStream.get(oneGetted);
	str+=oneGetted;
	if ( str[0] != '<' || str[1] == '/') {
		throw ios_base::failure ("Unexpected xml syntax: " + str);
	}
	bool closed = false;
	if ( str [str.size() - 2] == '/' ) {
		closed = true;
	}
	
	str.erase ( str.find('<'), 1);
	if ( closed ) {
		str.erase ( str.find("/>"), 2);
	} else {
		str.erase ( str.find('>'), 1);
	}
	str.erase ( str.find('='), 1);
	str.erase ( str.find("Chunk"), sizeof("Chunk"));
	for ( size_t pos = str.find (' '); pos < str.size(); pos = str.find (' ') ) {
		str.erase ( pos, 1);
	}

	if ( str == typeid(MatrixChunk).name() ) {
		data = new MatrixChunk();
		if ( !closed ) {
			data->serialize (*this);
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	if ( str == typeid(LandscapeChunk).name() ) {
		data = new LandscapeChunk();
		if ( !closed ) {
			data->serialize (*this);
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	if ( str == "NULL" ) {
		if ( !closed ) {
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	throw ios_base::failure ("Can not read chunk type: " + str);
}
void InStringFileSerializer::serializeWater ( Water *&data )
{
	char oneGetted =' ';
	while ( isspace(oneGetted) ) {
		m_ifStream.get(oneGetted);
	}
	m_ifStream.putback(oneGetted);

	stringbuf strBuffer;
	m_ifStream.get (strBuffer, '>');
	string str = strBuffer.str();
	m_ifStream.get(oneGetted);
	str+=oneGetted;
	if ( str[0] != '<' || str[1] == '/') {
		throw ios_base::failure ("Unexpected xml syntax: " + str);
	}
	bool closed = false;
	if ( str [str.size() - 2] == '/' ) {
		closed = true;
	}

	str.erase ( str.find('<'), 1);
	if ( closed ) {
		str.erase ( str.find("/>"), 2);
	} else {
		str.erase ( str.find('>'), 1);
	}
	str.erase ( str.find('='), 1);
	str.erase ( str.find("Water"), sizeof("Water"));
	for ( size_t pos = str.find (' '); pos < str.size(); pos = str.find (' ') ) {
		str.erase ( pos, 1);
	}

	if ( str == typeid(SimpleWater).name() ) {
		data = new SimpleWater();
		if ( !closed ) {
			data->serialize (*this);
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	if ( str == typeid(ReflectiveWater).name() ) {
		data = new ReflectiveWater();
		if ( !closed ) {
			data->serialize (*this);
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		if ( !closed ) {
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	throw ios_base::failure ("Can not read water type: " + str);
}
void InStringFileSerializer::serializeFog ( Fog *&data )
{
	char oneGetted =' ';
	while ( isspace(oneGetted) ) {
		m_ifStream.get(oneGetted);
	}
	m_ifStream.putback(oneGetted);

	stringbuf strBuffer;
	m_ifStream.get (strBuffer, '>');
	string str = strBuffer.str();
	m_ifStream.get(oneGetted);
	str+=oneGetted;
	if ( str[0] != '<' || str[1] == '/') {
		throw ios_base::failure ("Unexpected xml syntax: " + str);
	}
	bool closed = false;
	if ( str [str.size() - 2] == '/' ) {
		closed = true;
	}

	str.erase ( str.find('<'), 1);
	if ( closed ) {
		str.erase ( str.find("/>"), 2);
	} else {
		str.erase ( str.find('>'), 1);
	}
	str.erase ( str.find('='), 1);
	str.erase ( str.find("Fog"), sizeof("Fog"));
	for ( size_t pos = str.find (' '); pos < str.size(); pos = str.find (' ') ) {
		str.erase ( pos, 1);
	}

	if ( str == typeid(SimpleFog).name() ) {
		data = new SimpleFog();
		if ( !closed ) {
			data->serialize (*this);
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		if ( !closed ) {
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	throw ios_base::failure ("Can not read fog type: " + str);
}
void InStringFileSerializer::serializeObject ( Object *&data )
{
	char oneGetted =' ';
	while ( isspace(oneGetted) ) {
		m_ifStream.get(oneGetted);
	}
	m_ifStream.putback(oneGetted);

	stringbuf strBuffer;
	m_ifStream.get (strBuffer, '>');
	string str = strBuffer.str();
	m_ifStream.get(oneGetted);
	str+=oneGetted;
	if ( str[0] != '<' || str[1] == '/') {
		throw ios_base::failure ("Unexpected xml syntax: " + str);
	}
	bool closed = false;
	if ( str [str.size() - 2] == '/' ) {
		closed = true;
	}

	str.erase ( str.find('<'), 1);
	if ( closed ) {
		str.erase ( str.find("/>"), 2);
	} else {
		str.erase ( str.find('>'), 1);
	}
	str.erase ( str.find('='), 1);
	str.erase ( str.find("Object"), sizeof("Object"));
	for ( size_t pos = str.find (' '); pos < str.size(); pos = str.find (' ') ) {
		str.erase ( pos, 1);
	}
	
	if ( str == typeid(Tree).name() ) {
		data = new Tree();
		if ( !closed ) {
			data->serialize (*this);
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		if ( !closed ) {
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	throw ios_base::failure ("Can not read fog type: " + str);
}
void InStringFileSerializer::serializeColorer ( Colorer *&data )
{
	char oneGetted =' ';
	while ( isspace(oneGetted) ) {
		m_ifStream.get(oneGetted);
	}
	m_ifStream.putback(oneGetted);

	stringbuf strBuffer;
	m_ifStream.get (strBuffer, '>');
	string str = strBuffer.str();
	m_ifStream.get(oneGetted);
	str+=oneGetted;
	if ( str[0] != '<' || str[1] == '/') {
		throw ios_base::failure ("Unexpected xml syntax: " + str);
	}
	bool closed = false;
	if ( str [str.size() - 2] == '/' ) {
		closed = true;
	}

	str.erase ( str.find('<'), 1);
	if ( closed ) {
		str.erase ( str.find("/>"), 2);
	} else {
		str.erase ( str.find('>'), 1);
	}
	str.erase ( str.find('='), 1);
	str.erase ( str.find("Colorer"), sizeof("Colorer"));
	for ( size_t pos = str.find (' '); pos < str.size(); pos = str.find (' ') ) {
		str.erase ( pos, 1);
	}
	if ( str == typeid(SimpleColorer).name() ) {
		data = new SimpleColorer();
		if ( !closed ) {
			data->serialize (*this);
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	if ( str == "NULL" ) {
		data = NULL;
		if ( !closed ) {
			m_ifStream.get (strBuffer, '>');
			m_ifStream.get(oneGetted);
		}
		return;
	}
	throw ios_base::failure ("Can not read fog type: " + str);
}