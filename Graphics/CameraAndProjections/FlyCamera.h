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
	float speed;
	vec3 up;
	double lastX;
	double lastY;
};
#endif
