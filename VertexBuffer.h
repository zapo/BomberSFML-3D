/*
 * VertexBuffer.h
 *
 *  Created on: 2011-11-07
 *      Author: zapo
 */

#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include <GL/glew.h>
#include <vector>
#include "Vertex3D.h"
#include "iostream"
#include <boost/unordered_map.hpp>

namespace Bomber {


class VertexBuffer {
public:
	VertexBuffer(GLenum usage, std::vector<Vertex3D*> & vertices, std::vector<GLuint> & indexes) {
		this->usage = usage;


		SetVertices(vertices);
		SetIndexes(indexes);

		glGenBuffers(2, buffers);
		Setup(vertices, indexes);

		for(unsigned int i = 0; i < vertices.size(); i++) {

			Vertex3D * vertex = vertices[i];
			vertices_indexes[vertex] = i;
		}


	}

	virtual ~VertexBuffer() {
	}


	void SetVertices(std::vector<Vertex3D*> & vertices) {
		this->vertices = &vertices;
	}
	std::vector<Vertex3D*> & GetVertices() {
		return *vertices;
	}

	void SetIndexes(std::vector<GLuint> & indexes) {
		this->indexes = &indexes;
	}
	std::vector<GLuint> & GetIndexes() {
		return *indexes;
	}

	void UploadIndexes() {

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEXES]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->size() * sizeof(GLuint), NULL, usage);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexes->size() * sizeof(GLuint), &(indexes->front()));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	void UploadVertices() {


		std::vector<Vertex3D> _vertices;

		for(vertices_it = vertices->begin(); vertices_it != vertices->end(); vertices_it++) {

			Vertex3D * vertex = *vertices_it;
			_vertices.push_back(*vertex);
		}

		std::cout << "Vertices Upload" << std::endl;


		glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex3D), NULL, usage);
		glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(Vertex3D), &(_vertices.front()));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void Setup(std::vector<Vertex3D*> & vertices, std::vector<GLuint> & indexes) {

		SetVertices(vertices);
		SetIndexes(indexes);

		UploadIndexes();
		UploadVertices();
	}

	void Render() {

		glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEXES]);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState( GL_COLOR_ARRAY );
		glEnableClientState(GL_NORMAL_ARRAY);

		glNormalPointer(GL_FLOAT, sizeof(Vertex3D), BUFFER_OFFSET(12));
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), BUFFER_OFFSET(24));
		glColorPointer(4, GL_FLOAT, sizeof(Vertex3D), BUFFER_OFFSET(32));
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), BUFFER_OFFSET(0));

		glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}


	inline GLuint GetIndexAt(Vertex3D & vertex) {
		return vertices_indexes[&vertex];
	}

private:

	GLenum usage;
	GLuint buffers[2];

	enum {
		VERTICES = 0,
		INDEXES = 1
	};

	std::vector<Vertex3D*> * vertices;
	std::vector<Vertex3D*>::iterator vertices_it;

	std::vector<GLuint> * indexes;
	std::vector<GLuint>::iterator indexes_it;

	boost::unordered_map<Vertex3D*, GLuint> vertices_indexes;

};
}

#endif /* VERTEXBUFFER_H_ */
