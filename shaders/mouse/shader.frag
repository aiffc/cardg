#version 450

layout(binding = 1) uniform sampler2DArray mouse_sampler;

layout(location = 0) in float mouse_status;
layout(location = 1) in vec2 window_size;
layout(location = 2) in vec2 mouse_scale;
layout(location = 3) in vec2 mouse_pos;
layout(location = 4) in vec2 text_coord;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(mouse_sampler, vec3(text_coord, mouse_status));
}
