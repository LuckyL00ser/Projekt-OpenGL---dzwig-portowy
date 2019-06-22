
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Crane.h"
#include "WorldBar.h"
#include "AntTweakBar\include\AntTweakBar.h"
#include "Skybox.h"

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

Crane crane;

float yWater;

WorldBar otoczenie;
TwBar* dzwigBar;

float objectColor[] = { 1.0,1.0,1.0 };
float lightColor[] = {1.0, 1.0, 1.0};
float lightPos[] = { 1.0, 10.0, 1.0 };

/*int pos[2], size[2];
TwGetParam(bar, NULL, "size", TW_PARAM_INT32, 2, size);
pos[0] = width - size[0] - 16;
pos[1] = 16;
TwSetParam(bar, NULL, "position", TW_PARAM_INT32, 2, pos);
cout << "\nSzer: " << width << "\nWys: " << height << endl;*/

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }

inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos) { TwMouseMotion(int(xpos), int(ypos)); }

inline void TwEventMouseWheelGLFW3(GLFWwindow* window, double xoffset, double yoffset) { TwEventMouseWheelGLFW(yoffset); }

inline void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods) { TwEventKeyGLFW(key, action); }

inline void TwEventCharGLFW3(GLFWwindow* window, int codepoint) { TwEventCharGLFW(codepoint, GLFW_PRESS); }

void TW_CALL cameraRun(void *clientData)
{
	(*(Camera*)clientData).LookingAround = !(*(Camera*)clientData).LookingAround;
}

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	crane.load();
	Model containers("./crane/containers.obj");
	Model ground("./crane/ground.obj");
	Model water("./crane/water.obj");
	Skybox skybox;
	

	
	
	// render loop
	// -----------
	//TwBar* dzwigBar;
	

	TwInit(TW_OPENGL_CORE, NULL);
	otoczenie.init(cameraRun, &camera);
	dzwigBar = TwNewBar("dzwig");
	TwDefine(" dzwig label='Parametry dzwigu' position='16 16' ");

	
	float rotationSpeedZ = 2.0;
	float rotationSpeedX = 5.0;
	float ropeSpeed = 0.5;
	// Add 'time' to 'bar': it is a read-only (RO) variable of type TW_TYPE_DOUBLE, with 1 precision digit

	TwAddVarRW(dzwigBar, "ropeLength", TW_TYPE_FLOAT, &crane.turret.ropeLength, " label='Dlugosc liny' min=0 max=10 precision=2 group=Lina ");
	TwAddVarRW(dzwigBar, "ropeSpeed", TW_TYPE_FLOAT, &crane.ropeSpeed, " label='Prêdkoœæ wysuwania liny' min=0.25 max=4.5 precision=2 step=0.1 keyincr=K keydecr=L group=Lina ");
	TwAddVarRO(dzwigBar, "rotZ", TW_TYPE_FLOAT, &crane.currentRotation, " label='Obrot dzwigu' precision=2 group=Ramie ");
	TwAddVarRW(dzwigBar, "speedRotZ", TW_TYPE_FLOAT, &crane.rotationSpeedZ, " label='Szybkosc obrotu ramienia' min=0.5 max=5 step=0.1 precision=2 keyincr=Z keydecr=X group=Ramie ");
	TwAddVarRO(dzwigBar, "rotX", TW_TYPE_FLOAT, &crane.angleX, " label='Pochylenie ramienia dzwigu' precision=2 group=Ramie ");
	TwAddVarRW(dzwigBar, "speedRotX", TW_TYPE_FLOAT, &crane.rotationSpeedX, " label='Szybkosc opuszczania ramienia' min=0.5 max=10 step=0.1 precision=2 keyincr=N keydecr=M group=Ramie ");
	/*TwAddVarRW(swiatBar, "lightPos", TW_TYPE_DIR3F, &lightPos, " label='Kierunek oswietlenia' ");
	TwAddVarRW(swiatBar, "lightColor", TW_TYPE_COLOR3F, &lightColor, " label='Kolor swiatla' ");
	TwAddButton(swiatBar, "Kamera", cameraRun, &camera, " label='Wlaczenie kamery' key=SPACE ");*/

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
		glClearColor(0.2f, 0.2f, 0.20f, 1.0f);
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
		crane.drawModel(ourShader);

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
		
		skybox.drawSkybox(camera, projection);
		
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------

		TwRefreshBar(dzwigBar);
		otoczenie.refresh();

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

	if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)&&(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)) {
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			crane.rotateZ(crane.rotationSpeedZ*deltaTime);
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			crane.rotateZ(-crane.rotationSpeedZ*deltaTime);
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
			crane.rotateX(crane.rotationSpeedX* deltaTime);
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
			crane.rotateX(-crane.rotationSpeedX*deltaTime);
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			crane.turret.extendRope(crane.ropeSpeed*deltaTime);
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			crane.turret.extendRope(-crane.ropeSpeed*deltaTime);
	}

if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
	camera.LookingAround = !camera.LookingAround;
	glfwSetInputMode(window, GLFW_CURSOR, camera.LookingAround ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	
	glViewport(0, 0, width, height);
	TwWindowSize(width, height);
	otoczenie.windowResize(width, height);
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

	TwEventMousePosGLFW3(window,xpos, ypos);
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
