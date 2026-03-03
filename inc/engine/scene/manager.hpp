#pragma once

#include "../core/context.hpp"
#include <vector>

namespace cg::engine {

class Scene;

class SceneManager final {
  private:
    enum class PendingActions {
        None,
        Push,
        Pop,
        Replace,
    };
    Context &m_context;
    std::vector<std::unique_ptr<Scene>> m_scenes;
    std::unique_ptr<Scene> m_pending;
    PendingActions m_action{PendingActions::None};

  private:
    void processPending();

  public:
    SceneManager(Context &context);
    ~SceneManager();

    void render();
    void update(float);
    void event();

    void push(std::unique_ptr<Scene> &&scene);
    void replace(std::unique_ptr<Scene> &&scene);
    void pop();

    SceneManager(SceneManager &) = delete;
    SceneManager(SceneManager &&) = delete;
    SceneManager &operator=(SceneManager &) = delete;
    SceneManager &operator=(SceneManager &&) = delete;
};

} // namespace cg::engine
