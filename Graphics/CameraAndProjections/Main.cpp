#include <iostream>

#include <gizmos\Gizmos.h>
#include "src\gl_core_4_4.h"
#include <glfw\include\GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>
#include "FlyCamera.h"

using glm::mat4;
using glm::vec3;
using glm::vec4;

bool firstMouse = true;
float pitch = 0.0f;
float yaw = -90.0f;
glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float lastX = 1080 / 2;
	float lastY = 720 / 2;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.005;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

bool Create()
{
	if (glfwInit() == false)
		return false;

	GLFWwindow* window = glfwCreateWindow(1080, 720, "Window", nullptr, nullptr);

	FlyCamera* myCamera = new FlyCamera;

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	Gizmos::create();

	float deltaTime = (float)glfwGetTime();

	glfwSetCursorPosCallback(window, mouse_callback);


	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
		GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
		OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		Gizmos::clear();
		Gizmos::addTransform(glm::mat4(1));

		vec4 white(1);
		vec4 black(0, 0, 0, 1);

		for (int i = 0; i < 21; ++i) {
			Gizmos::addLine(vec3(-10 + i, 0, 10),
				vec3(-10 + i, 0, -10),
				i == 10 ? white : black);
			Gizmos::addLine(vec3(10, 0, -10 + i),
				vec3(-10, 0, -10 + i),
				i == 10 ? white : black);
		}


		myCamera->cameraFront = cameraFront;

		/* Camera/View transformation
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		 Projection 
		glm::mat4 projection;
		projection = glm::perspective(45.0f, (float)1080 / 720, 0.1f, 100.0f);*/

		myCamera->update(deltaTime);

		//Gizmos::draw(projection * view);

		Gizmos::draw(myCamera->getProjectionView());

		//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
		//anything rendered by us with OpenGL.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return true;
}

int main()
{

	
	Create();


	int tmp;
	std::cin >> tmp;
	return 0;

}