#include "userInterface.h"

namespace UI
{
    UserInterface::UserInterface()
        : m_mainFrameBufferAspectRatio(1024.f / 768.f), m_toolsWidth(300), m_propertiesWidth(300), m_isPropertiesWidthSet(false),
        m_isToolsWidthSet(false), m_isResHeightSet(false), m_resHeight(300), m_folderImageRes(0), m_fps(0)
    {
        m_mainWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
        m_windowFocused = "";
    }

    void UserInterface::Init(GLFWwindow* _glfwWindow, const char* _glslVersion)
    {
        ImGui::CreateContext();
	    ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, true);
	    ImGui_ImplOpenGL3_Init(_glslVersion);
	    ImGui::StyleColorsDark();
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

        float image_width, image_height;
        float image_x = 0;
        float image_y = 0;
        if(calc_width_from_height > display_size.x)
        {
            image_width = display_size.x;
            image_height = calc_height_from_width;
            image_y = 0.5f * (display_size.y - calc_height_from_width);
        } else
        {
            image_width = calc_width_from_height;
            image_height = display_size.y;
            image_x = 0.5f * (display_size.x - calc_width_from_height);
        }

        /* display image */
        ImGui::SetCursorPos(ImVec2(image_x, image_y));
        ImGui::Image(reinterpret_cast<void*>(m_mainFramebuffer), ImVec2(image_width, image_height), ImVec2(0, 1), ImVec2(1, 0));
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
		ImGui::End();
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