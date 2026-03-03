#pragma once

#include "SDL3/SDL_events.h"
#include <memory>

namespace cg::engine {

class Time;
class Renderer;
class InputManager;
struct Context;
class Scene;
class SceneManager;

class App final {
  private:
    std::unique_ptr<Time> m_time;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<InputManager> m_input_manager;
    std::unique_ptr<Context> m_context;
    std::unique_ptr<SceneManager> m_scene_manager;

  private:
    bool initAppInfo();
    bool initSDL();

  public:
    App();
    ~App();

    [[nodiscard]] bool init();
    void deinit();
    bool render();
    bool update();
    bool event(const SDL_Event *);

    void pushScene(std::unique_ptr<Scene> &&scene);
    void replaceScene(std::unique_ptr<Scene> &&scene);
    void popScene();

    App(App &) = delete;
    App(App &&) = delete;
    App &operator=(App &) = delete;
    App &operator=(App &&) = delete;
};

} // namespace cg::engine
