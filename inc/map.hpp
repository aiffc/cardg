#pragma once

#include "character.hpp"
#include "glm/glm.hpp"
#include <vector>

namespace cg {

class Map {
  private:
    glm::vec2 m_pos;
    glm::ivec2 m_size;
    std::vector<Character> m_player_objs;
    std::vector<Character> m_monster_objs;

  public:
    Map(const glm::vec2 &m_pos, const glm::ivec2 &size);
    ~Map();

    Map(Map &) = delete;
    Map(Map &&) = delete;
    Map &operator=(Map &) = delete;
    Map &operator=(Map &&) = delete;
};

} // namespace cg
