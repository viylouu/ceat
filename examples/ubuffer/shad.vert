#version 450

layout(std140, binding = 0) uniform ubuf {
    float time;
} ubo;

const vec2 verts[3] = vec2[](
    vec2(0,-.5), vec2(.5,.5), vec2(-.5,.5)
    );

void main() {
    vec2 pos = verts[gl_VertexID];
    gl_Position = vec4(pos + sin(ubo.time + pos.x + pos.y) * .25, 0,1);
}
