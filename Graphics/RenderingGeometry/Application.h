#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <gizmos\Gizmos.h>
#include "src\gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>

using glm::vec3;
using glm::vec4;

struct Vertex
{
	vec4 position;
	vec4 colour;

};
class Application
{
public:
	Application();
	bool startUp();
	bool update();
	void Draw();
	void generateGrid(unsigned int rows, unsigned int cols);
	void Destroy();

private:
	GLFWwindow* window;
	unsigned int m_programID;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	glm::mat4 m_projectionViewMatrix;

};
#endif
