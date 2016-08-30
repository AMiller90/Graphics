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


bool Create()
{
	if (glfwInit() == false)
		return false;

	GLFWwindow* window = glfwCreateWindow(1080, 720, "Window", nullptr, nullptr);

	Camera* myCamera = new FlyCamera;

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

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);

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

		myCamera->update(deltaTime);


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