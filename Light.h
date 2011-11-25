/*
 * Light.h
 *
 *  Created on: 2011-11-09
 *      Author: zapo
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Model.h"

namespace Bomber {

class Light {

public:

	enum Type {
		DIRECTIONAL = 0,
		POSITIONAL = 1
	};

	Light(Type type, sf::Vector3f position);
	void Render();
	void Update() {}
	virtual ~Light();

	sf::Vector3f & GetPosition() {
		return position;
	}

	static void Setup();

private:
	Type type;
	sf::Vector3f position;
	GLUquadricObj * quadric;
};

}

#endif /* LIGHT_H_ */
