#include "Draw.h"
#include <allegro5/allegro_primitives.h>

void DrawRect(Vec2 pos, Vec2 dim, Color color, float border)
{
  al_draw_rectangle(pos.x, pos.y,
    pos.x + dim.w, pos.y + dim.h,
    al_map_rgb(color.r, color.g, color.b),
    border);
}

void DrawRectFilled(Vec2 pos, Vec2 dim, Color color)
{
  al_draw_filled_rectangle(pos.x, pos.y,
    pos.x + dim.w, pos.y + dim.h,
    al_map_rgb(color.r, color.g, color.b));
}

void DrawRectTrans(Vec2 pos, Vec2 dim, ColorA color, float border)
{
  al_draw_rectangle(pos.x, pos.y,
    pos.x + dim.w, pos.y + dim.h,
    al_map_rgba(color.r, color.g, color.b, color.a),
    border);
}

void DrawRectFilledTrans(Vec2 pos, Vec2 dim, ColorA color)
{
  al_draw_filled_rectangle(pos.x, pos.y,
    pos.x + dim.w, pos.y + dim.h,
    al_map_rgba(color.r, color.g, color.b, color.a));
}

void ClearToColor(const Color &color)
{
  al_clear_to_color(al_map_rgb(color.r, color.g, color.b));
}

void FlipDisplay(void)
{
  al_flip_display();
}
