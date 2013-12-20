/*
 * InFileSerializer.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 11.11.2012
 */

/** @file InFileSerializer.hpp
 * Описывает итерфейс записи в файл.
 * @see Client.hpp
 */

#ifndef _Serialization_IN_FILE_SERIALIZER_HPP_
#define _Serialization_IN_FILE_SERIALIZER_HPP_

#include <fstream>

#include "Serializer.hpp"


namespace Serialization {
	/** @class InFileSerializer
	 * Производит чтение из файла.\n
	 * Класс реализует операции чтения из файла, но требует от наследников определения
	 * метода открытия файлов.
	 * @see InBinaryFileSerializer
	 * @see InStringFileSerializer
	 */
	class /* interface */ InFileSerializer : virtual public Serializer {
	public:
		/**
		 * Конструктор.\n Готовит поле <b>m_ifStream</b> (сам файл) к работе. Подписываемся на
		 * всевозможные исключения.
		 */
		InFileSerializer();
		/**
		 * Виртуальный деструктор.\n Закрывает файл, если нужно.
		 */
		virtual ~InFileSerializer();
		/**
		 * Сериализация long int.
		 */
		virtual void serialize ( long &data );
		/**
		 * Сериализация double.
		 */
		virtual void serialize ( double &data );
		/**
		 * Сериализация строки.
		 */
		virtual void serialize ( std::string &data );

		/**
		 * Чисто виртуальный метод. Открытие файла.
		 * @param filename имя открываемого файла
		 */
		virtual void open ( const std::string &filename ) =0;

	protected:
		std::ifstream m_ifStream; ///< Поток чтения из файла.
	};

	/** @class InBinaryFileSerializer
	 * Производит чтение из бинарного файла.\n
	 * Класс реализует только открытие файла, все остальное наследует от ШтFileSerializer.
	 * @see Serialization::OutBinaryFileSerializer
	 */
	class InBinaryFileSerializer : virtual public InFileSerializer {
	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~InBinaryFileSerializer();

		/**
		 * Открытие файла c меткой <i>binary</i>.
		 * @param filename имя открываемого файла
		 */
		virtual void open ( const std::string &filename );

		/**
		 * Сериализация long int.
		 */
		virtual void serialize ( long &data );
		/**
		 * Сериализация double.
		 */
		virtual void serialize ( double &data );
		/**
		 * Сериализация строки.
		 */
		virtual void serialize ( std::string &data );
	};

	/** @class InStringFileSerializer
	 * Производит запись в строковый файл.\n
	 * Класс реализует только открытие файла, все остальное наследует от OutFileSerializer.\n
	 * <i>Причина, почему этот коасс не осуществляет вывод тэгов, описана в классе 
	 * Serialization::OutStringFileSerializer.</i>
	 * @see Serialization::InStringFileSerializer
	 */
	class InStringFileSerializer : virtual public InFileSerializer {
	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~InStringFileSerializer();

		/**
		 * Открытие файла.
		 * @param filename имя открываемого файла
		 */
		virtual void open ( const std::string &filename );
	};
}
#endif //_Serialization_OUT_FILE_SERIALIZER_HPP_