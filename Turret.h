#pragma once
#include "Model.h"
#include "gtx/matrix_decompose.hpp"
#include <math.h>
#include "Line.h"
#include "Math.h"
#include "Container.h"

class Turret :
	public Model
{
	float rotation = 0;
	Model arm;
	Model hook;
	float hookDistance=10;
	Line line;
	glm::vec3 operatorPosition;
	glm::mat4 hookM;

public:
	Turret();
	~Turret();
	float ropeLength;
	void loadModel();
	void rotate(float dAngle);
	void extendRope(float dist);
	void drawModel(glm::mat4 parentMatrix, Shader & shader,float objColor[3]);
	glm::vec3 getOperatorPosition();
	glm::mat4 getHookMatrix();
	glm::vec3 hookPos;
	bool adjusted = false;
	void cancelAdjust();
	Container* adjustedContainer;
};

