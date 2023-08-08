#include "userInterface.h"

namespace UI
{
    UserInterface::UserInterface()
        : m_mainFrameBufferAspectRatio(1024.f / 768.f), m_toolsWidth(300), m_propertiesWidth(300), m_isPropertiesWidthSet(false),
        m_isToolsWidthSet(false), m_isResHeightSet(false), m_resHeight(300), m_folderImageRes(0), m_fps(0)
    {
        m_mainWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
        m_windowFocused = "";

        m_rotateX = 0;
        m_rotateY = 0;
        m_rotateZ = 0;

        m_renderWindowSize = ImVec2(0, 0);
        m_renderWindowPosition = ImVec2(0, 0);
    }

    void UserInterface::Init(Graphics::Window* _window, Graphics::Renderer* _renderer, const char* _glslVersion)
    {
        ImGui::CreateContext();
	    ImGui_ImplGlfw_InitForOpenGL(_window->GetWindow(), true);
	    ImGui_ImplOpenGL3_Init(_glslVersion);
	    ImGui::StyleColorsDark();

        m_window = _window;
        m_renderer = _renderer;
    }

    void UserInterface::StartOfFrame(unsigned int _windowWidth, unsigned int _windowHeight, float _fps)
    {
        ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

        m_windowWidth = _windowWidth;
        m_windowHeight = _windowHeight;
        m_fps = _fps;
    }

    void UserInterface::DisplayUI()
    {
        ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.f);
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);

        DisplayToolsWindow();
        DisplayPropertiesWindow();
        DisplayRenderWindow();
        DisplayResWindow();

		//ImGui::ShowDemoWindow();
    }

    void UserInterface::DisplayRenderWindow()
    {
        /* set window size and position */
        ImGui::SetNextWindowSize(ImVec2(m_windowWidth - m_toolsWidth - m_propertiesWidth, m_windowHeight - m_resHeight));
        ImGui::SetNextWindowPos(ImVec2(m_toolsWidth, 0));

        /* set the style of the window */
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        CreateWindow("Display", ImGuiWindowFlags_NoResize | m_mainWindowFlags);

        /* calculate image size and position on window */
        ImVec2 display_size = ImGui::GetWindowSize();
        float calc_height_from_width = display_size.x / m_mainFrameBufferAspectRatio;
        float calc_width_from_height = display_size.y * m_mainFrameBufferAspectRatio;

        if(calc_width_from_height > display_size.x)
        {
            m_renderWindowSize.x = display_size.x;
            m_renderWindowSize.y = calc_height_from_width;
            m_renderWindowPosition.x = 0.f;
            m_renderWindowPosition.y = 0.5f * (display_size.y - calc_height_from_width);
        } else
        {
            m_renderWindowSize.x = calc_width_from_height;
            m_renderWindowSize.y = display_size.y;
            m_renderWindowPosition.x = 0.5f * (display_size.x - calc_width_from_height);
            m_renderWindowPosition.y = 0.f;
        }

        std::cout << m_renderWindowPosition.x << " : " << m_renderWindowPosition.y << std::endl;
        /* display image */
        ImGui::SetCursorPos(ImVec2(m_renderWindowPosition.x, m_renderWindowPosition.y));
        ImGui::Image(reinterpret_cast<void*>(m_mainFramebuffer), ImVec2(m_renderWindowSize.x, m_renderWindowSize.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        ImGui::PopStyleVar();
    }

    void UserInterface::DisplayResWindow()
    {
        /* set the position and size of the window */
        unsigned int res_window_width = m_windowWidth - m_toolsWidth - m_propertiesWidth;
        if(!m_isResHeightSet)
        {
            ImGui::SetNextWindowSize(ImVec2(res_window_width, m_resHeight));
            m_isResHeightSet = true;
        }
        ImGui::SetNextWindowSizeConstraints(ImVec2(res_window_width, 200), ImVec2(res_window_width, 400));
		ImGui::SetNextWindowPos(ImVec2(m_toolsWidth, m_windowHeight - m_resHeight));

        /* create the window */
		CreateWindow("Resources", m_mainWindowFlags);

        m_resHeight = ImGui::GetWindowSize().y;

        /* create the buttons */
        ImVec2 button_size(75, 75);
        ImVec2 uv_min(0, 1);
        ImVec2 uv_max(1, 0);
        float button_padding = 5;
        unsigned int button_spacing = 20;
        ImColor button_color(255, 255, 255, 255);

        unsigned int available_width = ImGui::GetContentRegionAvail().x;
        unsigned int items_per_row = static_cast<unsigned int>((available_width + button_spacing + 2 * button_padding) / (button_size.x + button_spacing + 2 * button_padding));

        ImGui::BeginTable("resources_table", items_per_row);
        for(unsigned int i=0;i<items_per_row;++i)
            ImGui::TableSetupColumn("Column 1");

        ImGui::TableNextRow();
        for(unsigned int i=0;i<20;++i)
        {
            ImGui::TableNextColumn();
            ImGui::ImageButton(
                reinterpret_cast<ImTextureID>(m_folderImageRes),
                button_size,
                uv_min,
                uv_max,
                button_padding,
                button_color,
                ImColor(255, 255, 255, 255)
            );

            ImGui::Text("Label %d", i);

            if((i + 1) % items_per_row == 0)
                ImGui::TableNextRow();
        }
        ImGui::EndTable();
		
		ImGui::End();
    }

    void UserInterface::DisplayToolsWindow()
    {
        /* set the position and size of the window */
        if(!m_isToolsWidthSet)
        {
            ImGui::SetNextWindowSize(ImVec2(m_toolsWidth, m_windowHeight));
            m_isToolsWidthSet = true;
        }
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, m_windowHeight), ImVec2(500, m_windowHeight));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

        /* create the window */
		CreateWindow("Tools", m_mainWindowFlags);

        m_toolsWidth = ImGui::GetWindowSize().x;

        m_uiTools.DisplayUI(m_fps);

		ImGui::End();
    }

    void UserInterface::DisplayPropertiesWindow()
    {
        /* set the position and size of the window */
        if(!m_isPropertiesWidthSet)
        {
            ImGui::SetNextWindowSize(ImVec2(m_propertiesWidth, m_windowHeight));
            m_isPropertiesWidthSet = true;
        }
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, m_windowHeight), ImVec2(500, m_windowHeight));
		ImGui::SetNextWindowPos(ImVec2(m_windowWidth-m_propertiesWidth, 0));

        /* create the window */
		CreateWindow("Properties", m_mainWindowFlags);

        m_propertiesWidth = ImGui::GetWindowSize().x;
		ImGui::Text("Hello World!");
		if(ImGui::Button("Click Me"))
		{
			std::cout << "Hello World" << std::endl;
		}

        /* test the shape rotations */
        ImGui::DragFloat("Rotate X", &m_rotateX, 1.f, -360.f, 360.f, "%.2f");
        ImGui::DragFloat("Rotate Y", &m_rotateY, 1.f, -360.f, 360.f, "%.2f");
        ImGui::DragFloat("Rotate Z", &m_rotateZ, 1.f, -360.f, 360.f, "%.2f");

        m_shapeCache->SetShapeRotation("test_plane", m_rotateX * 3.14 / 360.0, m_rotateY * 3.14 / 360.0, m_rotateZ * 3.14 / 360.0);

		ImGui::End();
    }

    void UserInterface::SetCameraControls(float _dt)
    {
        if(m_windowFocused == "Display")
		{
			/* rotate by mouse drag */
			if(m_window->IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				m_renderer->GetDefaultCamera()->RotateHorizontal(CAMERA_ROTATE_SPEED * (m_window->GetMouseX() - m_window->GetPreviousMouseX()));
				m_renderer->GetDefaultCamera()->RotateVertical(CAMERA_ROTATE_SPEED * (m_window->GetMouseY() - m_window->GetPreviousMouseY()));
			}

			/* scrolling moves the camera in and out */
			double camera_distance = m_renderer->GetDefaultCamera()->GetPositionCenterDistance();
			double camera_zoom_amount = CAMERA_ZOOM_SPEED * m_renderer->GetDefaultCamera()->GetPositionCenterDistance();
			m_renderer->GetDefaultCamera()->MoveForward(m_window->GetDeltaScrollPosition() * camera_zoom_amount, false);

			// wasd movements
			if(m_window->IsKeyPressed(GLFW_KEY_A) || m_window->IsKeyPressed(GLFW_KEY_LEFT))
			{
				m_renderer->GetDefaultCamera()->MoveRight(-CAMERA_MOVEMENT_SPEED * camera_distance * _dt);
			}
			if(m_window->IsKeyPressed(GLFW_KEY_D) || m_window->IsKeyPressed(GLFW_KEY_RIGHT))
			{
				m_renderer->GetDefaultCamera()->MoveRight(CAMERA_MOVEMENT_SPEED * camera_distance * _dt);
			}
			if(m_window->IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || m_window->IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
			{
				if(m_window->IsKeyPressed(GLFW_KEY_W) || m_window->IsKeyPressed(GLFW_KEY_UP))
				{
					m_renderer->GetDefaultCamera()->MoveCenterAndPosition(0, CAMERA_MOVEMENT_SPEED * camera_distance * _dt, 0);
				}
				if(m_window->IsKeyPressed(GLFW_KEY_S) || m_window->IsKeyPressed(GLFW_KEY_DOWN))
				{
					m_renderer->GetDefaultCamera()->MoveCenterAndPosition(0, -CAMERA_MOVEMENT_SPEED * camera_distance * _dt, 0);
				}
			} else {
				if(m_window->IsKeyPressed(GLFW_KEY_W) || m_window->IsKeyPressed(GLFW_KEY_UP))
				{
					m_renderer->GetDefaultCamera()->MoveForwardXZPlane(CAMERA_MOVEMENT_SPEED * camera_distance * _dt);
				}
				if(m_window->IsKeyPressed(GLFW_KEY_S) || m_window->IsKeyPressed(GLFW_KEY_DOWN))
				{
					m_renderer->GetDefaultCamera()->MoveForwardXZPlane(-CAMERA_MOVEMENT_SPEED * camera_distance * _dt);
				}
				
			}
			
			m_renderer->GetDefaultCamera()->Update();
		}
    }

    void UserInterface::CreateWindow(const char* _windowName, ImGuiWindowFlags _windowFlags)
    {
        ImGui::Begin(_windowName, nullptr, _windowFlags);
        if(ImGui::IsWindowFocused())
            m_windowFocused = _windowName;
    }

    void UserInterface::CreateWindow(const char* _windowName)
    {
        ImGui::Begin(_windowName, nullptr, m_mainWindowFlags);
        if(ImGui::IsWindowFocused())
            m_windowFocused = _windowName;
    }

    void UserInterface::EndOfFrame()
    {
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    UserInterface::~UserInterface()
    {
        ImGui_ImplOpenGL3_Shutdown();
	    ImGui_ImplGlfw_Shutdown();
	    ImGui::DestroyContext();
    }
}