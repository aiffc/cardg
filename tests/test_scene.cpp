#include "test_scene.hpp"
#include "../inc/engine/render/backend/font.hpp"
#include "../inc/engine/render/backend/manager.hpp"
#include <spdlog/spdlog.h>

void TestScene::init(cg::engine::Context &context) {
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

    (*context.renderer)
        .addText(cg::engine::backend::PipelineType::Font,
                 "../asset/fusion-pixel-8px-proportional-zh_hant.ttf",
                 "hello world what a beautiful world, let's do some things");

    const std::vector<cg::engine::buffer::MouseVertex> mouse_vertex = {
        {{-0.05f, -0.05f}, {0.0f, 0.0f}},
        {{0.05f, -0.05f}, {1.0f, 0.0f}},
        {{0.05f, 0.05f}, {1.0f, 1.0f}},
        {{-0.05f, 0.05f}, {0.0f, 1.0f}},
    };
    (*context.renderer)
        .addVertexBuffer<cg::engine::buffer::MouseVertex>(
            cg::engine::backend::PipelineType::Mouse, mouse_vertex);
    (*context.renderer)
        .addIndexBuffer(cg::engine::backend::PipelineType::Mouse, indexs);
    (*context.renderer)
        .addTextureArray(cg::engine::backend::PipelineType::Mouse,
                         {
                             "../asset/mouse/1 cursor.png",
                             "../asset/mouse/2 move.png",
                             "../asset/mouse/3 pencil.png",
                             "../asset/mouse/4 eraser.png",
                             "../asset/mouse/5 color picker.png",
                             "../asset/mouse/6 cancel.png",
                         },
                         1);
}

void TestScene::update(float dt [[maybe_unused]],
                       cg::engine::Context &context [[maybe_unused]]) {
    cg::engine::buffer::FontU color;
    color.color = {1.0f, 0.0f, 0.0f, 1.0f};
    (*context.renderer)
        .mapUniform<cg::engine::buffer::FontU>(
            cg::engine::backend::PipelineType::Font, color);
    cg::engine::buffer::CommonUniform common;
    common.mouse_status = 0.0f;
    common.window_size = context.renderer.windowSizef();
    common.mouse_pos = context.input.getMousePos();
    common.mouse_scale = {1.0f, 1.0f};
    // glm::vec2 center = common.window_size / 2.0f;
    // common.mouse_pos = (common.mouse_pos - center) / center;
    // spdlog::info("{} {}", v.x, v.y);
    // spdlog::info("{} {} {} {}", common.window_size.x, common.window_size.y,
    //              common.mouse_pos.x, common.mouse_pos.y);
    (*context.renderer)
        .mapUniform<cg::engine::buffer::CommonUniform>(
            cg::engine::backend::PipelineType::Mouse, common);
}

void TestScene::render(cg::engine::Context &context [[maybe_unused]]) {
    (*context.renderer).drawFont();
    (*context.renderer).drawMouse();
}

void TestScene::event(cg::engine::Context &context [[maybe_unused]]) {}
