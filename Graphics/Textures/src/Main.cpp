#define GLM_FORCE_PURE
#include<iostream>
#include "Application.h"
#include "Textures.h"

int main()
{
	Application* textures = new Textures();

	if (textures->startUp() == true)
	{
		while (textures->update() == true)
			textures->Draw();
		textures->Destroy();
	}

	delete textures;
	return 0;
}