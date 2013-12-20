/*
 * MatrixChunk.hpp
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
 * Файл определяет класс Landscape::MatrixChunk.
 */

#ifndef _Landscape_MATRIX_CHUNK_H_
#define _Landscape_MATRIX_CHUNK_H_

#include <vector>
#include<GL/gl.h>
#include "../Features/Colorer.hpp"
#include "Chunk.hpp"

namespace Landscape {

	class Water;
	class Graphics;
	class Colorer;
	class Object;
	/** @class Landscape::MatrixChunk
	 * Квадратный чанк, который определяется таблицей(матрицей) высот.\n
	 * <i>На самом деле он не квадратный, квадратной является матрица, а чанк растягивается
	 * параметром m_sSize. Но лучше, чтобы он был квадратным.</i>\n
	 * Так же чанк содержит набор оюбъектов, которые расположены на этом чанке.
	 * @see Landscape::Chunk
	 * @see Landscape::Size
	 * @see Landscape::Position
	 */
	class MatrixChunk : public Chunk {

	public:
		/**
		 * Таблица высот.
		 */
		typedef std::vector< std::vector<double> > DoubleMatrix;
	public:
		/**
		 * @param in_pPosition позиция чанка.
		 * @param in_size размер сторон чанка.
		 */
		MatrixChunk ( const Position &in_pPosition = Position(),
			      double in_size = 0 );
		/**
		 * @param x позиция чанка по оси x.
		 * @param y позиция чанка по оси y.
		 * @param in_size размер сторон чанка.
		 */
		MatrixChunk ( double x, double y,
			      double in_size = 0 );
		/**
		 * Конструктор копирования.
		 * @param in_other чанк - прообраз.
		 */
		MatrixChunk ( const MatrixChunk &in_other);
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~MatrixChunk();

		virtual MatrixChunk *clone() const;
		virtual void draw ( const Graphics &in_lGraphics );

		virtual double height (double x, double y) const;
		virtual double absoluteHeight (double x, double y) const;
		virtual void setSize ( const Size &in_sSize );
		virtual void setPosition ( const Position &in_pPosition );

		/**
		 * Задает размер матрицы.
		 * @param in_dimension новый размер матрицы.
		 */
		void setMatrix ( size_t in_dimension );
		/**
		 * Задает размер матрицы.
		 * @param in_dimension новый размер матрицы.
		 * @param in_default значения высот по умолчанию.
		 */
		void setMatrix ( size_t in_dimension, double in_default );
		/**
		 * Задает матрицу.
		 * @param in_mMatrix новая матрица.
		 */
		void setMatrix ( const DoubleMatrix &in_mMatrix );
		/**
		 * Задает краситель.
		 * @param in_Colorer новый краситель.
		 * @see Landscape::Colorer
		 */
		void setColorer ( const Colorer *in_Colorer);

		/**
		 * @return размер матрицы.
		 */
		size_t getDimention () const;

		/**
		 * @return значение матрицы высот в строке x и столбце y
		 * @param x столбец таблицы.
		 * @param y строка таблицы.
		 */
		const double &operator () (size_t x, size_t y) const;
		/**
		 * @return значение матрицы высот в строке x и столбце y
		 * @param x столбец таблицы.
		 * @param y строка таблицы.
		 */
		double &operator () (size_t x, size_t y);

		/**
		 * Добавляет новый объект.
		 * @param in_Object новый объект
		 * @param x положение объекта по оси х в системе координат чанка.
		 * @param y положение объекта по оси y в системе координат чанка.
		 * Объект не перемещается, если одна из переданных координат меньше 0.
		 * @see Landscape::Object
		 */
		void addObject ( const Object &in_Object, int x = -1, int y = -1);
		virtual void serialize ( Serializer &serializer);

	private:
		void allocVBO ( const Graphics &in_lGraphics ) const;
		void deallocVBO();

		void calcBigView();
		bool viewable (const Position &in_Pos) const;

		bool shadowed (double in_x, double in_y, double in_z,
			       const Chunk *chunk, double maxHeight ) const;

	private:
		DoubleMatrix m_mMatrix;
		double lDelta;
		double wDelta;
		Colorer *m_pColorer;

		double m_viewLength;
		double m_bigViewLength;
		bool m_Viewable;

		GLuint m_vboId;
		GLuint m_vboIdColors;
		GLuint m_vboIdNormals;
		GLuint m_vboIdIndexes;

		std::vector<Object*> m_mObjects;

		/** @struct Landscape::MatrixChunk::Normal
		 * Вектор нормали.
		 */
		struct Normal {
			float x,y,z;
		};
	};
}

#endif //_Landscape_MATRIX_CHUNK_H_

