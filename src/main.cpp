/*
 * main.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 30.10.2012
 */


#include <ctime>
#include <iostream>
#include <cstdlib>
#include <Serialization>
#include <vector>

#include "Graphics.hpp"
#include "Generator/LandscapeGenerator.hpp"
#include "ControlEventHandler.hpp"
#include "Camera.hpp"
#include "Features/Water.hpp"
#include "Chunk/Serializer.hpp"

using namespace Landscape;
using namespace std;

int main(int argc, char *argv[])
{
	srand ( time(NULL) );

	GLFrontEnd::Client client ("Landscape");
	client.start();

	

	Camera camera(50,1000,Position(4000,4000,1000));
	LandscapeGenerator generator;

	InFileSerializer *in_pSerializer = NULL;
	vector <OutFileSerializer *> out_pSerializers;
	int index = 1;
	try {
		for (; index < argc; index+=2 ){
			if ( index + 1 >= argc ) {
				throw ios_base::failure("Usage: landscape [-ib in_binary_file] [-is in_string_file]\n"
								"\t [-ob out_binary_file] [-os out_string_file].\n" );
			}
			bool outFile = false;
			if ( !strcmp(argv[index],"-ob") ) {
				out_pSerializers.push_back(new OutBinaryFileSerializer);
				outFile = true;
			}  else if ( !strcmp(argv[index],"-os") ) {
				out_pSerializers.push_back(new OutStringFileSerializer);
				outFile = true;
			} else if ( ! strcmp (argv[index], "-ib") ) {
				if ( in_pSerializer != NULL ) {
					throw ios_base::failure( "There can be only one IN file.\n");
				}
				in_pSerializer = new InBinaryFileSerializer();
			} else if (! strcmp (argv[index], "-is" ) ) {
				if ( in_pSerializer != NULL ) {
					throw ios_base::failure( "There can be only one IN file.\n");
				}
				in_pSerializer = new InStringFileSerializer();
			} else {
				throw ios_base::failure("Usage: landscape [-ib in_binary_file] [-is in_string_file]\n"
								"\t [-ob out_binary_file] [-os out_string_file].\n" );
			}
			if ( outFile ) {
				out_pSerializers.back()->open(argv[index+1]);
			} else {
				in_pSerializer->open(argv[index+1]);
			}
		}
	} catch ( const ios_base::failure &exc ) {
		cerr << exc.what()<<endl;
		cerr << "Aborted.\n";
		return 1;
	}

	if ( in_pSerializer != NULL ) {
		generator.setSerializer ( *in_pSerializer );
	}
	generator.setOneSize (1280);
	generator.setChunksNumber(8,8);
	generator.setOneDimension(128);
	generator.setMaxHeight (800);
	generator.setMaxDifference (300);
	generator.setMoon ("textures/moon.bmp");
	ReflectiveWater water;
	generator.setWater (&water);
	Graphics graphics (generator, camera,12000);
	ControlEventHandler handler(camera);
	camera.setGraphics (&graphics);

	client.run (graphics,handler);
	client.destroy ();

	if ( out_pSerializers.size() != 0 ) {
		cout<<"Serializing. Please, wait.\n";
	}
	for ( index = 0; (unsigned)index < out_pSerializers.size(); ++index ) {
		graphics.serialize( *(out_pSerializers[index]));
	}
	return 0;
}

