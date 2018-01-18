#include "Shader.h"
#include <SDL2/SDL_log.h>
#include <glbinding/gl/gl.h>
#include <glm/mat4x4.hpp>
#include <fstream>
#include <string>
#include <map>

using namespace gl;

gl::GLint Shader::get_uniform_location(std::string name) {
  auto it = uniforms.find(name);
  if (it == uniforms.end()) {
    GLint loc = glGetUniformLocation(program_id, name.c_str());
    uniforms.insert(std::pair<std::string, GLint>(name, loc));
    if (loc < 0){
    }
    return loc;
  }
  return it->second;
}

void Shader::init_streams(std::ifstream vert, std::ifstream frag) {
  std::string vsource;
  if (vert.is_open()) {
    std::string line;
    while (getline(vert, line)) {
      vsource += line + "\n";
    }
    vert.close();
  }
  else {
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Could not load vert shader!");
  }

  std::string fsource;
  if (frag.is_open()) {
    std::string line;
    while (getline(frag, line)) {
      fsource += line + "\n";
    }
    frag.close();
  }
  else {
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Could not load frag shader!");
  }

  init_source(vsource, fsource);
}

void Shader::init_source(std::string vertsource, std::string fragsource) {
  vert_id = glCreateShader(GL_VERTEX_SHADER);
  frag_id = glCreateShader(GL_FRAGMENT_SHADER);

  char const * vs = vertsource.c_str();
  glShaderSource(vert_id, 1, &vs, nullptr);
  glCompileShader(vert_id);

  char const * fs = fragsource.c_str();
  glShaderSource(frag_id, 1, &fs, nullptr);
  glCompileShader(frag_id);

  GLboolean result = GL_FALSE;
  int infologlength = 0;
  glGetShaderiv(vert_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vert_id, GL_INFO_LOG_LENGTH, &infologlength);
  if (infologlength > 0) {
    std::string errorlog(infologlength + 1, ' ');
    glGetShaderInfoLog(vert_id, infologlength, NULL, &errorlog[0]);
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Errors compiling vertex shader:\n%s\n", &errorlog[0]);
  }

  result = GL_FALSE;
  infologlength = 0;
  glGetShaderiv(frag_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(frag_id, GL_INFO_LOG_LENGTH, &infologlength);
  if (infologlength > 0) {
    std::string errorlog(infologlength + 1, ' ');
    glGetShaderInfoLog(frag_id, infologlength, NULL, &errorlog[0]);
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Errors compiling frag shader:\n%s\n", &errorlog[0]);
  }

  program_id = glCreateProgram();
  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, frag_id);
  glLinkProgram(program_id);

  result = GL_FALSE;
  infologlength = 0;
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &infologlength);
  if (infologlength > 0) {
    std::string errorlog(infologlength + 1, ' ');
    glGetProgramInfoLog(program_id, infologlength, NULL, &errorlog[0]);
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Errors when linking shader program:\n%s\n", &errorlog[0]);
  }

  glDetachShader(program_id, vert_id);
  glDetachShader(program_id, frag_id);
  // glDeleteShader(vert_id);
  // glDeleteShader(frag_id);

  uniform_color = glGetUniformLocation(program_id, "colortint");
  if (uniform_color < 0){
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "No shader color?????\n");
  }
  uniform_mvp = glGetUniformLocation(program_id, "MVP");
  if (uniform_mvp < 0){
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "No shader mvp?????\n");
  }
  print_values();
}

Shader::Shader() {}

Shader::Shader(const char* vertpath, const char* fragpath) {
  SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Creating shader with '%s' and '%s'\n", vertpath, fragpath);
  init_streams(std::ifstream(vertpath), std::ifstream(fragpath));
}

Shader::Shader(std::string vertsource, std::string fragsource) {
  init_source(vertsource, fragsource);
}

void Shader::use() { glUseProgram(program_id); }

void Shader::set_color_tint(Color &col) { glUniform4f(uniform_color, col.r, col.g, col.b, col.a); }

void Shader::set_mvp(glm::mat4x4 &mvp) { glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, &mvp[0][0]); }

void Shader::set_aux_uniform(std::string name, Color &val) { glUniform4f(get_uniform_location(name), val.r, val.g, val.b, val.a); }

void Shader::set_aux_uniform(std::string name, glm::fvec4 &val) { glUniform4f(get_uniform_location(name), val.r, val.g, val.b, val.a); }

void Shader::set_aux_uniform(std::string name, float &val) { 
  glUniform1f(get_uniform_location(name), val);
}

void Shader::erase() {
  if (frag_id != 0)
  {
     glDeleteShader(frag_id);
     frag_id = 0;
  }
  if (vert_id != 0)
  {
     glDeleteShader(vert_id);
     vert_id = 0;
  }
  if (program_id != 0)
  {
    glDeleteProgram(program_id);
    program_id = 0;
  }
}

void Shader::print_values() {
  fprintf(stderr, "program_id: %i\nvert_id: %i\nfrag_id: %i\nuniform_mvp: %i\nuniform_color: %i\n", program_id, vert_id, frag_id, uniform_mvp, uniform_color);
}
