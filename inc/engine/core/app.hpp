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
    /**
     * @brief
     * set app meta
     *
     * @return bool
     * false: failed
     * true:  success
     */
    bool initAppInfo();
    /**
     * @brief
     * init SDL things, for now just init audio & vedio
     *
     * @return
     * false: failed
     * true:  success
     */
    bool initSDL();

  public:
    App();
    ~App();

    /**
     * @brief
     * init internal handler like time renderer
     * input_manager context scene_manager;
     *
     * @return
     * false: failed
     * true:  success
     */
    [[nodiscard]] bool init();
    /**
     * @brief
     * app quit function
     *
     */
    void deinit();
    /**
     * @brief
     * app render function, call a time ever frame
     *
     * @return
     * false: failed
     * true:  success
     */
    bool render();
    /**
     * @brief
     * app update function, call a time  frame
     *
     * @return
     * false: failed, maybe some error
     * true:  success
     */
    bool update();
    /**
     * @brief
     * app event function
     *
     * @return
     * false: app quit
     * true:  app continue
     */
    bool event(const SDL_Event *);

    /**
     * @brief
     * push a scene to app
     *
     * @param [scene] the scene ready push to application
     */
    void pushScene(std::unique_ptr<Scene> &&scene);
    /**
     * @brief
     * clear current scenes and push a new scene to app
     *
     * @param [scene] the scene ready push to application
     */
    void replaceScene(std::unique_ptr<Scene> &&scene);
    /**
     * @brief
     * remove the top scene from current scene
     */
    void popScene();

    App(App &) = delete;
    App(App &&) = delete;
    App &operator=(App &) = delete;
    App &operator=(App &&) = delete;
};

} // namespace cg::engine
