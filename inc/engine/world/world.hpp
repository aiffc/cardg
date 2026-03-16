#pragma once

#include "tile.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace cg::engine {

class World final {
  private:
    const glm::ivec2 m_max{100, 100};
    glm::vec2 m_size;
    glm::vec2 m_scale;
    std::vector<TiledU> m_grids;

  public:
    World(const glm::vec2 &world_size, const glm::vec2 &scale);
    ~World();

    World(World &) = delete;
    World(World &&) = delete;
    World &operator=(World &) = delete;
    World &operator=(World &&) = delete;
};

} // namespace cg::engine
