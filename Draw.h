#ifndef DRAW_H
#define DRAW_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <vector>
#include "Util.h"

void ClearToColor(const Color &color);
void FlipDisplay(void);

void DrawRect(Vec2 pos, Vec2 dim, Color color, float border);
void DrawRectFilled(Vec2 pos, Vec2 dim, Color color);

void DrawRectTrans(Vec2 pos, Vec2 dim, ColorA color, float border);
void DrawRectFilledTrans(Vec2 pos, Vec2 dim, ColorA color);

class Bitmap
{
public:
  Bitmap() // default constructor
    : m_bitmap(nullptr)
  {
     // should call SetBitmap() explicitly before draw
  }
  Bitmap(const char *bitmap_file)
  {
    m_bitmap_file = bitmap_file;
    m_bitmap = al_load_bitmap(m_bitmap_file);
    if (!m_bitmap)
      Log::Warn("failed to load bitmap file: %s", bitmap_file);
  }
  ~Bitmap()
  {
    if (m_bitmap)
      al_destroy_bitmap(m_bitmap);
  }
  void Draw(const Vec2 &pos)
  {
    al_draw_bitmap(m_bitmap, pos.x, pos.y, 0);
  }
  void DrawRotate(const Vec4 &pos, float angle)
  {
	al_draw_rotated_bitmap(m_bitmap, pos.w / 2, pos.h / 2, pos.x + pos.w / 2, pos.y + pos.h / 2, angle, 0);
  }
  void SetBitmap(const char *bitmap_file)
  {
    m_bitmap_file = bitmap_file;
    if (m_bitmap)
      al_destroy_bitmap(m_bitmap);
    m_bitmap = al_load_bitmap(m_bitmap_file);
    if (!m_bitmap)
      Log::Warn("failed to load bitmap file: %s", bitmap_file);
  }
private:
  const char *m_bitmap_file;
  ALLEGRO_BITMAP *m_bitmap;
};

class Text
{
public:
  Text() // default constructor
    : m_font(nullptr)
  {
   // should call SetText explicitly before draw
  }
  Text(const char *font_file, int size, const Color &color)
    :m_text_size(size), m_text_color(color)
  {
    m_font_file = font_file;
    m_font = al_load_ttf_font(font_file, size, 0);
    if (!m_font)
      Log::Warn("failed to load font file: %s", m_font_file);
  }
  ~Text()
  {
    if (m_font)
      al_destroy_font(m_font);
  }
  // Draw centered text at specified position
  void Draw(const std::string &s, const Vec2 &pos)
  {
    al_draw_text(m_font, al_map_rgb(m_text_color.r, m_text_color.g, m_text_color.b),
      pos.x, pos.y, ALLEGRO_ALIGN_CENTRE, s.c_str());
  }
  void DrawLeft(const std::string &s, const Vec2 &pos)
  {
    al_draw_text(m_font, al_map_rgb(m_text_color.r, m_text_color.g, m_text_color.b),
      pos.x, pos.y, ALLEGRO_ALIGN_LEFT, s.c_str());
  }
  // Set text size for next draw
  void SetSize(int new_size)
  {
    m_text_size = new_size;
    if (m_font)
      al_destroy_font(m_font);
    m_font = al_load_ttf_font(m_font_file, m_text_size, 0);
    if (!m_font)
      Log::Warn("failed to load font file: %s", m_font_file);
  }
  // Set text color for next draw
  void SetColor(const Color &color)
  {
    m_text_color = color;
  }
  void SetText(const char *font_file, int size, const Color &color)
  {
    m_font_file = font_file;
    m_text_size = size;
    m_text_color = color;
    if (m_font)
      al_destroy_font(m_font);
    m_font = al_load_ttf_font(m_font_file, m_text_size, 0);
    if (!m_font)
      Log::Warn("failed to load font file: %s", m_font_file);
  }
private:
  const char *m_font_file;
  ALLEGRO_FONT *m_font;
  int m_text_size;
  Color m_text_color;
};

#endif
