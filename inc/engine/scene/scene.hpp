#pragma once

#include "../core/context.hpp"
#include <string_view>

namespace cg::engine {

class Scene {
  protected:
    std::string m_name;
    bool m_init{false};

  private:
    void processPending();

  public:
    Scene(std::string_view name) : m_name{name} {}
    virtual ~Scene() = default;

    virtual void init(Context &context);
    virtual void update(float, Context &context) = 0;
    virtual void render(Context &context) = 0;
    virtual void event(Context &context) = 0;

    bool isInit() const { return m_init; }
    const std::string &getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    Scene(Scene &) = delete;
    Scene(Scene &&) = delete;
    Scene &operator=(Scene &) = delete;
    Scene &operator=(Scene &&) = delete;
};

} // namespace cg::engine
