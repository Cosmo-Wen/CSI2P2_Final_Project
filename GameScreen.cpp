#include "GameScreen.h"

GameplayInit::GameplayInit()
{
	// LOAD GAME
	bool shop;
	int id;

	m_update = true;
	Game::GetPlayer()->m_level = 0;
    Game::GetPlayer()->m_level_type.push_back(0);
	Game::GetPlayer()->m_level_type.push_back(0);
	Game::GetPlayer()->m_level_type.push_back(1);
	for (int i = 2, shop = true; i < 6; i++) {
		if (shop || i == 5) {
			Game::GetPlayer()->m_level_type.push_back(0);
			shop = false;
		}
		else {
			id = rand() % 2;
			Game::GetPlayer()->m_level_type.push_back(id);
			if (id)
				shop = true;
			else
				shop = false;
		}
	}
	Game::GetPlayer()->m_level_type.push_back(1);
	Game::GetPlayer()->m_level_type.push_back(0);
	Game::Save();
}

LoadInit::LoadInit()
{
	// LOAD GAME
	m_update = true;
	Game::GetPlayer()->m_level = 0;
    for (int i = 0; i < 9; i++)
		Game::GetPlayer()->m_level_type.push_back(0);
	Game::Load();
}

LevelSelect::LevelSelect()
{
	// LOAD MAP
	int level = Game::GetPlayer()->m_level > 5? 5: Game::GetPlayer()->m_level;
	m_map.SetBitmap("assets/images/map.png");
	m_player.SetBitmap("assets/images/pawn.png");
	m_skull.SetBitmap("assets/images/skull.png");
	m_shop.SetBitmap("assets/images/shop.png");
	m_level_pos.push_back({24 + 128. / 2, WINDOW_HEIGHT / 2.});
	m_level_pos.push_back({500, 200});
	m_level_pos.push_back({900, 500});
	m_level_pos.push_back({1400, 200});
	m_level_pos.push_back({1700, 500});
	m_level_pos.push_back({2000, 500});
	m_level_pos.push_back({2300, 200});
	m_level_pos.push_back({2600, 325});
	m_level_pos.push_back({3000, 350});
  m_map_x = -m_level_pos[level].x + 24 + 128. / 2;
	m_player_pos.x = m_level_pos[Game::GetPlayer()->m_level].x + m_map_x;
	m_player_pos.y = m_level_pos[Game::GetPlayer()->m_level].y;
	m_animation = false;
	m_animation_count = 0;
	m_update = true;
}

void LevelSelect::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
  ALLEGRO_EVENT event;
  int level = Game::GetPlayer()->m_level;

  al_wait_for_event(event_queue, &event);
  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
  SwitchTo<Exit>();
  if (m_animation && event.type == ALLEGRO_EVENT_TIMER) {
    if (m_player_pos.x + 64 / 2 >= m_level_pos[level + 1].x + m_map_x) {
      m_animation = false;
      if (Game::GetPlayer()->m_level_type[level + 1]) {
        SwitchTo<Shop>();
        return;
      }
      else {
        SwitchTo<Battle>();
        return;
      }
    }
    else {
      m_animation_count = (m_animation_count + 1) % 10;
      if (m_animation_count == 9) {
        m_player_pos.x += (m_level_pos[level + 1].x - m_level_pos[level].x) / 12;
        m_player_pos.y += (m_level_pos[level + 1].y - m_level_pos[level].y) / 12;
		m_update = true;
      }
    }
  }
  if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
    if (event.keyboard.keycode == ALLEGRO_KEY_B)
      SwitchTo<Battle>();
    else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
      m_animation = true;
    else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
      Player *p = Game::GetPlayer();
      p->m_level = 6;
      SwitchTo<Battle>();
    } else if (event.keyboard.keycode == ALLEGRO_KEY_C)
      SwitchTo<CardView>();
    else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
      Pause(event_queue);
		m_update = true;
	}
  }
}

void LevelSelect::Draw(void)
{
	Text font("assets/fonts/pixel.ttf", 60, {255, 255, 255});

	ClearToColor({0, 0, 0});
	m_map.Draw({m_map_x, 0});
	m_player.Draw({m_player_pos});
	for (int i = 1; i < m_level_pos.size(); i++) {
		if (Game::GetPlayer()->m_level_type[i])
			m_shop.Draw({m_level_pos[i].x + m_map_x, m_level_pos[i].y});
		else
			m_skull.Draw({m_level_pos[i].x + m_map_x, m_level_pos[i].y});
	}
	m_update = false;
}

Shop::Shop()
{
	// LOAD ITEMS
	m_update = true;
	m_background.SetBitmap("assets/images/shop_background.png");
	for (int i = 0; i < 3; i++) {
		m_item_id.push_back(rand() % 3);
		if (m_item_id[i] == 0)
			m_item.push_back(new Bash);
		else if (m_item_id[i] == 1)
			m_item.push_back(new Potion);
		else if (m_item_id[i] == 2)
			m_item.push_back(new Shield);
	}
	m_bgm.SetAudioLoop("assets/audio/shop_music.wav");
	m_bgm.Play();
}

Shop::~Shop()
{
	m_item_id.clear();
}

void Shop::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;
	double part = WINDOW_WIDTH / 6.;
	Vec4 card1({part * 2 - 84, WINDOW_HEIGHT / 3 + 21, 168, 223});
	Vec4 card2({part * 3 - 84, WINDOW_HEIGHT / 3 + 21, 168, 223});
	Vec4 card3({part * 4 - 84, WINDOW_HEIGHT / 3 + 21, 168, 223});

	al_wait_for_event(event_queue, &event);
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		SwitchTo<Exit>();
	if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
        Game::GetPlayer()->m_level++;
		Game::Save();
		SwitchTo<LevelSelect>();
	}
	else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		Pause(event_queue);
		m_bgm.SetVolume(Game::GetVolume() / 10.);
		m_update = true;
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		m_mouse.x = event.mouse.x;
		m_mouse.y = event.mouse.y;
		if (InRect(m_mouse, card1)) {
			if (m_highlight != Shop::HL::Item1)
				m_update = true;
			m_highlight = Shop::HL::Item1;
		}
		else if (InRect(m_mouse, card2)) {
			if (m_highlight != Shop::HL::Item2)
				m_update = true;
			m_highlight = Shop::HL::Item2;
		}
		else if (InRect(m_mouse, card3)) {
			if (m_highlight != Shop::HL::Item3)
				m_update = true;
			m_highlight = Shop::HL::Item3;
		}
		else {
			if (m_highlight != Shop::HL::None)
				m_update = true;
			m_highlight = Shop::HL::None;
		}
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		m_press.x = event.mouse.x;
		m_press.y = event.mouse.y;
		if (InRect(m_mouse, card1) && Game::GetPlayer()->m_gold < m_item[0]->price) {
			m_press.x = 0;
			m_press.y = 0;
		}
		else if (InRect(m_mouse, card2) && Game::GetPlayer()->m_gold < m_item[1]->price) {
			m_press.x = 0;
			m_press.y = 0;
		}
		else if (InRect(m_mouse, card3) && Game::GetPlayer()->m_gold < m_item[2]->price) {
			m_press.x = 0;
			m_press.y = 0;
		}
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		if (InRect(m_mouse, card1) && InRect(m_press, card1) && m_item_id[0] != -1) {
			Game::GetPlayer()->AddToDeck(m_item[0]);
			m_item_id[0] = -1;
			Game::GetPlayer()->m_gold -= m_item[0]->price;
			m_update = true;
		}
		else if (InRect(m_mouse, card2) && InRect(m_press, card2) && m_item_id[1] != -1) {
			Game::GetPlayer()->AddToDeck(m_item[1]);
			m_item_id[1] = -1;
			Game::GetPlayer()->m_gold -= m_item[1]->price;
			m_update = true;
		}
		else if (InRect(m_mouse, card3) && InRect(m_press, card3) && m_item_id[2] != -1) {
			Game::GetPlayer()->AddToDeck(m_item[2]);
			m_item_id[2] = -1;
			Game::GetPlayer()->m_gold -= m_item[2]->price;
			m_update = true;
		}
	}
}

void Shop::Draw(void)
{
	Text font("assets/fonts/pixel.ttf", 36, {255, 255, 255});
	double part = WINDOW_WIDTH / 6.;
	Vec2 overlay = {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 6};
	Color white(255, 255, 255), black(0, 0, 0);
	Bitmap card;
	char temp[20];

	ClearToColor({0, 0, 0});
	m_background.Draw({0, 0});
	sprintf(temp, "Gold: %d", Game::GetPlayer()->m_gold);
	font.Draw(temp, {100, 20});
	font.SetColor({0, 0, 0});
	font.SetSize(24);
	for (int i = 0; i < m_item_id.size(); i++) {
		if (m_item_id[i] == 0) {
			card.SetBitmap("assets/images/bash_card_price.png");
			card.Draw({part * (i + 2) - 128, WINDOW_HEIGHT / 3.5});
		}
		else if (m_item_id[i] == 1) {
			card.SetBitmap("assets/images/brew_card_price.png");
			card.Draw({part * (i + 2) - 128, WINDOW_HEIGHT / 3.5});
		}
		else if (m_item_id[i] == 2) {
			card.SetBitmap("assets/images/block_card_price.png");
			card.Draw({part * (i + 2) - 128, WINDOW_HEIGHT / 3.5});
		}

	}
	switch (m_highlight) {
		case Shop::HL::None:
			break;
		default:
			if (m_item_id[(int)m_highlight] != -1)
				DrawRectFilled({WINDOW_WIDTH / 2 - overlay.x / 2, 0}, overlay, white);
			switch (m_item_id[(int)m_highlight]) {
				case 0:
					if (Game::GetPlayer()->m_gold < m_item[0]->price)
						font.Draw("Insufficient Gold.", {WINDOW_WIDTH / 2, overlay.y / 2.5});
					else
						font.Draw("Deal 3 damage.", {WINDOW_WIDTH / 2, overlay.y / 2.5});
					break;
				case 1:
					if (Game::GetPlayer()->m_gold < m_item[1]->price)
						font.Draw("Insufficient Gold.", {WINDOW_WIDTH / 2, overlay.y / 2.5});
					else {
						font.Draw("Deal 3 extra damage", {WINDOW_WIDTH / 2, overlay.y / 3});
						font.Draw("for the turn.", {WINDOW_WIDTH / 2, overlay.y / 2});
					}
					break;
				case 2:
					if (Game::GetPlayer()->m_gold < m_item[2]->price)
						font.Draw("Insufficient Gold.", {WINDOW_WIDTH / 2, overlay.y / 2.5});
					else
						font.Draw("Block 4 damage.", {WINDOW_WIDTH / 2, overlay.y / 2.5});
					break;
			}
			break;
	}
	m_update = false;
}

void CardView::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;

	al_wait_for_event(event_queue, &event);
	if (event.type == ALLEGRO_EVENT_KEY_UP && event.keyboard.keycode == ALLEGRO_KEY_C) {
		SwitchTo<LevelSelect>();
		return ;
	}
	else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		SwitchTo<Exit>();
		return ;
	}
}

void CardView::Draw(void)
{
	Bitmap bash, potion, shield;
	int card_num = Game::GetPlayer()->GetDeck().size();
	int row = card_num / 5 + 1;

	if (!(card_num % 5))
		row--;
	ClearToColor({0, 0, 0});
	bash.SetBitmap("assets/images/bash_card.png");
	shield.SetBitmap("assets/images/block_card.png");
	potion.SetBitmap("assets/images/brew_card.png");
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < 5 && i * 5 + j < card_num; j++) {
			if (!strcmp(Game::GetPlayer()->GetDeck()[i * 5 + j]->name, "Bash"))
				bash.Draw({j * WINDOW_WIDTH / 5, (i + 1) * WINDOW_HEIGHT / (row + 1) - 128});
			else if (!strcmp(Game::GetPlayer()->GetDeck()[i * 5 + j]->name, "Potion"))
				potion.Draw({j * WINDOW_WIDTH / 5, (i + 1) * WINDOW_HEIGHT / (row + 1) - 128});
			else if (!strcmp(Game::GetPlayer()->GetDeck()[i * 5 + j]->name, "Shield"))
				shield.Draw({j * WINDOW_WIDTH / 5, (i + 1) * WINDOW_HEIGHT / (row + 1) - 128});
		}
	}
	m_update = false;
}
