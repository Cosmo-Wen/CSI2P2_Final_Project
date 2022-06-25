#include "Battle.h"
#include "Entity.h"
#include "Player.h"
#include "Screen.h"
#include "Util.h"
#include "Main.h"
#include "GameScreen.h"
#include "Draw.h"
#include <allegro5/events.h>

Battle::Battle()
{
	Log::Info("Entering Battle Screen");
	m_state = Battle::State::NewTurn;
	m_player = Game::GetPlayer();
	m_turn = 0;

	int level = m_player->m_level;
	if (level < 6) {
		for (int i = 0; i < level / 2 + 1; i++)
			if (rand() % 2)
				m_enemies.push_back(new Zombie(10, 3));
			else
				m_enemies.push_back(new Skeleton(5, 6));
	} else // boss at level 6
		m_enemies.push_back(new Boss(20, 4));

	m_highlight = -1;
	m_entity_hl = -2;
	m_gold = 0;
	m_tutorial = m_player->m_level == 0;
	m_bgm.SetAudioLoop("assets/audio/battle_music.wav");
	m_bgm.Play();
	m_hit.SetAudioOnce("assets/audio/hit.wav");
	m_get_hit.SetAudioOnce("assets/audio/get_hit.wav");
	m_potion.SetAudioOnce("assets/audio/potion.wav");
	m_shield.SetAudioOnce("assets/audio/shield.wav");
}

Battle::~Battle()
{
	Log::Info("Leaving Battle Screen");
	while (!m_enemies.empty()) {
		Entity *enemy = m_enemies.back();
		delete enemy;
		m_enemies.pop_back();
	}
}

void Battle::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;
	int keycode, num_cards;
	Vec4 card_rect, cur_card_rect;
	Vec4 enemy_rect;
	Vec4 item1, item2;
	Vec4 next({WINDOW_WIDTH * 6 / 7. - 50, WINDOW_HEIGHT - 150, 150, 75});
	double card_part = ((WINDOW_WIDTH - 80) - 450) / (m_player->GetCards().size() + 1.);
	double enemy_part = WINDOW_WIDTH / (double)(m_enemies.size() + 3.);

	al_wait_for_event(event_queue, &event);

	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		SwitchTo<Exit>();
		return;
	}
	if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
		if (event.keyboard.keycode == ALLEGRO_KEY_W) {
			SwitchTo<WinScreen>();
			return;
		}
		else if (event.keyboard.keycode == ALLEGRO_KEY_L) {
			SwitchTo<LoseScreen>();
			return ;
		}
		else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			Pause(event_queue);
			m_bgm.SetVolume(Game::GetVolume() / 10.);
			m_hit.SetVolume(Game::GetVolume() / 10.);
			m_get_hit.SetVolume(Game::GetVolume() / 10.);
			m_potion.SetVolume(Game::GetVolume() / 10.);
			m_shield.SetVolume(Game::GetVolume() / 10.);
			m_update = true;
		}
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		m_mouse.x = event.mouse.x;
		m_mouse.y = event.mouse.y;
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		m_press.x = event.mouse.x;
		m_press.y = event.mouse.y;
	}
	switch (m_state) {
		case Battle::State::NewTurn:
			// reset player status and draw cards
			m_player->SetShield(0);
			m_player->SetBuffDamage(0);
			m_player->m_energy = m_player->m_max_energy;
			if (m_turn == 0) {
				m_player->GetCards().clear();
				m_player->DrawCards(3);
			}
			else
				m_player->DrawCards(1);
			m_update = true;
			// log player and enemy status
			Log::Info("==== TURN %d ====", ++m_turn);
			LogStatus();
			m_state = Battle::State::SelectCard;
			break;
		case Battle::State::SelectCard:
			num_cards = m_player->GetCards().size();
			if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					m_state = Battle::State::EnemyTurn;
					m_update = true;
					break;
				}
				keycode = event.keyboard.keycode - ALLEGRO_KEY_1;
				if (0 <= keycode && keycode < num_cards
						&& (m_player->m_energy >= m_player->GetCards()[keycode]->cost)) {
					m_select_card = m_player->GetCards()[keycode];
					m_update = true;
				} else
					break;
				Log::Info("selected card: %s from %d cards", m_select_card->name, num_cards);
				m_state = Battle::State::SelectEntity;
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
				if (m_highlight == -1) {
					for (int i = 0; i < m_player->GetCards().size(); i++) {
						card_rect.x = 450 + card_part * (i + 1) - 128 + 44;
						card_rect.y = WINDOW_HEIGHT - 280 + 20;
						card_rect.w = (168 < 450 + card_part * (i + 2) - 128 + 44)? 168: (450 + card_part * (i + 2) - 128 + 44);
						card_rect.h = 224;
						if (InRect(m_mouse, card_rect)) {
							m_highlight = i;
							m_update = true;
						}
					}
				}
				else {
					cur_card_rect.x = 450 + card_part * (m_highlight + 1) - 128 + 44;
					cur_card_rect.y = WINDOW_HEIGHT - 280 + 20;
					cur_card_rect.w = 168;
					cur_card_rect.h = 224;
					for (int i = 0; i < m_player->GetCards().size(); i++) {
						if (i != m_highlight && !InRect(m_mouse, cur_card_rect)) {
							card_rect.x = 450 + card_part * (i + 1) - 128 + 44;
							card_rect.y = WINDOW_HEIGHT - 280 + 20;
							card_rect.w = (168 < 450 + card_part * (i + 2) - 128 + 44)? 168: (450 + card_part * (i + 2) - 128 + 44);
							card_rect.h = 224;
							if (InRect(m_mouse, card_rect)) {
								m_highlight = i;
								m_update = true;
								break;
							}
							if (i == m_player->GetCards().size() - 1) {
								m_highlight = -1;
								m_update = true;
								break;
							}
							else if ((m_highlight == m_player->GetCards().size() - 1) && i == m_player->GetCards().size() - 2) {
								m_highlight = -1;
								m_update = true;
								break;
							}
						}
					}
					if (!InRect(m_mouse, cur_card_rect)) {
						m_highlight = -1;
						m_update = true;
					}
				}
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && m_highlight != -1 && m_player->GetCards().size() > 0) {
				card_rect.x = 450 + card_part * (m_highlight + 1) - 128 + 44;
				card_rect.y = WINDOW_HEIGHT - 280 + 20;
				card_rect.w = (168 < 450 + card_part * (m_highlight + 2) - 128 + 44)? 168: (450 + card_part * (m_highlight + 2) - 128 + 44);
				card_rect.h = 224;
				if (InRect(m_mouse, card_rect) && InRect(m_press, card_rect) && m_player->m_energy >= m_player->GetCards()[m_highlight]->cost) {
					m_select_card = m_player->GetCards()[m_highlight];
					m_update = true;
					Log::Info("selected card: %s from %d cards", m_select_card->name, num_cards);
					m_state = Battle::State::SelectEntity;
				}
				m_update = true;
			}
			break;
		case Battle::State::SelectEntity:
			if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
				keycode = event.keyboard.keycode - ALLEGRO_KEY_1;
				if (keycode == -1) {
					m_target = m_player;
					m_update = true;
				} else if (0 <= keycode && keycode < m_enemies.size()) {
					m_target = m_enemies[keycode];
					m_update = true;
				} else
					break;
				Log::Info("selected m_target: %s", m_target->GetName());
				// play the selected card on entity
				m_player->m_energy -= m_select_card->cost;
				m_select_card->Play(*m_player, *m_target);
				auto iter = std::find(m_player->GetCards().begin(), m_player->GetCards().end(), m_select_card);
				m_player->GetCards().erase(iter);
				LogStatus();
				// cleanup dead targets (can be player???)
				if (m_target != m_player && m_target->Dead()) {
					auto iter = std::find(m_enemies.begin(), m_enemies.end(), m_target);
					m_enemies.erase(iter);
					delete m_target;
				}
				m_state = Battle::State::SelectCard;
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
				m_mouse.x = event.mouse.x;
				m_mouse.y = event.mouse.y;
				if (InRect(m_mouse, {80, WINDOW_HEIGHT - 275, 150, 100})) {
					if (m_entity_hl != -1)
						m_update = true;
					m_entity_hl = -1;
				}
				else if (m_entity_hl == -1) {
					m_entity_hl = -2;
					m_update = true;
				}
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				m_press.x = event.mouse.x;
				m_press.y = event.mouse.y;
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				cur_card_rect.x = 450 + card_part * (m_highlight + 1) - 128 + 44;
				cur_card_rect.y = WINDOW_HEIGHT - 280 + 20;
				cur_card_rect.w = 168;
				cur_card_rect.h = 224;
				if (InRect(m_mouse, cur_card_rect) && InRect(m_press, cur_card_rect)) {
					m_select_card = nullptr;
					m_state = Battle::State::SelectCard;
					m_update = true;
					m_entity_hl = -2;
					break;
				}
				else if (InRect(m_mouse, {80, WINDOW_HEIGHT - 275, 150, 100}) && InRect(m_press, {80, WINDOW_HEIGHT - 275, 150, 100})) {
					m_target = m_player;
					Log::Info("selected m_target: %s", m_target->GetName());
					// play the selected card on entity
					m_player->m_energy -= m_select_card->cost;
					m_select_card->Play(*m_player, *m_target);
					if (!strcmp(m_select_card->name, "Bash"))
						m_hit.Play();
					else if (!strcmp(m_select_card->name, "Shield"))
						m_shield.Play();
					else if (!strcmp(m_select_card->name, "Potion"))
						m_potion.Play();
					auto iter = std::find(m_player->GetCards().begin(), m_player->GetCards().end(), m_select_card);
					m_player->GetCards().erase(iter);
					LogStatus();
					m_update = true;
					m_entity_hl = -2;
					m_state = Battle::State::SelectCard;
					m_tutorial = false;
					m_highlight = -1;
					break;
				}
				for (int i = 0; i < m_enemies.size(); i++) {
					enemy_rect.x = enemy_part * (i + 2) - 128;
					enemy_rect.y = WINDOW_HEIGHT / 3.;
					enemy_rect.w = 256;
					enemy_rect.h = 256;
					if (InRect(m_mouse, enemy_rect) && InRect(m_press, enemy_rect)) {
						m_target = m_enemies[i];
						Log::Info("selected m_target: %s", m_target->GetName());
						// play the selected card on entity
						m_player->m_energy -= m_select_card->cost;
						m_select_card->Play(*m_player, *m_target);
						if (!strcmp(m_select_card->name, "Bash"))
							m_hit.Play();
						else if (!strcmp(m_select_card->name, "Shield"))
							m_shield.Play();
						else if (!strcmp(m_select_card->name, "Potion"))
							m_potion.Play();
						auto iter = std::find(m_player->GetCards().begin(), m_player->GetCards().end(), m_select_card);
						m_player->GetCards().erase(iter);
						LogStatus();
						// cleanup dead targets (can be player???)
						if (m_target != m_player && m_target->Dead()) {
							m_gold += m_target->GetHealth() * -1;
							auto iter = std::find(m_enemies.begin(), m_enemies.end(), m_target);
							m_enemies.erase(iter);
							delete m_target;
						}
						m_state = Battle::State::SelectCard;
						m_tutorial = false;
						m_update = true;
						m_entity_hl = -2;
						m_highlight = -1;
					}
				}
				if (m_enemies.empty()) {
					m_state = Battle::State::BattleEnds;
					break;
				}
			}
			break;
		case Battle::State::EnemyTurn:
			Log::Info("EnemyTurn");
			m_update = true;
			for (auto &enemy : m_enemies) {
				int dmg = enemy->GetBaseDamage();
				if (dmg > m_player->GetShield())
					m_get_hit.Play();
				else
					m_shield.Play();
				enemy->Attack(*m_player, dmg);
				Log::Info("%s attacked %s with %d damage", enemy->GetName(), m_player->GetName(), dmg);
			}
			if (m_player->Dead()) {
				SwitchTo<LoseScreen>();
				return;
			}
			else
				m_state = Battle::State::NewTurn;
			break;
		case Battle::State::BattleEnds:
			for (int i = 0; i < m_player->GetCards().size(); i++)
				m_player->GetCards()[i]->in_hand = false;
			m_player->m_gold += m_gold;
			if (m_gold != 0)
				m_update = true;
			m_gold = 0;
			item1.x = WINDOW_WIDTH / 7.;
			item1.y =  WINDOW_HEIGHT / 3.;
			item1.w = WINDOW_WIDTH * 2 / 7.;
			item1.h =  WINDOW_WIDTH * 1.5 / 7.;
			item2.x = WINDOW_WIDTH * 4 / 7.;
			item2.y = item1.y;
			item2.h = item1.h;
			item2.w = item1.w;
			if (Game::GetPlayer()->m_level == 7)
				SwitchTo<WinScreen>();
			else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				Game::GetPlayer()->m_level++;
				Game::Save();
				SwitchTo<LevelSelect>();
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
				if (InRect(m_mouse, item1)) {
					if (m_highlight != 1)
						m_update = true;
					m_highlight = 1;
				}
				else if (InRect(m_mouse, item2)){
					if (m_highlight != 2)
						m_update = true;
					m_highlight = 2;
				}
				else if (InRect(m_mouse, {WINDOW_WIDTH * 6 / 7. - 50, WINDOW_HEIGHT - 150, 150, 75})) {
					if (m_highlight != 3)
						m_update = true;
					m_highlight = 3;
				}
				else {
					if (m_highlight != 0)
						m_update = true;
					m_highlight = 0;
				}
			}
			else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				if (InRect(m_press, item1) && InRect(m_mouse, item1)) {
					if (m_player->m_gold >= 5) {
						m_player->m_gold -= 5;
						m_player->SetHealth(40);
						Game::GetPlayer()->m_level++;
						Game::Save();
						SwitchTo<LevelSelect>();
					}
				}
				else if (InRect(m_press, item2) && InRect(m_mouse, item2)) {
					if (m_player->m_gold >= 10) {
						m_player->m_gold -= 10;
						m_player->m_max_energy++;
						Game::GetPlayer()->m_level++;
						Game::Save();
						SwitchTo<LevelSelect>();
					}
				}
				else if (InRect(m_mouse, next) && InRect(m_press, next)) {
					Game::GetPlayer()->m_level++;
					Game::Save();
					SwitchTo<LevelSelect>();
				}
			}
			return;
	}
}

void Battle::Draw(void)
{
	Text font("assets/fonts/pixel.ttf", 36, {255, 255, 255});
	int select_flag = (m_state == Battle::State::SelectEntity);
	double enemy_part = WINDOW_WIDTH / (double)(m_enemies.size() + 3.);
	double card_part = ((WINDOW_WIDTH - 80) - 450) / (m_player->GetCards().size() + 1.);
	Bitmap sword, shield;
	Bitmap skele, zomb, boss;
	Bitmap bash_card, potion_card, shield_card;
	Bitmap heart, lightning;
	Color white(255, 255, 255), black{0, 0, 0};
	char temp[20];

	sword.SetBitmap("assets/images/sword.png");
	shield.SetBitmap("assets/images/shield.png");
	skele.SetBitmap("assets/images/skeleton.png");
	zomb.SetBitmap("assets/images/zombie.png");
	boss.SetBitmap("assets/images/death_tyrant.png");
	bash_card.SetBitmap("assets/images/bash_card.png");
	potion_card.SetBitmap("assets/images/brew_card.png");
	shield_card.SetBitmap("assets/images/block_card.png");
	heart.SetBitmap("assets/images/heart.png");
	lightning.SetBitmap("assets/images/lightning.png");
	ClearToColor({0, 0, 0});
	if (m_state != Battle::State::BattleEnds) {
		font.Draw("HP", {50, WINDOW_HEIGHT - 80});
		DrawRect({80, WINDOW_HEIGHT - 75}, {280, 40}, white, 4);
		DrawRectFilled({85, WINDOW_HEIGHT - 70}, {270 * Game::GetPlayer()->GetHealth() / 40, 30}, white);
		sword.Draw({80, WINDOW_HEIGHT - 150});
		sprintf(temp, "%d", m_player->GetBuffDamage());
		font.Draw(temp, {155, WINDOW_HEIGHT - 135});
		shield.Draw({170, WINDOW_HEIGHT - 150});
		sprintf(temp, "%d", m_player->GetShield());
		font.Draw(temp, {250, WINDOW_HEIGHT - 135});
		sprintf(temp, "%d/%d", m_player->m_energy, m_player->m_max_energy);
		font.Draw(temp, {325, WINDOW_HEIGHT - 135});
		if (m_entity_hl != -1) {
			DrawRect({80, WINDOW_HEIGHT - 275}, {150, 100}, white, 4);
			font.Draw("Self", {155, WINDOW_HEIGHT - 250});
		}
		else {
			DrawRectFilled({80, WINDOW_HEIGHT - 275}, {150, 100}, white);
			font.SetColor(black);
			font.Draw("Self", {155, WINDOW_HEIGHT - 250});
			font.SetColor(white);
		}
		sprintf(temp, "Level %d", m_player->m_level + 1);
		font.SetSize(60);
		font.Draw(temp, {WINDOW_WIDTH - 150, 20});
		for (int i = 0; i < m_enemies.size(); i++) {
			if (!strcmp(m_enemies[i]->GetName(), "zombie")) {
				zomb.Draw({enemy_part * (i + 2) - 128, WINDOW_HEIGHT / 3.});
				DrawRectFilled({enemy_part * (i + 2) - 80,  WINDOW_HEIGHT / 3 - 15}, {160 * m_enemies[i]->GetHealth() / 10, 10}, white);
				DrawRect({enemy_part * (i + 2) - 85, WINDOW_HEIGHT / 3 - 20}, {170, 20}, white, 4);
			} else if (!strcmp(m_enemies[i]->GetName(), "skeleton")) {
				skele.Draw({enemy_part * (i + 2) - 128, WINDOW_HEIGHT / 3.});
				DrawRectFilled({enemy_part * (i + 2) - 80,  WINDOW_HEIGHT / 3 - 15}, {160 * m_enemies[i]->GetHealth() / 5, 10}, white);
				DrawRect({enemy_part * (i + 2) - 85, WINDOW_HEIGHT / 3 - 20}, {170, 20}, white, 4);
			} else if (!strcmp(m_enemies[i]->GetName(), "boss")) {
				boss.Draw({enemy_part * (i + 2) - 256, WINDOW_HEIGHT / 10.});
				DrawRectFilled({enemy_part * (i + 2) - 326,  WINDOW_HEIGHT / 10. - 15}, {320 * m_enemies[i]->GetHealth() / 10, 10}, white);
				DrawRect({enemy_part * (i + 2) - 331, WINDOW_HEIGHT / 10. - 20}, {650, 20}, white, 4);
			}
		}
		for (int i = 0; i < m_player->GetCards().size(); i++) {
			if (i != m_highlight) {
				if (!strcmp(m_player->GetCards()[i]->name, "Bash"))
					bash_card.Draw({450 + card_part * (i + 1) - 128, WINDOW_HEIGHT - 280});
				else if (!strcmp(m_player->GetCards()[i]->name, "Potion"))
					potion_card.Draw({450 + card_part * (i + 1) - 128, WINDOW_HEIGHT - 280});
				else if (!strcmp(m_player->GetCards()[i]->name, "Shield")) {
					shield_card.Draw({450+ card_part * (i + 1) - 128, WINDOW_HEIGHT - 280});
				}
			}
		}
		if (m_highlight != -1 && m_player->GetCards().size() > 0) {
			if (!strcmp(m_player->GetCards()[m_highlight]->name, "Bash"))
				bash_card.Draw({450 + card_part * (m_highlight + 1) - 128, WINDOW_HEIGHT - 280 - select_flag * 30});
			else if (!strcmp(m_player->GetCards()[m_highlight]->name, "Potion"))
				potion_card.Draw({450 + card_part * (m_highlight + 1) - 128, WINDOW_HEIGHT - 280 - select_flag * 30});
			else if (!strcmp(m_player->GetCards()[m_highlight]->name, "Shield"))
				shield_card.Draw({450 + card_part * (m_highlight + 1) - 128, WINDOW_HEIGHT - 280 - select_flag * 30});
			if (m_player->m_energy < m_player->GetCards()[m_highlight]->cost) {
				DrawRectFilled({WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 / 2, 0}, {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 6}, white);
				font.SetSize(24);
				font.SetColor(black);
				font.Draw("Too exhausted", {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 6 / 2.5});
			}
		}
		if (m_tutorial) {
			switch (m_state) {
				case Battle::State::SelectCard:
					DrawRectFilledTrans({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT - 270}, {75, 75, 75, 75});
					DrawRectFilled({750, WINDOW_HEIGHT - 350}, {300, 75}, white);
					font.SetSize(24);
					font.SetColor(black);
					font.Draw("Click to pick a card.", {900, WINDOW_HEIGHT - 325});
					break;
				case Battle::State::SelectEntity:
					DrawRectFilled({WINDOW_WIDTH / 2 - 160, 100}, {320, 75}, white);
					DrawRectFilled({50, WINDOW_HEIGHT / 2}, {300, 75}, white);
					font.SetSize(24);
					font.SetColor(black);
					font.Draw("Click to pick an entity,", {WINDOW_WIDTH / 2, 100 + 25});
					font.Draw("Or pick yourself.", {200, WINDOW_HEIGHT / 2 + 25});
					break;
			}
		}
	}
	else {
		font.SetSize(60);
		font.SetColor(white);
		font.Draw("Enemy Defeated", {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 7.});
		font.SetSize(36);
		sprintf(temp, "Gold: %d", Game::GetPlayer()->m_gold);
		font.Draw(temp, {100, 20});
		DrawRect({WINDOW_WIDTH / 7., WINDOW_HEIGHT / 3.}, {WINDOW_WIDTH * 2 / 7., WINDOW_WIDTH * 1.5 / 7.}, white, 5);
		DrawRect({WINDOW_WIDTH * 4 / 7., WINDOW_HEIGHT / 3.}, {WINDOW_WIDTH * 2 / 7., WINDOW_WIDTH * 1.5 / 7.}, white, 5);
		DrawRect({WINDOW_WIDTH * 6 / 7. - 50, WINDOW_HEIGHT - 150}, {150, 75}, white, 5);
		font.SetSize(36);
		font.SetColor(white);
		font.Draw("Next", {WINDOW_WIDTH * 6 / 7 + 25, WINDOW_HEIGHT - 140});
		if (m_highlight == 1) {
			font.SetSize(24);
			font.SetColor(black);
			DrawRectFilled({WINDOW_WIDTH * 3 / 8., 0}, {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 6}, white);
			if (m_player->m_gold < 5)
				font.Draw("Insufficient Gold.", {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 15.});
			else
				font.Draw("Restore Max Health.", {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 15.});
			DrawRectFilled({WINDOW_WIDTH / 7., WINDOW_HEIGHT / 3.}, {WINDOW_WIDTH * 2 / 7., WINDOW_WIDTH * 1.5 / 7.}, white);
		}
		else if (m_highlight == 2) {
			font.SetSize(24);
			font.SetColor(black);
			DrawRectFilled({WINDOW_WIDTH * 3 / 8., 0}, {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 6}, white);
			if (m_player->m_gold < 10)
				font.Draw("Insufficient Gold.", {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 15.});
			else
				font.Draw("Increase Max Energy.", {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 15.});
			DrawRectFilled({WINDOW_WIDTH * 4 / 7., WINDOW_HEIGHT / 3.}, {WINDOW_WIDTH * 2 / 7., WINDOW_WIDTH * 1.5 / 7.}, white);
		}
		else if (m_highlight == 3) {
			DrawRectFilled({WINDOW_WIDTH * 6 / 7. - 50, WINDOW_HEIGHT - 150}, {150, 75}, white);
			font.SetSize(36);
			font.SetColor(black);
			font.Draw("Next", {WINDOW_WIDTH * 6 / 7 + 25, WINDOW_HEIGHT - 140});
		}
		heart.Draw({WINDOW_WIDTH * 2 / 7 - 70., WINDOW_HEIGHT / 3. + 60});
		lightning.Draw({WINDOW_WIDTH * 5 / 7. - 50, WINDOW_HEIGHT / 3. + 60});
	}
	m_update = false;
}

void Battle::LogStatus(void)
{
	Log::Info(" (0) player %d HP, %d + %d DMG, (%d/%d) ENERGY",
			m_player->GetHealth(),
			m_player->GetBaseDamage(),
			m_player->GetBuffDamage(),
			m_player->m_energy,
			m_player->m_max_energy);
	for (int i = 0; i < m_player->GetCards().size(); i++)
		Log::Info(" (%d) card %s costs %d",
				i+1, m_player->GetCards()[i]->name, m_player->GetCards()[i]->cost);
	for (int i = 0; i < m_enemies.size(); i++)
		Log::Info(" (%d) enemy %s: (%d, %d)",
				i+1, m_enemies[i]->GetName(), m_enemies[i]->GetHealth(), m_enemies[i]->GetBaseDamage());
}
