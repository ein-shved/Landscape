/*
 * GLFrontEnd.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 3.11.2012
 */

/** @file 
 * Описывает интерфейсы взаимодействия с классом GLFrontEnd::Client.
 * @see Client.hpp
 */

#ifndef _GLFrontEnd_CLASSES_H_
#define _GLFrontEnd_CLASSES_H_

#include <cstring>

namespace GLFrontEnd {

	/** @struct GLFrontEnd::WindowGeometry
	 * Структура, описывающая геометрию окна в понятиях графики.
	 * @see Graphics
	 */
	struct WindowGeometry {
		size_t x; ///< Положение окна по оси x.
		size_t y; ///< Положение окна по оси y.
		
		double width; ///< Ширина окна.
		double height; ///< Высота окна.
	};

	/** @class GLFrontEnd::ControlEventHandler
	 * Интерфейс, который должны наследовать системы для того, чтобы
	 * воспользоваться механизмом класса Client. Реализует отклик на 
	 * события пользовательской активности.
	 * @see Client
	 */
	class /* interface */ControlEventHandler {
		
	public:
		/** 
		 * Виртуальный деструктор.
		 */
		virtual ~ControlEventHandler () {}
		/** 
		 * Client вызывает эту функцию, если произошло
		 * событие клавиатуры.
		 * @param key код клавиши.
		 * @param pressed значение TRUE - клавиша нажата, FALSE - отпущена.
		 */
		virtual void keyEvent ( size_t key, bool pressed ) =0;
		/** 
		 * Client вызывает эту функцию, если произошло
		 * событие кнопки мыши.
		 * @param button код кнопки мыши.
		 * @param pressed значение TRUE - кнопка нажата, FALSE - отпущена.
		 */
		virtual void buttonEvent ( size_t button, bool pressed ) =0;
		/** 
		 * Client вызывает эту функцию, если произошло
		 * событие смещения мыши. Client передает сюда смещение мыши, а не позицию в окне.
		 * @param x изменение положения мыши вдоль оси x.
		 * @param y изменение положения мыши вдоль оси y.
		 */
		virtual void mouseMoved ( int x, int y) =0;

		/** 
		 * Client вызывает эту функцию, на каждом шаге цикла
		 * перед вызовом метода step объекта класса Graphics.
		 * @return небходима ли перерисовка сцены или нет?
		 * @see Graphics
		 */
		virtual bool step() =0;
	};

	/** @class GLFrontEnd::Graphics
	 * Интерфейс, который должны наследовать системы для того, чтобы
	 * воспользоваться механизмом класса Client. Реализует операции 
	 * рисования в GL.
	 * @see Client
	 */
	class /* interface */ Graphics {

	public:
		/** 
		 * Виртуальный деструктор.
		 */
		virtual ~Graphics() {};
		/** 
		 * Виртуальный метод. Вызывается клиентом на каждлм шаге работы 
		 * главного цикла.
		 * @param in_lWindow содержит информацию о геометрии окна.
		 */
		virtual void step ( const WindowGeometry &in_lWindow ) =0;
		/** 
		 * Виртуальный метод. Вызывается клиентом для инициализации
		 * или переинциализации графики, после, например показа окна,
		 * или изменении его размеров.
		 * @param in_lWindow содержит информацию о геометрии окна.
		 */
		virtual void init ( const WindowGeometry &in_lWindow ) =0;
	};
}

#endif //_GLFrontEnd_CLASSES_H_