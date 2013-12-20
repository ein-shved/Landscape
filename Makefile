CC		= gcc
CC		= gcc
CXX		= g++
UIC		= uic-qt4
LINK		= g++

MAKE		= mingw32-make.exe
CD		=

CFLAGS		= -Wall -Werror $(DEFINES) $(USERFLAGS) -IGLFrontEnd/include -ISerialization/include
LINKFLAGS	= -Wl,-O1 $(USERFLAGS)
LIBS		= $(SUBLIBS) -lX11 -lGLU -lGL -lSDL -lSDL_image -lrt -LGLFrontEnd/lib -LSerialization/lib -lglfrontend -lserialization

OBJECTS		= src/Graphics.o \
		  src/ControlEventHandler.o \
		  src/Camera.o \
		  src/Position.o \
		  src/Chunk/Chunk.o \
		  src/Chunk/MatrixChunk.o \
		  src/Chunk/LandscapeChunk.o \
		  src/Chunk/Serializer.o \
		  src/Generator/Generator.o \
		  src/Generator/LandscapeGenerator.o \
		  src/Generator/MatrixGenerator.o \
		  src/Features/Water.o \
		  src/Features/Fog.o \
		  src/Features/Colorer.o \
		  src/Features/Objects.o \
		  src/main.o
TARGET		= bin/landscape

Debug: all

all: $(OBJECTS) $(TARGET)

FORCE :

$(TARGET):	$(OBJECTS)
	$(LINK) $(LINKFLAGS) $(LIBS) $(OBJECTS) -o $@

src/Graphics.o:	src/Graphics.cpp \
		src/Graphics.hpp \
		src/Chunk/Chunk.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/ControlEventHandler.o:	src/ControlEventHandler.cpp \
				src/ControlEventHandler.hpp \
				src/Position.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Camera.o:	src/Camera.cpp \
		src/Camera.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Position.o:	src/Position.cpp \
		src/Position.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Chunk/Chunk.o:	src/Chunk/Chunk.cpp \
			src/Chunk/Chunk.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Chunk/MatrixChunk.o:	src/Chunk/MatrixChunk.cpp \
				src/Chunk/MatrixChunk.hpp \
				src/Chunk/Chunk.hpp \
				src/Features/Water.hpp \
				src/Features/Colorer.hpp \
				src/Features/Water.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Chunk/Serializer.o:	src/Chunk/Serializer.cpp \
			src/Chunk/Serializer.hpp \
			Serialization \
			src/Chunk/MatrixChunk.hpp \
			src/Chunk/LandscapeChunk.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Chunk/LandscapeChunk.o:	src/Chunk/LandscapeChunk.cpp \
				src/Chunk/LandscapeChunk.hpp \
				src/Chunk/MatrixChunk.hpp \
				src/Features/Water.hpp \
				src/Features/Colorer.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Generator/Generator.o: 	src/Generator/Generator.cpp \
				src/Generator/Generator.hpp \
				src/Chunk/Chunk.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Generator/LandscapeGenerator.o:	src/Generator/LandscapeGenerator.cpp \
					src/Generator/LandscapeGenerator.hpp \
					src/Generator/Generator.hpp \
					src/Generator/MatrixGenerator.hpp \
					src/Features/Colorer.hpp \
					src/Features/Water.hpp
	$(CXX) -c $(CFLAGS) $< -o $@


src/Generator/MatrixGenerator.o: 	src/Generator/MatrixGenerator.cpp \
					src/Generator/MatrixGenerator.hpp \
					src/Generator/Generator.hpp \
					src/Features/Water.hpp \
					src/Features/Colorer.hpp \
					src/Features/Objects.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Features/Water.o:	src/Features/Water.cpp \
			src/Features/Water.hpp \
			src/Graphics.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Features/Fog.o:	src/Features/Fog.cpp \
			src/Features/Fog.hpp \
			src/Graphics.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/Features/Colorer.o:	src/Features/Colorer.cpp \
			src/Features/Colorer.hpp
	$(CXX) -c $(CFLAGS) $< -o $@
src/Features/Objects.o: src/Features/Objects.cpp \
			src/Features/Objects.hpp \
			src/Graphics.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/main.o:	src/main.cpp \
		GLFrontEnd \
		Serialization \
		src/Graphics.hpp \
		src/Chunk/Chunk.hpp \
		src/ControlEventHandler.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

GLFrontEnd: FORCE
	@( cd GLFrontEnd; make;)
Serialization: FORCE
	@( cd Serialization; make;)

clean:
	rm -f $(OBJECTS) $(TARGET); ( cd GLFrontEnd; make clean; ) ; ( cd Serialization; make clean; )
