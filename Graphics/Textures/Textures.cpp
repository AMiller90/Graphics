#include "Textures.h"
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master\stb_image.h>

///<summary>
///Constructor
///</summary>
Textures::Textures()
{
	//Create a shader directory when constructor is called
	CreateDirectory("Shaders/", NULL);

	//Init glfw, window and camera
	GLInitWindow();

	//Function to create a default txt file for a shader when program is run to prevent errors
	CreateDefaultShaderFiles();

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer
}

///<summary>
///Function that calls functions to generate buffers and compile and link them
///</summary>
bool Textures::startUp()
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load("./data/textures/crate.png",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	CompileAndLinkShaders();
	GeneratePlaneBuffers();
	lastFrame = 0.0f;
	return true;
}

///<summary>
///Function that handles the window staying open and camera update
///</summary>
bool Textures::update()
{
	float currentFrame = glfwGetTime();
	m_time = currentFrame - lastFrame;
	lastFrame = currentFrame;

	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		//update time and camera
		myCamera->update(m_time * 10.f);
		m_projectionViewMatrix = myCamera->getProjectionView();

		return true;
	}
	return false;

}

///<summary>
///Draws to the window
///</summary>
void Textures::Draw()
{
	/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
	GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
	OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	glUseProgram(m_programID);

	// bind the camera
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
		&(myCamera->getProjectionView()[0][0]));

	//// where to send the matrix
	//m_projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");

	//GLint loc = glGetUniformLocation(m_programID, "Time");


	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// tell the shader where it is
	loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(loc, 0);
	
	// draw
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	 

	//glUniform1f(loc, m_time);
	//printf("m_time: %f \n", m_time);


	//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
	//anything rendered by us with OpenGL.
	glfwSwapBuffers(window);
	glfwPollEvents();
}

///<summary>
///Handles the cleanup of buffers and the window
///</summary>
void Textures::Destroy()
{
	// cleanup render data
	glDeleteProgram(m_programID);

	glfwDestroyWindow(window);
	glfwTerminate();
}

///<summary>
///Creates the window and view matrix
///</summary>
bool Textures::GLInitWindow()
{
	//Initialize glfw
	glfwInit();

	// create a basic window
	window = glfwCreateWindow(1080, 720, "Window", nullptr, nullptr);

	//Check if window is equal to null
	if (window == nullptr)
	{//Terminate if it is
		glfwTerminate();
	}

	//Make the current window the window object
	glfwMakeContextCurrent(window);

	//Check if loaded correctly
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	//Initilaize the camera
	myCamera = new FlyCamera;

	myCamera->setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	myCamera->setPerspective(glm::pi<float>() * 0.35f, 16 / 9.f, 0.1f, 1000.f);

	//Set matrix
	m_projectionViewMatrix = myCamera->getProjection() * myCamera->getView();

	return true;
}

///<summary>
///Function that compiles, attaches, links, and deletes the shaders.
///<para></para>
///<remarks><paramref name=" a_directory"></paramref> -The name of file to load data from</remarks>
///</summary>
bool Textures::CompileAndLinkShaders()
{
	//Store the returned string into a variable
	std::string vertex = ReadShaderFromFile("vertexTexture.txt");

	//Convert to const char* so it can be used in the glShaderSourceFunction
	const char* vsSource = vertex.c_str();

	//Store the returned string into a variable
	std::string fragment = ReadShaderFromFile("fragTexture.txt");

	//Convert to const char* so it can be used in the glShaderSourceFunction
	const char* fsSource = fragment.c_str();

//	const char* vsSource = "#version 410\n \
//layout(location=0) in vec4 Position; \
//layout(location=1) in vec2 TexCoord; \
//out vec2 vTexCoord; \
//uniform mat4 ProjectionView; \
//void main() { \
//vTexCoord = TexCoord; \
//gl_Position= ProjectionView * Position;\
//}";
//	const char* fsSource = "#version 410\n \
//in vec2 vTexCoord; \
//out vec4 FragColor; \
//uniform sampler2D diffuse; \
//uniform sampler2D white; \
//void main() { \
//FragColor = texture2D(white,vTexCoord) * texture(diffuse,vTexCoord);\
//}";

	//Compiles the shader..
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// check that it compiled and linked correctly
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
		return false;
	}

	//// we don't need to keep the individual shaders around
	//glDeleteShader(m_fragmentShader);
	//glDeleteShader(m_vertexShader);

	return true;
}

///<summary>
///Function that reads from the passed in file the shader data
///<para></para>
///<remarks><paramref name=" a_File"></paramref> -The name of file to load data from</remarks>
///</summary>
std::string Textures::ReadShaderFromFile(const std::string &a_File)
{
	//Create string variable to store data in
	std::string data;
	//This variable will be for all the contents to be returned from function
	std::string contents;

	//Open the specified file from the Shaders folder 
	std::ifstream file("Shaders/" + a_File);

	if (file.fail())
	{
		std::cout << "Failed to open file!\n";
	}
	else
	{
		//Read from file here
		while (getline(file, data))
		{//This will not work correctly without the \n added!
		 //getline() grabs everything up to the endline so adding the endline will allow it to be read
			contents += data + "\n";
		}
	}
	//Return the data
	return contents;
}

///<summary>
///Function that creates default shader files
///</summary>
bool Textures::CreateDefaultShaderFiles()
{
	//Create default vertShader incase user doesnt have a file to read from at first
	const char* vertShader = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec4 colour; \
							out vec4 vColour; \
							uniform mat4 projectionViewWorldMatrix; \
							void main() {vColour = colour; gl_Position = projectionViewWorldMatrix * position; }";

	//Create default fragShader incase user doesnt have a file to read from at first
	const char* fragShader = "#version 410\n \
							in vec4 vColour; \
							out vec4 fragColor; \
							void main() { fragColor = vColour; }";

	//Create these files
	std::ofstream vertfile("Shaders/Defaultvert.txt");
	std::ofstream fragfile("Shaders/Defaultfrag.txt");

	//Vert shader
	if (vertfile.fail())
	{
		std::cout << "File failed to open!\n";
	}
	else
	{
		vertfile << vertShader;
	}

	//Frag shader
	if (fragfile.fail())
	{
		std::cout << "File failed to open!\n";
	}
	else
	{
		fragfile << fragShader;
	}

	return true;
}

bool Textures::GeneratePlaneBuffers()
{
	float vertexData[] = {
		-5, 0, 5, 1, 0, 1,
		5, 0, 5, 1, 1, 1,
		5, 0, -5, 1, 1, 0,
		-5, 0, -5, 1, 0, 0,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
		vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
		indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}