# MAKEFILE for
#	GlFrontend shared library
# Designed and written by
# 	Shvedov Yury 321 group.
# 
# Powered by
# 	MediaLab, CMC MSU
# 
# 3.11.2012

CC		= gcc
CC		= gcc
CXX		= g++
UIC		= uic-qt4
LINK		= g++
LDCONFIG	= /sbin/ldconfig

CFLAGS		= -Wall -Werror -fPIC $(DEFINES) $(USERFLAGS)
LINKFLAGS	= -Wl,-O1 -fPIC $(USERFLAGS)
LIBS		= $(SUBLIBS) -lX11 -lGLU -lGL

SONAME		= libglfrontend.so
MAJOR		= 1
MINOR		= 0.0
OBJECTS		= src/Client.o
SHLIB		= lib/$(SONAME).$(MAJOR).$(MINOR)
TARGET		= lib/$(SONAME)

all: $(OBJECTS) $(TARGET)

$(TARGET):	$(SHLIB) $(OBJECTS)
	$(LDCONFIG) -n lib

$(SHLIB): $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $@ $(OBJECTS)

src/Client.o:	src/Client.cpp \
		src/Client.hpp \
		src/GLFrontEnd.hpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(SHLIB)
