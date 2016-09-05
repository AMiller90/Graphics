#define GLM_FORCE_PURE

#include "SolarSystem.h"

int main()
{
	
	Application* solarsystem = new SolarSystem();

	if(solarsystem->startUp() == true)
	{
		while (solarsystem->update() == true)
			solarsystem->Draw();
		solarsystem->Destroy();
	}
	
	delete solarsystem;
	return 0;
}