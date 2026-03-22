#version 330 core

const vec2 verts[3] = vec2[](
        vec2(0,-.5), vec2(.5,.5), vec2(-.5,.5)
    );

void main() {
    gl_Position = vec4(verts[gl_VertexIndex], 0,1);
}
