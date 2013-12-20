/*
 * MatrixGenerator.hpp
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
 * Файл определяет класс Landscape::MatrixGenerator.
 */

#ifndef _Landscape_MATRIX_GENERATOR_HPP_
#define _Landscape_MATRIX_GENERATOR_HPP_

#include <cstdlib>
#include "Generator.hpp"

namespace Landscape {

	class MatrixChunk;
	class Colorer;
	/** @class Landscape::MatrixGenerator
	 * Генератор чанков Landscape::MatrixChunk.
	 * @see Landscape::Generator
	 * @see Landscape::MatrixChunk
	 */
	class MatrixGenerator : public Generator {

	public:
		/**
		 * Устанавливает все параметры генерации по умолчанию.
		 */
		MatrixGenerator ();
		virtual ~MatrixGenerator ();

		/**
		 * Устанавливает позицию нового чанка.
		 * @param in_Position новая позиция. (0,0)
		 */
		void setPosition ( const Position &in_Position );
		/**
		 * Устанавливает размеры нового чанка.
		 * @param in_Size новые размеры. (1)
		 */
		void setSize ( double in_Size );
		/**
		 * Устанавливает размерность матрицы нового чанка.
		 * @param in_Dimension новая размерность матрицы. (0) <i>Луче, чтобы она была степенью двойки</i>
		 */
		void setDimension ( size_t in_Dimension );
		/**
		 * Устанавливает максимально возможную высоту нового чанка.
		 * @param in_MaxValue новая максимально возможная высота. (100)
		 */
		void setMaxValue ( size_t in_MaxValue );
		/**
		 * Устанавливает коэффициент резкости нового чанка.
		 * @param in_Difference новый коэффициент резкости. (100)
		 */
		void setDifference (size_t in_Difference );
		/**
		 * Устанавливает краситель нового чанка.
		 * @param in_Colorer новый краситель. (Landscape::SimpleColorer)
		 * @see Landscape::Colorer
		 * @see Landscape::SimpleColorer
		 */
		void setColorer ( const Colorer *in_Colorer);
		/**
		 * Устанавливает соседей. Относилеьно них будут генерироваться края чанка.
		 * @param in_top чанк сверху. (NULL)
		 * @param in_botoom чанк снизу. (NULL)
		 * @param in_left чанк слева. (NULL)
		 * @param in_right чанк справа. (NULL)
		 */
		void setNeighbours ( const MatrixChunk *in_top,
				     const MatrixChunk *in_botoom,
				     const MatrixChunk *in_left,
				     const MatrixChunk *in_right );
		
		virtual Chunk *generate () const;
		/**
		 * Ничего не делает.
		 */
		virtual void generate (Chunk* chunk, const Position &in_Position) const;
		virtual Generator *clone () const;
		virtual double getMaxHeight() const;

	private:
		void generate ( MatrixChunk* chunk,
				int x00, int y00,
				int x11, int y11,
				const Colorer &in_lcolorer) const;
		double get ( const MatrixChunk* chunk, int x, int y ) const;
		void accept ( MatrixChunk* chunk, int x, int y, double z) const;
		
	private:
		Position m_Position;
		double m_Size;
		size_t m_Dimension;
		double m_MaxValue;
		size_t m_Difference;
		Colorer *m_pColorer;

		const MatrixChunk *m_top, *m_botoom, *m_left, *m_right; //Cornor Values
	};

}

#endif //_Landscape_MATRIX_GENERATOR_HPP_
