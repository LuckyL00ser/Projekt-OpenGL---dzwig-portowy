#pragma once
#include "Model.h"
#include "Turret.h"
class Crane 	
{
	
	Model base;
	
public:
	float currentRotation;
	float rotationSpeedZ = 2.0;
	float rotationSpeedX = 5.0;
	float ropeSpeed = 0.5;
	float angleX;
	Turret turret;
	Crane() {};
	void load();
	virtual ~Crane();
	void rotateZ(float angle);
	void rotateX(float angle);
	void drawModel(Shader& ourShader);
	
};

