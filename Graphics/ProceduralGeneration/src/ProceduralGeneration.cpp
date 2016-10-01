#include "ProceduralGeneration.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///<summary>
///Constructor
///</summary>
ProceduralGeneration::ProceduralGeneration()
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
bool ProceduralGeneration::startUp()
{
	int dims = 25;
	float *perlin_data = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;

	for (int x = 0; x < dims; ++x)
	{
		for (int y = 0; y < dims; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlin_data[y * dims + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample =
					glm::perlin(vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlin_data[y * dims + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	int imageWidth = dims, imageHeight = dims, imageFormat = 0;
	//perlin
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, imageWidth, imageHeight, 0, GL_RED, GL_FLOAT, perlin_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	stbi_image_free(perlin_data);

	//rocks
	unsigned char* data = stbi_load("./data/textures/rocky_ground.tga",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_rocks);
	glBindTexture(GL_TEXTURE_2D, m_rocks);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	stbi_image_free(data);

	//snow
	data = stbi_load("./data/textures/gravel.tga",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_snow);
	glBindTexture(GL_TEXTURE_2D, m_snow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	//grass
	data = stbi_load("./data/textures/grass.tga",
		&imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_grass);
	glBindTexture(GL_TEXTURE_2D, m_grass);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);


	GeneratePlaneBuffers(dims, dims);
	CompileAndLinkShaders();

	return true;
}

///<summary>
///Function that handles the window staying open and camera update
///</summary>
bool ProceduralGeneration::update()
{
	float currentFrame = glfwGetTime();
	m_time = currentFrame - lastFrame;
	lastFrame = currentFrame;

	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		//update time and camera
		myCamera->update(m_time * 10.0f);
		m_projectionViewMatrix = myCamera->getProjectionView();

		return true;
	}
	return false;

}

///<summary>
///Draws to the window
///</summary>
void ProceduralGeneration::Draw()
{
	/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
	GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
	OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	glUseProgram(m_programID);

	// where to send the matrix
	m_projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");


	//Plane
	DrawPlane();

	//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
	//anything rendered by us with OpenGL.
	glfwSwapBuffers(window);
	glfwPollEvents();
}

///<summary>
///Handles the cleanup of buffers and the window
///</summary>
void ProceduralGeneration::Destroy()
{
	// cleanup render data
	glDeleteProgram(m_programID);

	glDeleteVertexArrays(1, &m_planeVAO);
	glDeleteBuffers(1, &m_planeVBO);
	glDeleteBuffers(1, &m_planeIBO);

	glfwDestroyWindow(window);
	glfwTerminate();
}

///<summary>
///Creates the window and view matrix
///</summary>
bool ProceduralGeneration::GLInitWindow()
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
	//m_projectionViewMatrix = projection * view;
	m_projectionViewMatrix = myCamera->getProjection() * myCamera->getView();

	return true;
}

///<summary>
///Function that generates the plane buffers, binds them, enables them and where the code will be for vertices setup
///<para></para>
///<remarks><paramref name=" Width"></paramref> -The width of the plane</remarks>
///<para></para>
///<remarks><paramref name=" Height"></paramref> -The height of the plane</remarks>
///</summary>
bool ProceduralGeneration::GeneratePlaneBuffers(const int & width, const int & height)
{
	//// create vertex and index data
	//Vertex vertices[4];
	//unsigned int indices[4] = { 0,1,2,3 };

	////Set the positions
	//vertices[0].position = vec4(-width, 0, -height, 1);
	//vertices[1].position = vec4(width, 0, -height, 1);
	//vertices[2].position = vec4(-width, 0, height, 1);
	//vertices[3].position = vec4(width, 0, height, 1);


	//vertices[0].UV = vec2(0.0f, 0.0f);
	//vertices[1].UV = vec2(1.0f, 0.0f);
	//vertices[2].UV = vec2(0.0f, 1.0f);
	//vertices[3].UV = vec2(1.0f, 1.0f);

	 //create opengl data for a grid
		Vertex* vertices = new Vertex[width * height];
		for (int r = 0; r < width; ++r) {
			for (int c = 0; c < height; ++c) {
	
				// offset position so that the terrain is centered
				vertices[r * height + c].position = vec4(c - height * 0.5f, 0, r - width * 0.5f, 1);
	
				// setting up UVs
				vertices[r * height + c].UV = vec2(c * (1.f / height), r * (1.f / width));
			}
		}
	
		// keep track of number of indices for rendering
		m_indexCount = (width - 1) * (height - 1) * 6;
	
		unsigned int* indices = new unsigned int[m_indexCount];
	
		unsigned int index = 0;
		for (int r = 0; r < (width - 1); ++r) {
			for (int c = 0; c < (height - 1); ++c) {
				// triangle 1
				indices[index++] = r * height + c;
				indices[index++] = (r + 1) * height + c;
				indices[index++] = (r + 1) * height + (c + 1);
				// triangle 2
				indices[index++] = r * height + c;
				indices[index++] = (r + 1) * height + (c + 1);
				indices[index++] = r * height + (c + 1);
			}
		}

	// generate buffers
	glGenBuffers(1, &m_planeVBO);
	glGenBuffers(1, &m_planeIBO);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &m_planeVAO);


	// all changes will apply to this handle
	glBindVertexArray(m_planeVAO);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeIBO);

	//Set the buffer data for the vertices
	glBufferData(GL_ARRAY_BUFFER, (width * height) * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	//Set the buffer data for the indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount *
		sizeof(unsigned int), indices, GL_STATIC_DRAW);

	////Set the buffer data for the vertices
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex),
	//	vertices, GL_STATIC_DRAW);
	//
	////Set the buffer data for the indices
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 *
	//	sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec4));


	return true;
}

///<summary>
///Function that compiles, attaches, links, and deletes the shaders.
///<para></para>
///<remarks><paramref name=" a_directory"></paramref> -The name of file to load data from</remarks>
///</summary>
bool ProceduralGeneration::CompileAndLinkShaders()
{
	//Store the returned string into a variable
	std::string vertex = ReadShaderFromFile("vertexShader.txt");

	//Convert to const char* so it can be used in the glShaderSourceFunction
	const char* vsSource = vertex.c_str();

	//Store the returned string into a variable
	std::string fragment = ReadShaderFromFile("fragShader.txt");

	//Convert to const char* so it can be used in the glShaderSourceFunction
	const char* fsSource = fragment.c_str();

	//Compiles the shader..
	int success = GL_FALSE;
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(m_vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(m_vertexShader);
	glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(m_fragmentShader);
	//..

	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShader);
	glAttachShader(m_programID, m_fragmentShader);
	glLinkProgram(m_programID);

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

	// we don't need to keep the individual shaders around
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);

	return true;
}

///<summary>
///Function that reads from the passed in file the shader data
///<para></para>
///<remarks><paramref name=" a_File"></paramref> -The name of file to load data from</remarks>
///</summary>
std::string ProceduralGeneration::ReadShaderFromFile(const std::string &a_File)
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
bool ProceduralGeneration::CreateDefaultShaderFiles()
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
///Function that draws a plane
///</summary>
void ProceduralGeneration::DrawPlane()
{
	glUniformMatrix4fv(m_projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));
	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// set texture slot
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_rocks);

	// set texture slot
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_snow);

	// set texture slot
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_grass);

	// tell the shader where it is
	m_projectionViewUniform = glGetUniformLocation(m_programID, "perlin_texture");
	glUniform1i(m_projectionViewUniform, 0);

	// tell the shader where it is
	m_projectionViewUniform = glGetUniformLocation(m_programID, "rocks");
	glUniform1i(m_projectionViewUniform, 1);

	// tell the shader where it is
	m_projectionViewUniform = glGetUniformLocation(m_programID, "snow");
	glUniform1i(m_projectionViewUniform, 2);

	// tell the shader where it is
	m_projectionViewUniform = glGetUniformLocation(m_programID, "grass");
	glUniform1i(m_projectionViewUniform, 3);


	glBindVertexArray(m_planeVAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



