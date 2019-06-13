#pragma once

#ifndef  MESH
#define MESH

#include <vector>
#include "structs.hpp"
#include <GL\glew.h>
#include "Shader.h"
using namespace std;
class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader shader);
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};
#endif // ! MESH

