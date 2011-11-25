/*
 * Light.cpp
 *
 *  Created on: 2011-11-09
 *      Author: zapo
 */

#include "Light.h"

namespace Bomber {

void Light::Setup() {

}

Light::Light(Type type, sf::Vector3f position): type(type), position(position) {
}

void Light::Render() {


	float gl_position[4] = {position.x, position.y, position.z, type};

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING) ;
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, gl_position);

}
Light::~Light() {
}

}
