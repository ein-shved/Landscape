/*
 * Chunk.hpp
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
 * Файл определяет абстрактный класс Landscape::Chunk.
 */

#ifndef _Landscape_CHUNK_H_
#define _Landscape_CHUNK_H_

#include "../Position.hpp"

namespace Landscape {

	class Graphics;
	class Serializer;

	/** @class Landscape::Chunk
	 * Абстрактный класс, который представляет из себя какой либо прямоугольный
	 * кусок ландшафта. Он по определенному закону каждой точке (x,y)
	 * части горизонтальной плоскости, которую чанк покрывает, задает 
	 * значение z высоты ландашафта в этой точке. Кроме отображения, 
	 * класс рисует сам кусок ландшафта, который он задает. Это используется
	 * классом Landscape::Graphics. Каждый чанк имеет размеры (Landscape::Size)
	 * и положение (Landscape::Position) причем положение задает положение точки (0,0)
	 * в системе координат чанка.
	 * @see Landscape::Graphics
	 * @see Landscape::Size
	 * @see Landscape::Position
	 */
	class /* abstract */ Chunk {
	
	public:
		/**
		 * @param in_pPosition поожение чанка.
		 * @param in_sSize размеры чанка.
		 */
		Chunk ( const Position &in_pPosition = Position(),
			const Size &in_sSize = Size() );
		/**
		 * @param x положение чанка по оси x.
		 * @param y положение чанка по оси y.
		 * @param length протяженность чанка по оси x.
		 * @param width ширина чанка по оси y.
		 */
		Chunk ( double x, double y,
			double length = 0.0, double width = 0.0 );
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~Chunk();

		/**
		 * Клонирование чанка.
		 * @return точную копию чанка, для которого вызван этот метод.
		 */
		virtual Chunk* clone() const =0;
		/**
		 * Рисует чанк.
		 * @param in_lGraphics ссылка на объект графики. Необходима для доступа чанка к его окружению.
		 */
		virtual void draw ( const Graphics &in_lGraphics ) =0;

		/**
		 * @return высоту ландшафта z в точке (x,y).
		 * @param x координата x в мировой системе координат.
		 * @param y координата y в мировой системе координат.
		 */
		virtual double height (double x, double y) const =0;
		/**
		 * @return высоту ландшафта z в точке (x,y).
		 * @param x координата x в системе координат чанка.
		 * @param y координата y в системе координат чанка.
		 */
		virtual double absoluteHeight (double x, double y) const =0;
		/**
		 * @return высоту ландшафта z в точке (in_Position.x,in_Position.y).
		 * @param in_Position точка в мировой системе координат.
		 */
		virtual double height (const Position &in_Position ) const;

		/**
		 * @return позицию чанка.
		 */
		const Position &getPosition () const;
		/**
		 * @return размеры чанка.
		 */
		const Size &getSize () const;

		/**
		 * Изменяет размеры чанка
		 * @param in_sSize новый размер чанка.
		 */
		virtual void setSize ( const Size &in_sSize );
		/**
		 * Передвигает чанк в позицию in_pPosition;
		 * @param in_pPosition новое положение чанка.
		 */
		virtual void setPosition ( const Position &in_pPosition );
		/**
		 * Передвигает чанк в позицию (x,y,z);
		 * @param x новая координата x положения чанка.
		 * @param y новая координата y положения чанка.
		 * @param z новая координата z положения чанка.
		 */
		void setPosition ( double x, double y, double z );

		/**
		 * Сериализация. Базовый класс сериализует положение 
		 * и размеры чанка.
		 * @param serializer сериализатор. <i>Спасибо, кэп!</i>
		 */
		virtual void serialize ( Serializer &serializer);

	protected:

		/** @struct Landscape::Chunk::Sun
		 * Задает положение солнца в сферических координатах.\n
		 * <i>Не используется, так как не включены тени</i>
		 */
		struct Sun {
			double dAngleZ; ///< угол по оси Z
			double dAngleXY; ///< угол в плоскости OXY
			Sun();
		};
		Landscape::Position m_pPosition; ///< Положение чанка.
		Landscape::Size m_sSize; ///<Размеры чанка.
		Sun m_Sun; ///< Направление солнца. Нужно для <i>незапиленной</i> работы теней.

	public:
		bool drawWater; ///< Выодить ли воду? <i>Очень очень оооооооочень криво.</i>.
	};
}

#endif //_Landscape_CHUNK_H_

