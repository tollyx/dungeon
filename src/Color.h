#ifndef DUNGEON_COLOR_H
#define DUNGEON_COLOR_H

struct Color {
  float r = 1;
  float g = 1;
  float b = 1;
  float a = 1;

  Color() {}

  Color(float r, float g, float b, float a) {
    this->r = r; 
    this->g = g; 
    this->b = b; 
    this->a = a;
  }
  
  Color operator*(float n) {
    return Color(r*n, g*n, b*n, a);
  }

  void operator*=(float n) {
    r *= n;
    g *= n;
    b *= n;
  }

  Color operator/(float n) { return (*this) * (1.f / n); }

  Color operator*(Color &col) {
    return Color(r*col.r, g*col.g, b*col.b, a*col.a);
  }

  Color operator+(Color &col) {
    return Color(r+col.r, g+col.g, b+col.b, a+col.a);
  }

  Color operator-(Color &col) {
    return Color(r - col.r, g - col.g, b - col.b, a - col.a);
  }
};

#endif