#include "../../../inc/engine/core/app.hpp"
#include "../../../inc/engine/core/context.hpp"
#include "../../../inc/engine/core/time.hpp"
#include "../../../inc/engine/input/manager.hpp"
#include "../../../inc/engine/render/renderer.hpp"
#include "../../../inc/engine/scene/manager.hpp"
#include "../../../inc/engine/scene/scene.hpp"
#include "spdlog/spdlog.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace cg::engine {

App::App() = default;
App::~App() = default;

bool App::initAppInfo() {
    if (!SDL_SetAppMetadata("trial", "0.1", "card game")) {
        spdlog::error("failed to set app meta{}", SDL_GetError());
        return false;
    }
    std::unordered_map<std::string, std::string> meta_data{
        {SDL_PROP_APP_METADATA_URL_STRING, "None"},
        {SDL_PROP_APP_METADATA_CREATOR_STRING, "AIF TEAM"},
        {SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "AIF TEAM"},
        {SDL_PROP_APP_METADATA_TYPE_STRING, "TRIAL"}};
    for (const auto &[key, val] : meta_data) {
        if (!SDL_SetAppMetadataProperty(key.data(), val.data())) {
            spdlog::error("failed to set app meta data {}", SDL_GetError());
            return false;
        }
    }
    return true;
}

bool App::initSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
        spdlog::error("failed to init sdl {}", SDL_GetError());
        return false;
    }
    return true;
}

bool App::init() {
    if (!initAppInfo()) {
        return false;
    }
    if (!initSDL()) {
        return false;
    }

    m_time = std::make_unique<Time>(144);
    m_time->init();

    m_renderer = std::make_unique<Renderer>(glm::ivec2{1280, 720});
    if (!m_renderer->init()) {
        spdlog::error("failed to create vulkan render");
        return false;
    }

    m_input_manager = std::make_unique<InputManager>();

    m_context = std::make_unique<Context>(*m_renderer, *m_input_manager);
    m_scene_manager = std::make_unique<SceneManager>(*m_context);

    return true;
}

void App::deinit() {
    // TODO save scene data
    m_scene_manager.reset();
    m_input_manager.reset();
    m_renderer.reset();
    m_context.reset();
    m_time->deinit();
    SDL_Quit();
}

bool App::render() {
    if (m_renderer->begin()) {
        m_scene_manager->render();
        m_renderer->end();
    }

    return true;
}

bool App::update() {
    m_time->update();
    float dt = m_time->getDeltaTime();
    m_scene_manager->update(dt);
    return true;
}

bool App::event(const SDL_Event *event) {
    m_input_manager->update(*event);
    if (m_input_manager->shouldQuit()) {
        return false;
    }

    m_scene_manager->event();
    return true;
}

void App::pushScene(std::unique_ptr<Scene> &&scene) {
    m_scene_manager->push(std::move(scene));
}
void App::replaceScene(std::unique_ptr<Scene> &&scene) {
    m_scene_manager->replace(std::move(scene));
}
void App::popScene() { m_scene_manager->pop(); }
} // namespace cg::engine
