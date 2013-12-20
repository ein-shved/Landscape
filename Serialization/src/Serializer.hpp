/*
 * Serializer.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 11.11.2012
 */

/** @file
 * Описывает итерфейс Serialization::Serializer.
 * @see OutFileSerializer.hpp
 * @see InFileSerializer.hpp
 */

#ifndef _Serialization_SERIALIZER_HPP_
#define _Serialization_SERIALIZER_HPP_

#include <string>

/** @namespace Serialization
 * Пространство имен Serialization содержит
 * объявление и описание классов и интерфейсов 
 * сериализации.
 * @see Serialization::Serializer
 * @see Serialization::InFileSerializer
 * @see Serialization::OutFileSerializer
 */
namespace Serialization {

	/** @class Serializer
	 * Интерфейс Serializer содержит набор абсолютно виртуальных
	 * методов, которые должны наследоваться классами, производящими
	 * сериализацию.
	 */
	class /* interface */ Serializer {
	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~Serializer() {};
		/**
		 * Сериализация long int.
		 */
		virtual void serialize ( long &data ) =0;
		/**
		 * Сериализация double.
		 */
		virtual void serialize ( double &data ) =0;
		/**
		 * Сериализация строки.
		 */
		virtual void serialize ( std::string &data ) =0;
	};
}

#endif //_Serialization_SERIALIZER_HPP_