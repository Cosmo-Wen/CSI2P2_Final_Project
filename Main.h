#ifndef MAIN_H
#define MAIN_H

#include <cstdlib>
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>


///
/// GLOBAL CONFIGURATION
///

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define LOGFILE stdout

///
/// GAME CLASS DECLARATION
///

class Player;


class Game
{
public:
	Game();
	~Game();
	void Run();
  static Player *GetPlayer() { return &m_player;}
  static int GetVolume() { return m_volume; };
  static int SetVolume(int vol) { m_volume = vol; };
  static void Save();
  static void Load();
  ALLEGRO_TIMER *m_game_timer = NULL;
private:
	// game loop
	ALLEGRO_DISPLAY *m_display = nullptr;
	ALLEGRO_EVENT_QUEUE *m_event_queue = nullptr;
	ALLEGRO_TIMER *m_timer = nullptr;
	// media ??
	ALLEGRO_FONT *m_font = nullptr;
	ALLEGRO_FONT *m_Medium_font = nullptr;
	ALLEGRO_FONT *m_Large_font = nullptr;
	// audio ??
	ALLEGRO_SAMPLE *m_sample = nullptr;
	ALLEGRO_SAMPLE_INSTANCE *m_startSound = nullptr;
	ALLEGRO_SAMPLE_INSTANCE *m_clearSound = nullptr;
	ALLEGRO_SAMPLE_INSTANCE *m_failSound = nullptr;
	ALLEGRO_SAMPLE_INSTANCE *m_backgroundSound = nullptr;
	bool m_running = false;
  bool m_draw_update = false;
  static Player m_player;
  static int m_volume;
};

#endif
