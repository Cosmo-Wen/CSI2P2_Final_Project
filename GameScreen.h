#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "Screen.h"
#include "Card.h"
#include "Battle.h"
#include "Player.h"
#include "Main.h"

class LevelSelect: public Screen
{
	public:
		LevelSelect();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override;
	private:
		Bitmap m_map, m_player, m_skull, m_shop;
		int m_map_x;
		Vec2 m_player_pos;
		std::vector<Vec2> m_level_pos;
		bool m_animation;
		int m_animation_count;
};

class GameplayInit: public Screen
{
	public:
		GameplayInit();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override { SwitchTo<LevelSelect>(); }
		void Draw(void) override { return ; }
	private:
		Text m_loading;
};

class LoadInit: public Screen
{
	public:
		LoadInit();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override { SwitchTo<LevelSelect>(); }
		void Draw(void) override { return ; }
	private:
		Text m_loading;
};

class Shop: public Screen
{
	public:
		enum class HL {
			Item1,
			Item2,
			Item3,
			None
		};
		Shop();
		~Shop();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override;
	private:
		Vec2 m_mouse, m_press;
		Shop::HL m_highlight = Shop::HL::None;
		Bitmap m_background;
		std::vector<int> m_item_id;
		std::vector<Card*> m_item;
		Audio m_bgm;
};

class CardView: public Screen
{
	public:
		CardView() { m_update = true; }
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override;
};
#endif
