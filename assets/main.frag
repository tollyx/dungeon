#version 330 core

uniform vec4 colortint;
uniform sampler2D textureSampler;

in vec2 UV;

out vec4 color;

void main() {
  vec4 col = texture(textureSampler, UV);
  if (col.rgb == vec3(1.0, 0.0, 1.0))
    discard;
  color = col * colortint;
}
