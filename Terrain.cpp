/*
 * Terrain.cpp
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */



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

const unsigned int Terrain::ordering[24] = {
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 5,
	0, 5, 6,
	0, 6, 7,
	0, 7, 8,
	0, 8, 1
};


Terrain::Terrain(const std::string & texturePath, Camera & camera) {
	maxResolution = 8.f;
	minResolution = 1.f;

	this->camera = &camera;

	if(mainTexture.LoadFromFile(texturePath)) {
		std::cout << "texture loaded" << std::endl;
	}
	mainTexture.SetSmooth(true);
	heights = NULL;
	buffer = NULL;

	scale = sf::Vector3f(1, 1, 1);

}

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

	SetCenter(sf::Vector2f((GetPosition().x + size) / 2, (GetPosition().y + size) / 2));

	root = new TerrainNode(sf::Vector2i(size / 2, size / 2), size - 1, *this);

	unsigned int start = 0;

	for(unsigned int i=0; i < nodes.size(); i++) {

		TerrainNode & node = *(nodes[i]);
		start = vertices.size();

		for(unsigned int j=0; j < 9; j++) {
			Vertex3D & vertex = *(node.GetVertex((Vertex::Location)j));
			vertices.push_back(vertex);
		}

		for(unsigned int j=0; j < 24; j++) {
			indexes.push_back(ordering[j] + start);
		}
	}

	buffer = new VertexBuffer(GL_DYNAMIC_DRAW, vertices, indexes);
	return true;
}


bool Terrain::GenMap() {

	unsigned int seconds;
	seconds = time(NULL);

	sf::Randomizer random;
	random.SetSeed(seconds);

	module::Perlin noiseModule;
	noiseModule.SetOctaveCount(5);
	::noise::utils::NoiseMap heightMap;

	::noise::utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(noiseModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);

	heightMapBuilder.SetDestSize(513, 513);
	heightMapBuilder.SetBounds(6.0, 10.0, 2.0, 5.0);
	heightMapBuilder.Build();

	::noise::utils::RendererImage renderer;
	::noise::utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);

	renderer.Render();

	::noise::utils::WriterBMP writer;
	writer.SetSourceImage (image);
	writer.SetDestFilename ("gen.bmp");
	writer.WriteDestFile ();

	return true;
}


void Terrain::Update() {

	vertices.clear();
	indexes.clear();

	numbTriangles = 0;
	numbNodes = 0;

	RefineNode(*root);

	buffer->Setup(vertices, indexes);
}


void Terrain::Render(float framerate) {

	this->framerate = framerate;
	buffer->Render();
}

void Terrain::RefineNode(TerrainNode & node) {

	if(Subdivide(node)) {
		//subdivide

		node.isLeaf = false;

		for(unsigned int i=0; i <= 3; i++) {

			TerrainNode & child = *(node.GetChild((TerrainNode::Type)i));
			RefineNode(child);
		}

	} else {
		node.isLeaf = true;

		numbNodes ++;

		if(camera->IsInFrustrum(node)) {

			numbTriangles += 6;

			unsigned int start = vertices.size();

			for(unsigned int j=0; j < 9; j++) {
				Vertex3D & vertex = *(node.GetVertex((Vertex::Location)j));
				vertices.push_back(vertex);
			}

			for(unsigned int j=0; j < 24; j++) {
				indexes.push_back(ordering[j] + start);
			}

		} else {
			numbCulledNodes ++;
		}
	}
}

bool Terrain::Subdivide(TerrainNode & node) {
	unsigned int size = node.GetSize();

	float distance = ( float ) utils::VectorLength(node.GetVertex(Vertex::CENTER)->pos, camera->GetPosition());

	float maxheight = 0;
	float _heights[5];

	sf::IntRect boundBox = node.GetBoundBox();

	_heights[0] = GetRelHeightAt(node.GetCenter().x	, boundBox.Top);
	_heights[1] = GetRelHeightAt(node.GetCenter().x	, boundBox.Bottom);
	_heights[2] = GetRelHeightAt(boundBox.Right	, node.GetCenter().y);
	_heights[3] = GetRelHeightAt(boundBox.Left	, node.GetCenter().y);

	float _center_height = GetRelHeightAt(node.GetCenter().x , node.GetCenter().y);

	for(int i = 0; i < 4; i++) {
		maxheight = std::max(maxheight, (float) fabs(_center_height - _heights[i]));
	}


	float f = (distance * 0.1) / ((float) size * std::max(maxResolution, 0.1f) * std::max(maxheight ,1.f));

	return f < 1.0f && (size / 2) >= TerrainNode::MIN_SIZE;
}

}















