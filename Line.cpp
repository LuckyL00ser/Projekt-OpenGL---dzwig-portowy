#include "Line.h"

void Line::setLineData(GLfloat  newCoords[6])
{
	for (int i = 0; i < 6; i++) {
		g_vertex_buffer_data[i] = newCoords[i];
	}
}

void Line::setStart(glm::vec3 start)
{
	g_vertex_buffer_data[0] = start[0];
	g_vertex_buffer_data[1] = start[1];
	g_vertex_buffer_data[2] = start[2];
}

void Line::setEnd(glm::vec3 end)
{
	g_vertex_buffer_data[3] =  end[0];
	g_vertex_buffer_data[4] = end[1];
	g_vertex_buffer_data[5] =end[2];
}

void Line::setupLine(glm::vec3 start, glm::vec3 end)
{
	setStart(start);
	setEnd(end);
	
}

Line::Line()
{
	
}


Line::~Line()
{
}

void Line::draw()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glLineWidth(width);
	
	glDrawArrays(GL_LINES, 0, 2); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	
}
