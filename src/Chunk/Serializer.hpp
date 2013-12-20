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
 * Файл описывает собственную сторуктуру сериализации
 * проекта Landscape.
 */
#ifndef _Landscape_SERIALIZER_HPP_
#define _Landscape_SERIALIZER_HPP_

#include <Serialization>

namespace Landscape  {
	class Chunk;
	class Water;
	class Fog;
	class Object;
	class Colorer;

	/** @class Landscape::Serializer
	 * В силу того, что в проекте Landscape используется динамический полиморфизм
	 * в описании чанков и других объектов, приходится в некотором смысле повторять
	 * структуру библиотеки Serialization для того, чтобы определить тип записанного
	 * объекта. Идея проста: перед вызовом метода serialize сериализируемого объекта
	 * предварительно сериализируется его имя (с помощью std::type_info) потом - сам
	 * объект.\n 
	 * @see Serialization::Serializer
	 * <i>Тут я очень пожалел, что отказался от идеи наследования всех класснов
	 * от одного базового класса.</i>
	 * 
	 */
	class /* interface */ Serializer: virtual public Serialization::Serializer {
	public:
		/**
		 * Виртуальный деструктор.
		 */
		virtual ~Serializer() {};
		/* We have troubles with overloading between virtuals */

		/**
		 * Чисто виртуальный метод. Сериализация чанка.
		 * @see Landscape::Chunk
		 */
		virtual void serializeChunk ( Chunk *&data ) =0;
		/**
		 * Чисто виртуальный метод. Сериализация воды.
		 * @see Landscape::Water
		 */
		virtual void serializeWater ( Water *&data ) =0;
		/**
		 * Чисто виртуальный метод. Сериализация тумана.
		 * @see Landscape::Fog
		 */
		virtual void serializeFog ( Fog *&data ) =0;
		/**
		 * Чисто виртуальный метод. Сериализация объектов.
		 * @see Landscape::Object
		 */
		virtual void serializeObject ( Object *&data ) =0;
		/**
		 * Чисто виртуальный метод. Сериализация красителя.
		 * @see Landscape::Colorer
		 */
		virtual void serializeColorer ( Colorer *&data ) =0;
	};

	/** @class Landscape::OutFileSerializer
	 * Объединяет в себе фукциональность Landscape::Serializer и
	 * Serialization::OutFileSerializer.
	 * @see InFileSerializer
	 */
	class OutFileSerializer:	virtual public Serialization::OutFileSerializer,
					virtual public Landscape::Serializer
	{};

	/** @class Landscape::OutBinaryFileSerializer
	 * Объединяет в себе фукциональность Landscape::OutFileSerializer и
	 * Serialization::OutBinaryFileSerializer. Описывает методы сериализации
	 * объектов проекта Landscape.
	 * @see InBinaryFileSerializer
	 */
	class OutBinaryFileSerializer: 	virtual public Serialization::OutBinaryFileSerializer,
					virtual public Landscape::OutFileSerializer
	{
		virtual void serializeChunk ( Chunk *&data );
		virtual void serializeWater ( Water *&data );
		virtual void serializeFog ( Fog *&data );
		virtual void serializeObject ( Object *&data );
		virtual void serializeColorer ( Colorer *&data );
	};

	/** @class Landscape::OutStringFileSerializer
	 * Объединяет в себе фукциональность Landscape::OutFileSerializer и
	 * Serialization::OutStringFileSerializer.\n Описывает методы сериализации
	 * объектов проекта Landscape. При этом пораждается некторая xml-структура,
	 * тэги которой окружают только имена записываемых объектов.
	 * @see InStringFileSerializer
	 */
	class OutStringFileSerializer: 	virtual public Serialization::OutStringFileSerializer,
					virtual public Landscape::OutFileSerializer
	{
		virtual void serializeChunk ( Chunk *&data );
		virtual void serializeWater ( Water *&data );
		virtual void serializeFog ( Fog *&data );
		virtual void serializeObject ( Object *&data );
		virtual void serializeColorer ( Colorer *&data );
	};

	/** @class Landscape::InFileSerializer
	 * Объединяет в себе фукциональность Landscape::Serializer и
	 * Serialization::InFileSerializer.
	 * @see OutFileSerializer
	 */
	class InFileSerializer:	virtual public Serialization::InFileSerializer,
				virtual public Landscape::Serializer
	{};

	/** @class Landscape::InBinaryFileSerializer
	 * Объединяет в себе фукциональность Landscape::Serializer и
	 * Serialization::InBinaryFileSerializer.
	 * @see OutBinaryFileSerializer
	 */
	class InBinaryFileSerializer: 	virtual public Serialization::InBinaryFileSerializer,
					virtual public Landscape::InFileSerializer
	{

		virtual void serializeChunk ( Chunk *&data );
		virtual void serializeWater ( Water *&data );
		virtual void serializeFog ( Fog *&data );
		virtual void serializeObject ( Object *&data );
		virtual void serializeColorer ( Colorer *&data );
	};

	/** @class Landscape::InStringFileSerializer
	 * Объединяет в себе фукциональность Landscape::OutFileSerializer и
	 * Serialization::OutStringFileSerializer.\n Описывает методы сериализации
	 * объектов проекта Landscape. При этом считывается структура, пораждаемая
	 * Landscape::OutStringFileSerializer
	 * @see OutStringFileSerializer
	 */
	class InStringFileSerializer: 	virtual public Serialization::InStringFileSerializer,
					virtual public Landscape::InFileSerializer
	{

		virtual void serializeChunk ( Chunk *&data );
		virtual void serializeWater ( Water *&data );
		virtual void serializeFog ( Fog *&data );
		virtual void serializeObject ( Object *&data );
		virtual void serializeColorer ( Colorer *&data );
	};
}

#endif //_Landscape_SERIALIZER_HPP_