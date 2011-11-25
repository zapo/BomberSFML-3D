/*
 * Model.h
 *
 *  Created on: 2011-11-12
 *      Author: zapo
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "Shader.h"
#include "VertexBuffer.h"
#include <boost/unordered_map.hpp>
#include <string>
#include <map>

namespace Bomber {

class Model {
public:

	Model();
	virtual ~Model();


	void SetTextureRepeat(bool repeat) {
		textureRepeat = repeat;
	}

	virtual void Render();

	virtual void Update() {
		update_time.Reset();
	}


	static const unsigned int ordering[24];

	inline GLuint GetIndexAt(Vertex3D & vertex) {
		return vertices_indexes[&vertex];
	}

protected:
	std::string LoadSource(std::string filename);
	GLuint LoadShader(GLenum type, std::string filename);

	boost::unordered_map<unsigned int, Shader *> shaders;
	boost::unordered_map<unsigned int, Shader *>::iterator shaders_it;
	sf::Image mainTexture;



	sf::Clock update_time;



	bool textureRepeat;

	boost::unordered_map<unsigned int, VertexBuffer *> buffers;
	boost::unordered_map<unsigned int, VertexBuffer *>::iterator buffers_it;

	std::vector<Vertex3D*> vertices;
	std::vector<GLuint> indexes;
	boost::unordered_map<Vertex3D*, GLuint> vertices_indexes;


};

}

#endif /* MODEL_H_ */
