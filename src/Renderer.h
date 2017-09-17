#pragma once
#include <vector>
#include <map>
#include <string>

struct SDL_Window;
struct SDL_Texture;
union SDL_Event;
typedef void *SDL_GLContext;

struct Color 
{
  float r; 
  float g;
  float b;
  float a;
};

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
  bool Init(char* title, int width, int height);
  void SetColor(float r, float g, float b, float a);
  void SetColor(Color col);
  void SetTitle(const char* title);
  void SetWindowSize(int width, int height);
  void SetClearColor(float r, float g, float b, float a);
  void SetClearColor(Color col);
  void SetVSyncEnabled(bool enabled);
  bool IsVSyncEnabled();
  void SetWireframesEnabled(bool enabled);
  bool IsWireframesEnabled();
  int GetRendererWidth();
  int GetRendererHeight();
  bool ImguiProcessEvents(SDL_Event *e);
  void ImguiNewFrame();
  Texture * LoadTexture(std::string path);
  Sprite CreateSprite(std::string path, int x, int y, int w, int h);
  void DrawSprite(Sprite * sprite, int x, int y, float sx = 1, float sy = 1);
  void Clear();
  void Present();
};

