#include "Application.h"

Application::Application()
{
	float time = (float)glfwGetTime();

    glfwInit();

	// create a basic window
	window = glfwCreateWindow(1080, 720, "Window", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

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

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer
}

bool Application::startUp()
{
	// create vertex and index data for a quad
	Vertex vertices[4];
	unsigned int indices[4] = { 0,1,2,3};

	vertices[0].position = vec4(-5, 0, -5, 1);
	vertices[1].position = vec4(5, 0, -5, 1);
	vertices[2].position = vec4(-5, 0, 5, 1);
	vertices[3].position = vec4(5, 0, 5, 1);

	vertices[0].colour = vec4(1, 0, 0, 1);
	vertices[1].colour = vec4(0, 1, 0, 1);
	vertices[2].colour = vec4(0, 0, 1, 1);
	vertices[3].colour = vec4(1, 1, 1, 1);

	// create opengl data

	// generate buffers
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &m_VAO);

	// all changes will apply to this handle
	glBindVertexArray(m_VAO);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 *
		sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//Create Shaders
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec4 colour; \
							out vec4 vColour; \
							uniform mat4 projectionViewWorldMatrix; \
							void main() {vColour = colour; gl_Position = projectionViewWorldMatrix * position; }";

	const char* fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 fragColor; \
							void main() { fragColor = vColour; }";


	//Compiles the shader..
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	//..

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
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
	}

	// we don't need to keep the individual shaders around
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;
}

bool Application::update()
{
	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		return true;
	}
	return false;

}

void Application::Draw()
{
	/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
	GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
	OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	glUseProgram(m_programID);

	// where to send the matrix
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");

	// send the matrix
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	// draw quad
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0); //jesusGL take the wheel this goes up to opengl

	//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
	//anything rendered by us with OpenGL.
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::Destroy()
{
	// cleanup render data
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);

	glfwDestroyWindow(window);
	glfwTerminate();
}

