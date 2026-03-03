#pragma once

#include "../inc/engine/scene/scene.hpp"
#include <memory>

namespace cg {
class Map;

}

class TestScene final : public cg::engine::Scene {
  private:
    std::unique_ptr<cg::Map> m_map;

  public:
    using cg::engine::Scene::Scene;
    ~TestScene() override = default;

    void init(cg::engine::Context &context) override;
    void update(float dt, cg::engine::Context &context) override;
    void render(cg::engine::Context &context) override;
    void event(cg::engine::Context &context) override;
};
