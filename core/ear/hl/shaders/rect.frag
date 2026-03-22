#version 450

layout(location = 0) flat in vec4 fCol;

layout(location = 0) out vec4 oCol;

void main() {
    if (fCol.a == 0)
        discard;

    oCol = fCol;
}
