#version 330 core

uniform vec4 colortint;
uniform sampler2D textureSampler;

in vec2 UV;

out vec4 color;

void main() {
  color = colortint;
}
