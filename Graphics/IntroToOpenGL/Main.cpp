#define GLM_FORCE_PURE

#include "SolarSystem.h"

int main()
{
	
	SolarSystem* solarsystem = new SolarSystem;

	if(solarsystem->Create() == true)
	{
		while (solarsystem->Update() == true)
			solarsystem->Draw();
		solarsystem->Destroy();
	}
	
	delete solarsystem;
	return 0;
}