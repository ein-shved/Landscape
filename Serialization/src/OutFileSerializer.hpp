/*
 * OutFileSerializer.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 11.11.2012
 */


/** @file OutFileSerializer.hpp
 * Описывает итерфейс записи в файл.
 * @see Client.hpp
 */

#ifndef _Serialization_OUT_FILE_SERIALIZER_HPP_
#define _Serialization_OUT_FILE_SERIALIZER_HPP_

#include <fstream>

#include "Serializer.hpp"


namespace Serialization {
	/** @class OutFileSerializer
	 * Производит запись в файл.\n
	 * Класс реализует операции записи в файл, но требует от наследников определения
	 * метода открытия файлов.
	 * @see OutBinaryFileSerializer
	 * @see OutStringFileSerializer
	 */
	class OutFileSerializer : virtual public Serializer {
	public:
		/**
		 * Конструктор.\n Готовит поле <b>m_ofStream</b> (сам файл) к работе. Подписываемся на
		 * всевозможные исключения.
		 */
		OutFileSerializer();
		/**
		 * Виртуальный деструктор.\n Закрывает файл, если нужно.
		 */
		virtual ~OutFileSerializer();
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
		std::ofstream m_ofStream; ///< Поток вывода в файл.
	};

	/** @class OutBinaryFileSerializer
	 * Производит запись в бинарный файл.\n
	 * Класс реализует только открытие файла, все остальное наследует от OutFileSerializer.
	 * @see Serialization::InBinaryFileSerializer
	 */
	class OutBinaryFileSerializer : virtual public OutFileSerializer {
	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~OutBinaryFileSerializer();

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

	/** @class OutStringFileSerializer
	 * Производит запись в строковый файл.\n
	 * Класс реализует только открытие файла, все остальное наследует от OutFileSerializer.\n
	 * <i>По идее, класс должен реализовывать запись в одном из текстовых форматов (html, xml и т.п.),
	 * и перегружать методы сериализации. Но этого сделано не было, так как записывать каждое число с
	 * сопровождающими его тэгами - достаточно накладная операция в случае если чисел много. Если 
	 * необходим такой вид записи, то всегда есть возможность наследовать этот класс и описать свою
	 * процедуру записи в файл. </i>
	 * @see Serialization::InStringFileSerializer
	 */
	class OutStringFileSerializer : virtual public OutFileSerializer {
	public:
		/**
		 * Конструктор устанавливает начальный отступ в 0.
		 */
		OutStringFileSerializer();
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~OutStringFileSerializer();

		/**
		 * Открытие файла.
		 * @param filename имя открываемого файла
		 */
		virtual void open ( const std::string &filename );
	protected:
		int m_Indent; ///<Текущий отступ в xml-фалйле
	};
}
#endif //_Serialization_OUT_FILE_SERIALIZER_HPP_