#version 450

layout(binding = 0) uniform sampler2DArray texture_sampler;

layout(location = 0) in vec2 in_coord;
layout(location = 1) in float texture_index;
layout(location = 2) in float status_index;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(texture_sampler, vec3(in_coord, texture_index));
    if (status_index != 0.0) {
        out_color = mix(out_color, texture(texture_sampler, vec3(in_coord, status_index)), 1);
    }
}
