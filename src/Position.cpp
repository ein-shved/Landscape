/*
 * Position.cpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */

#include "Position.hpp"

using namespace Landscape;
using namespace std;

Position::Position()
    : x(0)
    , y(0)
    , z(0)
{}
Position::Position ( double X, double Y, double Z)
    : x(X)
    , y(Y)
    , z(Z)
{}
bool Position::operator == ( const Position &in_other) const
{
	return (x == in_other.x) && (z == in_other.z) && (z == in_other.z);
}
bool Position::operator != ( const Position &in_other) const
{
	return (x != in_other.x) && (z != in_other.z) && (z != in_other.z);
}

Size::Size()
    : length(0)
    , width(0)
{}
Size::Size ( double in_length, double in_width)
    : length(in_length)
    , width(in_width)
{}
bool Size::operator == ( const Size &in_other) const
{
	return (length == in_other.length) && (width == in_other.width);
}
bool Size::operator != ( const Size &in_other) const
{
	return (length != in_other.length) || (width != in_other.width);
}