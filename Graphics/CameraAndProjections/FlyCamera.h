#ifndef _FLYCAMERA_H_
#define _FLYCAMERA_H_

#include "Camera.h"
class FlyCamera : public Camera
{

public:
	FlyCamera();
	void update(float deltaTime);
	void setSpeed(float speed);

private:
	mat4 cameraMove;
	float speed;
	vec3 up;
	float inc;
};

#endif
