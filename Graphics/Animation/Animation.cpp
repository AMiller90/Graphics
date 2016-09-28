#include "Animation.h"

#include <fstream>
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///<summary>
///Constructor
///</summary>
Animation::Animation()
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
bool Animation::startUp()
{
	//Load fbx
	m_fbx = new FBXFile();
	m_fbx->load("./FBXModels/characters/Pyro/pyro.fbx");
	createOpenGLBuffers(m_fbx);

	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	//"./data/textures/crate.png"
	unsigned char* data = stbi_load("./FBXModels/characters/Pyro/Pyro_D.tga",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);


	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	stbi_image_free(data);

	// load normal map
	data = stbi_load("./data/textures/rock_normal.tga",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_normalmap);
	glBindTexture(GL_TEXTURE_2D, m_normalmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	CompileAndLinkShaders();

	lastFrame = 0.0f;
	return true;
}

///<summary>
///Function that handles the window staying open and camera update
///</summary>
bool Animation::update()
{
	float currentFrame = glfwGetTime();
	m_time = currentFrame - lastFrame;
	lastFrame = currentFrame;

	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		FBXSkeleton* skeleton = m_fbx->getSkeletonByIndex(0);
		FBXAnimation* animation = m_fbx->getAnimationByIndex(0);

		// evaluate the animation to update bones
		skeleton->evaluate(animation, m_time);

		for (unsigned int bone_index = 0;
			bone_index < skeleton->m_boneCount;
			++bone_index)
		{
			skeleton->m_nodes[bone_index]->updateGlobalTransform();
		}

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
void Animation::Draw()
{
	/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
	GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
	OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	glUseProgram(m_programID);

	//// bind the camera
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	//glUniformMatrix4fv(loc, 1, GL_FALSE, &(myCamera->getProjectionView()[0][0]));
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m_projectionViewMatrix));

	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// set texture slot
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalmap);

	//// tell the shader where it is
	//loc = glGetUniformLocation(m_programID, "position");
	//glUniform1i(loc, 0);
	//
	//// tell the shader where it is
	//loc = glGetUniformLocation(m_programID, "normal");
	//glUniform1i(loc, 1);
	//
	//// tell the shader where it is
	//loc = glGetUniformLocation(m_programID, "tangent");
	//glUniform1i(loc, 2);
	//
	//// tell the shader where it is
	//loc = glGetUniformLocation(m_programID, "texcoord");
	//glUniform1i(loc, 3);
	//
	//// tell the shader where it is
	//loc = glGetUniformLocation(m_programID, "weights");
	//glUniform1i(loc, 4);
	//
	//// tell the shader where it is
	//loc = glGetUniformLocation(m_programID, "indices");
	//glUniform1i(loc, 5);

	DrawFBX();


	//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
	//anything rendered by us with OpenGL.
	glfwSwapBuffers(window);
	glfwPollEvents();
}

///<summary>
///Handles the cleanup of buffers and the window
///</summary>
void Animation::Destroy()
{
	//Cleanup fbx
	cleanupOpenGLBuffers(m_fbx);

	// cleanup render data
	glDeleteProgram(m_programID);

	glfwDestroyWindow(window);
	glfwTerminate();
}

///<summary>
///Function that creates the buffers according to the mesh count of the passed in FBX file
///<para></para>
///<remarks><paramref name=" fbx"></paramref> -The name of file to load data from</remarks>
///</summary>
void Animation::createOpenGLBuffers(FBXFile* fbx)
{
	// create the GL VAO/VBO/IBO data for each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);
		// storage for the opengl data in 3 unsigned int
		unsigned int* glData = new unsigned int[3];
		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER,
			mesh->m_vertices.size() * sizeof(FBXVertex),
			mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->m_indices.size() * sizeof(unsigned int),
			mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normals
		glEnableVertexAttribArray(2); //tangents
		glEnableVertexAttribArray(3); //texcoords
		glEnableVertexAttribArray(4); //weights
		glEnableVertexAttribArray(5); //indices
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),
			(void*)FBXVertex::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),
			(void*)FBXVertex::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
			(void*)FBXVertex::IndicesOffset);


		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}

}

///<summary>
///Function that deletes all the pointer data from the FBX object
///<para></para>
///<remarks><paramref name=" fbx"></paramref> -The name of file to load data from</remarks>
///</summary>
void Animation::cleanupOpenGLBuffers(FBXFile* fbx)
{
	// clean up the vertex data attached to each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i) {
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;
		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);
		delete[] glData;
	}

}

///<summary>
///Creates the window and view matrix
///</summary>
bool Animation::GLInitWindow()
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
bool Animation::CompileAndLinkShaders()
{
	//Store the returned string into a variable
	std::string vertex = ReadShaderFromFile("Animationvert.txt");

	//Convert to const char* so it can be used in the glShaderSourceFunction
	const char* vsSource = vertex.c_str();

	//Store the returned string into a variable
	std::string fragment = ReadShaderFromFile("Animationfrag.txt");

	//Convert to const char* so it can be used in the glShaderSourceFunction
	const char* fsSource = fragment.c_str();

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

	return true;
}

///<summary>
///Function that reads from the passed in file the shader data
///<para></para>
///<remarks><paramref name=" a_File"></paramref> -The name of file to load data from</remarks>
///</summary>
std::string Animation::ReadShaderFromFile(const std::string &a_File)
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
bool Animation::CreateDefaultShaderFiles()
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

///<summary>
///Function that draws the loaded GBX model
///</summary>
bool Animation::DrawFBX()
{
	// grab the skeleton and animation we want to use 
	FBXSkeleton* skeleton = m_fbx->getSkeletonByIndex(0);
	skeleton->updateBones();

	int bones_location = glGetUniformLocation(m_programID, "bones");
	glUniformMatrix4fv(bones_location, skeleton->m_boneCount, GL_FALSE,
		(float*)skeleton->m_bones);

	// bind our vertex array object and draw the mesh
	//for Fbx
	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {
		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;
		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES,
			(unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	return true;
}
