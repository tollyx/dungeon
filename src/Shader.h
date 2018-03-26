#pragma once

#include <string>
#include <map>
#include <fstream>
#include <glbinding/gl/types.h>
#include <glm/common.hpp>
#include "Renderer.h"

class Shader {
  gl::GLuint program_id = 0;
  gl::GLint uniform_color;
  gl::GLint uniform_mvp;
  std::map<std::string, gl::GLint> uniforms;
  gl::GLint get_uniform_location(std::string name);
public:
  Shader(std::ifstream vert, std::ifstream frag);
  Shader(std::string vert, std::string frag);
  void use();
  void set_color_tint(Color col);
  void set_mvp(glm::mat4x4 mvp);
  void set_aux_uniform(std::string name, glm::fvec4 val);
  void set_aux_uniform(std::string name, float val);
  ~Shader();
  void erase();
};

