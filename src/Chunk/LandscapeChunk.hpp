/*
 * LandscapeChunk.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 5.11.2012
 */

/** @file
 * Файл определяет класс Landscape::LandscapeChunk.
 */

#ifndef _Landscape_LANDSCAPE_CHUNK_H_
#define _Landscape_LANDSCAPE_CHUNK_H_

#include <vector>
#include <GL/gl.h>

#include "Chunk.hpp"
namespace Landscape {

	class Water;
	class Colorer;
	/** @class Landscape::LandscapeChunk
	 * Чанк, представляющий из себя сетку чанков одинакового размера. По сути этот чанк покрывает
	 * весь ландшафт<i>, но можно придумать что угодно</i>. Чанк может управлять рисованием воды
	 * и луны.
	 */
	class  LandscapeChunk : public Chunk {
	public:
		/**
		 * @param in_pPosition поожение чанка.
		 * @param in_sSize размеры чанка.
		 */
		LandscapeChunk ( const Position &in_pPosition = Position(),
				 const Size &in_sSize = Size() );
		/**
		 * @param x положение чанка по оси x.
		 * @param y положение чанка по оси y.
		 * @param length протяженность чанка по оси x.
		 * @param width ширина чанка по оси y.
		 */
		LandscapeChunk ( double x, double y,
				 double length = 0.0, double width = 0.0 );
		/**
		 * Конструктор копирования.
		 * @param in_other чанк - прообраз.
		 */
		LandscapeChunk ( const LandscapeChunk &in_other );
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~LandscapeChunk();
		
		virtual LandscapeChunk* clone() const;
		virtual void draw ( const Graphics &in_lGraphics );

		virtual double height (double x, double y) const;
		virtual double absoluteHeight (double x, double y) const;

		/**
		 * Задает воду.
		 * @param in_pWater новый объект воды.
		 * @see Landscape::Water
		 */
		void setWater ( const Water *in_pWater );
		/**
		 * Задает луну.
		 * @param path расположение текстуры луны.
		 * @see Landscape::LandscapeChunk::Moon
		 */
		void setMoon ( const std::string &path);

		virtual void serialize ( Serializer &serializer);

	public:
		/**
		 * Добавляет in_chunk в положение (in_x,in_y) сетки.
		 * индекцация сетки может сдвинуться (если индексы отрицательные),
		 * поэтому эта функция может изменить индексы, так, чтобы они
		 * указывали на только что добавленный чанк.
		 * @param in_chunk новый чанк (будет копирован).
		 * @param in_x столбец в сетке.
		 * @param in_y строка в сетке.
		 */
		void add ( Chunk *in_chunk, int &in_x, int &in_y);

		/**
		 * @return чанк, которому принадлежит точка (dX,dY) или NULL.
		 * @param dX координата оси x в мировой системе координат.
		 * @param dY координата оси y в мировой системе координат.
		 */
		const Chunk *chunkFrom ( double dX, double dY ) const;
		/**
		 * @return чанк, которому принадлежит точка (in_pos.x,in_pos.y) или NULL.
		 * @param in_pos точка в мировой системе координат.
		 */
		const Chunk *chunkFrom ( const Position &in_pos ) const;
		/**
		 * Находит чанк, которому принадлежит точка (in_pos.x,in_pos.y). Его координаты
		 * в сетке записываются в параметры out_x, out_y. Возможен выход за пределы.
		 * @param in_pos точка в мировой системе координат.
		 */
		void chunkFrom ( const Position &in_pos, int &out_x, int &out_y ) const;

		/**
		 * @return чанк из сетки в позиции (x,y)
		 * @param x столбец в сетке.
		 * @param y строка в сетке.
		 */
		const Chunk *operator () ( size_t x, size_t y) const;

		/**
		 * @return количество столбцов в сетке.
		 */
		size_t getLength () const;
		/**
		 * @return количество строк в сетке.
		 */
		size_t getWidth () const;

	private:
		void resize ();

	private:
		std::vector < std::vector<Chunk *> > m_mMatrix;
		Size oneChunkSize;
		Water *m_pWater;
	public:
		/** @class Landscape::LandscapeChunk::Moon
		 * Класс загружает текстуру луны и рисует ее.
		 */
		class Moon {
		public:
			/**
			 * @param path расположение текстуры.
			 */
			explicit Moon (const std::string &path);
			/**
			 * Рисует луну.
			 */
			void draw (const Graphics &in_lGraphics);
		private:
			void loadTexture();
			Moon ();
			GLuint m_Texture;
			std::string m_path;
			bool m_bLoaded;

			friend class LandscapeChunk;
		} *m_pMoon;
	};
}

#endif //_Landscape_LANDSCAPE_CHUNK_H_