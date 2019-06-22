#pragma once
#ifndef CONTAINER
#define CONTAINER
#include "glm.hpp"
#include "Model.h"
#include "Shader.h"
#include "gtx/matrix_decompose.hpp"

class Container
{
	Model containerModel;
public:
	glm::vec3 position;
	glm::mat4 mat;
	bool adjustable(glm::vec3 hookPosition);
	Container();
	~Container();
	void draw(Shader& shader);
	void move(glm::vec3);
	glm::vec3 getPos();
	bool alreadyAdjusted = false;
};

#endif

