#define GLM_FORCE_PURE
#include<iostream>
#include "Application.h"
#include "RenderGeometry.h"
#include "FlyCamera.h"


int main()
{
	Application* renderGeometry = new RenderGeometry();
	Camera* myCamera = new FlyCamera;
	if (renderGeometry->startUp() == true)
	{
		while (renderGeometry->update() == true)
			renderGeometry->Draw();
		renderGeometry->Destroy();
	}

	delete renderGeometry;
	return 0;

}
