/*
		To Do
			Add UI
				clean up ui code
			Create a cache class to store all objects rendered to a scene
			Low priority: add support for large shapes w/ (vertex or index) > batch size
*/

#include <iostream>
#include <gtc/type_ptr.hpp>

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

	Graphics::Renderer renderer(1, 500000);
	renderer.Init();

	renderer.GetDefaultCamera()->SetCenter(0, 0, 0);
	renderer.GetDefaultCamera()->SetPosition(0, 0, 20);

	/* initialize ImGui */
	UI::UserInterface ui;
	ui.Init(window.GetWindow(), "#version 330 core");

	/* create a new shader to handle this surface */
	Graphics::Shader surface_shader;
	surface_shader.Create(6, "res/shaders/surface_test/vertex.glsl", "res/shaders/surface_test/fragment.glsl");
	surface_shader.AddVertexBuffer(1, 3, 0);
	surface_shader.AddVertexBuffer(2, 3, 3);
	renderer.GetDefaultCamera()->AttachShader("u_projection", surface_shader);

	Graphics::FloatContainer gamma_value;
	surface_shader.AttachUniform("u_gamma", gamma_value);
	gamma_value.SetData(2.2);

	Graphics::FloatContainer exposure;
	surface_shader.AttachUniform("u_exposure", exposure);
	exposure.SetData(1.f);

	Graphics::FloatContainer sun_intensity;
	surface_shader.AttachUniform("u_intensity", sun_intensity);
	sun_intensity.SetData(1.f);

	Graphics::Vec3Container sun_color;
	surface_shader.AttachUniform("u_sunColor", sun_color);
	float sun_color_raw[] = {1.f, 1.f, 1.f};
	sun_color.SetData(sun_color_raw);

	Graphics::Vec3Container sun_position;
	surface_shader.AttachUniform("u_sunPosition", sun_position);
	float sun_position_raw[] = {0.f, 1000.f, 0.f};
	sun_position.SetData(sun_position_raw);

	Graphics::Vec3Container camera_position;
	surface_shader.AttachUniform("u_cameraPosition", camera_position);
	//camera_position.SetData(camera_position_raw);

	/* create a texture to display perlin noise */
	unsigned int surface_x_sample_count = 800;
	unsigned int surface_z_sample_count = 800;

	Generate::PerlinNoise2D new_perlin_noise(10);
	unsigned char texture_data[surface_x_sample_count * surface_z_sample_count];
	for(unsigned int i=0;i<surface_z_sample_count;++i)
		for(unsigned int j=0;j<surface_x_sample_count;++j)
			texture_data[i * surface_x_sample_count + j] = (unsigned char)(new_perlin_noise.Sample(j, i) * 255);

	Graphics::Texture surface_texture(1);
	surface_texture.Create(1, surface_x_sample_count, surface_z_sample_count, texture_data, TEX_MAG_NEAREST);

	/* generate the vertices of the surface */
	float* surface_vertices = new float[surface_x_sample_count * surface_z_sample_count * 6];
	for(unsigned int i=0;i<surface_z_sample_count;++i)
	{
		for(unsigned int j=0;j<surface_x_sample_count;++j)
		{
			unsigned int vertex_index = ((i * surface_x_sample_count) + j) * 6;
			float height = new_perlin_noise.Sample((int)j, (int)i) * 400.f;
			surface_vertices[vertex_index + 0] = (float)j * 50.f - (25.f * surface_x_sample_count);
			surface_vertices[vertex_index + 1] = height;
			surface_vertices[vertex_index + 2] = (float)i * 50.f - (50.f * surface_z_sample_count);

			glm::vec3 up_dir = glm::vec3(0.f, new_perlin_noise.Sample((int)j, (int)(i-1)) * 400.f - height, -50.f);
			glm::vec3 down_dir = glm::vec3(0.f, new_perlin_noise.Sample((int)j, (int)(i+1)) * 400.f - height, 50.f);
			glm::vec3 left_dir = glm::vec3(-50.f, new_perlin_noise.Sample((int)(j-1), (int)i) * 400.f - height, 0.f);
			glm::vec3 right_dir = glm::vec3(50.f, new_perlin_noise.Sample((int)(j+1), (int)i) * 400.f - height, 0.f);

			glm::vec3 up_left_cross = glm::normalize(glm::cross(up_dir, left_dir));
			glm::vec3 left_down_cross = glm::normalize(glm::cross(left_dir, down_dir));
			glm::vec3 down_right_cross = glm::normalize(glm::cross(down_dir, right_dir));
			glm::vec3 right_up_cross = glm::normalize(glm::cross(right_dir, up_dir));

			glm::vec3 dir_x = glm::vec3(1, 0, 0);
			glm::vec3 dir_y = glm::vec3(0, 1, 0);
			glm::vec3 dir_z = glm::vec3(0, 0, 1);
			glm::vec3 normal = (up_left_cross + left_down_cross + down_right_cross + right_up_cross) / 4.f;

			surface_vertices[vertex_index + 3] = glm::dot(dir_x, normal);
			surface_vertices[vertex_index + 4] = glm::dot(dir_y, normal);
			surface_vertices[vertex_index + 5] = glm::dot(dir_z, normal);
		}
	}

	/* generate the indices of the surface */
	unsigned int counter = 0;
	unsigned int* surface_indices = new unsigned int[2 * (surface_x_sample_count - 1) * (surface_z_sample_count - 1) * 3];
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

	ui.SetFolderImgRes(surface_texture.GetId());
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

		float* camera_pos = const_cast<float*>(glm::value_ptr(renderer.GetDefaultCamera()->GetPosition()));
		camera_position.SetData(camera_pos);

		gamma_value.SetData(ui.GetToolsUI().GetGammaCorrection());
		exposure.SetData(ui.GetToolsUI().GetExposure());
		sun_intensity.SetData(ui.GetToolsUI().GetSunIntensity());
		float calc_sun_pos[] = { ui.GetToolsUI().GetSunPositionX(), ui.GetToolsUI().GetSunPositionY(), ui.GetToolsUI().GetSunPositionZ() };
		sun_position.SetData(calc_sun_pos);
		float sun_col[] = { ui.GetToolsUI().GetSunColorRed(), ui.GetToolsUI().GetSunColorGreen(), ui.GetToolsUI().GetSunColorBlue() };
		sun_color.SetData(sun_col);

		/* generate the vertices */

		/*renderer.BindFrameBuffer(test_buffer);
		renderer.Fill(255, 255);
		renderer.DrawImage(surface_texture, -10, -10, 20, 20);*/

		//renderer.BindDefaultFrameBuffer();
		renderer.BindFrameBuffer(test_buffer);
		renderer.BindShader(surface_shader);
		renderer.DrawPolygons(surface_x_sample_count * surface_z_sample_count * 6, surface_vertices, counter, surface_indices);
		
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
	delete[] surface_vertices;
	delete[] surface_indices;

	return 0;
}
