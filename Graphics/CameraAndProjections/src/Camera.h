#ifndef _CAMERA_H
#define _CAMERA_H

#include <gizmos\Gizmos.h>
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>

using glm::vec3;
using glm::mat4;

class Camera
{

public:
	mat4 viewTransform;
	virtual void update(float deltaTime) = 0;
	void setPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform();
	mat4 getView();
	mat4 getProjection();
	mat4 getProjectionView();
	void updateProjectionViewTransform();
	
private:
	mat4 worldTransform;
	mat4 projectionTransform;
	mat4 projectionViewTransform;

};
#endif