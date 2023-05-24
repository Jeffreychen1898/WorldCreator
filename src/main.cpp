/*
		To Do
			Add UI
			Create a cache class to store all objects rendered to a scene
			Low priority: add support for large shapes w/ (vertex or index) > batch size
*/

#include <iostream>

#include "graphics/render.h"
#include "graphics/window.h"
#include "graphics/camera.h"
#include "utils/helperFunctions.h"
#include "generate/perlinNoise.h"
#include "generate/worleyNoise.h"
#include "graphics/opengl/frameBuffer.h"
#include "ui/userInterface.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define CAMERA_ROTATE_SPEED 0.005
#define CAMERA_ZOOM_SPEED 0.1

int main()
{
	#if DEBUG_MODE
	std::cout << "Debug Mode On" << std::endl;
	#endif

	Utils::HelperInit();

	Graphics::Window window(1024, 768, "World Creator");
	if(!window.Init())
		return -1;

	Graphics::Renderer renderer(1, 5000);
	renderer.Init();

	renderer.GetDefaultCamera()->SetCenter(0, 0, 0);
	renderer.GetDefaultCamera()->SetPosition(0, 0, 20);

	/* initialize ImGui */
	UI::UserInterface ui;
	ui.Init(window.GetWindow(), "#version 330 core");

	/* create a new shader to handle this surface */
	Graphics::Shader surface_shader;
	surface_shader.Create(3, "res/shaders/surface_test/vertex.glsl", "res/shaders/surface_test/fragment.glsl");
	surface_shader.AddVertexBuffer(1, 3, 0);
	renderer.GetDefaultCamera()->AttachShader("u_projection", surface_shader);

	/* create a texture to display perlin noise */
	unsigned int surface_x_sample_count = 20;
	unsigned int surface_z_sample_count = 20;

	Generate::PerlinNoise2D new_perlin_noise(10);
	unsigned char texture_data[surface_x_sample_count * surface_z_sample_count];
	for(unsigned int i=0;i<surface_z_sample_count;++i)
		for(unsigned int j=0;j<surface_x_sample_count;++j)
			texture_data[i * surface_x_sample_count + j] = (unsigned char)(new_perlin_noise.Sample(j, i) * 255);

	Graphics::Texture surface_texture(1);
	surface_texture.Create(1, surface_x_sample_count, surface_z_sample_count, texture_data, TEX_MAG_NEAREST);

	/* generate the vertices of the surface */
	float surface_vertices[surface_x_sample_count * surface_z_sample_count * 3];
	for(unsigned int i=0;i<surface_z_sample_count;++i)
	{
		for(unsigned int j=0;j<surface_x_sample_count;++j)
		{
			unsigned int vertex_index = ((i * surface_x_sample_count) + j) * 3;
			float height = new_perlin_noise.Sample((int)j-10, (int)i-10) * 400.f;
			surface_vertices[vertex_index + 0] = (float)j * 50.f - (25.f * surface_x_sample_count);
			surface_vertices[vertex_index + 1] = height;
			surface_vertices[vertex_index + 2] = (float)i * 50.f - (50.f * surface_z_sample_count);
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
	/* generate frame buffer */
	Graphics::FrameBuffer test_buffer(4);
	test_buffer.Create(1024, 768);

	ui.SetMainFrameBuffer(test_buffer.GetTexture()->GetId());

	renderer.BindDefaultShader();
	while(window.IsOpen())
	{
		window.StartOfFrame();
		renderer.StartOfFrame();
		ui.StartOfFrame(window.GetWidth(), window.GetHeight());

		test_buffer.Clear();

		if(ui.GetFocusedWindow() == "Display")
		{
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
		}

		/* generate the vertices */

		/*renderer.BindFrameBuffer(test_buffer);
		renderer.Fill(255, 255);
		renderer.DrawImage(surface_texture, -10, -10, 20, 20);*/

		//renderer.BindDefaultFrameBuffer();
		renderer.BindFrameBuffer(test_buffer);
		renderer.BindShader(surface_shader);
		renderer.DrawPolygons(surface_x_sample_count * surface_z_sample_count * 3, surface_vertices, counter, surface_indices);
		
		/*renderer.BindDefaultFrameBuffer();
		renderer.Fill(255, 255, 0);
		renderer.DrawImage(*test_buffer.GetTexture(), -10, -10, 20, 20);*/

		renderer.BindDefaultFrameBuffer();
		ui.DisplayUI();

		renderer.EndOfFrame();

		ui.EndOfFrame();

		//std::cout << renderer.GetDrawCallCount() << std::endl;

		window.EndOfFrame();
	}

	Utils::HelperClose();

	return 0;
}
