#include "Main.h"
#include "Draw.h"
#include "Audio.h"
#include "Entity.h"
#include "Screen.h"
#include "Player.h"
#include <allegro5/display.h>

Player Game::m_player(40, 5);

int Game::m_volume = 10;

Game::Game()
{
	if (!al_init())
		Log::Fatal("failed to init allegro5");
	al_init_primitives_addon();
	al_init_font_addon();        // initialize the font addon
	al_init_ttf_addon();         // initialize the ttf (True Type Font) addon
	al_init_image_addon();       // initialize the image addon
	al_init_acodec_addon();      // initialize acodec addon
	al_install_keyboard();       // install keyboard event
	al_install_mouse();          // install mouse event
    Audio::Init();

	m_display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!m_display)
		Log::Fatal("failed to create display");
	Log::Info("created display window (%dx%d)", WINDOW_WIDTH, WINDOW_HEIGHT);

	m_event_queue = al_create_event_queue();
	m_game_timer = al_create_timer(1.0 / 60);
	al_register_event_source(m_event_queue, al_get_display_event_source(m_display));
	al_register_event_source(m_event_queue, al_get_keyboard_event_source());
	al_register_event_source(m_event_queue, al_get_mouse_event_source());
	al_register_event_source(m_event_queue, al_get_timer_event_source(m_game_timer));
    al_start_timer(m_game_timer);
  // TODO: Initialize Player

 	srand(time(NULL));

	m_running = true;
	m_draw_update = true;
}

Game::~Game()
{
	al_destroy_event_queue(m_event_queue);
	al_destroy_display(m_display);

	Log::Info("game ends");
}

void Game::Run()
{

    Screen *CurrentScreen;

    // game loop
	CurrentScreen = new Init;
	while (m_running) {
		// al_register_tiemr();
		Screen::ActiveScreen->Process(m_event_queue);
		if (Screen::ActiveScreen->m_state == Screen::Type::Exit)
			m_running = false;
		else if (Screen::ActiveScreen->m_update) {
			Screen::ActiveScreen->Draw();
			FlipDisplay();
		}
	}
}

void Game::Load()
{
	ALLEGRO_CONFIG *cfg;
	char temp[20];
	int card_number;

	if ((cfg = al_load_config_file("SaveFile.cfg")) != NULL) {
		m_player.SetHealth(atoi(al_get_config_value(cfg, "Player", "Health")));
		m_player.m_level = atoi(al_get_config_value(cfg, "Player", "Level"));
		card_number = atoi(al_get_config_value(cfg, "Player", "CardNumber"));
		m_player.m_gold = atoi(al_get_config_value(cfg, "Player", "Gold"));
		m_player.GetDeck().clear();
		m_player.m_max_energy = atoi(al_get_config_value(cfg, "Player", "MaxEnergy"));
		Game::SetVolume(atoi(al_get_config_value(cfg, "Game", "Volume")));
		for (int i = 0; i < card_number; i++) {
			sprintf(temp, "Card%d", i);
			if (!strcmp("Bash", al_get_config_value(cfg, "Player", temp)))
				m_player.GetDeck().push_back(new Bash);
			else if (!strcmp("Shield", al_get_config_value(cfg, "Player", temp)))
				m_player.GetDeck().push_back(new Shield);
			else if (!strcmp("Potion", al_get_config_value(cfg, "Player", temp)))
				m_player.GetDeck().push_back(new Potion);
		}
		strcpy(temp, al_get_config_value(cfg, "Map", "Levels"));
		for (int i = 0; i < 9; i++) {
			m_player.m_level_type[i] = temp[i] - '0';
		}
	}
	al_destroy_config(cfg);
}

void Game::Save()
{
	ALLEGRO_CONFIG *cfg;
	int card_size;
	char temp[20];

	if ((cfg = al_load_config_file("SaveFile.cfg")) == NULL)
		cfg = al_create_config();
	sprintf(temp, "%d", m_player.GetHealth());
	al_set_config_value(cfg, "Player", "Health", temp);
	sprintf(temp, "%d", m_player.m_level);
	al_set_config_value(cfg, "Player", "Level", temp);
	sprintf(temp, "%d", m_player.m_gold);
	al_set_config_value(cfg, "Player", "Gold", temp);
	card_size = m_player.GetDeck().size();
	sprintf(temp, "%d", card_size);
	al_set_config_value(cfg, "Player", "CardNumber", temp);
	sprintf(temp, "%d", m_player.m_max_energy);
	al_set_config_value(cfg, "Player", "MaxEnergy", temp);
	sprintf(temp, "%d", Game::GetVolume());
	al_set_config_value(cfg, "Game", "Volume", temp);
	for (int i = 0; i < card_size; i++) {
		sprintf(temp, "Card%d", i);
		al_set_config_value(cfg, "Player", temp, m_player.GetDeck()[i]->name);
	}
	for (int i = 0; i < m_player.m_level_type.size(); i++)
		temp[i] = m_player.m_level_type[i] + '0';
	temp[m_player.m_level_type.size()] = '\0';
	al_set_config_value(cfg, "Map", "Levels", temp);
	al_save_config_file("SaveFile.cfg", cfg);
}

// space for command line options
void options(int argc, char* argv[])
{
	if (argc == 2 && !strcmp(argv[1], "-h")) {
		std::cout << "just run the game" << std::endl;
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char* argv[])
{
	options(argc, argv);

	Game game;
	game.Run();

	return EXIT_SUCCESS;
}
