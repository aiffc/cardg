#version 450

layout(std140, binding = 0) uniform MouseObj {
    float mouse_status;
    vec2 window_size;
    vec2 scale;
    vec2 pos;
} mouse;

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 coord;

layout(location = 0) out float mouse_status;
layout(location = 1) out vec2 window_size;
layout(location = 2) out vec2 mouse_scale;
layout(location = 3) out vec2 mouse_pos;
layout(location = 4) out vec2 text_coord;

void main() {
    vec2 center = mouse.window_size * 0.5;
    vec2 dist = mouse.pos - center;
    vec2 mpos = dist/ center;
    gl_Position = vec4(pos + mpos, 0.0, 1.0);

    mouse_status = mouse.mouse_status;
    window_size = mouse.window_size;
    mouse_scale = mouse.scale;
    mouse_pos = mouse.pos;
    text_coord = coord;
}
