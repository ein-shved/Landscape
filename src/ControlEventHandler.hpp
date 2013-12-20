/*
 * ControlEventHandler.hpp
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
 * Файл определяет класс Landscape::ControlEventHandler - наследник
 * интерфейса GLFrontEnd::ControlEventHandler для взаимодействия с
 * библиотекой GLFrontEnd.
 */

#ifndef _Landscape_CONTROL_EVENT_HANDLER_HPP_
#define _Landscape_CONTROL_EVENT_HANDLER_HPP_

#include <cstddef>
#include <vector>
#include <ctime>

#ifdef __linux__
#include <GLFrontEnd>
#elif defined _WIN32 || _WIN64
#include "../GLFrontEnd/include/GLFrontEnd"
#endif

#include "Position.hpp"

namespace Landscape {

	class Camera;
	/** @class Landscape::ControlEventHandler
	 * Класс реализует интерфейс GLFrontEnd::ControlEventHandler для управления
	 * камерой Landscape::Camera. Объект класса содержит в себе координаты
	 * изменения позиции мыши и два булевых вектора.
	 * Один - для клавиш клавиатуры, другой - для кнопок мыши.
	 * @see Landscape::Camera
	 */
	class ControlEventHandler : public GLFrontEnd::ControlEventHandler {

	public:
		/**
		 * Конструктор принимает ссылку на объект камеры, которым объект
		 * Landscape::ControlEventHandler будет в последствии управлять.
		 * @see Landscape::Camera
		 */
		ControlEventHandler ( Landscape::Camera &in_lCamera );

		virtual void keyEvent ( size_t key, bool pressed );
		virtual void buttonEvent ( size_t button, bool pressed );
		virtual void mouseMoved ( int x, int y);

		/**
		 * Этот метод передвигает и поворачивает камеру в соответствии с 
		 * клавишами, записанными в вектор клавиатуры и изменением позиции мыши.
		 * 
		 * Управление клавиатурой:\n
		 * W - вперед.\n
		 * S - назад.\n
		 * A - влево.\n
		 * D - вправо.\n
		 * Space - вверх.\n
		 * Ctrl - вниз.\n
		 * 
		 * 
		 * После вызова этого метода изменение позиции мыши сбрасывается.
		 * @return TRUE, если камера изменила положение и/или направление взгляда,\n иначе - FALSE.
		 * @see GLFrontEnd::ControlEventHandler::step
		 * @see Landscape::Camera
		 */
		virtual bool step();

		struct XWindowPosition;

	private:
		std::vector<bool> m_vKeys;
		std::vector<bool> m_vButtons;

		struct XWindowPosition {
			int x,y;
			XWindowPosition ();
			XWindowPosition ( const Landscape::Position &position );
			operator Landscape::Position ();
		} _mousePosition;

		Landscape::Camera &m_lCamera;

		struct timespec m_cTime;
		double m_speed;

	};
}


#endif //_Landscape_CONTROL_EVENT_HANDLER_HPP_
