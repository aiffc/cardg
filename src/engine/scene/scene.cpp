#include "../../../inc/engine/scene/scene.hpp"
#include <spdlog/spdlog.h>

namespace cg::engine {

void Scene::init(Context &context [[maybe_unused]]) {
    m_init = true;
    spdlog::info("scene {} init done", m_name);
}

} // namespace cg::engine
