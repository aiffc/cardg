#version 450

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1) uniform infoU {
               vec4 color;
} info;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    float alpha = texture(texSampler, fragTexCoord).r;
    outColor = info.color * alpha;
}
