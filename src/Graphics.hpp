/*
 * Graphics.hpp
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
 * Файл определяет класс Landscape::Graphics - наследник
 * интерфейса GLFrontEnd::Graphics для взаимодействия с
 * библиотекой GLFrontEnd.
 */
#ifndef _Landscape_GRAPHICS_HPP_
#define _Landscape_GRAPHICS_HPP_

#ifdef __linux__
#include <GLFrontEnd>
#elif defined  _WIN32 || _WIN64
#include "../GLFrontEnd/include/GLFrontEnd"
#endif

#include "Chunk/Chunk.hpp"
#include "Camera.hpp"

/** @namespace Landscape
 * Пространство имен проекта Landscape. Содержит набор базовых классов ландшавта,
 * классы для взаимодействия с библиотекой GLFrontEnd и дублирующая ветка классов
 * библиотеки Serialization.
 * @see GLFrontEnd
 * @see Serialization
 * @see Landscape::Serializer
 * 
 * Ландшафт представляет из себя чанк (или набор чанков) Landscape::Chunk, который по 
 * опреленному закону каждой точке плоскости (x,y), лежащей горизонтально, задает 
 * значение z высоты ландшафта в этой точке. Так как в понятиях проекта Landscape
 * высота откладывается по координате z, то во всех функциях библиотеки OpenGL 
 * координаты z и y меняются местами.\n
 * <i>В данном случае, ландшафт - это объект класса Landscape::LandscapeChunk, матрица
 * которого состоит из объектов класса Landscape::MatrixChunk.</i>
 * @see Landscape::LandscapeChunk
 * @see Landscape::MatrixChunk
 * 
 * Основой проекта является класс графики Landscape::Graphics и его главный чанк (Landscape::Chunk).
 * Этот класс рисует всю графику OpenGl. Главный чанк генерируется объектом класса
 * Landscape::Generator, который принимается объектом графики.
 * @see Landscape::Graphics
 * @see Landscape::Chunk
 * @see Landscape::Generator
 * 
 * Камера представляется классом Landscape::Camera, объект которого управляется классом Landscape::ControlEventHandler.
 * @see Landscape::Camera
 * @see Landscape::ControlEventHandler
 */

namespace Landscape  {

	class Generator;
	class Fog;
	class Serializer;
	/** @class Landscape::Graphics
	 * Класс реализует интерфейс GLFrontEnd::Graphics для прорисовки
	 * сцены GL. Класс управляет генерацией чанков, перемещением и поворотом
	 * сцены относительно камеры, наложением тумана. Класс содержит в себе главный
	 * чанк, который один раз генерируется 
	 * @see Landscape::Camera
	 * @see Landscape::Generator
	 * @see Landscape::Fog
	 */
	class Graphics : public GLFrontEnd::Graphics {

	public:
		/**
		 * Здесь генерируется главный чанк.
		 * @param in_lGenerator ссылка на генератор ландшафта.
		 * @param in_lCamera ссылка на камеру
		 * @param in_viewLength дальность обзора
		 */
		Graphics( const Generator &in_lGenerator,
			  const Camera &in_lCamera,
			  double in_viewLength = 8000);
		/**
		 * Удаляются генератор, чанк и туман, если таковые имелись.
		 */
		~Graphics();
		/**
		 * Прорисовка сцены. Сбрасывается модельная матрица, передвигается и поворачивается
		 * сцена в соответсвии с камерой, закрашивается фон, ресуется туман, ресуется главный
		 * чанк.
		 * @param in_lWindow геометрия окна.
		 */
		virtual void step ( const GLFrontEnd::WindowGeometry &in_lWindow );
		/**
		 * Создается перспективная матрицу, создается свет, вызывается множество необходимых
		 * glEnable.
		 * @param in_lWindow геометрия окна.
		 */
		virtual void init ( const GLFrontEnd::WindowGeometry &in_lWindow );
		/**
		 * Прорисовка тумана.
		 */
		void drawFog () const;

		/**
		 * @return высоту главного чанка в точке (x,y).
		 * <i>Почему то умнее идеи оформить это в виде оператора,
		 * мне тогда на ум ничего не пришло.</i>
		 * @param x координата оси Х
		 * @param y координата оси Y
		 */
		double operator() (double x, double y) const;
		/**
		 * @return дальность обзора.
		 */
		double getViewLength() const;
		/**
		 * @return ссылку на объект камеры.
		 */
		const Camera &getCamera() const;
		/**
		 * @return ссылку на гланый чанк.
		 */
		const Chunk *getChunk() const;
		/**
		 * @return максимально возможную высоту ландшафта.
		 */
		double getMaxHeight() const;

		/**
		 * Сериализация ландшафта и тумана.
		 * @param serializer сериализатор. <i>Спасибо, кэп.</i>
		 */
		void serialize (Serializer &serializer);
	private:
		double m_viewLength;

		Chunk *m_pChunk;
		const Camera &m_lCamera;
		const Generator *m_pGenerator;

		Fog *m_pFog;
	};
};

#endif //_Landscape_GRAPHICS_HPP_
