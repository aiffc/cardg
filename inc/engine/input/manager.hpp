#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace cg::engine {
enum class InputActionState {
    None,
    Press,
    Held,
    Release,
};
class InputManager final {
  private:
    bool m_quit{false};
    glm::vec2 m_mouse_pos;
    const std::unordered_map<std::string, std::vector<std::string>>
        m_key_binding{{"esc", {"show menu"}},
                      {"mouse left", {"select"}},
                      {"mouse right", {"cancle"}}};

    std::unordered_map<std::string, InputActionState> m_action_status{
        {"show menu", InputActionState::None},
        {"select", InputActionState::None},
        {"cancle", InputActionState::None},
    };
    bool m_resized{false};

  private:
    static inline SDL_Scancode getScancode(std::string_view key) {
        return SDL_GetScancodeFromName(key.data());
    }
    static inline uint32_t getMouseButtonUin32FromString(std::string_view key) {
        if (key == "mouse left")
            return SDL_BUTTON_LEFT;
        else if (key == "mouse right")
            return SDL_BUTTON_RIGHT;
        return 0;
    }
    void updateActionState(const std::string &, bool, bool repeat = false);

  public:
    InputManager() = default;
    ~InputManager() = default;

    bool shouldQuit() const { return m_quit; }
    void setQuit(bool p = true) { m_quit = p; }

    bool resized() const { return m_resized; }

    void update(const SDL_Event &);

    bool isActionPress(const std::string &) const;
    bool isActionHeld(const std::string &) const;
    bool isActionRelease(const std::string &) const;

    const glm::vec2 &getMousePos() const { return m_mouse_pos; }

    InputManager(InputManager &) = delete;
    InputManager(InputManager &&) = delete;
    InputManager &operator=(InputManager &) = delete;
    InputManager &operator=(InputManager &&) = delete;
};

} // namespace cg::engine
