#ifndef CARD_H
#define CARD_H

#include "Entity.h"
#include "Main.h"
#include "Util.h"

class Player;

struct Card
{
  virtual void Play(Entity &player, Entity &target) = 0;
  const char *name;
  int cost;
  int price;
  bool in_hand = false;
};

struct AttackCard : Card
{
  virtual void AttackEntity(Entity &player, Entity &target) = 0;
  void Play(Entity &player, Entity &target) override
  {
    Log::Info("playing attack card: %s", name);
    AttackEntity(player, target); 
    in_hand = false;
  }
};

struct PowerCard : Card
{
  virtual void ApplyPower(Entity &player, Entity &target) = 0;  
  void Play(Entity &player, Entity &target) override
  {
    Log::Info("playing power card: %s", name);
    ApplyPower(player, target);
    in_hand = false;
  }
};

struct SkillCard : Card
{
  virtual void UseSkill(Entity &player, Entity &target) = 0;  
  void Play(Entity &player, Entity &target) override
  {
    Log::Info("playing skill card: %s", name);
    UseSkill(player, target);
    in_hand = false;
  }
};

//
// ATTACK CARDS
//

struct Bash : AttackCard
{
  Bash() { name = "Bash"; cost = 1; price = 2;}
  void AttackEntity(Entity &player, Entity &target) override
  {
    int buff_damage = player.GetBuffDamage();
    Log::Info("Bashing %s for %d + %d damage",
      target.GetName(), BashDamage, buff_damage);
    target.TakeDamage(BashDamage + buff_damage);
  }
  const int BashDamage = 3;
};

//
// POWER CARDS
//

struct Potion : PowerCard
{
  Potion() : potion_buff(3) { name = "Potion"; cost = 2; price = 3;}
  void ApplyPower(Entity &player, Entity &entity) override
  {
    Log::Info("%s gained %d bonus damage from potion", entity.GetName(), potion_buff);
    int buff_damage = entity.GetBuffDamage();
    entity.SetBuffDamage(buff_damage + potion_buff);
  }
  int potion_buff;
};

//
// SKILL CARDS
//

struct Shield : SkillCard
{
  Shield() : shield_strength(4) { name = "Shield"; cost = 2; price = 3; }
  void UseSkill(Entity &player, Entity &entity) override
  {
    Log::Info("%s gained %d shield", entity.GetName(), shield_strength);
    int shield = entity.GetShield();
    entity.SetShield(shield + shield_strength);
  }
  int shield_strength;
};

#endif
