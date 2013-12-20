/*
 * Water.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 7.11.2012
 */

/** @file
 * Файл определяет классы воды:\n
 * 1) интерфейс Landscape::Water,\n
 * 2) его простая реализация Landscape::SimpleWater,\n
 * 3) реализация воды с отражение Landscape::ReflectiveWater.
 */

#ifndef _Landscape_WATER_HPP_
#define _Landscape_WATER_HPP_

#include <Serialization>
#include "../Position.hpp"

namespace Landscape  {
	class Graphics;

	class Chunk;
	/** @class Landscape::Water
	 * Интерфейс для классов воды, используемых в проекте Landscape.
	 * Положение здесь интересно только в смысле уровня воды.
	 */
	class /* interface */ Water {

	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~Water ();
		/**
		 * Прорисовка воды.
		 * @param in_graphics объект графики для доступа к окружению.
		 * @param in_pChunk неконстантный указатель на главный чанк.
		 */
		virtual void draw ( const Graphics& in_graphics, Chunk *in_pChunk ) =0;
		/**
		 * Устанавливает размер плоскости воды и ее положение.
		 * @param in_Position новое положение.
		 * @param in_Size новые размеры.
		 */
		virtual void setSize ( const Position &in_Position, const Size &in_Size ) =0;
		/**
		 * @return положение воды.
		 */
		virtual const Position &getPosition () const =0;
		/**
		 * @return точная копия объекта класса воды.
		 */
		virtual Water *clone () const =0;
		/**
		 * Сериализация воды.
		 * @param serializer сериализатор. <i>Спасибо, кэп!</i>
		 */
		virtual void serialize ( Serialization::Serializer &serializer ) =0;
	};

	/** @class Landscape::SimpleWater
	 * Класс реализующий обычную воду в виде синей прозрачной плоскости.
	 */
	class SimpleWater : public Water {
		
	public:
		virtual ~SimpleWater ();
		virtual void draw ( const Graphics& in_graphics, Chunk *in_pChunk );
		virtual void setSize ( const Position &in_Position, const Size &in_Size );
		virtual const Position &getPosition () const;
		virtual SimpleWater *clone () const;
		virtual void serialize ( Serialization::Serializer &serializer );
	protected:
		Position m_Position;
		Size m_Size;
	};

	/** @class Landscape::SimpleWater
	 * Класс воду в виде синей плоскости, которая отражает мир, при положении камеры
	 * над уровнем воды, и представляет собой прозрачную плоскость, при положении камеры
	 * под водой
	 */
	class ReflectiveWater : public SimpleWater {

	public:
		virtual ~ReflectiveWater ();
		virtual void draw ( const Graphics& in_graphics, Chunk *in_pChunk );
		virtual ReflectiveWater *clone () const;
	};
}

#endif //_Landscape_WATER_HPP_