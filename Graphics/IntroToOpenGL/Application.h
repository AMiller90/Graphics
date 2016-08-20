#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <gizmos\Gizmos.h>
#include "src\gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>

class Application
{
public:

	virtual bool Create()=0;
	virtual void Destroy()=0;
	virtual bool Update()=0;
	virtual void Draw()=0;

};
#endif
