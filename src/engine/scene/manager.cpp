#include "../../../inc/engine/scene/manager.hpp"
#include "../../../inc/engine/scene/scene.hpp"
#include <spdlog/spdlog.h>

namespace cg::engine {

SceneManager::SceneManager(Context &context) : m_context(context) {}
SceneManager::~SceneManager() = default;

void SceneManager::render() {
    if (!m_scenes.empty()) {
        for (const auto &scene : m_scenes) {
            scene->render(m_context);
        }
    }
}

void SceneManager::update(float dt) {
    if (!m_scenes.empty()) {
        auto &last_scene = m_scenes.back();
        last_scene->update(dt, m_context);
    }
    processPending();
}

void SceneManager::event() {
    if (!m_scenes.empty()) {
        auto &last_scene = m_scenes.back();
        last_scene->event(m_context);
    }
}

void SceneManager::processPending() {
    if (!m_pending && (m_action == PendingActions::Push ||
                       m_action == PendingActions::Replace)) {
        spdlog::warn("process empty scene");
        return;
    }
    switch (m_action) {
    case PendingActions::Pop:
        if (!m_scenes.empty()) {
            m_scenes.pop_back();
        } else {
            spdlog::warn("pop empty scene");
        }
        break;

    case PendingActions::Push:
        if (!m_pending->isInit())
            m_pending->init(m_context);
        spdlog::info("push scene {} done", m_pending->getName());
        m_scenes.push_back(std::move(m_pending));
        break;

    case PendingActions::Replace: {
        spdlog::info("use scene {} replace scene {}", m_pending->getName(),
                     m_scenes.back()->getName());
        m_scenes.clear();
        if (!m_pending->isInit())
            m_pending->init(m_context);

        m_scenes.push_back(std::move(m_pending));
    } break;

    case PendingActions::None:
    default:
        break;
    }
    m_action = PendingActions::None;
}

void SceneManager::push(std::unique_ptr<Scene> &&scene) {
    m_pending = std::move(scene);
    m_action = PendingActions::Push;
}

void SceneManager::replace(std::unique_ptr<Scene> &&scene) {
    m_pending = std::move(scene);
    m_action = PendingActions::Replace;
}

void SceneManager::pop() { m_action = PendingActions::Pop; }

} // namespace cg::engine
