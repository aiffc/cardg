#pragma once

#include "character.hpp"
#include "engine/core/context.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace cg {

class Map {
  private:
    glm::vec2 m_pos;
    glm::ivec2 m_size;
    std::vector<std::unique_ptr<Character>> m_player_objs;
    std::vector<std::unique_ptr<Character>> m_monster_objs;

  private:
    void layout(const glm::ivec2 &render_size);

  public:
    Map(const glm::ivec2 &render_size);
    ~Map();

    void init();
    void render(cg::engine::Context &context);

    Map(Map &) = delete;
    Map(Map &&) = delete;
    Map &operator=(Map &) = delete;
    Map &operator=(Map &&) = delete;
};

} // namespace cg
