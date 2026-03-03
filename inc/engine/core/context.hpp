#pragma once

#include "../render/renderer.hpp"

namespace cg::engine {

struct Context {
    Renderer &renderer;

    Context(Renderer &r) : renderer(r) {}
    ~Context() = default;
};

} // namespace cg::engine
