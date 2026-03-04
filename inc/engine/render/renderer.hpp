#pragma once

#include "backend/graphics_pipeline.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <glm/glm.hpp>
#include <memory>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace cg::engine::backend {
class Device;
class Swapchain;
struct Buffer;
class GraphicsPipeline;
class RendererManager;
} // namespace cg::engine::backend

namespace cg::engine {

class Renderer final {
  private:
    bool m_debug = true;
    glm::ivec2 m_window_size;
    struct WindowDeleter {
        void operator()(SDL_Window *window) {
            if (window) {
                SDL_DestroyWindow(window);
                window = nullptr;
            }
        }
    };
    std::unique_ptr<SDL_Window, WindowDeleter> m_window;
    // vulkan things
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_dbg_messager;
    VkSurfaceKHR m_surface;
    std::unique_ptr<cg::engine::backend::Device> m_device;
    std::unique_ptr<cg::engine::backend::Swapchain> m_swapchain;
    std::unique_ptr<cg::engine::backend::RendererManager> m_manager;

  private:
    // internal function for sdl
    void updateWindowSize();
    // internal function for vulkan init
    [[nodiscard]] bool initInstance();
    [[nodiscard]] bool initSurface();

  public:
    bool begin(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);
    bool end();

  public:
    void drawBase();

  public:
    Renderer(const glm::ivec2 &size);
    ~Renderer();

    [[nodiscard]] bool
    init(VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT);
    void quit();

    cg::engine::backend::RendererManager &operator*() { return *m_manager; }

    Renderer(Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &) = delete;
    Renderer &operator=(Renderer &&) = delete;
};

} // namespace cg::engine
