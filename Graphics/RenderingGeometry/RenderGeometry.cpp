#include "RenderGeometry.h"

///<summary>
///Constructor
///</summary>
RenderGeometry::RenderGeometry()
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
bool RenderGeometry::startUp()
{
	GenerateTriangleBuffers(5, 5);
	GenerateCircleBuffers(5);
	GeneratePlaneBuffers(5, 5);
	GenerateCubeBuffers(5, 5);
	GenerateSphereBuffers(5, 12, 4);
	CompileAndLinkShaders();

	return true;
}

///<summary>
///Function that handles the window staying open
///</summary>
bool RenderGeometry::update()
{
	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		return true;
	}
	return false;

}

///<summary>
///Draws to the window
///</summary>
void RenderGeometry::Draw()
{
	/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
	GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
	OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_time = glfwGetTime();
	// bind shader
	glUseProgram(m_programID);

	// where to send the matrix
	m_projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");

	GLint loc = glGetUniformLocation(m_programID, "Time");
	/*glUniform1f(loc, m_time);
	printf("m_time: %f \n", m_time);*/
	
	

	//Triangle
	//DrawTriangle();

	//Plane
	//DrawPlane();

	//Cube
	//DrawCube();

	//Circle
	//DrawCircle(false);

	//Sphere
	DrawSphere();

	//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
	//anything rendered by us with OpenGL.
	glfwSwapBuffers(window);
	glfwPollEvents();
}

///<summary>
///Handles the cleanup of buffers and the window
///</summary>
void RenderGeometry::Destroy()
{
	// cleanup render data
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_triangleVAO);
	glDeleteBuffers(1, &m_triangleVBO);
	glDeleteBuffers(1, &m_triangleIBO);

	glDeleteVertexArrays(1, &m_cubeVAO);
	glDeleteBuffers(1, &m_cubeVBO);
	glDeleteBuffers(1, &m_cubeIBO);

	glDeleteVertexArrays(1, &m_planeVAO);
	glDeleteBuffers(1, &m_planeVBO);
	glDeleteBuffers(1, &m_planeIBO);

	glDeleteVertexArrays(1, &m_circleVAO);
	glDeleteBuffers(1, &m_circleVBO);
	glDeleteBuffers(1, &m_circleIBO);

	glDeleteVertexArrays(1, &m_sphereVAO);
	glDeleteBuffers(1, &m_sphereVBO);
	glDeleteBuffers(1, &m_sphereIBO);

	glfwDestroyWindow(window);
	glfwTerminate();
}

///<summary>
///Creates the window and view matrix
///</summary>
bool RenderGeometry::GLInitWindow()
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

	//Create the view matrix
	glm::mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.35f,
		16 / 9.f, 0.1f, 1000.f);
	//Set matrix
	m_projectionViewMatrix = projection * view;

	return true;
}

///<summary>
///Function that generates the cube buffers, binds them, enables them and where the code will be for vertices setup
///<para></para>
///<remarks><paramref name=" Width"></paramref> -The width of the cube</remarks>
///<para></para>
///<remarks><paramref name=" Height"></paramref> -The height of the cube</remarks>
///</summary>
bool RenderGeometry::GenerateCubeBuffers(const int &width, const int &height)
{
	const unsigned int top = width;

	// create vertex and index data
	Vertex vertices[8];
	unsigned int indices[13] = { 0,1,2,3,4,5,1,6,0,7,2,4,6 };

	//Set the positions
	vertices[0].position = vec4(-width, 0, -height, 1);
	vertices[1].position = vec4(width, 0, -height, 1);
	vertices[2].position = vec4(-width, 0, height, 1);
	vertices[3].position = vec4(width, 0, height, 1);
	vertices[4].position = vec4(-width, top, height, 1);
	vertices[5].position = vec4(width, top, height, 1);
	vertices[6].position = vec4(width, top, -height, 1);
	vertices[7].position = vec4(-width, top, -height, 1);


	// generate buffers
	glGenBuffers(1, &m_cubeVBO);
	glGenBuffers(1, &m_cubeIBO);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &m_cubeVAO);

	// all changes will apply to this handle
	glBindVertexArray(m_cubeVAO);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeIBO);

	//Set the buffer data for the vertices
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	//Set the buffer data for the indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 13 *
		sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	return true;
}

///<summary>
///Function that generates the Triangle buffers, binds them, enables them and where the code will be for vertices setup
///<para></para>
///<remarks><paramref name=" Width"></paramref> -The width of the triangle</remarks>
///<para></para>
///<remarks><paramref name=" Height"></paramref> -The height of the triangle</remarks>
///</summary>
bool RenderGeometry::GenerateTriangleBuffers(const int & width, const int & height)
{
	// create vertex and index data
	Vertex vertices[3];
	unsigned int indices[3] = { 0,1,2 };

	//Set the positions
	vertices[0].position = vec4(-width, 0, -height, 1);
	vertices[1].position = vec4(width, 0, -height, 1);
	vertices[2].position = vec4(-width, 0, height, 1);

	// generate buffers
	glGenBuffers(1, &m_triangleVBO);
	glGenBuffers(1, &m_triangleIBO);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &m_triangleVAO);


	// all changes will apply to this handle
	glBindVertexArray(m_triangleVAO);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triangleIBO);

	//Set the buffer data for the vertices
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	//Set the buffer data for the indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 *
		sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	return true;
}

///<summary>
///Function that generates the plane buffers, binds them, enables them and where the code will be for vertices setup
///<para></para>
///<remarks><paramref name=" Width"></paramref> -The width of the plane</remarks>
///<para></para>
///<remarks><paramref name=" Height"></paramref> -The height of the plane</remarks>
///</summary>
bool RenderGeometry::GeneratePlaneBuffers(const int & width, const int & height)
{
	// create vertex and index data
	Vertex vertices[4];
	unsigned int indices[4] = { 0,1,2,3 };

	//Set the positions
	vertices[0].position = vec4(-width, 0, -height, 1);
	vertices[1].position = vec4(width, 0, -height, 1);
	vertices[2].position = vec4(-width, 0, height, 1);
	vertices[3].position = vec4(width, 0, height, 1);

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
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	//Set the buffer data for the indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 *
		sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));


	return true;
}

///<summary>
///Function that generates the circle buffers, binds them, enables them and where the code will be for vertices setup
///<para></para>
///<remarks><paramref name=" Width"></paramref> -The radius of the circle</remarks>
///<para></para>
///</summary>
bool RenderGeometry::GenerateCircleBuffers(const int & radius)
{
	// create vertex and index data for circle
	Vertex vertices[24];
	unsigned int indices[24] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 };

	//Loop through the number of indices
	for (int i = 0; i <= 23; i++)
	{//Angle between each index
		double angle = i * (2 * 3.14159 / 22);
		//get the cos of the angle and multiply by the radius
		double X = cos(angle) * radius;
		//get the sin of the angle and multiply by the radius
		double Z = sin(angle) * radius;
		//Set the appropriate values per vertex
		vertices[i].position = vec4(X, 0, Z, 1);
	}

	// generate buffers
	glGenBuffers(1, &m_circleVBO);
	glGenBuffers(1, &m_circleIBO);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &m_circleVAO);


	// all changes will apply to this handle
	glBindVertexArray(m_circleVAO);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_circleVBO);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_circleIBO);

	//Set the buffer data for the vertices
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	//Set the buffer data for the indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 *
		sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	return true;
}

///<summary>
///Function that generates the sphere buffers, binds them, enables them and where the code will be for vertices setup
///<para></para>
///<remarks><paramref name=" Radius"></paramref> -The radius of the half circle</remarks>
///<para></para>
///<remarks><paramref name=" Np"></paramref> -The number of points</remarks>
///<para></para>
///<remarks><paramref name=" NumMeridians"></paramref> -The number of subdivisions</remarks>
///</summary>
bool RenderGeometry::GenerateSphereBuffers(const int& radius, const int& np, const int& numMeridians)
{
	//create vertex and index data for circle
	Vertex* vertices = new Vertex[np];

	unsigned int indices[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };

	DrawHalfCircle(np, vertices, 5);


	for (int i = 0; i < numMeridians; i++)
	{
		double phi = 2 * 3.14159265359 * i / numMeridians;
		for (int j = 0; j < np; j++)
		{
			//Angle between each index
			//get the cos of the angle and multiply by the radius
			double X = vertices[i].position.x * cos(phi) + vertices[i].position.z * sin(phi);
			//get the sin of the angle and multiply by the radius
			double Z = vertices[i].position.z * cos(phi) - vertices[i].position.x * sin(phi);
			//Set the appropriate values per vertex
			vertices[i].position = vec4(X, vertices[i].position.y, Z, 1);
		}

	}

	// generate buffers
	glGenBuffers(1, &m_sphereVBO);
	glGenBuffers(1, &m_sphereIBO);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &m_sphereVAO);


	// all changes will apply to this handle
	glBindVertexArray(m_sphereVAO);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereIBO);

	//Set the buffer data for the vertices
	glBufferData(GL_ARRAY_BUFFER, np * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	//Set the buffer data for the indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12*
		sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	return true;
}

///<summary>
///Function that compiles, attaches, links, and deletes the shaders.
///<para></para>
///<remarks><paramref name=" a_directory"></paramref> -The name of file to load data from</remarks>
///</summary>
bool RenderGeometry::CompileAndLinkShaders()
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
std::string RenderGeometry::ReadShaderFromFile(const std::string &a_File)
{
	//Create string variable to store data in
	std::string data;
	//This variable will be for all the contents to be returned from function
	std::string contents;

	//Open the specified file from the Shaders folder 
	std::ifstream file("Shaders/" + a_File);

	if(file.fail())
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
bool RenderGeometry::CreateDefaultShaderFiles()
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
	if(vertfile.fail())
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
void RenderGeometry::DrawPlane()
{
	glBindVertexArray(m_planeVAO);
	glUniformMatrix4fv(m_projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix * glm::translate(vec3(5, 2, -5))));
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
}

///<summary>
///Function that draws a Circle
///<para></para>
///<remarks><paramref name=" isFilled"></paramref> -If true circle will be filled in, If false circle will be empty</remarks>
///<para></para>
///</summary>
void RenderGeometry::DrawCircle(bool isFilled)
{
		glBindVertexArray(m_circleVAO);
		glUniformMatrix4fv(m_projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));
		//If false, then dont fill circle
		if(!isFilled)
			glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_INT, 0);
		else //Triangle fan fills in the circle
			glDrawElements(GL_TRIANGLE_FAN, 24, GL_UNSIGNED_INT, 0);
	
}

///<summary>
///Function that draws a Triangle
///</summary>
void RenderGeometry::DrawTriangle()
{
	glBindVertexArray(m_triangleVAO);
	glUniformMatrix4fv(m_projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix * glm::translate(vec3(0, -15, 0))));
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, 0);
}

///<summary>
///Function that draws a Cube
///</summary>
void RenderGeometry::DrawCube()
{	
	glBindVertexArray(m_cubeVAO);
	glUniformMatrix4fv(m_projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix * glm::translate(vec3(-25, 0, 0))));
	glDrawElements(GL_TRIANGLE_STRIP, 13, GL_UNSIGNED_INT, 0);
}

///<summary>
///Function that draws a Sphere
///</summary>
void RenderGeometry::DrawSphere()
{
	glBindVertexArray(m_sphereVAO);
	glUniformMatrix4fv(m_projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));
	glDrawElements(GL_LINE_LOOP, 12, GL_UNSIGNED_INT, 0);
}

///<summary>
///Function that draws a half circle
///<para></para>
///<remarks><paramref name=" Np"></paramref> -The number of points</remarks>
///<para></para>
///<remarks><paramref name=" Vertices"></paramref> -The vertices object to set</remarks>
///<para></para>
///<remarks><paramref name=" Radius"></paramref> -The radius of the half circle</remarks>
///</summary>
void RenderGeometry::DrawHalfCircle(const int& np, Vertex* vertices, const int& radius)
{
	for (int i = 0; i < np; i++)
	{
		//Angle between each index
		double theta = i  * 3.14159265359 / np;
		//get the cos of the angle and multiply by the radius
		double X = cos(theta) * radius;
		//get the sin of the angle and multiply by the radius
		double Y = sin(theta) * radius;
		//Set the appropriate values per vertex
		vertices[i].position = vec4(X, Y, 0, 1);
	}
}