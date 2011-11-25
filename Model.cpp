/*
 * Model.cpp
 *
 *  Created on: 2011-11-12
 *      Author: zapo
 */

#include "Model.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


namespace Bomber {

const unsigned int Model::ordering[24] = {
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 5,
	0, 5, 6,
	0, 6, 7,
	0, 7, 8,
	0, 8, 1
};

Model::Model() {
	// TODO Auto-generated constructor stub

}

Model::~Model() {
	// TODO Auto-generated destructor stub
}

void Model::Render() {

	//glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);

	//mainTexture.Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

	for(shaders_it = shaders.begin(); shaders_it != shaders.end(); shaders_it ++) {
		if(shaders_it->second->enabled) {
			shaders_it->second->Use();
		}
	}

	glPushMatrix();


	for(buffers_it = buffers.begin(); buffers_it != buffers.end(); buffers_it++) {
		buffers_it->second->Render();
	}

	glPopMatrix();

	glUseProgram(0);

	glDisable(GL_TEXTURE_2D);
}


}
