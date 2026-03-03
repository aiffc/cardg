#include "test_scene.hpp"
#include "../inc/map.hpp"
#include <memory>
#include <spdlog/spdlog.h>

void TestScene::init(cg::engine::Context &context) {
    cg::engine::Scene::init(context);
}

void TestScene::update(float dt [[maybe_unused]],
                       cg::engine::Context &context [[maybe_unused]]) {}

void TestScene::render(cg::engine::Context &context [[maybe_unused]]) {
    // context.renderer.color(1.0f, 0.0f, 0.0f);
    // context.renderer.rect({10.0f, 10.0f}, {150.0f, 150.0f});
    // m_map->render(context);
}

void TestScene::event(cg::engine::Context &context [[maybe_unused]]) {}
