/*
 * Shader.h
 *
 *  Created on: 2011-11-15
 *      Author: zapo
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace Bomber {

class Shader {
public:
	Shader(std::string vertex_filename, std::string fragment_filename) {

		return;

		vertex   = LoadShader(GL_VERTEX_SHADER,   vertex_filename);
		fragment = LoadShader(GL_FRAGMENT_SHADER, fragment_filename);

		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);

		glLinkProgram(program);

		enabled = false;

	}

	std::string LoadSource(std::string filename) {

		std::string line;

		std::stringstream ss;

		std::fstream file(filename.data());
		if(file.is_open()){
			while (file.good()) {
			  getline(file, line);
			  ss << line << std::endl;
			}
			file.close();
		} else {
			std::cerr << "Unable to open file : " << filename << std::endl;
		}

		return ss.str();
	}


	GLuint LoadShader(GLenum type, std::string filename)
	{
	    GLuint shader = 0;
	    GLsizei logsize = 0;
	    GLint compile_status = GL_TRUE;
	    std::string src = "";

	    shader = glCreateShader(type);
	    if(shader == 0) {
	        std::cerr << "Can't create the shader" << std::endl;
	        return 0;
	    }

	    src = LoadSource(filename);
	    if(src.empty()) {
	        glDeleteShader(shader);
	        return 0;
	    }

	    const char * sources = src.data();
	    glShaderSource(shader, 1, &sources, NULL);

	    glCompileShader(shader);

	    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

	    if(compile_status != GL_TRUE) {

	        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);

	        char * p_log = new char[logsize + 1];

	        glGetShaderInfoLog(shader, logsize, &logsize, p_log);
	        std::cerr << "Can't compile the shader '" << filename << "':" << std::endl << p_log;

	        delete p_log;
	        glDeleteShader(shader);

	        return 0;
	    }

	    return shader;
	}

	virtual ~Shader() {
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Enable() {
		enabled = true;
	}

	void Disable() {
		enabled = false;
	}

	void Use() {
		glUseProgram(program);
	}

	void SendUniformFloat(std::string var, GLfloat i) {

			 GLint id = glGetUniformLocation(program, var.data());
			 glUseProgram(program);
			 glUniform1f(id, i);

		}

	void SendUniformInt(std::string var, GLint i) {

		 GLint id = glGetUniformLocation(program, var.data());
		 glUseProgram(program);
		 glUniform1i(id, i);

	}

	void SendUniformVec(std::string var, const sf::Vector3f vec) {

		 int id = glGetUniformLocation(program, var.data());
		 glUseProgram(program);
		 glUniform3f(id, vec.x, vec.y, vec.z);

	}


	bool enabled;

private:
	GLuint program, vertex, fragment;


};

}

#endif /* SHADER_H_ */
