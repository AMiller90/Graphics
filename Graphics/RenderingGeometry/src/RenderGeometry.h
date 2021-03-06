#ifndef _RENDERGEOMETRY_H_
#define _RENDERGEOMETRY_H_

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

//PLEASE READ:
//The ReadShaderFromFile function does exactly that. The glShaderSource openGL function has a parameter that takes a const char* data type
//variable. However, I could not get the ReadShaderFromfile function to return a const char* correctly. It would give a linker error.
//But when i return a string then use the c_str() function on it, it will be converted to a const char* then be stored into a variable to be used.
//This actually works great when putting that variable into the glShaderSource parameter.
//No idea why if i use the c_str() on a string variable in the ReadShaderFromFile function and return it, with the functions return type as
// cont char*, it gives linker errors and says the void main function cant be found. So just convert the string to a const char* before
//putting it into the glShaderSource function and it will work. Now the paramater for the ReadShaderFromFile functon is a const string
//reference variable. All that needs to be passed in is the filename and extention. The Directory is already taken care of, however,
//whichever file you choose to open, MAKE SURE, the file is located inside the Shaders folder in the folder of your project. The file will
//not be read from any other directory other than the Shaders/ folder.
//So when writing your shaders you can do so in notepad or however you choose. Then save the file and put it into your Shaders folder.
//Then go into the code and put the name of the file you want to open into the ReadShaderFromFile function. If you want to open multiple
//Files then you will have to call the function more than once with a different file name in the parameter each time.
//The Shaders folder is created when the constructor is called for the RenderingGeometry class just incase the user doesnt create it.
//I have created a function called CreateDefaultShaderFiles. This function will create a shader and a vertex file when its called in
//the constructor , so by default, this program will create a colored plane and wont throw any errors if there are no files yet created,
//by someone else.


using glm::vec3;
using glm::vec4;

struct Vertex
{
	vec4 position;
	vec4 colour;

};

class RenderGeometry : public Application
{
public:
	RenderGeometry();
	bool startUp() override;
	bool update() override;
	void Draw() override;
	void Destroy() override;

private:

	Camera* myCamera;
	GLFWwindow* window;

	float m_time;
	glm::mat4 m_projectionViewMatrix;

	unsigned int m_programID;
	unsigned int m_cubeVBO, m_cubeIBO, m_cubeVAO;
	unsigned int m_triangleVBO, m_triangleIBO, m_triangleVAO;
	unsigned int m_planeVBO, m_planeIBO, m_planeVAO;
	unsigned int m_circleVBO, m_circleIBO, m_circleVAO;
	unsigned int m_sphereVBO, m_sphereIBO, m_sphereVAO;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_projectionViewUniform;

	//Global variable to set total verts of Sphere
	int indicesCounter;
	std::vector<unsigned int> indicesHolder;

	bool GLInitWindow();
	bool GenerateCubeBuffers(const int &width, const int &height);
	bool GenerateTriangleBuffers(const int &width, const int &height);
	bool GeneratePlaneBuffers(const int &width, const int &height);
	bool GenerateCircleBuffers(const int &radius);
	bool GenerateSphereBuffers(const int &radius, const int &np, const int &numMeridians);

	bool CompileAndLinkShaders();
	std::string ReadShaderFromFile(const std::string &a_File);
	bool CreateDefaultShaderFiles();

	void DrawPlane();
	void DrawCircle(bool isFilled);
	void DrawCube();
	void DrawTriangle();
	void DrawSphere();
	Vertex* DrawHalfCircle(const int &np, const int &radius);
	Vertex* generateSphereVertices(const unsigned int &sides, const unsigned int &mirid, Vertex* &halfSphere);
	unsigned int* generateSphereIndicies(const unsigned int &vertices, const unsigned int &mirid);
};
#endif
