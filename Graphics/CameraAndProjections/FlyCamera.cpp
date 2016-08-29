#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraPos = glm::vec3(10.0f, 10.0f, 10.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	setLookAt(cameraPos,cameraFront, up);
	setPerspective(glm::pi<float>() * 0.35f, 16 / 9.f, 0.1f, 1000.f);
}

void FlyCamera::update(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();

	mat4 camTrans = mat4(1);
	mat4 camRot = mat4(1);
	mat4 camScale = mat4(1);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{
		viewTransform = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3(0, 0, (-0.10f * deltaTime) * 10));
	    //viewTransform *= glm::inverse(camTrans) ;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3((-0.10f * deltaTime) * 10, 0, 0));
		//viewTransform *= glm::inverse());
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3(0, 0, (0.10f * deltaTime) * 10));
		//viewTransform *= glm::inverse(glm::translate(vec3(0, 0, (0.10f * deltaTime) * 10)));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3((0.10f * deltaTime) * 10, 0, 0));
		//viewTransform *= glm::inverse(glm::translate(vec3((0.10f * deltaTime) * 10, 0, 0)));
	}
	
	viewTransform *= glm::inverse(camTrans* camRot* camScale);
	updateProjectionViewTransform();
}

void FlyCamera::setSpeed(float speed)
{
}

