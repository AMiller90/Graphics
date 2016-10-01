#define GLM_FORCE_PURE
#include<iostream>
#include "Application.h"
#include "ProceduralGeneration.h"


int main()
{
	Application* proceduralGeneration = new ProceduralGeneration();
	if (proceduralGeneration->startUp() == true)
	{
		while (proceduralGeneration->update() == true)
			proceduralGeneration->Draw();
		proceduralGeneration->Destroy();
	}

	delete proceduralGeneration;
	return 0;

}