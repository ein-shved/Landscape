/*
 * LandscapeGenerator.hpp
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
 * Файл определяет класс Landscape::LandscapeGenerator.
 */

#ifndef _Landscape_LANDSCAPE_GENERATOR_H_
#define _Landscape_LANDSCAPE_GENERATOR_H_

#include <cstring>

#include "../Chunk/Chunk.hpp"
#include "Generator.hpp"

namespace Landscape {
	class Colorer;
	class Water;
	class Serializer;
	/** @class Landscape::MatrixGenerator
	 * Генератор чанков Landscape::LandscapeGenerator. Генерирует чанк
	 * Landscape::LandscapeGenerator, сетка которого заполнена чанками
	 * Landscape::MatrixChunk, которые, в свою очередь генируются с 
	 * помощью Landscape::MatrixGenerator.
	 * Если был задан сериализатор, что новый чанк загружается сериализатором.
	 * @see Landscape::Generator
	 * @see Landscape::LandscapeGenerator
	 * @see Landscape::MatrixChunk
	 * @see Landscape::MatrixGenerator
	 */
	class LandscapeGenerator : public Generator {

	public:

		/**
		 * Устанавливает все параметры генерации по умолчанию.
		 */
		LandscapeGenerator ();
		virtual ~LandscapeGenerator ();
		
		virtual Chunk *generate () const;
		virtual void generate (Chunk* chunk, const Position &in_Position) const;
		virtual Generator *clone () const;
		virtual double getMaxHeight() const;

	public:
		/**
		 * Устанавливает размеры всех чанков сетки.
		 * @param in_Size новые размеры. (1)
		 */
		void setOneSize ( double in_size );
		/**
		 * Устанавливает размерность матриц всех чанков сетки.
		 * @param in_Dimension новая размерность матрицы. (0) <i>Луче, чтобы она была степенью двойки</i>
		 */
		void setOneDimension ( size_t in_dim );
		/**
		 * Устанавливает максимально возможную высоту нового чанка.
		 * @param in_MaxValue новая максимально возможная высота. (100)
		 */
		void setMaxHeight ( unsigned int in_maxHeight );
		/**
		 * Устанавливает коэффициент резкости нового чанка.
		 * @param in_Difference новый коэффициент резкости. (100)
		 */
		void setMaxDifference ( unsigned int in_maxDiff );
		/**
		 * Устанавливает позицию нового чанка.
		 * @param in_Position новая позиция. (0,0)
		 */
		void setPosition ( const Position &in_Position );
		/**
		 * Устанавливает количество чанков в сетке.
		 * @param x число толбцов. (1)
		 * @param y число строк. (1)
		 */
		void setChunksNumber (size_t x, size_t y); //matrix X x Y
		/**
		 * Задает объект воды нового чанка.
		 * @param in_Water новый объект воды (копируется). (NULL)
		 * @see Landscape::Water
		 */
		void setWater ( const Water *in_Water);
		/**
		 * Устанавливает краситель нового чанка.
		 * @param in_Colorer новый краситель. (Landscape::SimpleColorer)
		 * @see Landscape::Colorer
		 * @see Landscape::SimpleColorer
		 */
		void setColorer ( const Colorer *in_Colorer);
		/**
		 * Задается путь к текстуре луны.
		 * @param path путь к текстуре луны.
		 */
		void setMoon ( const std::string &path );

		/**
		 * Устанавливается сериализатор ввода.
		 * @param serializer сериализатор. <i>Спасибо, кэп!</i>
		 */
		void setSerializer ( Serializer &in_serializer);

	private:
		double m_chunkSize;
		size_t m_chunkDim;
		unsigned int m_maxHeight;
		unsigned int m_maxDiff;
		Position m_Position;
		size_t m_xNumber, m_yNumber;
		Colorer *m_pColorer;
		Water *m_pWater;
		
		std::string m_MoonPath;
		bool m_bMoon;

		Serializer *m_pSerializer;
	};
}


#endif //_Landscape_LANDSCAPE_GENERATOR_H_