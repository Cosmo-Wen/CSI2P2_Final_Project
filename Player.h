#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Screen.h"
#include "Card.h"

class Player : public Entity
{
public:
  Player(int health, int damage)
    : Entity(health, damage, 0, "player")
  {
    OnSpawn();
  }
  ~Player() {}
  void OnSpawn(void) override
  {
    Log::Info("player on spawn");
    m_deck.push_back(new Bash);
    m_deck.push_back(new Bash);
    m_deck.push_back(new Bash);
    m_deck.push_back(new Shield);
    m_deck.push_back(new Potion);
  }
  void OnDeath(void) override
  {
    Log::Info("player on death");
  }
  void Attack(Entity &target, int damage) override
  {
    // select and play card
    Log::Info("%s attacked %s with %d damage", this->GetName(), target.GetName(), damage);
  }
  void TakeDamage(int damage) override
  {
    if (m_shield >= damage)
      m_shield -= damage;
    else {
      damage -= m_shield;
      m_shield = 0;
      m_health -= damage;
    }
    if (m_health < 0)
      OnDeath();
  }
  void DrawCards(int count)
  {
    Log::Info("draw %d cards", count);
    int deck_size = m_deck.size(), index;
    assert(deck_size != 0);
    while (count-- && m_cards.size() != m_deck.size()) {
      index = rand() % deck_size;
      while (m_deck[index]->in_hand) {
        index = rand() % deck_size;
      }
      m_cards.push_back(m_deck[index]);
      m_deck[index]->in_hand = true;
      Log::Info("  drawn card: %s", m_deck[index]->name);
    }
  }
  std::vector<Card*> &GetDeck() {return m_deck; }
  void SetHealth(int hp) {	
    m_health = hp;	
  }
  void AddToDeck(Card *add) {
    m_deck.push_back(add);
  }
  std::vector<Card*> &GetCards() { return m_cards; }
  std::vector<int> m_level_type;
	int m_level = 0;
  int m_energy;
  int m_gold = 0;
  int m_max_energy = 3;
private:
  std::vector<Card*> m_cards;
  std::vector<Card*> m_deck;
};

#endif
