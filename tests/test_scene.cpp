#include "test_scene.hpp"
#include "../inc/engine/render/backend/manager.hpp"
#include "../inc/map.hpp"
#include <memory>
#include <spdlog/spdlog.h>

static cg::engine::buffer::BaseTextureArrayU globleV{0.0f};

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
        {{-0.2f, -0.2f}, {0.0f, 0.0f}},
        {{0.2f, -0.2f}, {1.0f, 0.0f}},
        {{0.2f, 0.2f}, {1.0f, 1.0f}},
        {{-0.2f, 0.2f}, {0.0f, 1.0f}},
    };
    (*context.renderer)
        .addVertexBuffer<cg::engine::buffer::BaseTexture>(
            cg::engine::backend::PipelineType::BaseTextureArray, vertices);
    std::vector<uint32_t> indexs{0, 1, 2, 2, 3, 0};
    (*context.renderer)
        .addIndexBuffer(cg::engine::backend::PipelineType::BaseTextureArray,
                        indexs);
    // (*context.renderer)
    //     .addTextureArray(cg::engine::backend::PipelineType::BaseTextureArray,
    //                      {"../asset/test.png", "../asset/test2.png"});
    (*context.renderer)
        .addTextureArray(cg::engine::backend::PipelineType::BaseTextureArray,
                         "../asset/test3.png", {2, 2});
}

void TestScene::update(float dt [[maybe_unused]],
                       cg::engine::Context &context [[maybe_unused]]) {
    std::vector<cg::engine::buffer::BaseTextureArrayDU> datas = {
        {{0.2f, -0.2f}}, {{0.7f, 0.7f}}, {{0.4f, 0.4f}}};

    (*context.renderer)
        .mapDynamicUniform(cg::engine::backend::PipelineType::BaseTextureArray,
                           datas);

    (*context.renderer)
        .mapUniform<cg::engine::buffer::BaseTextureArrayU>(
            cg::engine::backend::PipelineType::BaseTextureArray, globleV);
}

void TestScene::render(cg::engine::Context &context [[maybe_unused]]) {
    context.renderer.drawBaseTextureArray();
}

void TestScene::event(cg::engine::Context &context [[maybe_unused]]) {
    if (context.input.isActionPress("select")) {
        globleV.index += 1.0f;
        if (globleV.index > 3.0f) {
            globleV.index = 0.0f;
        }
    }
}
