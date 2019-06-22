#pragma once

#ifndef LINE
#define LINE

#include "glm.hpp"
#include <GL\glew.h>
class Line
{
	GLuint vertexbuffer;
	unsigned int vao;
	GLfloat g_vertex_buffer_data[6];
public:
	unsigned int width = 3;
	void setLineData(GLfloat newCoords[6]);
	void setStart(glm::vec3 start);
	void setEnd(glm::vec3 end);
	void setupLine(glm::vec3 start, glm::vec3 end);
	Line();
	~Line();
	void draw();
};
#endif

