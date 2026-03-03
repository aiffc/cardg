#include "../../../inc/engine/render/renderer.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>

namespace cg::engine {

Renderer::Renderer(const glm::ivec2 &size) : m_size(size) {}
Renderer::~Renderer() = default;

bool Renderer::init() {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    SDL_CreateWindowAndRenderer("card game", m_size.x, m_size.y,
                                SDL_WINDOW_RESIZABLE, &window, &renderer);
    if (window == nullptr || renderer == nullptr) {
        spdlog::error("failed to create window and renderer {} ",
                      SDL_GetError());
        return false;
    }
    m_window = std::unique_ptr<SDL_Window, WindowDeleter>(window);
    m_renderer = std::unique_ptr<SDL_Renderer, RendererDeleter>(renderer);
    return true;
}

} // namespace cg::engine
