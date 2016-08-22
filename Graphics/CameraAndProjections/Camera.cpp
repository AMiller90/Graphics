#include "Camera.h"

void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float)
{
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
}

void Camera::setPosition(vec3 position)
{
}

mat4 Camera::getWorldTransform()
{
	return worldTransform;
}

mat4 Camera::getView()
{
	viewTransform = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0)) * glm::inverse(glm::translate(vec3(0, 5, 0)));
	return viewTransform;
}

mat4 Camera::getProjection()
{
	projectionTransform = glm::perspective(glm::pi<float>() * 0.35f,
		16 / 9.f, 0.1f, 1000.f);
	return projectionTransform;
}

mat4 Camera::getProjectionView()
{
	return projectionViewTransform = projectionTransform * viewTransform;

}

void Camera::updateProjectionViewTransform()
{
	viewTransform = glm::inverse(worldTransform);
	projectionViewTransform = viewTransform * projectionTransform;
}
