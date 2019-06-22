#include "Crane.h"






void Crane::load()
{
	base.loadModel("./crane/crane.obj");
	turret.loadModel();

}

Crane::~Crane()
{
}

void Crane::rotateZ(float angle)
{
	currentRotation += angle;
}

void Crane::rotateX(float angle)
{
	if (angleX + angle > 0 && angleX + angle < 65) {
		angleX += angle;
		turret.rotate(angle);
	}
	
}

void Crane::drawModel(Shader& ourShader,float objColor[3])
{
	glm::mat4 crane = glm::mat4(1.0f);
	crane = glm::translate(crane, glm::vec3(0.0f, -1.75f, 0.0f));
	crane = glm::scale(crane, glm::vec3(0.2f, 0.2f, 0.2f));
	
	ourShader.setMat4("model", crane);
	base.Draw(ourShader);

	crane = glm::rotate(crane, currentRotation*3.1415f/180.0f, glm::vec3(0, 1, 0));
	turret.drawModel(crane, ourShader, objColor);



}
