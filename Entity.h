#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include "Util.h"

class Entity
{
public:
  Entity(int health, int base_damage, int shield, const char *name)
    : m_health(health),
      m_base_damage(base_damage),
      m_buff_damage(0),
      m_shield(shield),
      m_name(name)
  {}
  virtual ~Entity() = default;
  virtual void OnSpawn(void) = 0;
  virtual void OnDeath(void) = 0;
  virtual void Attack(Entity &target, int damage) = 0;
  virtual void TakeDamage(int damage) = 0;
  virtual bool Dead(void) const { return m_health < 0; }
  int GetBaseDamage() const { return m_base_damage; }
  int GetBuffDamage() const { return m_buff_damage; }
  int GetShield() const { return m_shield; }
  void SetBaseDamage(int damage) { m_base_damage = damage; }
  void SetBuffDamage(int buff) { m_buff_damage = buff; }
  void SetShield(int shield) { m_shield = shield; }
  const char *GetName() const { return m_name; }
  int GetHealth(void) const { return m_health; }
protected:
  int m_health;
  int m_shield;
  int m_base_damage;
  int m_buff_damage;
  const char *m_name;
};

class Zombie : public Entity
{
public:
  Zombie(int health, int base_damage)
    : Entity(health, base_damage, 0, "zombie")
  {
    m_health = health;
    OnSpawn(); // TODO: shouldn't be called by constructor
  }
  void OnSpawn(void) override
  {
    Log::Info("%s OnSpawn()", m_name);
    // initialization etc.
    // spawn animation, audio
  }
  void OnDeath(void) override
  {
    Log::Info("%s OnDeath()", m_name);
    // cleanup etc.
    // death animation, audio
  }
  void Attack(Entity &target, int damage) override
  {
    // calculate Buffs, Bonus Damage here.
    target.TakeDamage(damage);
  }
  void TakeDamage(int damage) override
  {
    // calculate Armour, Damage Reduction, Revival here.
    m_health -= damage;
    if (m_health < 0)
      OnDeath();
  }
};

class Skeleton : public Entity
{
public:
  Skeleton(int health, int base_damage)
    : Entity(health, base_damage, 0, "skeleton")
  {
    m_health = health;
    OnSpawn(); // TODO: shouldn't be called by constructor
  }
  void OnSpawn(void) override
  {
    Log::Info("%s OnSpawn()", m_name);
    // initialization etc.
    // spawn animation, audio
  }
  void OnDeath(void) override
  {
    Log::Info("%s OnDeath()", m_name);
    // cleanup etc.
    // death animation, audio
  }
  void Attack(Entity &target, int damage) override
  {
    // calculate Buffs, Bonus Damage here.
    target.TakeDamage(damage);
  }
  void TakeDamage(int damage) override
  {
    // calculate Armour, Damage Reduction, Revival here.
    m_health -= damage;
    if (m_health < 0)
      OnDeath();
  }
};

class Boss : public Entity
{
public:
  Boss(int health, int base_damage)
    : Entity(health, base_damage, 0, "boss")
  {
    m_health = health;
    OnSpawn();
  }
  void OnSpawn(void) override
  {
    Log::Info("%s OnSpawn()", m_name);
    // initialization etc.
    // spawn animation, audio
  }
  void OnDeath(void) override
  {
    Log::Info("%s OnDeath()", m_name);
    // cleanup etc.
    // death animation, audio
  }
  void Attack(Entity &target, int damage) override
  {
    // calculate Buffs, Bonus Damage here.
    if (rand() % 2)
      target.TakeDamage(damage);
    else {
      target.SetShield(0);
      target.TakeDamage(damage / 2);
    }
  }
  void TakeDamage(int damage) override
  {
    // calculate Armour, Damage Reduction, Revival here.
    m_health -= damage;
    if (m_health < 0)
      OnDeath();
  }
};

#endif
