#version 450

struct InstData {
    vec2 pos;
    vec2 size;
    vec4 col;
    mat4 transf;
};

const vec2 verts[6] = vec2[6](
        vec2(0,0), vec2(1,0),
        vec2(1,1), vec2(1,1),
        vec2(0,1), vec2(0,0)
    );

layout(std430, binding = 0) buffer ssbo {
    InstData insts[];
};

layout(std140, binding = 1) uniform uni {
    mat4 proj;
};

layout(location = 0) flat out vec4 fCol;

void main() {
    vec2 vert = verts[gl_VertexIndex];
    InstData inst = insts[gl_InstanceIndex];

    gl_Position = proj * inst.transf * vec4(vert * inst.size + inst.pos, 0,1);

    fCol = inst.col;
}
