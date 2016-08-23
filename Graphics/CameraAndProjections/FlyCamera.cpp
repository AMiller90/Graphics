#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
	up = vec3(0, 1, 0);
	setLookAt(vec3(10, 10, 10), vec3(0), up);
	setPerspective(glm::pi<float>() * 0.35f, 16 / 9.f, 0.1f, 1000.f);


}

void FlyCamera::update(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
	    viewTransform *= glm::inverse(glm::translate(vec3(0, 0, (-0.10f * deltaTime) * 10)));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		viewTransform *= glm::inverse(glm::translate(vec3((-0.10f * deltaTime) * 10, 0, 0)));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		viewTransform *= glm::inverse(glm::translate(vec3(0, 0, (0.10f * deltaTime) * 10)));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		viewTransform *= glm::inverse(glm::translate(vec3((0.10f * deltaTime) * 10, 0, 0)));
	}

	updateProjectionViewTransform();
}

void FlyCamera::setSpeed(float speed)
{
}