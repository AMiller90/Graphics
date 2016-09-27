#ifndef _TEXTURES_H
#define _TEXTURES_H

#include <iostream>
#include "Application.h"
#include "FlyCamera.h"
#include "FBXFile.h"

struct Vertex {
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float s, t;
};

class Textures : public Application
{

public:
	Textures();
	bool startUp() override;
	bool update() override;
	void Draw() override;
	void Destroy() override;

private:

	FBXFile* m_fbx;
	void createOpenGLBuffers(FBXFile* fbx);
	void cleanupOpenGLBuffers(FBXFile* fbx);

	Camera* myCamera;
	GLFWwindow* window;

	float m_time;
	float lastFrame;

	glm::mat4 m_projectionViewMatrix;
	unsigned int m_texture, m_normalmap;

	unsigned int m_programID;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_projectionViewUniform;
	unsigned int m_vbo, m_ibo, m_vao;

	bool GLInitWindow();
	bool CompileAndLinkShaders();
	std::string ReadShaderFromFile(const std::string &a_File);
	bool CreateDefaultShaderFiles();
	bool GeneratePlaneBuffers();

};
#endif