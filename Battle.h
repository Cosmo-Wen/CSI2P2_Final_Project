#ifndef BATTLE_H
#define BATTLE_H

#include "Screen.h"
#include "Entity.h"
#include "Card.h"

class Battle : public Screen
{
public:
  enum class State
  {
    NewTurn,
    SelectCard,
    SelectEntity,
    EnemyTurn,
    BattleEnds,
  };
  Battle();
  ~Battle();
  void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
  void Draw(void) override;
  void LogStatus(void);
private:
  Vec2 m_mouse, m_press;
  int m_highlight;
  int m_entity_hl;
  std::vector<Entity*> m_enemies;
  Battle::State m_state;
  Card *m_select_card;
  Entity *m_target;
  Player *m_player;
  int m_turn;
  int m_gold;
  bool m_tutorial;
  Audio m_bgm;
  Audio m_hit, m_get_hit, m_shield, m_potion;
};

#endif
