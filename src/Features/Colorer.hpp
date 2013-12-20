/*
 * Colorer.hpp
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
 * Файл определяет классы красителей ландшафта:\n
 * 1) интефейс красителей Landscape::Colorer,\n
 * 2) обычный краситель Landscape::SimpleColorer.
 */

#ifndef _Landscape_COLORER_H_
#define _Landscape_COLORER_H_

#include <Serialization>

namespace Landscape {

	class Graphics;
	/** @class Landscape::Colorer
	 * Интерфейс красителей, используемых в проекте Landscape.
	 * Имеются уровни (по возрастанию) песка, травы, гор и белых
	 * горных шапок. Уровень воды находится где-то в верхней половине уровня песка.
	 */
	class /* interface */ Colorer {
	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~Colorer ();
		/**
		 * @return точная копия объекта класса красителя.
		 */
		virtual Colorer *clone () const =0;
		/**
		 * @return уровень воды.
		 */
		virtual double getWater () const =0;
		/**
		 * @return TRUE, если высота height попадает на уровень травы, иначе - FALSE.
		 * @param height высота.
		 */
		virtual bool grass ( double height ) const =0;
		/**
		 * @return TRUE, если высота height попадает на уровень гор (включая снег), иначе - FALSE
		 * @param height высота.
		 */
		virtual bool mountain ( double height ) const =0;
		/**
		 * @return TRUE, если высота height попадает на уровень снежной вершины, иначе - FALSE
		 * @param height высота.
		 */
		virtual bool peak ( double height ) const =0;

		/** @struct Landscape::Colorer::Color3f
		 * Структура, описывающая цвет в формате RGB.
		 */
		struct Color3f {
			Color3f();
			float r,g,b;
			/**
			 * Затеняет цвет на уровень shadow.
			 * @param shadow степень затененности.
			 */
			void shade( float shadow);
		};
		/**
		 * @return цвет уровня, определяемый высотой height.
		 * @param height высота.
		 */
		Color3f virtual getColor ( double height ) const =0;

		/**
		 * Сериализация красителя.
		 * @param serializer сериализатор. <i>Спасибо, кэп!</i>
		 */
		virtual void serialize ( Serialization::Serializer &serializer ) =0;
	};

	/** @class Landscape::SimpleColorer
	 * Клас обычного красителя. Который в зависимости от максимального
	 * значения определяет цвета ланшафта.
	 */
	class SimpleColorer: public Colorer {
	public:
		/**
		 * Устанавливает максимальную высоту в height и пересчитывает
		 * уровни.
		 * @param height максимальная высота.
		 */
		SimpleColorer( double height = 100 );
		virtual ~SimpleColorer ();

		virtual SimpleColorer *clone () const;
		virtual Colorer::Color3f getColor ( double height ) const;
		virtual double getWater () const;
		virtual bool grass ( double height ) const;
		virtual bool mountain ( double height ) const;
		virtual bool peak ( double height ) const;

		virtual void serialize ( Serialization::Serializer &serializer );


		/**
		 * Устанавливает максимальную высоту в height и пересчитывает
		 * уровни.
		 * @param height максимальная высота.
		 */
		void setMaxHeight( double height );
	private:
		void calcLevels();
	private:
		double m_MaxHeight;
		double m_Sand;
		double m_Grass;
		double m_Stone;
		double m_Snow; //== m_MaxHeight
	};
}

#endif //_Landscape_COLORER_H_