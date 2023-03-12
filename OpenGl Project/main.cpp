//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>


int glWindowWidth = 1920;
int glWindowHeight = 1080;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 10096;
const unsigned int SHADOW_HEIGHT = 10096;

glm::mat4 model;
glm::mat4 model1;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

glm::vec3 lightDir2;
GLuint lightDirLoc2;
glm::vec3 lightColor2;
GLuint lightColorLoc2;
glm::vec3 lightColor3;
glm::vec3 lightPos[3];
GLuint lightDirLoc3;
GLuint lightColorLoc3;
GLuint lightDirLoc4;
GLuint lightDirLoc5;

int lighttype1=0;
int lighttype2=0;
float snowx[6001];
float snowy[6001];
float snowz[6001];
float mouseSensitivity=0.1f;
float lastX = 960, lastY = 540;
float yaw = 180.0f;
bool firstMouse = true;
float pitch = 0.0f;

gps::Camera myCamera(
	glm::vec3(100.0f, 2.2f, -40.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.50f;

bool pressedKeys[1024];
float angleY = 0.0f;
float ang = 0.0f;
float ang2 = 0.0f;
GLfloat lightAngle;

gps::Model3D nanosuit;
gps::Model3D delfin;
gps::Model3D ground;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D leaf;
gps::Model3D window;
gps::Model3D trooper;
gps::Model3D windmill;
gps::Model3D snow;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;
gps::SkyBox mySkyBox;
gps::Shader skyboxShader;



GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		myCamera.rotate(pitch, yaw);
	
}
int tourlevel = 0,inceput=0;
float unghi = 180.0,unghi2=90;

int tour()
{
	float viteza = 0.5;
	if (inceput == 0)
	{
		myCamera = gps::Camera(glm::vec3(132.0f, 2.5f, -40.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		inceput = 1;
	}
	if (tourlevel == 0)
	{
		if (myCamera.mycamera().x > 70.98)
			myCamera.move(gps::MOVE_FORWARD, viteza);
		else
			tourlevel++;
	}
	if (tourlevel == 1)
	{
		if (myCamera.mycamera().z < -26.0f)
			myCamera.move(gps::MOVE_LEFT, viteza);
		else
			tourlevel++;
	}
	if (tourlevel == 2)
	{
		if (myCamera.mycamera().z > -42.3f)
			myCamera.move(gps::MOVE_RIGHT, viteza);
		else
			tourlevel++;
	}
	if (tourlevel == 3)
	{
		if (myCamera.mycamera().x > 42.35)
			myCamera.move(gps::MOVE_FORWARD, viteza);
		else
			tourlevel++;
	}
	if (tourlevel == 4)
	{
		if (unghi < 450)
		{
			unghi = unghi + 2.0;
			myCamera.rotate(0.0, unghi);
		}
		else
			tourlevel++;
	}
	if (tourlevel == 5)
	{
		if ((myCamera.mycamera().z < -33.71))
			myCamera.move(gps::MOVE_FORWARD, viteza);
		else
			tourlevel++;
	}
	if (tourlevel == 6)
	{
		if (unghi2 > 0)
		{
			unghi2 = unghi2 - 2.0;
			myCamera.rotate(0.0, unghi2);
		}
		else
			tourlevel++;
	}
	if (tourlevel == 7)
	{
		if (myCamera.mycamera().x < 92.0)
			myCamera.move(gps::MOVE_FORWARD, viteza);
		else
			tourlevel++;
	}
	if (tourlevel == 8)
	{
		myCamera.move(gps::MOVE_LEFT, 4.0);
		myCamera.move(gps::MOVE_DOWN, 1.0);
		tourlevel++;
	}
	
	return 1;
}
bool tur = false;
void processMovement()
{
	if (pressedKeys[GLFW_KEY_Q]) {
		yaw -= 2.0f;
		myCamera.rotate(0.0f,yaw);
		printf("%f", yaw);
	}

	if (pressedKeys[GLFW_KEY_E]) {
		yaw += 2.0f;
		myCamera.rotate( 0.0f,yaw);
		printf("%f", yaw);
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_Z]) {
		myCamera.move(gps::MOVE_UP, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_X]) {
		myCamera.move(gps::MOVE_DOWN, cameraSpeed);
	}
	
	if (pressedKeys[GLFW_KEY_B]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		//Punctiform
	}

	if (pressedKeys[GLFW_KEY_V]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//Wireframe
	}
	

	if (pressedKeys[GLFW_KEY_C]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//normal
	}

	if (pressedKeys[GLFW_KEY_3]) {

		lighttype2 = 1;
		myCustomShader.useShaderProgram();
		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lighttype2");
		glUniform1i(lightColorLoc, lighttype2);
	}

	if (pressedKeys[GLFW_KEY_4]) {

		lighttype2 = 0;
		myCustomShader.useShaderProgram();
		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lighttype2");
		glUniform1i(lightColorLoc, lighttype2);
	}

	if (pressedKeys[GLFW_KEY_1]) {
		
		lighttype1 = 1;

		myCustomShader.useShaderProgram();
		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lighttype1");
		glUniform1i(lightColorLoc, lighttype1);
	}

	if (pressedKeys[GLFW_KEY_2]) {
		lighttype1 = 0;
		myCustomShader.useShaderProgram();
		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lighttype1");
		glUniform1i(lightColorLoc, lighttype1);
	}

	if (pressedKeys[GLFW_KEY_T])
	{
		tur = true;
		//tour();
	}

}



bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	
	screenQuad.LoadModel("objects/quad/quad.obj");
	trooper.LoadModel("objects/tema/tema.obj");
	windmill.LoadModel("objects/wings/wings.obj");
	delfin.LoadModel("objects/delfin/delfin.obj");
	snow.LoadModel("objects/zapada/snow.obj");
}


void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	depthMapShader.loadShader("shaders/shadow.vert", "shaders/shadow.frag");
	depthMapShader.useShaderProgram();
}

void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(30.0f, 20.0f, 40.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color1
	lightColor = glm::vec3(0.29f, 0.25f, 0.55f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightDir2 = glm::vec3(30.0f, 20.0f, 40.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir2");
	glUniform3fv(lightDirLoc2, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir2));

	//set light color2
	lightColor2 = glm::vec3(1.0f, 1.0f, 1.0f);
	lightColorLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor2");
	glUniform3fv(lightColorLoc2, 1, glm::value_ptr(lightColor2));


	lightColor3 = glm::vec3(0.88f, 0.34f, 0.13f);
	lightColorLoc3 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor3");
	glUniform3fv(lightColorLoc3, 1, glm::value_ptr(lightColor3));

	
	lightPos[0] = glm::vec3(38.84f, 2.24f, -48.66f);
	lightPos[1] = glm::vec3(82.79f, 0.39f,-37.28f);
	lightPos[2] = glm::vec3(45.43f,2.60f, -28.64f);
	
	lightDirLoc3 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos1");
	glUniform3fv(lightDirLoc3, 1,glm::value_ptr(lightPos[0]));

	lightDirLoc4 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos2");
	glUniform3fv(lightDirLoc4, 1, glm::value_ptr(lightPos[1]));

	lightDirLoc5 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos3");
	glUniform3fv(lightDirLoc5, 1, glm::value_ptr(lightPos[2]));


	lightShader.useShaderProgram();
	
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	//glm::mat4 lightView = glm::lookAt(glm::inverseTranspose(glm::mat3(view)) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightView = glm::lookAt( lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 150.0f;
	glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}

void RendertheScene()
{

	glViewport(0, 0, retina_width, retina_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenQuadShader.useShaderProgram();

	//bind the depth map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

	glDisable(GL_DEPTH_TEST);
	screenQuad.Draw(screenQuadShader);
	glEnable(GL_DEPTH_TEST);
}

float x = 1.0f;

void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();
	
	for(int i=0;i<6001;i++)
	{
	
		model = glm::translate(glm::mat4(1.0f), glm::vec3(snowx[i], snowy[i]-x, snowz[i]));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// do not send the normal matrix if we are rendering in the depth map
		if (!depthPass) {
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
			glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		}
		snow.Draw(shader);
	}

	x = x + 0.1f;
	if (x > 25.0f)
		x = 1.0f;
	

	

	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	trooper.Draw(shader);

	model1 = glm::mat4(1.0f);

	model1 = glm::translate(model1, glm::vec3(32.81f, 6.76f, -34.657f));
	model1 = glm::rotate(model1, glm::radians(ang), glm::vec3(1.0f, 0.0f, 0.0f));
	ang = ang + 1.5f;
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model1));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	windmill.Draw(shader);

	if (myCamera.mycamera().x > 90 && myCamera.mycamera().x < 130 && myCamera.mycamera().z<-23.0 && myCamera.mycamera().x >-43)
	{
		model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(105.57f, -6.11f, -33.647f));
		model1 = glm::rotate(model1, glm::radians(ang), glm::vec3(-1.0f, 0.0f, 0.0f));
		ang2 = ang2 + 1.0f;
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model1));

		// do not send the normal matrix if we are rendering in the depth map
		if (!depthPass) {
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model1));
			glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		}

		delfin.Draw(shader);
	}
	else
		ang2 =0.0f ;
}

void renderScene() {

	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map



	// render depth map on screen - toggled with the M key
	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects(depthMapShader, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);
		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);


	}
	else {

		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		drawObjects(myCustomShader, false);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//draw a white cube around the light

		lightShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = lightRotation;
		model = glm::translate(model, 1.5f * lightDir);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		lightCube.Draw(lightShader);
			
		skyboxShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
			glm::value_ptr(view));

		projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
			glm::value_ptr(projection));

		mySkyBox.Draw(skyboxShader, view, projection);

	}
	
	if (tur == true)
	{
		if(tour()==-1);
		//tourlevel++;
	}
}


void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO

	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}
void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}


int main(int argc, const char* argv[]) {

	if(!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	for (int i = 0; i <= 6000; i++)
	{
		snowx[i] = -34.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (136.0f - (-52.0f))));
		snowy[i] = 2.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40.0f - 2.5f)));
		snowz[i] = -113.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (68.0f - (-113.0f))));
	}

	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initFBO();

	std::vector<const GLchar*> faces;
	faces.push_back("textures/skybox/right.tga");
	faces.push_back("textures/skybox/left.tga");
	faces.push_back("textures/skybox/top.tga");
	faces.push_back("textures/skybox/bottom.tga");
	faces.push_back("textures/skybox/back.tga");
	faces.push_back("textures/skybox/front.tga");
	mySkyBox.Load(faces);
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");


	glClearColor(0.5, 0.5, 0.5, 1.0);
	glCheckError();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
