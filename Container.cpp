#include "Container.h"



bool Container::adjustable(glm::vec3 hookPosition)
{
	if (abs(glm::distance(position, hookPosition)) < 0.15) {
	
		if (alreadyAdjusted)
			return false;
		
		return true;
	
	}
	alreadyAdjusted = false;
	std::cout << abs(glm::distance(position, hookPosition)) << std::endl;
		return false;
}

Container::Container()
{
	containerModel.loadModel("./crane/container.obj");
	mat = glm::mat4(1.0f);
	mat = glm::scale(mat, glm::vec3(0.2f, 0.2f, 0.2f));
}


Container::~Container()
{
}

void Container::draw(Shader & shader)
{
	shader.setMat4("model", mat);
	containerModel.Draw(shader);

}

void Container::move(glm::vec3 vector)
{
	mat = glm::translate(mat, vector);


	glm::mat4 transformation; // your transformation matrix.
	glm::vec3 scale;
	glm::quat rot;

	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, rot, position, skew, perspective);


}

glm::vec3 Container::getPos()
{
	
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, rotation, translation, skew, perspective);
	return translation;
}
