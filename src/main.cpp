#include <iostream>

#include "graphics/render.h"
#include "graphics/window.h"
#include "graphics/camera.h"

int main()
{
	#if DEBUG_MODE
	std::cout << "Debug Mode On" << std::endl;
	#endif

	Graphics::Window window(1024, 768, "World Creator");
	if(!window.Init())
		return -1;

	Graphics::Renderer renderer(10, 5000);
	renderer.Init();

	renderer.GetDefaultCamera()->SetCenter(0, 0, -30.f);

	while(window.IsOpen())
	{
		window.StartOfFrame();
		renderer.StartOfFrame();

		if(window.IsKeyPressed(GLFW_KEY_W))
		{
			renderer.GetDefaultCamera()->MoveForward(2);
			//renderer.GetDefaultCamera()->MoveCenter(0, 2, 0);
		}
		if(window.IsKeyPressed(GLFW_KEY_A))
		{
			renderer.GetDefaultCamera()->MoveRight(-2);
			//renderer.GetDefaultCamera()->MoveCenter(-2, 0, 0);
		}
		if(window.IsKeyPressed(GLFW_KEY_S))
		{
			renderer.GetDefaultCamera()->MoveForward(-2);
			//renderer.GetDefaultCamera()->MoveCenter(0, -2, 0);
		}
		if(window.IsKeyPressed(GLFW_KEY_D))
		{
			renderer.GetDefaultCamera()->MoveRight(2);
			//renderer.GetDefaultCamera()->MoveCenter(2, 0, 0);
		}

		if(window.IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			renderer.GetDefaultCamera()->RotateHorizontal(0.001 * (window.GetMouseX() - window.GetPreviousMouseX()));
			renderer.GetDefaultCamera()->RotateVertical(0.001 * (window.GetMouseY() - window.GetPreviousMouseY()));
		}
		
		renderer.GetDefaultCamera()->Update();

		//renderer.DrawRect(-0.2, -0.2, 0.3, 0.3);
		//renderer.DrawRect(0.2, 0.2, 0.3, 0.3);
		renderer.DrawRect(-10, -10, 20, 20);
		renderer.DrawRect(300, 300, 200, 200);

		renderer.EndOfFrame();

		window.EndOfFrame();
	}

	return 0;
}
