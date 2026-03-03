#include "../inc/map.hpp"
#include "../inc/engine/core/context.hpp"
#include "../inc/engine/render/renderer.hpp"

namespace cg {

void Map::layout(const glm::ivec2 &render_size) {
    m_size = glm::vec2{
        (float)render_size.x * 0.8f,
        (float)render_size.y * 0.8f,
    };
    m_pos = glm::vec2{(float)render_size.x * 0.2f, 0.0f};
}

Map::Map(const glm::ivec2 &render_size) { layout(render_size); }

Map::~Map() = default;

void Map::init() {
    // TODO
}

void Map::render(cg::engine::Context &context) {
    context.renderer.color(0.0f, 1.0f, 0.0f);
    context.renderer.rect(m_pos, m_size);
}

} // namespace cg
