#pragma once
#include <vector>
#include <map>
#include <string>
#include "Color.h"

struct SDL_Window;
struct SDL_Texture;
union SDL_Event;
typedef void *SDL_GLContext;

struct Rect {
  int x = 0, y = 0, w = 0, h = 0;
};

struct Texture {
  unsigned int id;
  int w, h;
};

struct Sprite {
  Texture * texture;
  Rect region;
  unsigned int uvBuf;
};

class Renderer 
{
  SDL_Window* window;
  SDL_GLContext context;
  Color currentcolor;
  unsigned int spriteVBuf;
  std::vector<Sprite> sprites;
  std::map<std::string, Texture> textures;
  int windowheight;
  int windowwidth;
  bool wireframe;
public:
  Renderer();
  ~Renderer();
  bool Init(std::string title, int width, int height);
  void set_color(float r, float g, float b, float a);
  void set_color(Color col);
  void set_title(const char *title);
  void set_window_size(int width, int height);
  void set_clear_color(float r, float g, float b, float a);
  void set_clear_color(Color col);
  void set_vsync_enabled(bool enabled);
  bool is_vsync_enabled();
  void set_wireframes_enabled(bool enabled);
  bool is_wireframes_enabled();
  int get_renderer_width();
  int get_renderer_height();
  bool ImguiProcessEvents(SDL_Event *e);
  void ImguiNewFrame();
  Texture * LoadTexture(std::string path);
  Sprite CreateSprite(std::string path, int x, int y, int w, int h);
  void draw_sprite(Sprite *sprite, int x, int y, float sx = 1, float sy = 1);
  void Clear();
  void Present();
};

