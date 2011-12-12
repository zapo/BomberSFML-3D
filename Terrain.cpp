/*
 * Terrain.cpp
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */


#include "Shader.h"
#include "VertexBuffer.h"

#include "Terrain.h"
#include "Camera.h"
#include <iostream>
#include <algorithm>

#include "TerrainNode.h"
#include "VectorUtils.h"
#include <libnoise/noise.h>
#include "noiseutils.h"


using namespace noise;


namespace Bomber {


Terrain::Terrain(const std::string & texturePath, Camera & camera) : camera(&camera){

	maxResolution = 8.f;
	minResolution = 6.f;

	GenMap();

	if(mainTexture.LoadFromFile(texturePath)) {
		std::cout << "texture loaded" << std::endl;
	}
	mainTexture.SetSmooth(true);
	heights = NULL;
	scale = sf::Vector3f(1, 1, 1);


	/*shaders[0] = new Shader(
		"resources/v.sfx",
		"resources/f.sfx"
	);

	shaders[0]->Enable();
	shaders[0]->SendUniformInt("tex", 0);*/

}

const unsigned int Terrain::direction_adjacent_vertices[4][3] = {
		{(unsigned int)Vertex::TOPLEFT, 	(unsigned int)Vertex::TOPRIGHT, 	(unsigned int)Vertex::TOP}, 	// BOTTOM
		{(unsigned int)Vertex::BOTTOMRIGHT, (unsigned int)Vertex::TOPRIGHT, 	(unsigned int)Vertex::RIGHT}, 	// LEFT
		{(unsigned int)Vertex::BOTTOMLEFT, 	(unsigned int)Vertex::BOTTOMRIGHT, 	(unsigned int)Vertex::BOTTOM}, 	// TOP
		{(unsigned int)Vertex::BOTTOMLEFT, 	(unsigned int)Vertex::TOPLEFT, 		(unsigned int)Vertex::LEFT}  	// RIGHT
};


Terrain::~Terrain() {

	UnloadHeightMap();
	delete root;
}

void Terrain::UnloadHeightMap() {
	delete [] heights;
}



float Terrain::GetRelHeightAt(unsigned int x, unsigned y) const {

	if(heightMap.GetHeight() > 0) {

		return heights[x + size * y];

	}else {
		return 0;
	}

}

float Terrain::GetHeightAt(unsigned int x, unsigned z) const {
	float height = GetRelHeightAt(x / scale.x, z / scale.z);

	return height * scale.y;
}


bool Terrain::LoadHeightMap(const std::string & filename) {

	// Check if our map has a good format and is loadable

	if(!heightMap.LoadFromFile(filename)
			|| heightMap.GetWidth() != heightMap.GetHeight()) {
		return false;
	}

	// Check if our map width is of type 2^n + 1

	float n = log(heightMap.GetWidth() - 1) / log(2);
	int n_temp = n;


	// n is not integer, not good !
	if(static_cast<float>(n_temp) != n) {
		return false;
	}

	size = heightMap.GetWidth();

	heights = new float[size * size];

	const sf::Uint8 * pixel = heightMap.GetPixelsPtr();

	for(unsigned int i = 0; i < (size * size * 4); i+= (4)) {
		heights[i/4] = pixel[i] * 100.0f / 255.0f;
	}

	root = new TerrainNode(sf::Vector2f(size / 2, size / 2), size - 1, *this, NULL, TerrainNode::NONE);

	for(pv_it = position_vertice.begin(); pv_it != position_vertice.end(); pv_it ++) {
		vertices.push_back(pv_it->second);
		vertices_indexes[pv_it->second] = vertices.size() - 1;
	}

	for(nodes_it = nodes.begin(); nodes_it != nodes.end(); nodes_it ++) {

		TerrainNode & node = *(*(nodes_it));

		for(unsigned int j=0; j < 24; j++) {

			unsigned int index = GetIndexAt(*node.GetVertex((Vertex::Location)ordering[j]));
			indexes.push_back(index);
			node.indexes_ref.push_back(index);
		}

		node.SetLocalNeighbors();
		node.SetGlobalNeighbors();

	}

    buffers[0] = new VertexBuffer(GL_DYNAMIC_DRAW, vertices, indexes);
	return true;
}


bool Terrain::GenMap() {

	unsigned int seconds;
	seconds = time(NULL);

	sf::Randomizer random;
	random.SetSeed(seconds);

	module::Perlin noiseModule;
	noiseModule.SetOctaveCount(1);
	::noise::utils::NoiseMap heightMap;

	::noise::utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(noiseModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);

	heightMapBuilder.SetDestSize(257, 257);
	heightMapBuilder.SetBounds(6.0, 10.0, 2.0, 5.0);
	heightMapBuilder.Build();

	::noise::utils::RendererImage renderer;
	::noise::utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);


	renderer.Render();

	::noise::utils::WriterBMP writer;
	writer.SetSourceImage (image);
	writer.SetDestFilename ("heights.bmp");
	writer.WriteDestFile ();


	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, ::noise::utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, ::noise::utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint ( 0.0000, ::noise::utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0625, ::noise::utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, ::noise::utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, ::noise::utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, ::noise::utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, ::noise::utils::Color (255, 255, 255, 255)); // snow

	renderer.Render();
	writer.SetSourceImage (image);
	writer.SetDestFilename ("text.bmp");
	writer.WriteDestFile ();

	return true;
}


void Terrain::Update() {

	indexes.clear();

	numbTriangles = 0;
	numbCulledNodes = 0;
	numbNodes = 0;
	numbSkirts = 0;

	RefineNode(*root);

	buffers[0]->SetIndexes(indexes);
	buffers[0]->UploadIndexes();

}

void Terrain::Render() {
	glColorMaterial(GL_FRONT, GL_SPECULAR);

	glColor3f(1.f, 0.f, 0.f);
	glMateriali(GL_FRONT,GL_SHININESS,50);


	Model::Render();
}


void Terrain::RefineNode(TerrainNode & node) {

	if(Subdivide(node)) {
		//subdivide

		node.isLeaf = false;

		for(unsigned int i=0; i < 4; i++) {

			TerrainNode & child = *(node.GetChild((TerrainNode::Type)i));
			RefineNode(child);
		}

	} else {
		node.isLeaf = true;

		numbNodes ++;

		if(camera->IsInFrustrum(node)) {

			numbTriangles += 6;

			indexes.insert(indexes.end(), node.indexes_ref.begin(), node.indexes_ref.end());

			std::vector<TerrainNode*> adjacents;
			std::vector<TerrainNode*>::iterator adj_it;

			if(node.GetSize() < root->GetSize() / 2) {

				for(unsigned int i = 0; i < 4; i++) {

					adjacents = node.GetAjacentNodes((TerrainNode::Border)i);

					if(adjacents.size() > 1) {

						for(adj_it = adjacents.begin(); adj_it != adjacents.end(); adj_it++) {
							if(camera->IsInFrustrum(**adj_it) && (*adj_it)->GetLod() > node.GetLod()) {

								for(unsigned int j = 0; j < 3; j++) {
									//(*adj_it)->GetVertex((Vertex::Location)direction_adjacent_vertices[i][j])->col = sf::Vector3f(1.f, 0.f, 0.f);
									indexes.push_back(GetIndexAt(*(*adj_it)->GetVertex((Vertex::Location)direction_adjacent_vertices[i][j])));

								}
								numbSkirts ++;
								numbTriangles ++;
							}
						}
					}
				}

			}


		} else {
			numbCulledNodes ++;
		}
	}



}

bool Terrain::Subdivide(TerrainNode & node) {
	unsigned int size = node.GetSize();

	float distance = utils::VectorLength(node.GetVertex(Vertex::CENTER)->pos, camera->GetPosition());

	float maxheight = 0;
	float _heights[5] = {
			node.GetVertex(Vertex::BOTTOMLEFT)	->pos.y,
			node.GetVertex(Vertex::BOTTOMRIGHT)	->pos.y,
			node.GetVertex(Vertex::TOPLEFT)		->pos.y,
			node.GetVertex(Vertex::TOPRIGHT)	->pos.y
	};

	float _center_height = node.GetVertex(Vertex::CENTER)->pos.y;

	for(int i = 0; i < 4; i++) {
		maxheight = std::max(maxheight, (float)fabs(_center_height - _heights[i]));
	}


	float f = distance / ((float) size * std::max(maxResolution, 0.1f) * std::max(maxheight, 1.f));

	return f < 1.0f && (size / 2) >= TerrainNode::MIN_SIZE;
}




}















