#include "Renderer.h"

#define GLEW_STATIC
#include "glew.h"

#include <fstream>
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>

const GLfloat rectVertData[]{
  0,0,0, 1,0,0, 0,1,0, 1,1,0
};

glm::mat4 screenVPmat;
float widthmult;
float heightmult;

GLuint LoadShader(const char* path, GLenum shadertype) {
  GLuint shaderId = glCreateShader(shadertype);
  std::string shadersource;
  std::ifstream shaderstream(path, std::ios::in);
  if (shaderstream.is_open()) {
    std::string line = "";
    while (getline(shaderstream, line)) {
      shadersource += line + "\n";
    }
    shaderstream.close();
  }
  else {
    printf("ERROR: Could not open shader file \"%s\".\n", path);
    getchar();
    return 0;
  }
  
  GLint result = GL_FALSE;
  int infologlength;

  printf("Compiling shader: %s\n", path);
  char const * source = shadersource.c_str();
  glShaderSource(shaderId, 1, &source, NULL);
  glCompileShader(shaderId);

  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infologlength);
  if (infologlength > 0) {
    std::vector<char> errorlog(infologlength + 1);
    glGetShaderInfoLog(shaderId, infologlength, NULL, &errorlog[0]);
    printf("Errors in \"%s\":\n%s\n", path, &errorlog[0]);
  }
  return shaderId;
}

GLuint CreateShaderProgram(GLuint vertshader, GLuint fragshader) {
  printf("Linking shader program...\n");
  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertshader);
  glAttachShader(programId, fragshader);
  glLinkProgram(programId);

  GLint result = GL_FALSE;
  int infologlength;

  glGetShaderiv(programId, GL_LINK_STATUS, &result);
  glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &infologlength);
  if (infologlength > 0) {
    std::vector<char> errorlog(infologlength + 1);
    glGetShaderInfoLog(programId, infologlength, NULL, &errorlog[0]);
    printf("Errors when creating shader program:\n%s\n", &errorlog[0]);
  }

  glDetachShader(programId, vertshader);
  glDetachShader(programId, fragshader);

  return programId;
}

Renderer::Renderer() {
  window = nullptr;
}


Renderer::~Renderer() {
  ImGui_ImplSdlGL3_Shutdown();

  sprites.clear();
  auto it = textures.begin();
  while (it != textures.end()) {
    glDeleteTextures(1, &it->second.id);
    it++;
  }
  textures.clear();

  glDeleteBuffers(1, &spriteVBuf);

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  window = nullptr;
}

GLuint shaderProg;
GLuint wireShaderProg;
GLuint spriteVertArrayId;
GLuint colorUniform;
GLuint mvpUniform;

bool Renderer::Init(std::string title, int width, int height) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
  if (window == NULL) {
    return false;
  }
  
  context = SDL_GL_CreateContext(window);
  if (context == NULL) {
    return false;
  }

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "failed to initialize OpenGl: %s\n", glewGetErrorString(err));
    return false;
  }

  if (!GLEW_VERSION_3_2) {
    fprintf(stderr, "OpenGl 3.2 is not supported, please try updating your drivers!\n");
    return false;
  }

  glClearColor(0, 0, 0, 1);

  ImGui_ImplSdlGL3_Init(window);

  glGenVertexArrays(1, &spriteVertArrayId);
  glBindVertexArray(spriteVertArrayId);

  glGenBuffers(1, &spriteVBuf);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertData), rectVertData, GL_STATIC_DRAW);

  GLuint frag = LoadShader("assets/main.frag", GL_FRAGMENT_SHADER);
  GLuint vert = LoadShader("assets/main.vert", GL_VERTEX_SHADER);
  shaderProg = CreateShaderProgram(vert, frag);
  GLuint wirefrag = LoadShader("assets/wireframe.frag", GL_FRAGMENT_SHADER);
  GLuint wirevert = LoadShader("assets/wireframe.vert", GL_VERTEX_SHADER);
  wireShaderProg = CreateShaderProgram(wirevert, wirefrag);
  glDeleteShader(frag);
  glDeleteShader(vert);
  glDeleteShader(wirefrag);
  glDeleteShader(wirevert);

  glUseProgram(shaderProg);

  colorUniform = glGetUniformLocation(shaderProg, "colortint");
  mvpUniform = glGetUniformLocation(shaderProg, "MVP");

  SetWindowSize(width, height);

  return true;
}

void Renderer::SetColor(float r, float g, float b, float a) {
  currentcolor = { r, g, b, a };
}

void Renderer::SetColor(Color col) {
  SetColor(col.r, col.g, col.b, col.a);
}

void Renderer::SetTitle(const char * title) {
  SDL_SetWindowTitle(window, title);
}

void Renderer::SetWindowSize(int width, int height) {
  SDL_SetWindowSize(window, width, height);

  windowwidth = width;
  windowheight = height;

  widthmult = 1.f / windowwidth * 2;
  heightmult = 1.f / windowheight * 2;

  glm::mat4 projection = glm::ortho(-1, 1, 1, -1);
  glm::mat4 view = glm::scale(glm::vec3(widthmult, heightmult, 1)) * glm::translate(glm::vec3(-windowwidth / 2, -windowheight / 2, 0)) * glm::mat4(1);

  screenVPmat = projection * view;
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Renderer::SetClearColor(Color col) {
  glClearColor(col.r, col.g, col.b, col.a);
}

void Renderer::SetVSyncEnabled(bool enabled) {
  SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}

bool Renderer::IsVSyncEnabled() {
  return SDL_GL_GetSwapInterval() == 1;
}

void Renderer::SetWireframesEnabled(bool enabled) {
  wireframe = enabled;
}

bool Renderer::IsWireframesEnabled() {
  return wireframe;
}

int Renderer::GetRendererWidth() {
  return windowwidth;
}

int Renderer::GetRendererHeight() {
  return windowheight;
}

bool Renderer::ImguiProcessEvents(SDL_Event *e) {
  return ImGui_ImplSdlGL3_ProcessEvent(e);
}
void Renderer::ImguiNewFrame() {
  ImGui_ImplSdlGL3_NewFrame(window);
}
Texture * Renderer::LoadTexture(std::string path) {
  auto it = textures.find(path);
  if (it == textures.end()) {
    // Texture not loaded, let's load it.
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (surface != NULL) {      
      GLuint textureId;
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);

      int mode = GL_RGB;
      if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
      }

      glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      Texture t;
      t.id = textureId;
      t.w = surface->w;
      t.h = surface->h;

      SDL_FreeSurface(surface);
      textures.insert(std::pair<std::string, Texture>(path, t));
      return &textures[path];
    }
    else {
      SDL_Log("Error: Could not load texture \"%s\"\n\t%s\n", path.c_str(), SDL_GetError());
      return nullptr;
    }
  }
  else {
    // Texture already loaded, no need to load it again
    return &it->second;
  }
}


Sprite Renderer::CreateSprite(std::string path, int x, int y, int w, int h) {
  Sprite sprite;
  sprite.region.x = x;
  sprite.region.y = y;
  sprite.region.w = w;
  sprite.region.h = h;
  sprite.texture = LoadTexture(path);
  if (sprite.texture != nullptr) {
    float tw = (float)sprite.texture->w;
    float th = (float)sprite.texture->h;
    const float uvs[] = {
      x / tw, y / th,
      (x + w) / tw, y / th,
      x / tw, (y + h) / th,
      (x + w) / tw, (y + h) / th,
    };

    
    glCreateBuffers(1, &sprite.uvBuf);
    glBindBuffer(GL_ARRAY_BUFFER, sprite.uvBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
  }
  return sprite;
}


void Renderer::DrawSprite(Sprite * sprite, int x, int y, float sx, float sy) {

  glm::mat4 model = glm::translate(glm::vec3(x, y, 0)) * glm::scale(glm::vec3(sprite->region.w * sx, sprite->region.h * sy, 1)) * glm::mat4(1);

  glm::mat4 mvp = screenVPmat * model;

    
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_BLEND);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    
  glUseProgram(shaderProg);
  glUniform4f(colorUniform, currentcolor.r, currentcolor.g, currentcolor.b, currentcolor.a);

  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]);
  
  glBindVertexArray(spriteVertArrayId);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVBuf);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, sprite->uvBuf);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(wireShaderProg);
    glUniform4f(colorUniform, 1, 1, 1, 1);
    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(spriteVertArrayId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, spriteVBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->uvBuf);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  
}

void Renderer::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Present() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  ImGui::Render();
  SDL_GL_SwapWindow(window);
}
