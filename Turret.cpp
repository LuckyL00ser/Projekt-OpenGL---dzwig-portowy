#include "Turret.h"



Turret::Turret()
{
	
}


Turret::~Turret()
{
}

void Turret::loadModel()
{
	Model::loadModel("./crane/turret.obj");
	arm.loadModel("./crane/arm.obj");
	hook.loadModel("./crane/hook.obj");
}

void Turret::rotate(float dAngle)
{

	if(rotation+dAngle>0 && rotation+dAngle<65)
	rotation += dAngle;
	
}

void Turret::extendRope(float dist)
{
	std::cout << dist;
	if (ropeLength+dist > 0 && ropeLength < 20 + dist)
		ropeLength += dist;
}




void Turret::drawModel(glm::mat4 parentMatrix,Shader & shader)
{	
	
	
	shader.setMat4("model", parentMatrix);
	Draw(shader);
	
	
	parentMatrix = glm::translate(parentMatrix, glm::vec3(0.3,3.5,0));
	
	

	float x, y;
	x = hookDistance * cos(rotation*3.1415/180.f);
	y = hookDistance * sin(rotation*3.1415 / 180.f);
	shader.setMat4("model",glm::translate(parentMatrix,glm::vec3(x,y-ropeLength-1,0)));
	hook.Draw(shader);

	parentMatrix = glm::rotate(parentMatrix, rotation*3.1415f / 180.f, glm::vec3(0, 0, 1));
	shader.setMat4("model", parentMatrix);
	arm.Draw(shader);
	
}
