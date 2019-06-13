#pragma once
#include "Model.h"
#include "Turret.h"
class Crane 	
{
	
	Model base;
	
public:
	float currentRotation;
	float angleX;
	Turret turret;
	Crane() {};
	void load();
	virtual ~Crane();
	void rotateZ(float angle);
	void rotateX(float angle);
	void drawModel(Shader& ourShader);
};

