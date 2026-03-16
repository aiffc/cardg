#pragma once

#include <glm/glm.hpp>

namespace cg::engine {

struct TiledV {
    glm::vec2 pos;
    glm::vec2 coord;
};

struct TiledU {
    glm::vec2 pos;
    glm::vec2 size;
    float texture_index = 0.0f;
    float status_index = 0.0f;
};

} // namespace cg::engine
