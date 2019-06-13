#pragma once
#include "Model.h"
#include "gtx/matrix_decompose.hpp"
#include <math.h>
class Turret :
	public Model
{
	float rotation = 0;
	Model arm;
	Model hook;
	float hookDistance=10;
	
public:
	Turret();
	~Turret();
	float ropeLength;
	void loadModel();
	void rotate(float dAngle);
	void extendRope(float dist);
	void drawModel(glm::mat4 parentMatrix, Shader & shader);
};

