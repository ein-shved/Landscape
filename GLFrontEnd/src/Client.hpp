/*
 * Client.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */

#ifndef _GLFrontEnd_CLIENT_HPP_
#define _GLFrontEnd_CLIENT_HPP_

/** @file Client.hpp
 * Описывает класс GLFrontEnd::Client.
 * @see GLFrontEnd.hpp
 */

#include <string>
#include "GLFrontEnd.hpp"

#ifdef __linux__

#include<X11/Xlib.h>
#include<GL/glx.h>

#elif defined _WIN32 || _WIN64

#include <windows.h>
#include <GL/gl.h>

#endif //__linux__ _WIN32 _WIN64


/** @namespace GLFrontEnd
 * Пространство имен GLFrontEnd содержит
 * механизм взаимодействия библиотеки GL и
 * оконных интерфесов с остальной программой.
 */

namespace GLFrontEnd {

	/** @class Client
	 * Класс Client создает и запускает клиент
	 * оконного мэнеджера (стабилно работает только
	 * с X Window System). Для использования нужно
	 * создать объект Client и последовательно вызывать
	 * методы этого объекта.
	 * @see start()
	 * Вызов метода start создает окно.
	 * @see run()
	 * Вызов метода run выведет окно на экран и запустит
	 * цикл приложения.
	 * @see stop()
	 * Вызов метода stop остановит цикл и спрячет окно.
	 * @see destroy()
	 * Вызов метода destroy уничтожит окно. Это же
	 * действие производит и деструктор, так что вызов
	 * destroy не обязателен.
	 */
	class Client {

	public:
		/**
		 * Конструктор
		 * @param applicationName строкавая переменная с именем приложения
		 * @param displayName строковая переменная (только на X Window) с именем дисплея
		 */
		Client ( const std::string &applicationName = std::string("Application")
		#ifdef __linux__
			 , const std::string &displayName = std::string()
		#endif //__linux__
		);
		/**
		 * Деструктор.
		 * Уничтожает окно, если оно имеется.
		 * @see destroy()
		 */
		~Client ();

		/**
		 * Метод инициализирует клиент. Создает окно, подписывается на необхоимые события,
		 * готовится к созданию GL-контекста и к looping'у.
		 */
		void start();
		/**
		 * Метод создает GL-контекст, показывает окно и входит в цикл. Мышь граббится и прячется.
		 * Нажатия клавиш Escape и RCtrl обрабатываются методом самостоятельно. Выход из цикла
		 * производится при нажатии клавиши Escape, нажатии кнопки [Х] или при вызове метода
		 * stop() во время работы цикла. Мышь отпускается при нажатии клавиши RCtrl.
		 * @see stop()
		 * @param grphics будет дергаться на каждом шаге цикла с требованием перерисовать сцену.
		 * @see GLFrontEnd::Graphics
		 * @param handler - обработчик событий, которому будут передоваться информация о нажатии клавиш, кнопок и смещении мыши.
		 * @see GLFrontEnd::ControlEventHandler
		 */
		void run ( GLFrontEnd::Graphics &graphics, GLFrontEnd::ControlEventHandler &handler );
		/**
		 * Метод просто опускает флажок, "вокруг" которого крутится главный цикл.
		 * @see start()
		 */
		void stop ();
		/** @fn destroy()
		 * Удаляет окно приложения.
		 * @see ~Client()
		 */
		void destroy ();
		/** 
		 * Переименовывает приложение.
		 * @param applicationName новое имя приложения.
		 */
		void setApplicationName ( const std::string &applicationName );

	private:
		/**
		 * Флаг, вокруг которого крутится главный цикл.
		 * @see run()
		 * @see stop()
		 */
		bool _loopFlag;
		std::string _applicationName; ///< имя приложения.

#		ifdef __linux__
		std::string _displayName;
		Display *_xDisplay; //if not NULL - start() calling wil be incorrect
		Window _xRootWindow;
		Window _xWindow;
		XVisualInfo *_xVisualInfo;
		GLXContext _glxContext;
#		elif defined _WIN32 || _WIN64

		WindowGeometry _winGeometry;
		GLFrontEnd::Graphics *_graphics;
		GLFrontEnd::ControlEventHandler *_handler;

		static Client *s_globalClient;
		static void win_display ();
		static void win_reshape (GLint w, GLint h);
		static void win_keyboard ( unsigned char key, int x, int y);

		HGLRC  _hRC;
		HDC  _hDC;
		HWND  _hWnd;
		HINSTANCE  _hInstance;

		bool _bActive;
		struct {
			int x;
			int y;
		} _mousePos;

		static LRESULT CALLBACK wndProc( HWND, UINT, WPARAM, LPARAM );

#		endif //__linux__ _WIN32 _WIN64
	};
};


#endif //_GLFrontEnd_CLIENT_HPP_
