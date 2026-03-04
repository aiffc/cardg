#version 450

layout(binding = 0) uniform sampler2DArray texSampler;
layout(binding = 1) uniform textureIndexObj {
               float index;
} textureIndex;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, vec3(fragTexCoord, textureIndex.index));
}
