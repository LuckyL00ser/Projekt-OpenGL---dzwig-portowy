#pragma once

#ifndef MODEL
#define MODEL

#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#include <iostream>


#include "Shader.h"

using namespace std;
class Model
{
public:
	/*  Functions   */
	Model() {};
	Model(string const &path, bool gamma = false);
	void Draw(Shader shader);
	void loadModel(string path);
protected:
	bool gammaCorrection;
	/*  Model Data  */
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	string directory;
	/*  Functions   */
	
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);
};
#endif