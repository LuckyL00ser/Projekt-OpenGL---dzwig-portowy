
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Crane.h"
#include "AntTweakBar\include\AntTweakBar.h"

#include <math.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Crane ourModel;
float yWater;




float objectColor[] = { 1.0,1.0,1.0 };
float lightColor[] = {1.0, 1.0, 1.0};
float lightPos[] = { 1.0, 10.0, 1.0 };



inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }

inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos) { TwMouseMotion(int(xpos), int(ypos)); }

inline void TwEventMouseWheelGLFW3(GLFWwindow* window, double xoffset, double yoffset) { TwEventMouseWheelGLFW(yoffset); }

inline void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods) { TwEventKeyGLFW(key, action); }

inline void TwEventCharGLFW3(GLFWwindow* window, int codepoint) { TwEventCharGLFW(codepoint, GLFW_PRESS); }
int main()
{
	
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//{
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("vertexShader.vs", "fragmentShader.fs");
	
	// load models
	// -----------
	ourModel.load();
	Model containers("./crane/containers.obj");
	Model ground("./crane/ground.obj");
	Model water("./crane/water.obj");
	
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// render loop
	// -----------
	TwBar* bar;
	TwInit(TW_OPENGL_CORE,NULL);
	bar = TwNewBar("Dzwig portowy");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.
	


	
	// Add 'time' to 'bar': it is a read-only (RO) variable of type TW_TYPE_DOUBLE, with 1 precision digit
	TwAddVarRW(bar, "ropeLength", TW_TYPE_FLOAT, &ourModel.turret.ropeLength, " label='Dlugosc liny' min=0 max=10 precision=2  ");
	TwAddVarRO(bar, "rotY", TW_TYPE_FLOAT, &ourModel.currentRotation, " label='Obrot dzwigu' precision=2  ");
	TwAddVarRO(bar, "rotZ", TW_TYPE_FLOAT, &ourModel.angleX, " label='Pochylenie ramienia dzwigu' precision=2  ");
	TwAddVarRW(bar, "lightPos", TW_TYPE_DIR3F, &lightPos, " label='Kierunek oswietlenia' ");
	TwAddVarRW(bar, "lightColor", TW_TYPE_COLOR3F, &lightColor, " label='Kolor swiatla' ");
	
	TwWindowSize(1600, 900);
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);	
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW3);
	glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW3);
	

	while (!glfwWindowShouldClose(window))
	{ 
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.20f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();
		ourShader.setVec3("objectColor", objectColor[0],objectColor[1],objectColor[2]);
		ourShader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
		ourShader.setVec3("lightPos", lightPos[0], lightPos[1], lightPos[2]);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		ourModel.drawModel(ourShader);

		glm::mat4 containersMatrix = glm::mat4(1.0f);
		containersMatrix = glm::translate(containersMatrix, glm::vec3(0.0f, -1.6f, 1.5f));
		containersMatrix = glm::scale(containersMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", containersMatrix);
		containers.Draw(ourShader);

		glm::mat4 groundMatrix = glm::mat4(1.0f);
		groundMatrix = glm::translate(groundMatrix, glm::vec3(-1.5f, -1.75f, 0));
		groundMatrix = glm::scale(groundMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", groundMatrix);
		ground.Draw(ourShader);


		yWater += 35*deltaTime;
		glm::mat4 waterMatrix = glm::mat4(1.0f);
		waterMatrix = glm::translate(waterMatrix, glm::vec3(4, -2.3f+0.15*sin(yWater*3.1415/180.f), 0));
		waterMatrix = glm::scale(waterMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", waterMatrix);
		water.Draw(ourShader);

		glm::mat4 hookMatrix = glm::mat4(1.0f);
		hookMatrix = glm::translate(hookMatrix, glm::vec3(1, 0.5f, 0));
		hookMatrix = glm::scale(hookMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", hookMatrix);
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		TwRefreshBar(bar);
		TwDraw();
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	TwTerminate();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		ourModel.rotateZ(2*deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		ourModel.rotateZ(-2*deltaTime);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		ourModel.rotateX(5* deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		ourModel.rotateX(-5*deltaTime);

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		ourModel.turret.extendRope(0.5*deltaTime);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		ourModel.turret.extendRope(-0.5*deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.LookingAround = !camera.LookingAround;


}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	TwWindowSize(width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	;
	TwEventMousePosGLFW3(window,xpos, ypos);
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}