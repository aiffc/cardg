#pragma once

#include "SDL3/SDL_events.h"
#include <memory>

namespace cg::engine {

class Time;
class Renderer;
struct Context;

class App final {
  private:
    std::unique_ptr<Time> m_time;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Context> m_context;

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

    App(App &) = delete;
    App(App &&) = delete;
    App &operator=(App &) = delete;
    App &operator=(App &&) = delete;
};

} // namespace cg::engine
