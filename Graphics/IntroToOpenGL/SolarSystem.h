#ifndef _SOLARSYSTEM_H_
#define _SOLARSYSTEM_H_

#include "Application.h"

using glm::mat4;
using glm::vec3;
using glm::vec4;

class SolarSystem : public Application
{

public:
	bool Create();
	void Destroy();
	bool Update();
	void Draw();

private:

	GLFWwindow* window;
	mat4 view;
	mat4 projection;
	mat4 sun;
	mat4 earth;
	mat4 moon;
	mat4 pluto;
	mat4 mars;

};





#endif
