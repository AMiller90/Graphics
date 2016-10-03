#include "SolarSystem.h"
#include<iostream>

//void OnMouseButton(GLFWwindow*, int b, int a, int m) {
//	TwEventMouseButtonGLFW(b, a);
//}
//void OnMousePosition(GLFWwindow*, double x, double y) {
//	TwEventMousePosGLFW((int)x, (int)y);
//}
//void OnMouseScroll(GLFWwindow*, double x, double y) {
//	TwEventMouseWheelGLFW((int)y);
//}
//void OnKey(GLFWwindow*, int k, int s, int a, int m) {
//	TwEventKeyGLFW(k, a);
//}
//void OnChar(GLFWwindow*, unsigned int c) {
//	TwEventCharGLFW(c, GLFW_PRESS);
//}
//void OnWindowResize(GLFWwindow*, int w, int h) {
//	TwWindowSize(w, h);
//	glViewport(0, 0, w, h);
//}

SolarSystem::SolarSystem()
{
	m_bDrawGizmoGrid = true;
}

bool SolarSystem::startUp()
{
	float time = (float)glfwGetTime();

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1080, 720, "Window", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}


	Gizmos::create();
	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.35f,
		16 / 9.f, 0.1f, 1000.f);


	mat4 worldspacecamera = glm::inverse(view);

	worldspacecamera = glm::translate(vec3(10, 10, 10)) * time;

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	sun = mat4(1);

	earth = mat4(1);
	earth[3] = vec4(-8, 0, 0, 0);

	moon = mat4(1);
	moon[3] = vec4(-5, 0, 0, 0);

	pluto = mat4(1);
	pluto[3] = vec4(8, 0, 0, 0);

	mars = mat4(1);
	mars[3] = vec4(5, 0, 0, 0);

	m_clearColour = vec3(1, 1, 1);

	ImGui_ImplGlfwGL3_Init(window, true);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1080;
	io.DisplaySize.y = 720;

	//TwInit(TW_OPENGL_CORE, nullptr);
	//TwWindowSize(1080, 720);
	//
	//m_bar = TwNewBar("MyTweakBar");
	//TwAddVarRW(m_bar, "clear colour",
	//	TW_TYPE_COLOR4F, &m_clearColour[0], "");
	//
	//glfwSetMouseButtonCallback(window, OnMouseButton);
	//glfwSetCursorPosCallback(window, OnMousePosition);
	//glfwSetScrollCallback(window, OnMouseScroll);
	//glfwSetKeyCallback(window, OnKey);
	//glfwSetCharCallback(window, OnChar);
	//glfwSetWindowSizeCallback(window, OnWindowResize);


	return true;
}

void SolarSystem::Destroy()
{
	Gizmos::destroy();

	ImGui_ImplGlfwGL3_Shutdown();
	//TwDeleteAllBars();
	//TwTerminate();

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool SolarSystem::update()
{
	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

	    float time = (float)glfwGetTime();

		glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, 1);

		/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
		GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
		OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		Gizmos::clear();
		
		//Clear ImGui
		ImGui_ImplGlfwGL3_NewFrame();

		Gizmos::addTransform(glm::mat4(1));

		//From the time of build it is a 3.5sec delay that causes the planets to "snap" to the starting location.
		//This -3.5sec will reduce the delay and have the planets start at the correct positions rather than snap to catch up with time
		time -= 3.5;

		sun = glm::rotate(time, vec3(0, 1, 0));
		earth = sun * glm::translate(vec3(5, 0, 0));
		moon = earth * glm::rotate(time *.25f, vec3(0, 1, 0)) * glm::translate(vec3(2, 0, 0));

		pluto = sun * glm::translate(vec3(10, 0, 2));
		mars = sun * glm::translate(vec3(8, 0, -5));
		

		return true;
	}
	return false;
}

void SolarSystem::Draw()
{
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	if (m_bDrawGizmoGrid)
	{
		for (int i = 0; i < 21; ++i) {
			Gizmos::addLine(vec3(-10 + i, 0, 10),
				vec3(-10 + i, 0, -10),
				i == 10 ? white : black);
			Gizmos::addLine(vec3(10, 0, -10 + i),
				vec3(-10, 0, -10 + i),
				i == 10 ? white : black);
		}
	}

	vec4 orange(255, 128, 0, 1);
	vec4 blue(0, 0, 255, 1);
	vec4 red(255, 0, 0, 1);
	vec4 purple(128, 0, 128, 1);

	Gizmos::addLine(vec3(moon[3]), vec3(earth[3]), red);
	Gizmos::addLine(vec3(earth[3]), vec3(sun[3]), blue);

	Gizmos::addLine(vec3(sun[3]), vec3(mars[3]), black);
	Gizmos::addLine(vec3(sun[3]), vec3(pluto[3]), purple);

	Gizmos::addSphere(vec3(sun[3]), 1, 10, 10, orange, nullptr, 0.0f, 360.0f, -90.0f, 90.0f);
	Gizmos::addSphere(vec3(earth[3]), 0.75f, 10, 10, blue, nullptr, 0.0f, 360.0f, -90.0f, 90.0f);
	Gizmos::addSphere(vec3(moon[3]), 0.5f, 10, 10, red, nullptr, 0.0f, 360.0f, -90.0f, 90.0f);
	Gizmos::addSphere(vec3(mars[3]), 1, 10, 10, black, nullptr, 0.0f, 360.0f, -90.0f, 90.0f);
	Gizmos::addSphere(vec3(pluto[3]), 0.75f, 10, 10, purple, nullptr, 0.0f, 360.0f, -90.0f, 90.0f);

	Gizmos::draw(projection * view);

	ImGui::Begin("Rendering Options");
	ImGui::SetWindowPos("Rendering Options", ImVec2(0, 0), 0);
	ImGui::ColorEdit3("clear color", glm::value_ptr(m_clearColour));
	ImGui::Checkbox("Gizmo Grid", &m_bDrawGizmoGrid);
	ImGui::End();

	ImGui::Begin("Hierarchy");
	ImGui::End();

	ImGui::Render();
	//TwDraw();

	//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
	//anything rendered by us with OpenGL.
	glfwSwapBuffers(window);
	glfwPollEvents();
	
}