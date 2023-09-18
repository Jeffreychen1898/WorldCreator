#pragma once

#include <iostream>
#include <list>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "popups/popups.h"
#include "uiTools.h"
#include "../geometryCache/geometryCache.h"
#include "../graphics/window.h"
#include "../graphics/render.h"
#include "../graphics/opengl/frameBuffer.h"

#define CAMERA_ROTATE_SPEED 0.005
#define CAMERA_ZOOM_SPEED 0.1
#define CAMERA_MOVEMENT_SPEED 5.0

namespace UI
{
    class UserInterface
    {
        private:
            unsigned int m_windowWidth;
            unsigned int m_windowHeight;
            float m_fps;

            GeometryCache::ShapesCache* m_shapeCache;
            float m_rotateX, m_rotateY, m_rotateZ;

            Graphics::Window* m_window;
            Graphics::Renderer* m_renderer;

            /* rendering window stuff */
            ImVec2 m_renderWindowSize;
            ImVec2 m_renderWindowPosition;

            /* different buffers */
            Graphics::FrameBuffer* m_geometryBuffer;

            /* align the different tabs */
            unsigned int m_toolsWidth;
            unsigned int m_propertiesWidth;
            unsigned int m_resHeight;

            bool m_isPropertiesWidthSet;
            bool m_isToolsWidthSet;
            bool m_isResHeightSet;

            ImGuiWindowFlags m_mainWindowFlags;

            unsigned int m_folderImageRes;

            unsigned int m_mainFramebuffer;
            float m_mainFrameBufferAspectRatio;

            std::string m_windowFocused;

            float m_sunDistance;

            /* call the different classes to handle each tab */
            UI::UITools m_uiTools;

            /* track list of popups */
            UI::PopupHandler* m_popupHandler;
        public:
            UserInterface();
            ~UserInterface();

            void Init(Graphics::Window* _window, Graphics::Renderer* _renderer, const char* _glslVersion);
            void StartOfFrame(unsigned int _windowWidth, unsigned int _windowHeight, float _fps);
            void Update(float _dt) { SetCameraControls(_dt); };
            void EndOfFrame();
            void DisplayUI();

            void AddShapeCache(GeometryCache::ShapesCache& _cache) { m_shapeCache = &_cache; };
            void SetGBuffer(Graphics::FrameBuffer* _buffer) { m_geometryBuffer = _buffer; };

            void SetFolderImgRes(unsigned int _textureId) { m_folderImageRes = _textureId; };
            void SetMainFrameBuffer(unsigned int _frameBuffer) { m_mainFramebuffer = _frameBuffer; };

            std::string GetFocusedWindow() const { return m_windowFocused; };

            UI::UITools& GetToolsUI() { return m_uiTools; };
        
        private:
            void CreateWindow(const char* _windowName, ImGuiWindowFlags _windowFlags);
            void CreateWindow(const char* _windowName);
            void DisplayToolsWindow();
            void DisplayRenderWindow();
            void DisplayResWindow();
            void DisplayPropertiesWindow();

            void SetCameraControls(float _dt);
    };
}