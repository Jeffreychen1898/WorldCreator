#include <iostream>

#include "graphics/render.h"
#include "graphics/window.h"

int main()
{
	#if DEBUG_MODE
	std::cout << "Debug Mode On" << std::endl;
	#endif

	Graphics::Window window(1024, 768, "World Creator");
	if(!window.Init())
		return -1;

	Graphics::Renderer renderer;
	renderer.Init();

	while(window.IsOpen())
	{
		renderer.StartOfFrame();

		//renderer.DrawRect(-0.2, -0.2, 0.3, 0.3);
		//renderer.DrawRect(0.2, 0.2, 0.3, 0.3);
		renderer.DrawRect(100, 100, 100, 100);
		renderer.DrawRect(300, 300, 200, 200);

		renderer.EndOfFrame();

		window.EndOfFrame();
	}

	return 0;
}
