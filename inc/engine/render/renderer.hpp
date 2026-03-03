#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <glm/glm.hpp>
#include <memory>

namespace cg::engine {
// TODO use vulkan for now just use SDL renderer
class Renderer final {
  private:
    glm::ivec2 m_size;
    struct RendererDeleter {
        void operator()(SDL_Renderer *renderer) {
            if (renderer) {
                SDL_DestroyRenderer(renderer);
                renderer = nullptr;
            }
        }
    };
    std::unique_ptr<SDL_Renderer, RendererDeleter> m_renderer;
    struct WindowDeleter {
        void operator()(SDL_Window *window) {
            if (window) {
                SDL_DestroyWindow(window);
                window = nullptr;
            }
        }
    };
    std::unique_ptr<SDL_Window, WindowDeleter> m_window;

  public:
    Renderer(const glm::ivec2 &size);
    ~Renderer();

    const glm::ivec2 &renderSize() const { return m_size; }

    bool init();

    void color(float r, float g, float b, float a = 1.0f);
    void clear();
    void draw();

    void rect(const glm::vec2 &pos, const glm::vec2 &size, bool fill = false);
    glm::vec2 getLogicPos(const glm::vec2 &mouse_pos);

    Renderer(Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(Renderer &) = delete;
    Renderer &operator=(Renderer &&) = delete;
};

} // namespace cg::engine
