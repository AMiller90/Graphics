#ifndef _PROCEDURALGENERATION_H_
#define _PROCEDURALGENERATION_H_

#include <iostream>
#include <gizmos\Gizmos.h>
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>
#include<fstream>
#include <vector>
#include "Application.h"
#include "FlyCamera.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

struct Vertex
{
	vec4 position;
	vec2 UV;
};

class ProceduralGeneration : public Application
{
public:
	ProceduralGeneration();
	bool startUp() override;
	bool update() override;
	void Draw() override;
	void Destroy() override;

private:

	Camera* myCamera;
	GLFWwindow* window;

	float m_time;
	float lastFrame;

	glm::mat4 m_projectionViewMatrix;

	unsigned int m_texture;
	unsigned int m_grass;
	unsigned int m_rocks;
	unsigned int m_snow;

	int m_indexCount;
	unsigned int m_programID;
	unsigned int m_planeVBO, m_planeIBO, m_planeVAO;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_projectionViewUniform;

	bool GLInitWindow();
	bool GeneratePlaneBuffers(const int &width, const int &height);

	bool CompileAndLinkShaders();
	std::string ReadShaderFromFile(const std::string &a_File);
	bool CreateDefaultShaderFiles();

	void DrawPlane();
};
#endif
