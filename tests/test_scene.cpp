#include "test_scene.hpp"
#include "../inc/engine/render/backend/font.hpp"
#include "../inc/engine/render/backend/manager.hpp"
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
        {{-0.2f, -0.2f}, {0.0f, 0.0f}},
        {{0.2f, -0.2f}, {1.0f, 0.0f}},
        {{0.2f, 0.2f}, {1.0f, 1.0f}},
        {{-0.2f, 0.2f}, {0.0f, 1.0f}},
    };
    (*context.renderer)
        .addVertexBuffer<cg::engine::buffer::BaseTexture>(
            cg::engine::backend::PipelineType::Font, vertices);
    std::vector<uint32_t> indexs{0, 1, 2, 2, 3, 0};
    (*context.renderer)
        .addIndexBuffer(cg::engine::backend::PipelineType::Font, indexs);
    // (*context.renderer)
    //     .addTextureArray(cg::engine::backend::PipelineType::Font,
    //                      {"../asset/test.png", "../asset/test2.png"});
    // cg::engine::backend::FontSize size{0, 50};
    // (*context.renderer)
    //     .addCharacters(cg::engine::backend::PipelineType::Font,
    //                    "../asset/fusion-pixel-8px-proportional-zh_hant.ttf",
    //                    size, 'P');
    (*context.renderer)
        .addText(cg::engine::backend::PipelineType::Font,
                 "../asset/fusion-pixel-8px-proportional-zh_hant.ttf",
                 "hello world what a beautiful world, let's do some things");
    // (*context.renderer)
    //     .addText(cg::engine::backend::PipelineType::Font,
    //              "../asset/fusion-pixel-8px-proportional-zh_hant.ttf",
    //              "hello world");
    // (*context.renderer)
    //     .addTextureArray(cg::engine::backend::PipelineType::Font,
    //                      "../asset/test3.png", {2, 2});
}

void TestScene::update(float dt [[maybe_unused]],
                       cg::engine::Context &context [[maybe_unused]]) {
    cg::engine::buffer::FontU color;
    color.color = {1.0f, 0.0f, 0.0f, 1.0f};
    (*context.renderer)
        .mapUniform<cg::engine::buffer::FontU>(
            cg::engine::backend::PipelineType::Font, color);
}

void TestScene::render(cg::engine::Context &context [[maybe_unused]]) {
    (*context.renderer).drawFont();
}

void TestScene::event(cg::engine::Context &context [[maybe_unused]]) {}
