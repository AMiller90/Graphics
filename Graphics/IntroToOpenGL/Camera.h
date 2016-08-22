#ifndef _CAMERA_H
#define _CAMERA_H

#include <gizmos\Gizmos.h>
#include "src\gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>

using glm::mat4;

class Camera
{

public:



protected:
	mat4 worldTransform;
	mat4 viewTransform;
	mat4 projectionTransform;
	mat4 projectionViewTransform;

};
#endif
