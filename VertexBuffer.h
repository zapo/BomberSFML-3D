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

namespace Bomber {


class VertexBuffer {
public:
	VertexBuffer(GLenum usage, std::vector<Vertex3D> & vertices, std::vector<GLuint> & indexes) {
		this->usage = usage;

		SetVertices(vertices);
		SetIndexes(indexes);

		glGenBuffers(2, buffers);
		Setup(vertices, indexes);

	}

	virtual ~VertexBuffer() {
	}


	void SetVertices(std::vector<Vertex3D> & vertices) {
		this->vertices = &vertices;
	}
	std::vector<Vertex3D> & GetVertices() {
		return *vertices;
	}

	void SetIndexes(std::vector<GLuint> & indexes) {
		this->indexes = &indexes;
	}
	std::vector<GLuint> & GetIndexes() {
		return *indexes;
	}


	void Setup(std::vector<Vertex3D> & vertices, std::vector<GLuint> & indexes) {

		SetVertices(vertices);
		SetIndexes(indexes);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEXES]);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), NULL, usage);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex3D), &(vertices[0]));

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(GLuint), NULL, usage);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexes.size() * sizeof(GLuint), &(indexes[0]));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Render() {

		glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEXES]);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState( GL_COLOR_ARRAY );

		glColorPointer(3, GL_FLOAT, sizeof(Vertex3D), BUFFER_OFFSET(32));
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), BUFFER_OFFSET(0));

		glDrawElements(GL_TRIANGLES, indexes->size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	}

private:

	GLenum usage;
	GLuint buffers[2];

	enum {
		VERTICES = 0,
		INDEXES = 1
	};

	std::vector<Vertex3D> * vertices;
	std::vector<GLuint> * indexes;
};
}

#endif /* VERTEXBUFFER_H_ */
