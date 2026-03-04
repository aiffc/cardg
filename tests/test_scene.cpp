#include "test_scene.hpp"
#include "../inc/engine/render/backend/manager.hpp"
#include "../inc/map.hpp"
#include <memory>
#include <spdlog/spdlog.h>

void TestScene::init(cg::engine::Context &context) {
    // cg::engine::Scene::init(context);
    // const std::vector<cg::engine::buffer::Base> vertices = {
    //     {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    //     {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    //     {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    //     {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
    // };
    // (*context.renderer).addBasePipelineVertexBuffer(vertices);
    // std::vector<uint32_t> indexs{0, 1, 2, 2, 3, 0};
    // (*context.renderer).addBasePipelineIndexBuffer(indexs);

    const std::vector<cg::engine::buffer::BaseTexture> vertices = {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f}, {0.0f, 1.0f}},
    };
    (*context.renderer).addBaseTexturePipelineVertexBuffer(vertices);
    std::vector<uint32_t> indexs{0, 1, 2, 2, 3, 0};
    (*context.renderer).addBaseTexturePipelineIndexBuffer(indexs);
    (*context.renderer).addBaseTextureTexture("../asset/test.png");
}

void TestScene::update(float dt [[maybe_unused]],
                       cg::engine::Context &context [[maybe_unused]]) {}

void TestScene::render(cg::engine::Context &context [[maybe_unused]]) {
    context.renderer.drawBaseTexture();
}

void TestScene::event(cg::engine::Context &context [[maybe_unused]]) {}
