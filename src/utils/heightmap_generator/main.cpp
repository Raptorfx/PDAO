#include <iostream>
#include "noise/noise.h"
#include "noiseutils.h"
#include <sstream>

//Folder Handling
#include <windows.h>

int main()
{
	//Config
	int total_x = 10;
	int total_y = 10;
	int chunk_size = 256;
	int preview_size_multiplier = 1; //Increase the preview size for testing, default = 1
	bool colour_output = false;
	bool colour_preview_output = true;


	if( !(CreateDirectory("img", NULL) || ERROR_ALREADY_EXISTS == GetLastError()))
		 std::cout << "Failed to create img folder" << std::endl;

	std::cout << "Building Terrain: " << std::endl
		<< "X size        : " << total_x*chunk_size << std::endl
		<< "Y size        : " << total_y*chunk_size << std::endl
		<< "Chunk Size    : " << chunk_size << std::endl
		<< "X elements    : " << total_x << std::endl
		<< "Y elements    : " << total_y << std::endl
		<< "Total elements: " << total_x*total_y << std::endl 
		<< "Preview size: : " << total_x*chunk_size*preview_size_multiplier << "x" << total_y*chunk_size*preview_size_multiplier << std::endl
		<< std::endl;



	noise::module::RidgedMulti mountainTerrain;
	mountainTerrain.SetSeed(1234);


	//Basic terrain
	noise::module::Billow baseFlatTerrain;
	baseFlatTerrain.SetSeed(31137);
	baseFlatTerrain.SetFrequency (2.0);

	noise::module::ScaleBias flatTerrain;
	flatTerrain.SetSourceModule(0,baseFlatTerrain);
	flatTerrain.SetScale(0.1);
	flatTerrain.SetBias(-0.65);


	//Flat Planes for the terrain
	noise::module::RidgedMulti baseflatPlanes;
	baseflatPlanes.SetSeed(56842);
	baseflatPlanes.SetFrequency(0.5);

	noise::module::ScaleBias flatPlanes;
	flatPlanes.SetSourceModule(0,baseflatPlanes);
	flatPlanes.SetScale(0.1);
	flatPlanes.SetBias(-0.65);

	//Merge Planes and Terrain

	noise::module::Perlin terrainPlanes;
	terrainPlanes.SetFrequency (0.5);
	terrainPlanes.SetPersistence (0.25);
	terrainPlanes.SetSeed(98574);
	terrainPlanes.SetOctaveCount(1);

	noise::module::Select finalTerrainPlains;
	finalTerrainPlains.SetSourceModule (0, flatTerrain);
	finalTerrainPlains.SetSourceModule (1, flatPlanes);
	finalTerrainPlains.SetControlModule (terrainPlanes);
	finalTerrainPlains.SetBounds (0.0, 1000.0);
	finalTerrainPlains.SetEdgeFalloff (0.3);


	//Final stage

	noise::module::Perlin terrainType;
	terrainType.SetFrequency (0.5);
	terrainType.SetPersistence (0.100);
	terrainType.SetOctaveCount(1);
	terrainType.SetSeed(50);


	noise::module::Select terrainSelector;
	terrainSelector.SetSourceModule (0, finalTerrainPlains);
	terrainSelector.SetSourceModule (1, mountainTerrain);
	terrainSelector.SetControlModule (terrainType);
	terrainSelector.SetBounds (0.0, 1000.0);
	terrainSelector.SetEdgeFalloff (0.125);

	module::Turbulence finalTerrain;
    finalTerrain.SetSourceModule (0, terrainSelector);
	finalTerrain.SetFrequency (2.0);
    finalTerrain.SetPower (0.125);

	noise::utils::NoiseMap height_map;
	
	noise::utils::NoiseMapBuilderPlane height_map_builder;
	//height_map_builder.SetSourceModule(mountainTerrain);
	height_map_builder.SetSourceModule(finalTerrain);
	height_map_builder.SetDestNoiseMap(height_map);

	height_map_builder.SetDestSize(chunk_size,chunk_size);
	//height_map_builder.SetBounds(0.0,1.0,0.0,1.0);

	double i_d = 0.0;
	double j_d = 0.0;

	for(int i = (total_x/2)*-1; i < total_x/2; ++i)
	{
		std::stringstream i_conv;
		i_conv << i;

		for(int j = (total_y/2)*-1; j < total_y/2; ++j)
		{
			std::cout << "Building " << i << " : " << j << std::endl;
			
			std::stringstream j_conv;
			j_conv << j;
			height_map_builder.SetBounds(i, i+1.0, j, j+1.0);

			height_map_builder.Build();

			noise::utils::RendererImage renderer;
			noise::utils::Image image;

			renderer.SetSourceNoiseMap(height_map);
			renderer.SetDestImage(image);

			if(colour_output)
			{
				renderer.ClearGradient ();
				renderer.AddGradientPoint (-1.00, utils::Color ( 32, 32,   224, 255)); // water
				renderer.AddGradientPoint (-0.75, utils::Color ( 32, 160,   0, 255)); // grass
				renderer.AddGradientPoint (-0.25, utils::Color (131, 103,   61, 255)); // dirt
				renderer.AddGradientPoint ( 0.25, utils::Color (128, 128, 128, 255)); // rock
				renderer.AddGradientPoint ( 1.00, utils::Color (255, 255, 255, 255)); // snow
				renderer.EnableLight ();
				renderer.SetLightContrast (3.0);
				renderer.SetLightBrightness (2.0);
			}

			renderer.Render();

			noise::utils::WriterBMP writer;
			writer.SetSourceImage(image);
			writer.SetDestFilename("img/height_map_"+i_conv.str()+"_"+j_conv.str()+".bmp");
			writer.WriteDestFile();

			j_d++;
		}
		j_d = 0.0;

		i_d++;
	}
	

	{
		std::cout << "Building map preview" << std::endl;

		//height_map_builder.SetDestSize(total_x*chunk_size,total_y*chunk_size);
		height_map_builder.SetDestSize(1024,1024);
		height_map_builder.SetBounds((total_x/2)*-1*preview_size_multiplier, 
										total_x/2*preview_size_multiplier, 
										(total_y/2)*-1*preview_size_multiplier, 
										total_y/2*preview_size_multiplier);

		//height_map_builder.EnableSeamless(true);
		height_map_builder.Build();
	
		noise::utils::RendererImage renderer;
		noise::utils::Image image;

		renderer.SetSourceNoiseMap(height_map);
		renderer.SetDestImage(image);

		if(colour_preview_output)
		{
			renderer.ClearGradient ();
			renderer.AddGradientPoint (-1.00, utils::Color ( 32, 32,   224, 255)); // water
			renderer.AddGradientPoint (-0.75, utils::Color ( 32, 160,   0, 255)); // grass
			renderer.AddGradientPoint (-0.25, utils::Color (131, 103,   61, 255)); // dirt
			renderer.AddGradientPoint ( 0.25, utils::Color (128, 128, 128, 255)); // rock
			renderer.AddGradientPoint ( 1.00, utils::Color (255, 255, 255, 255)); // snow
			renderer.EnableLight ();
			renderer.SetLightContrast (3.0);
			renderer.SetLightBrightness (2.0);
		}

		renderer.Render();

		noise::utils::WriterBMP writer;
		writer.SetSourceImage(image);
		writer.SetDestFilename("img/height_map_total.bmp");
		writer.WriteDestFile();
	}


	std::cout << "Return to exit" << std::endl;
	char c;
	std::cin.get(c);

	return 0;
}