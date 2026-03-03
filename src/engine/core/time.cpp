#include "../../../inc/engine/core/time.hpp"
#include "SDL3/SDL_timer.h"
#include "spdlog/spdlog.h"
#include <cstdint>

namespace cg::engine {

Time::Time(uint32_t fps) : m_fps{fps} {
    if (fps == 0) {
        spdlog::trace("init time manager");
        m_frame_interval = 0.0;
    } else {
        spdlog::trace("time manager set fps {}", fps);
        m_frame_interval = 1.0 / fps;
    }
}
Time::~Time() = default;

void Time::init() {
    spdlog::trace("time manager init done");
    m_last_frame_time = SDL_GetTicksNS();
    m_start_frame_time = m_last_frame_time;
}
void Time::deinit() { spdlog::trace("time manager quit"); }

void Time::limit(uint64_t l2s_interval) {

    uint64_t interval = static_cast<uint64_t>(m_frame_interval * 1000000000);
    if (l2s_interval < interval) {
        uint64_t delay_time = interval - l2s_interval;
        SDL_DelayNS(delay_time);
        m_delta_time = (SDL_GetTicksNS() - m_last_frame_time) / 1000000000.0;
    }
}

void Time::setfps(uint32_t fps) {
    m_fps = fps;
    if (fps == 0) {
        spdlog::trace("time manager set fps no limit");
        m_frame_interval = 0.0;
    } else {
        spdlog::trace("time manager set fps {}", fps);
        m_frame_interval = 1.0 / fps;
    }
}

void Time::update() {
    m_start_frame_time = SDL_GetTicksNS();
    uint64_t last_to_start_interval = m_start_frame_time - m_last_frame_time;
    if (m_frame_interval > 0.0) {
        limit(last_to_start_interval);
    } else {
        m_delta_time = last_to_start_interval / 1000000000.0;
    }
    m_last_frame_time = SDL_GetTicksNS();
}

} // namespace cg::engine
