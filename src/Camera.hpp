/*
 * Camera.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 2.11.2012
 */

/** @file
 * Файл определяет класс Landscape::Camera.
 */

#ifndef _Landscape_CAMERA_H_
#define _Landscape_CAMERA_H_

#include "Position.hpp"
#ifndef NULL
# define NULL 0
#endif

namespace Landscape {

	class Graphics;
	/** @class Landscape::Camera
	 * Класс определяет положения камеры(наблюдателя) в мировой системе координат. 
	 * Камера не проходит сквозь ландшафт и за его пределы.
	 */
	class Camera {

#	ifndef _MinCameraHeight
#	 define _MinCameraHeight 0
#	endif //_MinCameraHeight

#	ifndef _MaxCameraHeight
#	 define _MaxCameraHeight 1000
#	endif //_MaxCameraHeight

	public:
		/**
		 * @param in_dMinHeight минимальная высота камеры над точкой ландшафта
		 * @param in_dMaxHeight максимальная высота камеры.
		 * @param in_pPosition начальное положение камеры.
		 * @param in_pGraphics указатель на объект графики.
		 * @see Landscape::Graphics
		 */
		Camera ( double in_dMinHeight = _MinCameraHeight,
			 double in_dMaxHeight = _MaxCameraHeight,
			 const Position &in_pPosition = Position(0,0,_MinCameraHeight),
			 const Graphics *in_pGraphics = NULL );
		/**
		 * Минимальная и максимальная высота камеры устанавливается по-умолчанию.
		 * @param in_pPosition начальное положение камеры.
		 * @param in_pGraphics указатель на объект графики.
		 * @see Landscape::Graphics
		 */
		Camera ( const Position &in_pPosition,
			 const Graphics *in_pGraphics = NULL );

		/**
		 * Устанавливает новый указатель на объект графики. Объект графики нужен 
		 * камере для доступа к главному чанку и получение его высоты.
		 * @param in_pGraphics указатель на объект графики.
		 * @see Landscape::Graphics
		 */
		void setGraphics (const Graphics *in_pGraphics);

		/**
		 * Переместить камеру в точку in_pPosition;
		 * @param in_pPosition новое положение камеры.
		 */
		void move ( const Position &in_pPosition );
		/**
		 * Переместить камеру на растояние length по направлению
		 * ее взгляда. Положительное значение передвигает вперед,
		 * отрицательное - назад.
		 * @param length расстояние.
		 */
		void move ( double length );
		/**
		 * Переместить камеру на растояние length в сторону.
		 * Положительное значение передвигает вправо,
		 * отрицательное - влево.
		 * @param length расстояние.
		 */
		void moveSide ( double length );
		/**
		 * Переместить камеру на расстояние length по вертикали.
		 * Положительное значение передвигает вверх,
		 * отрицательное - вниз.
		 * @param length расстояние.
		 */
		void moveHeight ( double length );
		/**
		 * Повернуть камеру на угол z по вертикали,
		 * на угол xy по горизонтали.
		 * @param z угол по верикали.
		 * @param xy угол по горизонтали.
		 */
		void rotate ( double z, double xy );
		/**
		 * Установить угол направления взглфда в
		 * z по вертикали, в xy по горизонтали.
		 * @param z угол по верикали.
		 * @param xy угол по горизонтали.
		 */
		void setAngles ( double z, double xy );

		/**
		 * @return угол направления взгляда по вертикали.
		 */
		double getAngleZ () const;
		/**
		 * @return угол направления взгляда по горизонтали.
		 */
		double getAngleXY() const ;
		/**
		 * @return текущее положение камеры.
		 */
		const Position &getPosition() const;

	private:
		void checkPosition();
		
	private:
		const Graphics *m_pGraphics;
		Position m_pPosition;
		
		double m_dMinHeight;
		double m_dMaxHeight;

		double m_dAngleZ;
		double m_dAngleXY;
	};

}

#endif //_Landscape_CAMERA_H_