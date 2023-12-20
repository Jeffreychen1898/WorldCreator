/*
		To Do
			Add UI
				clean up ui code
			Create a cache class to store all objects rendered to a scene
			Low priority: add support for large shapes w/ (vertex or index) > batch size
*/

#include <iostream>
#include <gtc/type_ptr.hpp>
#include <chrono>
#include <cmath>

#include "graphics/render.h"
#include "graphics/window.h"
#include "graphics/camera.h"
#include "utils/helperFunctions.h"
#include "generate/perlinNoise.h"
#include "generate/worleyNoise.h"
#include "graphics/opengl/frameBuffer.h"
#include "ui/userInterface.h"
#include "utils/helperFunctions.h"
#include "utils/performance.h"
#include "geometryCache/geometryCache.h"
#include "storage/storage.hpp"
#include "storage/object3d.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
	ui.Init(&window, &renderer, "#version 330 core");

	/* create a new shader to handle this surface */
	Graphics::Shader surface_shader;
	surface_shader.Create(3, "res/shaders/surface_test/vertex.glsl", "res/shaders/surface_test/fragment.glsl");
	surface_shader.AddVertexBuffer(1, 3, 0);
	//surface_shader.AddVertexBuffer(2, 3, 3);
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

	GeometryCache::ShapesCache shape_cache;
	// create a texture to display perlin noise
	unsigned int surface_x_sample_count = 100;
	unsigned int surface_z_sample_count = 100;

	Generate::PerlinNoise2D new_perlin_noise(10);
	unsigned char texture_data[surface_x_sample_count * surface_z_sample_count];
	for(unsigned int i=0;i<surface_z_sample_count;++i)
		for(unsigned int j=0;j<surface_x_sample_count;++j)
			texture_data[i * surface_x_sample_count + j] = (unsigned char)(new_perlin_noise.Sample(j, i) * 255);

	Graphics::Texture surface_texture(1);
	surface_texture.Create(1, surface_x_sample_count, surface_z_sample_count, texture_data, TEX_MAG_NEAREST);

	// generate the vertices of the surface
	/*float* surface_vertices = new float[surface_x_sample_count * surface_z_sample_count * 6];
	for(unsigned int i=0;i<surface_z_sample_count;++i)
	{
		for(unsigned int j=0;j<surface_x_sample_count;++j)
		{
			unsigned int vertex_index = ((i * surface_x_sample_count) + j) * 6;
			float height = new_perlin_noise.Sample((int)j, (int)i) * 300.f - 300.f;
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

	// generate the indices of the surface
	unsigned int* surface_indices = new unsigned int[2 * (surface_x_sample_count - 1) * (surface_z_sample_count - 1) * 3];
	for(unsigned int i=0;i<surface_z_sample_count-1;++i)
	{
		for(unsigned int j=0;j<surface_x_sample_count-1;++j)
		{
			unsigned int counter = 6 * (i * (surface_x_sample_count - 1) + j);
			surface_indices[counter + 0] = surface_x_sample_count * i + j;
			surface_indices[counter + 1] = surface_x_sample_count * i + j + 1;
			surface_indices[counter + 2] = surface_x_sample_count * (i + 1) + j + 1;
			surface_indices[counter + 3] = surface_x_sample_count * i + j;
			surface_indices[counter + 4] = surface_x_sample_count * (i + 1) + j + 1;
			surface_indices[counter + 5] = surface_x_sample_count * (i + 1) + j;
		}
	}
	unsigned int cnter = (surface_x_sample_count - 1) * (surface_z_sample_count - 1) * 6;
	shape_cache.AppendShape("test_shape", surface_x_sample_count * surface_z_sample_count * 6, surface_vertices, cnter, surface_indices);
	shape_cache.AppendPlane("test_plane", 0, 300, 0);*/

	ui.AddShapeCache(shape_cache);

	// create the cube to mark the lookat position
	float* lookat_vertices = new float[24];
	unsigned int* lookat_indices = new unsigned int[6];

	lookat_indices[0] = 0;
	lookat_indices[1] = 1;
	lookat_indices[2] = 2;
	lookat_indices[3] = 0;
	lookat_indices[4] = 2;
	lookat_indices[5] = 3;

	/* generate frame buffer */
	Graphics::FrameBuffer test_buffer(2);
	unsigned int text_buffer_channels[] = { TEX_RGBA, TEX_R_UI };
	test_buffer.Create(1024, 768, text_buffer_channels);

	ui.SetFolderImgRes(surface_texture.GetId());
	ui.SetMainFrameBuffer(test_buffer.GetTexture(0)->GetId());
	ui.SetGBuffer(&test_buffer);

	renderer.BindDefaultShader();

	Utils::Performance timer;

	Storage::InitStorage();
	std::string plane_name = "Test";
	Storage::CreatePlane(plane_name, 200, 200, 1, 1);

	while(window.IsOpen())
	{
		//std::cout << timer.GetFps() << std::endl;
		float fps = timer.GetFps();
		float dt = timer.Dt();
		timer.ResetTime();

		window.StartOfFrame();
		renderer.StartOfFrame();
		ui.StartOfFrame(window.GetWidth(), window.GetHeight(), fps);
		ui.Update(dt);

		test_buffer.Clear();

		float* camera_pos = const_cast<float*>(glm::value_ptr(renderer.GetDefaultCamera()->GetPosition()));
		camera_position.SetData(camera_pos);

		gamma_value.SetData(ui.GetToolsUI().GetGammaCorrection());
		exposure.SetData(ui.GetToolsUI().GetExposure());
		sun_intensity.SetData(ui.GetToolsUI().GetSunIntensity());
		float calc_sun_pos[] = { ui.GetToolsUI().GetSunPositionX(), ui.GetToolsUI().GetSunPositionY(), ui.GetToolsUI().GetSunPositionZ() };
		sun_position.SetData(calc_sun_pos);
		float sun_col[] = { ui.GetToolsUI().GetSunColorRed(), ui.GetToolsUI().GetSunColorGreen(), ui.GetToolsUI().GetSunColorBlue() };
		sun_color.SetData(sun_col);

		/* create vertices for the lookat cube */
		{
			glm::vec3 center = renderer.GetDefaultCamera()->GetCenter();
			float lookat_x = glm::dot(center, glm::vec3(1, 0, 0));
			float lookat_y = glm::dot(center, glm::vec3(0, 1, 0));
			float lookat_z = glm::dot(center, glm::vec3(0, 0, 1));
			float lookat_size = 100;

			lookat_vertices[0] = lookat_x - 0.5 * lookat_size;
			lookat_vertices[1] = lookat_y + 0.5 * lookat_size;
			lookat_vertices[2] = lookat_z;
			lookat_vertices[3] = 0;
			lookat_vertices[4] = 0;
			lookat_vertices[5] = 1;

			lookat_vertices[6] = lookat_x + 0.5 * lookat_size;
			lookat_vertices[7] = lookat_y + 0.5 * lookat_size;
			lookat_vertices[8] = lookat_z;
			lookat_vertices[9] = 0;
			lookat_vertices[10] = 0;
			lookat_vertices[11] = 1;

			lookat_vertices[12] = lookat_x + 0.5 * lookat_size;
			lookat_vertices[13] = lookat_y - 0.5 * lookat_size;
			lookat_vertices[14] = lookat_z;
			lookat_vertices[15] = 0;
			lookat_vertices[16] = 0;
			lookat_vertices[17] = 1;

			lookat_vertices[18] = lookat_x - 0.5 * lookat_size;
			lookat_vertices[19] = lookat_y - 0.5 * lookat_size;
			lookat_vertices[20] = lookat_z;
			lookat_vertices[21] = 0;
			lookat_vertices[22] = 0;
			lookat_vertices[23] = 1;
		}

		/* generate the vertices */

		/*renderer.BindFrameBuffer(test_buffer);
		renderer.Fill(255, 255);
		renderer.DrawImage(surface_texture, -10, -10, 20, 20);*/

		//renderer.BindDefaultFrameBuffer();
		renderer.BindFrameBuffer(test_buffer);
		renderer.BindShader(surface_shader);
		// shape_cache.RenderShapes(renderer);
		//renderer.DrawPolygons(24, lookat_vertices, 6, lookat_indices);

		renderer.BindDefaultFrameBuffer();
		ui.DisplayUI();

		renderer.RenderObject3DPolygon();

		// read pixels
		//std::cout << test_buffer.ReadPixel(1, 0, 0) << std::endl;

		renderer.EndOfFrame();

		ui.EndOfFrame();

		window.EndOfFrame();
		return 1;
	}

	Utils::HelperClose();
	//delete[] surface_vertices;
	//delete[] surface_indices;
	delete[] lookat_vertices;
	delete[] lookat_indices;
	Storage::FreeStorage();

	return 0;
}
