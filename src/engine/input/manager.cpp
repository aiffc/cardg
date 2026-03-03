#include "../../../inc/engine/input/manager.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <fmt/format.h>
#include <string>
#include <utility>
#include <vector>

namespace cg::engine {

void InputManager::updateActionState(const std::string &action, bool is_down,
                                     bool repeat) {
    auto it = m_action_status.find(action);
    if (it == m_action_status.end()) {
        return;
    }

    if (is_down) {
        if (repeat)
            it->second = InputActionState::Held;
        else
            it->second = InputActionState::Press;
    } else {
        it->second = InputActionState::Release;
    }
}

void InputManager::update(const SDL_Event &event) {
    for (auto &[action, state] : m_action_status) {
        if (state == InputActionState::Press) {
            state = InputActionState::Held;
        } else if (state == InputActionState::Release) {
            state = InputActionState::None;
        }
    }
    m_resized = false;

    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP: {
        SDL_Scancode keycode = event.key.scancode;
        bool is_down = event.key.down;
        bool is_repeat = event.key.repeat;
        auto it = std::find_if(
            m_key_binding.begin(), m_key_binding.end(),
            [&keycode](
                const std::pair<std::string, std::vector<std::string>> &p) {
                return getScancode(p.first) == keycode;
            });
        if (it != m_key_binding.end()) {
            for (const auto &action : it->second) {
                updateActionState(action, is_down, is_repeat);
            }
        }
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        uint32_t button = event.button.button;
        bool is_down = event.button.down;
        auto it = std::find_if(
            m_key_binding.begin(), m_key_binding.end(),
            [&button](
                const std::pair<std::string, std::vector<std::string>> &p) {
                return getMouseButtonUin32FromString(p.first) == button;
            });
        if (it != m_key_binding.end()) {
            for (const auto &action : it->second) {
                updateActionState(action, is_down, false);
            }
        }
    } break;
    case SDL_EVENT_MOUSE_MOTION:
        m_mouse_pos = glm::vec2{event.motion.x, event.motion.y};
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        m_resized = true;
        break;
    case SDL_EVENT_QUIT:
        m_quit = true;
        break;
    default:
        break;
    }
}

bool InputManager::isActionPress(const std::string &action) const {
    const auto &it = m_action_status.find(action);
    if (it != m_action_status.end()) {
        return it->second == InputActionState::Press ||
               it->second == InputActionState::Held;
    }
    return false;
}
bool InputManager::isActionHeld(const std::string &action) const {
    const auto &it = m_action_status.find(action);
    if (it != m_action_status.end()) {
        return it->second == InputActionState::Held;
    }
    return false;
}
bool InputManager::isActionRelease(const std::string &action) const {
    const auto &it = m_action_status.find(action);
    if (it != m_action_status.end()) {
        return it->second == InputActionState::Release;
    }
    return false;
}

} // namespace cg::engine
