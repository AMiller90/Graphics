#include "FlyCamera.h"
#include<iostream>
FlyCamera::FlyCamera()
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	setLookAt(vec3(10.f,10.f,10.f),vec3(0.f,0.f,-1.0f), up);
	setPerspective(glm::pi<float>() * 0.35f, 16 / 9.f, 0.1f, 1000.f);
	lastX = 0;
	lastY = 0;
	
}

bool firstMouse = true;

void FlyCamera::update(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();


	mat4 camTrans = mat4(1);
	mat4 camScale = mat4(1);
	mat4 camRot = mat4(1);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{

		//Old mouse coordinates
		double xpos = 0;
		double ypos = 0;

		float theta = 0.0f;

		//Get the mouse coordinates
		glfwGetCursorPos(window, &xpos, &ypos);

		if (firstMouse) // this bool variable is initially set to true
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		//Get the offsets
		float xoffset = lastX - xpos;
		float yoffset = lastY - ypos; 
		
		//Mouse sensitivity
		xoffset *= 0.005f;
		yoffset *= 0.005f;

		//Check the direction in x axis of mouse movement
		if (xoffset < 1 || xoffset > 1)
		{
			//Rotate the y axis
			theta = xoffset;
			camRot[0][0] = cos(theta);
			camRot[0][2] = sin(theta);
			camRot[2][0] = -1.f * sin(theta);
			camRot[2][2] = cos(theta);
		}

		//Check the direction in y axis of mouse movement
		if (yoffset < 1 || yoffset > 1)
		{
			//Rotate the x axis
			theta = yoffset;
			camRot[1][1] = cos(theta);
			camRot[1][2] = -1.f * sin(theta);
			camRot[2][1] = sin(theta);
			camRot[2][2] = cos(theta);
		}

		//Set positions 
		lastX = xpos;
		lastY = ypos;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3(0, 0, (-0.10f * deltaTime) * 10));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3((-0.10f * deltaTime) * 10, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3(0, 0, (0.10f * deltaTime) * 10));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camTrans = glm::translate(vec3((0.10f * deltaTime) * 10, 0, 0));
	}


	viewTransform *= glm::inverse(camTrans * camRot * camScale);
	updateProjectionViewTransform();
}

void FlyCamera::setSpeed(float speed)
{
}

