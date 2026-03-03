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

    SDL_SetRenderLogicalPresentation(renderer, 640, 360,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    m_window = std::unique_ptr<SDL_Window, WindowDeleter>(window);
    m_renderer = std::unique_ptr<SDL_Renderer, RendererDeleter>(renderer);
    return true;
}

void Renderer::color(float r, float g, float b, float a) {
    if (m_renderer) {
        SDL_SetRenderDrawColorFloat(m_renderer.get(), r, g, b, a);
    }
}

void Renderer::clear() {
    if (m_renderer) {
        SDL_RenderClear(m_renderer.get());
    }
}

void Renderer::draw() {
    if (m_renderer) {
        SDL_RenderPresent(m_renderer.get());
    }
}

void Renderer::rect(const glm::vec2 &pos, const glm::vec2 &size, bool fill) {
    if (m_renderer) {
        SDL_FRect rect{
            .x = pos.x,
            .y = pos.y,
            .w = size.x,
            .h = size.y,

        };
        if (fill) {
            SDL_RenderFillRect(m_renderer.get(), &rect);
        } else {
            SDL_RenderRect(m_renderer.get(), &rect);
        }
    }
}

void Renderer::logicalPresentation(const glm::ivec2 &v) {
    if (m_renderer) {
        SDL_SetRenderLogicalPresentation(m_renderer.get(), v.x, v.y,
                                         SDL_LOGICAL_PRESENTATION_LETTERBOX);
    }
}
glm::ivec2 Renderer::logicalPresentation() const {
    glm::ivec2 ret{0, 0};
    SDL_RendererLogicalPresentation mode = SDL_LOGICAL_PRESENTATION_LETTERBOX;
    if (m_renderer) {
        SDL_GetRenderLogicalPresentation(m_renderer.get(), &ret.x, &ret.y,
                                         &mode);
    }
    return ret;
}

glm::vec2 Renderer::getLogicPos(const glm::vec2 &mouse_pos) {
    glm::ivec2 logic_size = logicalPresentation();
    glm::vec2 scale{
        static_cast<float>(logic_size.x) / static_cast<float>(m_size.x),
        static_cast<float>(logic_size.y) / static_cast<float>(m_size.y),
    };
    return mouse_pos * scale;
}

void Renderer::update() {
    if (m_window) {
        spdlog::info("update window size");
        SDL_GetWindowSize(m_window.get(), &m_size.x, &m_size.y);
    }
}
} // namespace cg::engine
