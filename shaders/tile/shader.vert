#version 450

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec2 in_coord;

layout(binding = 1) uniform TileU {
               vec2 pos;
               vec2 size;
               float tetxture;
               float status;
} tile;


layout(location = 0) out vec2 out_coord;
layout(location = 1) out float texture;
layout(location = 2) out float status;

void main() {
    gl_Position = vec4(in_pos * tile.size + tile.pos, 0.0, 1.0);
}
