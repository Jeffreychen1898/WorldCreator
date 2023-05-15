/*
		NOTE TO SELF
			refactor any ugly code
			code review
			draw diagram for code architechure
*/

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

	/* create a new shader to handle this surface */
	Graphics::Shader surface_shader;
	surface_shader.Create(3, "res/shaders/surface_test/vertex.glsl", "res/shaders/surface_test/fragment.glsl");
	surface_shader.AddVertexBuffer(1, 3, 0);
	renderer.GetDefaultCamera()->AttachShader("u_projection", surface_shader);

	/* generate the vertices of the surface */
	unsigned int surface_x_sample_count = 10;
	unsigned int surface_z_sample_count = 10;
	float surface_vertices[surface_x_sample_count * surface_z_sample_count * 3];
	for(unsigned int i=0;i<surface_z_sample_count;++i)
	{
		for(unsigned int j=0;j<surface_x_sample_count;++j)
		{
			unsigned int vertex_index = ((i * surface_x_sample_count) + j) * 3;
			surface_vertices[vertex_index + 0] = (float)j * 50 - 250;
			surface_vertices[vertex_index + 1] = 0;
			surface_vertices[vertex_index + 2] = (float)i * 50 - 500;
		}
	}

	/* generate the indices of the surface */
	unsigned int counter = 0;
	unsigned int surface_indices[2 * (surface_x_sample_count - 1) * (surface_z_sample_count - 1) * 3];
	for(unsigned int i=0;i<surface_z_sample_count-1;++i)
	{
		for(unsigned int j=0;j<surface_x_sample_count-1;++j)
		{
			surface_indices[counter + 0] = surface_x_sample_count * i + j;
			surface_indices[counter + 1] = surface_x_sample_count * i + j + 1;
			surface_indices[counter + 2] = surface_x_sample_count * (i + 1) + j + 1;
			surface_indices[counter + 3] = surface_x_sample_count * i + j;
			surface_indices[counter + 4] = surface_x_sample_count * (i + 1) + j + 1;
			surface_indices[counter + 5] = surface_x_sample_count * (i + 1) + j;

			counter += 6;
		}
	}

	renderer.BindDefaultShader();
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

		/* generate the vertices */

		renderer.Fill(255, 255, 0);
		renderer.DrawRect(-10, -10, 20, 20);

		renderer.BindShader(surface_shader);
		renderer.DrawPolygons(surface_x_sample_count * surface_z_sample_count * 3, surface_vertices, counter, surface_indices);

		renderer.EndOfFrame();

		std::cout << renderer.GetDrawCallCount() << std::endl;

		window.EndOfFrame();
	}

	return 0;
}
