/*
 * Generator.hpp
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
 * Файл определяет интерфейс Landscape::Generator.
 */

#ifndef _Landscape_GENERATOR_H_
#define _Landscape_GENERATOR_H_

#include "../Chunk/Chunk.hpp"

namespace Landscape {
	/** @class Landscape::Generator
	 * Интерфейс генератора чанков
	 * @see Landscape::Chunk
	 */
	class /* interface */ Generator {

	public:
		Generator ();
		virtual ~Generator();

		/**
		 * @return Сгенерирированный чанк.
		 */
		virtual Chunk *generate () const =0;
		/**
		 * Догенерация чанка. Расширяет chunk в некторой окрестности позиции
		 * in_Position.
		 * @param chunk изменяемый чанк.
		 * @param in_Position позиция в мировой системе координат.
		 */
		virtual void generate (Chunk* chunk, const Position &in_Position) const =0;
		/**
		 * @return Точную копию генератора.
		 */
		virtual Generator *clone () const =0;
		/**
		 * @return Маскисмально возможную высоту чанка.
		 */
		virtual double getMaxHeight() const =0;
	};
}


#endif //_Landscape_GENERATOR_H_