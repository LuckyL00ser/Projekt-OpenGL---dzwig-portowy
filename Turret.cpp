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
	line.setupLine(glm::vec3(0.f,10.f,0.f),glm::vec3(0.0f,0.0f,0.0f));
}

void Turret::rotate(float dAngle)
{

	if(rotation+dAngle>0 && rotation+dAngle<65)
	rotation += dAngle;
	
}

void Turret::extendRope(float dist)
{

	if (ropeLength+dist > 0 && ropeLength < 20 + dist)
		ropeLength += dist;
}



void Turret::drawModel(glm::mat4 parentMatrix,Shader & shader)
{	
	
	
	shader.setMat4("model", parentMatrix);
	Draw(shader);
	
	
	parentMatrix = glm::translate(parentMatrix, glm::vec3(0.3,3.5,0));
	
	
	//glDisableClientState(GL_VERTEX_ARRAY);
	float x, y;
	x = hookDistance * cos(rotation*3.1415/180.f);
	y = hookDistance * sin(rotation*3.1415 / 180.f);
	shader.setMat4("model",glm::translate(parentMatrix,glm::vec3(x,y-ropeLength-1,0)));
	hook.Draw(shader);
	shader.setMat4("model", glm::translate(parentMatrix, glm::vec3(x, y, 0)));
	line.setStart(glm::vec3(0,-ropeLength-1,-0.1));
	line.draw();
	line.setStart(glm::vec3(0, -ropeLength-1, 0.1));
	line.draw();

	glm::mat4 old = glm::translate(parentMatrix, glm::vec3(-0.4, 3.28, 0.052));
	parentMatrix = glm::rotate(parentMatrix, rotation*3.1415f / 180.f, glm::vec3(0, 0, 1));
	shader.setMat4("model", parentMatrix);
	arm.Draw(shader);




	//firstLine.setStart(translation);
	shader.setMat4("model", old);
	line.setStart(glm::vec3(0.4+10.*cos(rotation*3.1415f / 180.f),10.*sin(rotation*3.1415f / 180.f)-3.15,0.04f));
	line.draw();

	 old = glm::translate(old, glm::vec3(0,0, -0.124));
	shader.setMat4("model", old);
	line.setStart(glm::vec3(0.4 + 10.*cos(rotation*3.1415f / 180.f), 10.*sin(rotation*3.1415f / 180.f) - 3.15, 0));
	line.draw();

	old = glm::translate(old, glm::vec3(0.9, -0.5,-0.04));
	shader.setMat4("model", old);
	line.setStart(glm::vec3( 9.1*cos(rotation*3.1415f / 180.f),10*sin(rotation*3.1415f / 180.f) - 2.6, 0));
	line.draw();

	old = glm::translate(old, glm::vec3(0, 0,0.18));
	shader.setMat4("model", old);
	line.setStart(glm::vec3( 9.1*cos(rotation*3.1415f / 180.f), 10*sin(rotation*3.1415f / 180.f) - 2.6, 0));
	line.draw();

}
