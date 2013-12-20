/*
 * Position.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */

/** @file 
 * Описывает 3-хмерные позиции и 2-хмерные размеры в пространстве проекта Landscape.
 */
#ifndef _Landscape_POSITION_H_
#define _Landscape_POSITION_H_

namespace Landscape {
	/** @struct Landscape::Position
	 * Структура cодержит координаты пространства. Для нее определены
	 * операторы сравнения.
	 */
	struct Position
	{
		double x,y,z;
		Position ();
		Position ( double X, double Y, double Z = 0);
		bool operator == ( const Position &in_other) const;
		bool operator != ( const Position &in_other) const;
	};

	/** @struct Landscape::Size
	 * Структура содержит длину (по оси x) и ширину (по оси y). Для нее определены
	 * операторы сравнения.
	 */
	struct Size
	{
		double length; //x
		double width; //y
		Size ();
		Size ( double in_length, double in_width );
		bool operator == ( const Size &in_other) const;
		bool operator != ( const Size &in_other) const;
	};

}

#endif //_Landscape_POSITION_H_