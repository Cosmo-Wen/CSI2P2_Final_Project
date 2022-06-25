#include "Util.h"

bool InRect(Vec2 pos, Vec4 rect)
{
  return rect.x <= pos.x && pos.x <= rect.x + rect.w  
      && rect.y <= pos.y && pos.y <= rect.y + rect.h;
}
