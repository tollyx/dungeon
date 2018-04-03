#include "Renderer.h"

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/types.h>

#include <fstream>
#include <SDL2/SDL.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>
#include <utility>
#include "Shader.h"

using namespace gl;

const GLfloat rectVertData[]{
  0,0,0, 1,0,0, 0,1,0, 1,1,0
};

glm::mat4 screenVPmat;
double widthmult;
double heightmult;

GLuint LoadShader(const char* path, GLenum shadertype) {
  GLuint shaderId = glCreateShader(shadertype);
  std::string shadersource;
  std::ifstream shaderstream(path, std::ios::in);
  if (shaderstream.is_open()) {
    std::string line;
    while (getline(shaderstream, line)) {
      shadersource += line + "\n";
    }
    shaderstream.close();
  }
  else {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "ERROR: Could not open shader file \"%s\".\n", path);
    getchar();
    return 0;
  }
  
  GLboolean result = GL_FALSE;
  int infologlength = 0;

  SDL_Log("Compiling shader: %s\n", path);
  char const * source = shadersource.c_str();
  glShaderSource(shaderId, 1, &source, NULL);
  glCompileShader(shaderId);

  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infologlength);
  if (infologlength > 0) {
    std::vector<char> errorlog(infologlength + 1);
    glGetShaderInfoLog(shaderId, infologlength, NULL, &errorlog[0]);
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Errors in \"%s\":\n%s\n", path, &errorlog[0]);
  }
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Done.");
  return shaderId;
}

GLuint CreateShaderProgram(GLuint vertshader, GLuint fragshader) {
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linking shader program...\n");
  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertshader);
  glAttachShader(programId, fragshader);
  glLinkProgram(programId);

  GLboolean result = GL_FALSE;
  int infologlength = 0;

  glGetShaderiv(programId, GL_LINK_STATUS, &result);
  glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &infologlength);
  if (infologlength > 0) {
    std::vector<char> errorlog(infologlength + 1);
    glGetShaderInfoLog(programId, infologlength, NULL, &errorlog[0]);
    SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Errors when creating shader program:\n%s\n", &errorlog[0]);
  }

  glDetachShader(programId, vertshader);
  glDetachShader(programId, fragshader);
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Done.");
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
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Renderer destroyed.");
}

Shader mainshader;
Shader wireshader;

GLuint spriteVertArrayId;


void MessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam ) 
{
  const char* sev;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      sev = "HIGH";
    break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      sev = "MEDIUM";
    break;
    case GL_DEBUG_SEVERITY_LOW:
      sev = "LOW";
    break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      sev = "NOTIFY";
    break;
    default:
      sev = "UKNOWN";
  }
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = %s, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, sev, message );
}


bool Renderer::Init(std::string title, int width, int height) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_RENDER,"Failed to create a window!\n");
    return false;
  }
  
  context = SDL_GL_CreateContext(window);
  if (context == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_RENDER,"Failed to create OpenGL context!\n");
    return false;
  }

  glbinding::Binding::initialize();
  glEnable(GL_DEBUG_OUTPUT);
  //glDebugMessageCallback((GLDEBUGPROC)MessageCallback, 0);

  glClearColor(0, 0, 0, 1);

  ImGui_ImplSdlGL3_Init(window);

  glGenVertexArrays(1, &spriteVertArrayId);
  glBindVertexArray(spriteVertArrayId);

  glGenBuffers(1, &spriteVBuf);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertData), rectVertData, GL_STATIC_DRAW);

  mainshader = Shader("assets/main.vert", "assets/main.frag");
  mainshader.print_values();

  wireshader = Shader("assets/wireframe.vert", "assets/wireframe.frag");

  set_window_size(width, height);

  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Renderer initialized.\n");
  return true;
}

void Renderer::set_title(const char *title) {
  SDL_SetWindowTitle(window, title);
}

void Renderer::set_window_size(int width, int height) {
  SDL_SetWindowSize(window, width, height);
  // temp fix for sprites looking ugly af with an odd window resolution
  // but we're doing it after setting the actual window size, 
  // to prevent the game from locking if it was just maximized
  // TODO: Figure this out.
  if (width % 2 == 1) {
    width++;
  }
  if (height % 2 == 1) {
    height++;
  }

  glViewport(0, 0, width, height);

  windowwidth = width;
  windowheight = height;

  widthmult = 1. / windowwidth * 2;
  heightmult = 1. / windowheight * 2;

  glm::mat4 projection = glm::ortho(-1, 1, 1, -1);
  glm::mat4 view = glm::scale(glm::vec3(widthmult, heightmult, 1)) * glm::translate(glm::vec3(-windowwidth / 2, -windowheight / 2, 0));

  screenVPmat = projection * view;
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Window size set to %dx%d.\n", windowwidth, windowheight);
}

void Renderer::set_clear_color(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Renderer::set_clear_color(Color col) {
  glClearColor(col.r, col.g, col.b, col.a);
}

void Renderer::set_vsync_enabled(bool enabled) {
  SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}

bool Renderer::is_vsync_enabled() {
  return SDL_GL_GetSwapInterval() == 1;
}

void Renderer::set_wireframes_enabled(bool enabled) {
  wireframe = enabled;
}

bool Renderer::is_wireframes_enabled() {
  return wireframe;
}

int Renderer::get_renderer_width() {
  return windowwidth;
}

int Renderer::get_renderer_height() {
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
  if (it != textures.end()) return &it->second; // Texture already loaded

  SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Loading texture: %s\n", path.c_str());
  // Texture not loaded, let's load it.
  SDL_Surface* surface = SDL_LoadBMP(path.c_str());
  SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Loaded surface.");
  if (surface != NULL) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    GLenum mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Sending %dx%d texture to GPU...", surface->w, surface->h);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Loaded texture to GPU.");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    Texture t{};
    t.id = textureId;
    t.w = surface->w;
    t.h = surface->h;

    SDL_FreeSurface(surface);
    SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Freed surface.");
    textures.insert(std::pair<std::string, Texture>(path, t));
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Loaded texture \"%s\"\n", path.c_str());
    return &textures[path];
  }
  else {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not load texture \"%s\"\n\t%s\n", path.c_str(), SDL_GetError());
    return nullptr;
  }
}

Sprite Renderer::CreateSprite(std::string path, int x, int y, int w, int h) {
  Sprite sprite;
  sprite.region.x = x;
  sprite.region.y = y;
  sprite.region.w = w;
  sprite.region.h = h;
  sprite.texture = LoadTexture(std::move(path));
  if (sprite.texture != nullptr) {
    auto tw = (float)sprite.texture->w;
    auto th = (float)sprite.texture->h;
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

void Renderer::draw_sprite(Sprite *sprite, Color fg, Color bg, int x, int y, float sx, float sy) {

  glm::mat4 model = glm::translate(glm::vec3(x, y, 0)) * glm::scale(glm::vec3(sprite->region.w * sx, sprite->region.h * sy, 1)) * glm::mat4(1);

  glm::mat4 mvp = screenVPmat * model;
    
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_BLEND);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
  
  mainshader.use();
  mainshader.set_mvp(mvp);
  mainshader.set_color_tint(fg);
  mainshader.set_aux_uniform("colorbackground", bg);
  
  glBindVertexArray(spriteVertArrayId);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, spriteVBuf);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, sprite->uvBuf);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  if (wireframe) {
    wireshader.use();
    wireshader.set_mvp(mvp);
    Color white = Color(1.0f,1.0f,1.0f,1.0f);
    wireshader.set_color_tint(white);
  
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(spriteVertArrayId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, spriteVBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->uvBuf);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

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
