# MAKEFILE for
#	Serialization shared library
# Designed and written by
# 	Shvedov Yury 321 group.
#
# Powered by
# 	MediaLab, CMC MSU
#
# 11.11.2012

CC		= gcc
CXX		= g++
UIC		= uic-qt4
LINK		= g++
LDCONFIG	= /sbin/ldconfig

MAKE		= mingw32-make.exe
CD		=

CFLAGS		= -Wall -Werror -fPIC $(DEFINES) $(USERFLAGS)
LINKFLAGS	= -Wl,-O1 -fPIC $(USERFLAGS)
LIBS		= $(SUBLIBS) -lX11 -lGLU -lGL

SONAME		= libserialization.so
MAJOR		= 1
MINOR		= 0.0
OBJECTS		= src/OutFileSerializer.o \
		  src/InFileSerializer.o
SHLIB		= lib/$(SONAME).$(MAJOR).$(MINOR)
TARGET		= lib/$(SONAME)

all: $(OBJECTS) $(TARGET)

$(TARGET):	$(SHLIB) $(OBJECTS)
	$(LDCONFIG) -n lib

$(SHLIB): $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $@ $(OBJECTS)

src/OutFileSerializer.o:	src/OutFileSerializer.cpp \
				src/OutFileSerializer.hpp \
				src/Serializer.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

src/InFileSerializer.o:	src/InFileSerializer.cpp \
			src/InFileSerializer.hpp \
			src/Serializer.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(SHLIB)
