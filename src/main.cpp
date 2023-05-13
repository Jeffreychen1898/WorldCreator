#include <iostream>

#include "graphics/render.h"
#include "graphics/window.h"
#include "graphics/camera.h"

#define CAMERA_ROTATE_SPEED 0.005
#define CAMERA_ZOOM_SPEED 0.1

int main()
{
	#if DEBUG_MODE
	std::cout << "Debug Mode On" << std::endl;
	#endif

	Graphics::Window window(1024, 768, "World Creator");
	if(!window.Init())
		return -1;

	Graphics::Renderer renderer(1, 5000);
	renderer.Init();

	renderer.GetDefaultCamera()->SetCenter(0, 0, 0);
	renderer.GetDefaultCamera()->SetPosition(0, 0, 20);

	while(window.IsOpen())
	{
		window.StartOfFrame();
		renderer.StartOfFrame();

		/* rotate by mouse drag */
		if(window.IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			renderer.GetDefaultCamera()->RotateHorizontal(CAMERA_ROTATE_SPEED * (window.GetMouseX() - window.GetPreviousMouseX()));
			renderer.GetDefaultCamera()->RotateVertical(CAMERA_ROTATE_SPEED * (window.GetMouseY() - window.GetPreviousMouseY()));
		}

		/* scrolling moves the camera in and out */
		double camera_zoom_amount = CAMERA_ZOOM_SPEED * renderer.GetDefaultCamera()->GetPositionCenterDistance();
		renderer.GetDefaultCamera()->MoveForward(window.GetDeltaScrollPosition() * camera_zoom_amount, false);
		
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
