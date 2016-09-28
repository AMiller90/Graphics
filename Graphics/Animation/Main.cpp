#define GLM_FORCE_PURE
#include<iostream>
#include "Application.h"
#include "Animation.h"

int main()
{
	Application* animation = new Animation();

	if (animation->startUp() == true)
	{
		while (animation->update() == true)
			animation->Draw();
		animation->Destroy();
	}

	delete animation;
	return 0;
}