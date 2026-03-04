#include "test_scene.hpp"
#include "../inc/engine/render/backend/manager.hpp"
#include "../inc/map.hpp"
#include <memory>
#include <spdlog/spdlog.h>

void TestScene::init(cg::engine::Context &context) {
    cg::engine::Scene::init(context);
    const std::vector<cg::engine::buffer::Base> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    };
    (*context.renderer).addBasePipelineVertexBuffer(vertices);
}

void TestScene::update(float dt [[maybe_unused]],
                       cg::engine::Context &context [[maybe_unused]]) {}

void TestScene::render(cg::engine::Context &context [[maybe_unused]]) {
    context.renderer.drawBase();
}

void TestScene::event(cg::engine::Context &context [[maybe_unused]]) {}
