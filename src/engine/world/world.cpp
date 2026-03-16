#include "../../../inc/engine/world/world.hpp"

namespace cg::engine {

World::World(const glm::vec2 &world_size, const glm::vec2 &scale)
    : m_size(world_size), m_scale(scale) {

    const glm::vec2 grid_base_size{100.0f, 100.0f};
    glm::vec2 grid_scale = (world_size * scale) / grid_base_size;

    m_grids.resize(1000);
    for (int32_t y = 0; y < m_max.y; ++y) {
        for (int32_t x = 0; x < m_max.x; ++x) {
            int32_t index = y * m_max.x + x;
            m_grids[index].pos = glm::vec2{x * grid_scale.x, y * grid_scale.y};
            m_grids[index].size = grid_scale;
        }
    }
}
World::~World() = default;

} // namespace cg::engine
