#version 450

layout(location = 0) in vec2 fUv;
layout(location = 1) in vec4 fSample;
layout(location = 2) flat in vec4 fCol;

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) out vec4 oCol;

void main() {
    vec4 data = texture(tex, fUv * fSample.zw + fSample.xy);

    vec4 col = data * fCol;
    if (col.a == 0) discard;

    oCol = col;
}
