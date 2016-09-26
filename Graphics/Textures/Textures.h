#ifndef _TEXTURES_H
#define _TEXTURES_H

#include <iostream>
#include "Application.h"
#include "FlyCamera.h"

class Textures : public Application
{

public:
	Textures();
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
	unsigned int m_texture2;
	unsigned int m_programID;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_projectionViewUniform;
	unsigned int m_vbo, m_ibo, m_vao;
	unsigned int m_vbo1, m_ibo1, m_vao1;
	bool GLInitWindow();
	bool CompileAndLinkShaders();
	std::string ReadShaderFromFile(const std::string &a_File);
	bool CreateDefaultShaderFiles();
	bool GeneratePlaneBuffers();

};
#endif