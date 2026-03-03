#pragma once

#include "../input/manager.hpp"
#include "../render/renderer.hpp"

namespace cg::engine {

struct Context {
    Renderer &renderer;
    InputManager &input;

    Context(Renderer &r, InputManager &input_manager)
        : renderer(r), input(input_manager) {}
    ~Context() = default;
};

} // namespace cg::engine
