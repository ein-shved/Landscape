/*
 * Objects.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 9.11.2012
 */

/** @file
 * Файл определяет классы объектов ландшафта:\n
 * 1) абсрактный класс Landscape::Object,\n
 * 2) класс елки Landscape::Tree.
 */

#ifndef _Landscape_OBJECTS_HPP_
#define _Landscape_OBJECTS_HPP_

#include <Serialization>
#include "../Position.hpp"

namespace Landscape  {

	class Graphics;
	class Chunk;

	/** @class Landscape::Object
	 * Абсрактный класс объектов ландшафта, используемых в проекте Landscape.
	 */
	class /* abstract */ Object {

	public:
		/**
		 * @param in_Position положение объекта
		 * @param in_Size размеры объекта.
		 */
		Object ( const Position &in_Position = Position(), const Size &in_Size = Size());
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~Object ();

		/**
		 * Прорисовка объекта.
		 * @param in_graphics объект графики для доступа к окружению.
		 */
		virtual void draw ( const Graphics& in_graphics) =0;

		/**
		 * @return Размеры объекта.
		 */
		virtual const Size &getSize () const;
		/**
		 * @return Положение объекта.
		 */
		virtual const Position &getPosition () const;
		/**
		 * Устанавоивает положение объекта
		 * @param in_Position новое положение объекта.
		 */
		virtual void setPosition ( const Position &in_Position );

		/**
		 * Возвращает высоту объекта в точке (in_dX,in_dY)
		 * @param in_dX координата оси x в мировой системе координат.
		 * @param in_dY координата оси y в мировой системе координат.
		 */
		virtual double height ( double in_dX, double in_dY  ) const =0;
		/**
		 * Возвращает высоту объекта в точке (in_Position.x,in_Position.y)
		 * @param in_Position точка в мировой системе координат.
		 */
		virtual double height ( const Position &in_Position ) const;

		/**
		 * @return Точную копию объекта
		 */
		virtual Object *clone () const =0;

		/**
		 * Сериализация объекта.
		 * @param serializer сериализатор. <i>Спасибо, кэп!</i>
		 */
		virtual void serialize ( Serialization::Serializer &serializer );

	protected:
		Position m_Position;
		Size m_Size;
	};

	/** @class Landscape::Tree
	 * Класс елки.
	 */
	class Tree : public Object {

	public:
		/**
		 * @param in_Position положение елки.
		 * @param in_Height высота елки.
		 */
		Tree ( const Position &in_Position = Position(), double in_Height = 100);
		virtual ~Tree ();

		/**
		 * Рисует два скрещенных зеленых трекгольника, а между ними - четыре коричнивых.
		 * @param in_graphics объект графики для доступа к окружению.
		 */
		virtual void draw ( const Graphics& in_graphics);

		/**
		 * @return высоту елки.
		 */
		virtual double height ( double in_dX, double in_dY  ) const;
		
		virtual Tree *clone () const;

		virtual void serialize ( Serialization::Serializer &serializer );
	private:
		double m_Height;
	};

	
}

#endif //_Landscape_OBJECTS_HPP_