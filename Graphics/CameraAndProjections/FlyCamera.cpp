#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
	cameraMove = mat4(1);
	inc = 0;
}

void FlyCamera::update(float deltaTime)
{

	//int key = 0;
 //  
	//switch (key)
	//{
	//case GLFW_KEY_W:
	//	inc += 1;
	//	cameraMove = glm::translate(vec3(0, inc, 0));
	//	getWorldTransform() * cameraMove;
	//	printf("inc %f: \n", inc);
	//	break;
	//case GLFW_KEY_A:
	//	getWorldTransform();
	//	break;
	//case GLFW_KEY_S:
	//	getWorldTransform();
	//	break;
	//case GLFW_KEY_D:
	//	getWorldTransform();
	//	break;

	//default:
	//	break;
	//}

	updateProjectionViewTransform();
}

void FlyCamera::setSpeed(float speed)
{
}
