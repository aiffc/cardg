#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(binding = 2) uniform objPosU {
               vec2 offset;
} objPos;


layout(location = 0) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(inPosition + objPos.offset, 0.0, 1.0);
    fragTexCoord = inTexCoord;
}
