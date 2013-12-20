/*
 * Fog.hpp
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
 * Файл определяет классы тумана:\n
 * 1) интерфейс Landscape::Fog,\n
 * 2) его простая реализация Landscape::SimpleFog.
 */

#ifndef _Landscape_FOG_HPP_
#define _Landscape_FOG_HPP_

#include <Serialization>
#include <GL/gl.h>

namespace Landscape  {
	class Graphics;

	/** @class Landscape::Fog
	 * Интерфейс для классов тумана, используемых
	 * в проекте Landscape.
	 */
	class /* interface */ Fog {
		
	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~Fog();
		/**
		 * Прорисовка тумана.
		 * @param in_graphics объект графики для доступа к окружению.
		 */
		virtual void draw ( const Graphics& in_graphics ) =0;
		/**
		 * @return точная копия класс тумана.
		 */
		virtual Fog *clone () =0;
		/**
		 * Сериализация тумана.
		 * @param serializer сериализатор. <i>Спасибо, кэп!</i>
		 */
		virtual void serialize ( Serialization::Serializer &serializer )=0;
	};

	/** @class Landscape::SimpleFog
	 * Реализация простого тумана.
	 */
	class SimpleFog : public Fog {

	public:
		SimpleFog();
		virtual ~SimpleFog();
		virtual void draw ( const Graphics& in_graphics );
		virtual SimpleFog *clone ();
		virtual void serialize ( Serialization::Serializer &serializer );
	private:
		GLfloat m_fogColor[4];
	};
}

#endif //_Landscape_FOG_HPP_