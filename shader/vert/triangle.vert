#version 460

vec2 pos[] = vec2[](
    vec2(-0.5f, 0.5f),
    vec2(0.5f, 0.5f),
    vec2(0.0f, -0.5f)
);

void main() {
    gl_Position = vec4(pos[gl_VertexIndex], 0.0f, 1.0f);
}
