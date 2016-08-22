#include "SolarSystem.h"
#include<iostream>

bool SolarSystem::Create()
{
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

	return true;
}

void SolarSystem::Destroy()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool SolarSystem::Update()
{
	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

	    float time = (float)glfwGetTime();

		/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
		GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn’t do this then
		OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		Gizmos::clear();
		Gizmos::addTransform(glm::mat4(1));

		//From the time of build it is a 3.5sec delay that causes the planets to "snap" to the starting location.
		//This -3.5sec will reduce the delay and have the planets start at the correct positions rather than snap to catch up with time
		time -= 3.5;

		sun = glm::rotate(time, vec3(0, 1, 0));
		earth = sun * glm::translate(vec3(5, 0, 0));
		moon = earth * glm::rotate(time *.25f, vec3(0, 1, 0)) * glm::translate(vec3(2, 0, 0));

		pluto = sun * glm::translate(vec3(10, 0, 2));
		mars = sun * glm::translate(vec3(8, 0, -5));

		/*float ex = earth[3].x;
		float ey = earth[3].y;
		float ez = earth[3].z;

		float mx = moon[3].x;
		float my = moon[3].y;
		float mz = moon[3].z;*/
		
		
		/*moon = earth * glm::translate(vec3(2 * sin(time), 0.f, 2 * cos(time)));
		earth = sun * glm::translate(vec3(5 * sin(time* 0.5), 0.f, 5 * cos(time* 0.5)));

		ez = sun[3].z + (5 * cos(time * 0.5));
		ex = sun[3].x + (5 * sin(time * 0.5));

		earth[3].x = sun[3].x + ex;
		earth[3].z = sun[3].z + ez;

		mz = earth[3].z + (2 * cos(time * 2));
		mx = earth[3].x + (2 * sin(time * 2));

		moon[3].x = earth[3].z + mx;
		moon[3].z = earth[3].z + mz;*/

		return true;
	}
	return false;
}

void SolarSystem::Draw()
{
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
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

	//This updates the monitors display but swapping the rendered back buffer.If we did not call this then we wouldn’t be able to see
	//anything rendered by us with OpenGL.
	glfwSwapBuffers(window);
	glfwPollEvents();
}
