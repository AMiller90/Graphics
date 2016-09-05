#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <gizmos\Gizmos.h>
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>

class Application
{
public:
	virtual bool startUp() = 0;
	virtual bool update() = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;
};
#endif
