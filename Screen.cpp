#include "Screen.h"
#include "Draw.h"
#include "Util.h"
#include "Battle.h"
#include "GameScreen.h"
#include "Main.h"
#include "Player.h"
#include <allegro5/events.h>

Screen *Screen::ActiveScreen = new Init;

void Screen::Pause(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;
	double part = WINDOW_WIDTH / 4.;
	double btnw = WINDOW_WIDTH / 6., btnh = WINDOW_HEIGHT / 4.;
	Color white(255, 255, 255);
	Color black(0, 0, 0);
	bool first = true;
	bool draw = true;
	Text font("assets/fonts/pixel.ttf", 60, black);
	int highlight = 0;
	double y = WINDOW_HEIGHT / 2. + 100;
	double w = WINDOW_WIDTH  / 6.;
	double h = WINDOW_HEIGHT / 4.;
	Vec2 mouse(event.mouse.x, event.mouse.y);
	Vec2 press(press.x, press.y);
	Vec4 btn_back(part - w / 2, y, w, h);
	Vec4 btn_menu(part * 2- w / 2, y, w, h);
	Vec4 btn_exit(part * 3- w / 2, y, w, h);
	Vec4 btn_down({WINDOW_WIDTH / 3. - 25, WINDOW_HEIGHT / 2.5 + 9, 50, 50});
	Vec4 btn_up({WINDOW_WIDTH * 2 / 3. - 25, WINDOW_HEIGHT / 2.5 + 9, 50, 50});
	char temp[20];
	int volume = Game::GetVolume();

	while (1) {
		al_wait_for_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			SwitchTo<Exit>();
			return ;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			mouse.x = event.mouse.x;
			mouse.y = event.mouse.y;
			if (InRect(mouse, btn_back)) {
				if (highlight != 1)
					draw = true;
				highlight = 1;
			}
			else if (InRect(mouse, btn_menu)) {
				if (highlight != 2)
					draw = true;
				highlight = 2;
			}
			else if (InRect(mouse, btn_exit)) {
				if (highlight != 3)
					draw = true;
				highlight = 3;
			}
			else if (InRect(mouse, btn_down)) {
				if (highlight != 4)
					draw = true;
				highlight = 4;
			}
			else if (InRect(mouse, btn_up)) {
				if (highlight != 5)
					draw = true;
				highlight = 5;
			}
			else {
				if (highlight != 0)
					draw = true;
				highlight = 0;
			}
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			press.x = event.mouse.x;
			press.y = event.mouse.y;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (InRect(press, btn_back) && InRect(mouse, btn_back))
				return ;
			else if (InRect(press, btn_menu) && InRect(mouse, btn_menu)) {
				SwitchTo<Title>();
				return ;
			}
			else if (InRect(press, btn_exit) && InRect(mouse, btn_exit)) {
				SwitchTo<Exit>();
				return ;
			}
			else if (InRect(press, btn_up) && InRect(mouse, btn_up)) {
				if (volume < 10)
					volume++;
				Game::SetVolume(volume);
				draw = true;
			}
			else if (InRect(press, btn_down) && InRect(mouse, btn_down)) {
				if (volume > 0)
					volume--;
				Game::SetVolume(volume);
				draw = true;
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				return;
			}
		}
		if (draw) {
			if (first) {
				font.SetSize(60);
				DrawRectFilledTrans({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {175, 175, 175, 0});
				font.Draw("Pause", {WINDOW_WIDTH / 2., WINDOW_HEIGHT / 8.});
				first = false;
			}
			DrawRectFilled({part - btnw / 2, WINDOW_HEIGHT / 2. + 100},     {btnw, btnh}, black);
			DrawRectFilled({part * 2 - btnw / 2, WINDOW_HEIGHT / 2. + 100}, {btnw, btnh}, black);
			DrawRectFilled({part * 3 - btnw / 2, WINDOW_HEIGHT / 2. + 100}, {btnw, btnh}, black);
			DrawRectFilled({WINDOW_WIDTH / 3. - 25, WINDOW_HEIGHT / 2.5 + 9}, {50, 50}, black);
			DrawRectFilled({WINDOW_WIDTH * 2 / 3. - 25, WINDOW_HEIGHT / 2.5 + 9}, {50, 50}, black);
			for (int i = 1; i <= volume; i++) {
				double length = (WINDOW_WIDTH * 2 / 3. - 25 - (WINDOW_WIDTH / 3. + 25)) / 21.;
				double start = WINDOW_WIDTH / 3. + 25 + length * (2 * i - 1);
				DrawRectFilled({start, WINDOW_HEIGHT / 2.5 + 9}, {length, 50}, white);
			}
			for (int i = volume + 1; i <= 10; i++) {
				double length = (WINDOW_WIDTH * 2 / 3. - 25 - (WINDOW_WIDTH / 3. + 25)) / 21.;
				double start = WINDOW_WIDTH / 3. + 25 + length * (2 * i - 1);
				DrawRectFilled({start, WINDOW_HEIGHT / 2.5 + 9}, {length, 50}, black);
			}
			font.SetSize(48);
			font.SetColor(black);
			font.Draw("Volume:", {WINDOW_WIDTH / 2. + 3, WINDOW_HEIGHT / 3 - 10});
			font.SetColor(white);
			font.Draw("Back", {part, WINDOW_HEIGHT * 2.5 / 4 - 36 + 100});
			font.Draw("Menu", {part * 2, WINDOW_HEIGHT * 2.5 / 4 - 36 + 100});
			font.Draw("Exit",  {part * 3, WINDOW_HEIGHT * 2.5 / 4 - 36 + 100});
			font.Draw("-", {WINDOW_WIDTH / 3. + 3, WINDOW_HEIGHT / 2.5 - 5});
			font.Draw("+", {WINDOW_WIDTH * 2 / 3. + 2.5, WINDOW_HEIGHT / 2.5 - 5});
			if (highlight == 1) {
				DrawRectFilled({part - btnw / 2, WINDOW_HEIGHT / 2. + 100}, {btnw, btnh}, white);
				font.SetColor(black);
				font.Draw("Back", {part, WINDOW_HEIGHT * 2.5 / 4 - 36 + 100});
			}
			else if (highlight == 2) {
				DrawRectFilled({part * 2 - btnw / 2, WINDOW_HEIGHT / 2. + 100}, {btnw, btnh}, white);
				font.SetColor(black);
				font.Draw("Menu", {part * 2, WINDOW_HEIGHT * 2.5 / 4 - 36 + 100});
			}
			else if (highlight == 3) {
				DrawRectFilled({part * 3 - btnw / 2, WINDOW_HEIGHT / 2. + 100}, {btnw, btnh}, white);
				font.SetColor(black);
				font.Draw("Exit", {part * 3, WINDOW_HEIGHT * 2.5 / 4 - 36 + 100});
			}
			else if (highlight == 4) {
				DrawRectFilled({WINDOW_WIDTH / 3. - 25, WINDOW_HEIGHT / 2.5 + 9}, {50, 50}, white);
				font.SetColor(black);
				font.Draw("-", {WINDOW_WIDTH / 3. + 3, WINDOW_HEIGHT / 2.5 - 5});
			}
			else if (highlight == 5) {
				DrawRectFilled({WINDOW_WIDTH * 2 / 3. - 25, WINDOW_HEIGHT / 2.5 + 9}, {50, 50}, white);
				font.SetColor(black);
				font.Draw("+", {WINDOW_WIDTH * 2 / 3. + 2.5, WINDOW_HEIGHT / 2.5 - 5});
			}
			FlipDisplay();
			draw = false;
		}
	}
}

Screen::~Screen() {}

void Init::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	SwitchTo<Title>();
}

Title::Title()
{
	m_update = true;
	m_state = Screen::Type::Title;
	m_highlight = Title::HL::None;
	Game::GetPlayer()->SetHealth(40);
	m_font.SetText("assets/fonts/pixel.ttf", 60, {255, 255, 255});
	m_btn_sound.SetAudioOnce("assets/audio/button_sound.wav");
	Game::SetVolume(10);
	m_btn_sound.SetVolume(Game::GetVolume() / 10.);
}

void Title::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;
	ALLEGRO_CONFIG *cfg;
	double part = WINDOW_WIDTH / 6.;
	double y = WINDOW_HEIGHT / 2.;
	double w = WINDOW_WIDTH  / 6. - 20;
	double h = WINDOW_HEIGHT / 4.;

	al_wait_for_event(event_queue, &event);
	Vec2 mouse(event.mouse.x, event.mouse.y);
	Vec2 press(m_press_x, m_press_y);
	Vec4 btn_new(part + 10, y, w, h);
	Vec4 btn_cont(part * 2 + 10, y, w, h);
	Vec4 btn_about(part * 3, y, w, h);
	Vec4 btn_exit(part * 4 + 10, y, w, h);
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		SwitchTo<Exit>();
	if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		// hover highlight
		if (InRect(mouse, btn_new)) {
			if (m_highlight != Title::HL::NewGame)
				m_update = true;
			m_highlight = Title::HL::NewGame;
		}
		else if (InRect(mouse, btn_cont)) {
			if (m_highlight != Title::HL::Continue)
				m_update = true;
			m_highlight = Title::HL::Continue;
		}
		else if (InRect(mouse, btn_about)) {
			if (m_highlight != Title::HL::About)
				m_update = true;
			m_highlight = Title::HL::About;
		}
		else if (InRect(mouse, btn_exit)) {
			if (m_highlight != Title::HL::Exit)
				m_update = true;
			m_highlight = Title::HL::Exit;
		}
		else {
			if (m_highlight != Title::HL::None)
				m_update = true;
			m_highlight = Title::HL::None;
		}
	}
	// button press sound
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		m_press_x = event.mouse.x;
		m_press_y = event.mouse.y;
		press.x = m_press_x;
		press.y = m_press_y;
		if (InRect(press, btn_new))
			m_btn_sound.Play();
		else if (InRect(press, btn_cont))
			m_btn_sound.Play();
		else if (InRect(press, btn_about))
			m_btn_sound.Play();
		else if (InRect(press, btn_exit))
			m_btn_sound.Play();
	}
	// confirm selection (press down and up at the same button)
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		if (InRect(press, btn_new) && InRect(mouse, btn_new))
			SwitchTo<GameplayInit>();
		else if (InRect(press, btn_cont) && InRect(mouse, btn_cont) && (cfg = al_load_config_file("SaveFile.cfg")) != NULL) {
			al_destroy_config(cfg);
			SwitchTo<LoadInit>();
		}
		else if (InRect(press, btn_about) && InRect(mouse, btn_about))
			SwitchTo<About>();
		else if (InRect(press, btn_exit) && InRect(mouse, btn_exit))
			SwitchTo<Exit>();
	}
}

void Title::Draw()
{
	double part = WINDOW_WIDTH / 6.;
	double btnw = part - 20, btnh = WINDOW_HEIGHT / 4.;
	Color white(255, 255, 255);
	Color black(0, 0, 0);

	ClearToColor(black);
	m_font.SetSize(60);
	m_font.SetColor(white);
	m_font.Draw("Another Deckbuilder", {WINDOW_WIDTH / 2., WINDOW_HEIGHT / 4.});
	DrawRect({part + 10, WINDOW_HEIGHT / 2.},     {btnw, btnh}, white, 5);
	DrawRect({part * 2 + 10, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white, 5);
	DrawRect({part * 3 + 10, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white, 5);
	DrawRect({part * 4 + 10, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white, 5);
	m_font.SetSize(48);
	m_font.Draw("New",   {part * 1.5, WINDOW_HEIGHT * 2.5 / 4 - 72});
	m_font.Draw("Game",  {part * 1.5, WINDOW_HEIGHT * 2.5 / 4});
	m_font.Draw("Cont.", {part * 2.5, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_font.Draw("About", {part * 3.5, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_font.Draw("Exit",  {part * 4.5, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_font.SetColor(black);
	switch (m_highlight) {
		case Title::HL::NewGame:
			DrawRectFilled({part + 10, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("New",  {part * 1.5, WINDOW_HEIGHT * 2.5 / 4 - 72});
			m_font.Draw("Game", {part * 1.5, WINDOW_HEIGHT * 2.5 / 4});
			break;
		case Title::HL::Continue:
			DrawRectFilled({part * 2 + 10, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("Cont.", {part * 2.5, WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		case Title::HL::About:
			DrawRectFilled({part * 3 + 10, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("About", {part * 3.5, WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		case Title::HL::Exit:
			DrawRectFilled({part * 4 + 10, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("Exit", {part * 4.5, WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		default:
			break;
	}
	Screen::ActiveScreen->m_update = false;
}

WinScreen::WinScreen()
{
	m_update = true;
	m_state = Screen::Type::Win;
	m_highlight = WinScreen::HL::None;
	m_font.SetText("assets/fonts/pixel.ttf", 60, {255, 255, 255});
	m_trophy.SetBitmap("assets/images/trophy.png");
	m_btn_sound.SetAudioOnce("assets/audio/button_sound.wav");
	m_btn_sound.SetVolume(1);
//	m_win_bgm.SetAudio("");
//	m_win_bgm.SetVolume(1);
}

void WinScreen::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;
	double part = WINDOW_WIDTH / 4.;
	double y = WINDOW_HEIGHT / 2.;
	double w = WINDOW_WIDTH  / 6.;
	double h = WINDOW_HEIGHT / 4.;

	al_wait_for_event(event_queue, &event);
	Vec2 mouse(event.mouse.x, event.mouse.y);
	Vec2 press(m_press_x, m_press_y);
	Vec4 btn_endless(part - w / 2, y, w, h);
	Vec4 btn_menu(part * 2- w / 2, y, w, h);
	Vec4 btn_exit(part * 3- w / 2, y, w, h);
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		SwitchTo<Exit>();
	if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		// hover highlight
		if (InRect(mouse, btn_endless)) {
			if (m_highlight != WinScreen::HL::Endless)
				m_update = true;
			m_highlight = WinScreen::HL::Endless;
		}
		else if (InRect(mouse, btn_menu)) {
			if (m_highlight != WinScreen::HL::SaveMenu)
				m_update = true;
			m_highlight = WinScreen::HL::SaveMenu;
		}
		else if (InRect(mouse, btn_exit)) {
			if (m_highlight != WinScreen::HL::SaveExit)
				m_update = true;
			m_highlight = WinScreen::HL::SaveExit;
		}
		else {
			if (m_highlight != WinScreen::HL::None)
				m_update = true;
			m_highlight = WinScreen::HL::None;
		}
	}
	// button press sound
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		m_press_x = event.mouse.x;
		m_press_y = event.mouse.y;
		press.x = m_press_x;
		press.y = m_press_y;
		if (InRect(press, btn_endless))
			m_btn_sound.Play();
		else if (InRect(press, btn_menu))
			m_btn_sound.Play();
		else if (InRect(press, btn_exit))
			m_btn_sound.Play();
	}
	// confirm selection (press down and up at the same button)
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		if (InRect(press, btn_endless) && InRect(mouse, btn_endless))
			SwitchTo<Test>();
		else if (InRect(press, btn_menu) && InRect(mouse, btn_menu))
			SwitchTo<Title>();
		else if (InRect(press, btn_exit) && InRect(mouse, btn_exit))
			SwitchTo<Exit>();
	}
}

void WinScreen::Draw()
{
	double part = WINDOW_WIDTH / 4.;
	double btnw = WINDOW_WIDTH / 6., btnh = WINDOW_HEIGHT / 4.;
	Color white(255, 255, 255);
	Color black(0, 0, 0);

	ClearToColor(black);
	m_font.SetSize(60);
	m_font.SetColor(white);
	m_font.Draw("VICTORY", {WINDOW_WIDTH / 2., WINDOW_HEIGHT / 4.});
	m_trophy.DrawRotate({part / 3, WINDOW_HEIGHT / 8., 256, 256}, -0.5);
	m_trophy.DrawRotate({part  * 3 - 42, WINDOW_HEIGHT / 8., 256, 256}, 0.5);
	DrawRect({part - btnw / 2, WINDOW_HEIGHT / 2.},     {btnw, btnh}, white, 5);
	DrawRect({part * 2 - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white, 5);
	DrawRect({part * 3 - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white, 5);
	m_font.SetSize(48);
	m_font.Draw("End?", {part, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_font.Draw("Menu", {part * 2, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_font.Draw("Exit",  {part * 3, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_font.SetColor(black);
	switch (m_highlight) {
		case WinScreen::HL::Endless:
			DrawRectFilled({part - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("End?", {part, WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		case WinScreen::HL::SaveMenu:
			DrawRectFilled({part * 2 - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("Menu", {part * 2, WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		case WinScreen::HL::SaveExit:
			DrawRectFilled({part * 3 - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("Exit", {part * 3, WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		default:
			break;
	}
	Screen::ActiveScreen->m_update = false;
}
LoseScreen::LoseScreen()
{
	m_update = true;
	m_state = Screen::Type::Win;
	m_highlight = LoseScreen::HL::None;
	m_font.SetText("assets/fonts/pixel.ttf", 60, {255, 255, 255});
	m_sad_face.SetBitmap("assets/images/sad_face.png");
	m_btn_sound.SetAudioOnce("assets/audio/button_sound.wav");
	m_btn_sound.SetVolume(1);
		Log::Info("Lose");
//	m_lose_bgm.SetAudio("");
//	m_lose_bgm.SetVolume(1);
}

void LoseScreen::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;
	double part = WINDOW_WIDTH / 3.;
	double y = WINDOW_HEIGHT / 2.;
	double w = WINDOW_WIDTH  / 6.;
	double h = WINDOW_HEIGHT / 4.;

	al_wait_for_event(event_queue, &event);
	Vec2 mouse(event.mouse.x, event.mouse.y);
	Vec2 press(m_press_x, m_press_y);
	Vec4 btn_menu(part - w / 2, y, w, h);
	Vec4 btn_exit(part * 2 - w / 2, y, w, h);
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		SwitchTo<Exit>();
	if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		// hover highlight
		 if (InRect(mouse, btn_menu)) {
			if (m_highlight != LoseScreen::HL::Menu)
				m_update = true;
			m_highlight = LoseScreen::HL::Menu;
		}
		else if (InRect(mouse, btn_exit)) {
			if (m_highlight != LoseScreen::HL::Exit)
				m_update = true;
			m_highlight = LoseScreen::HL::Exit;
		}
		else {
			if (m_highlight != LoseScreen::HL::None)
				m_update = true;
			m_highlight = LoseScreen::HL::None;
		}
	}
	// button press sound
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		m_press_x = event.mouse.x;
		m_press_y = event.mouse.y;
		press.x = m_press_x;
		press.y = m_press_y;
		if (InRect(press, btn_menu))
			m_btn_sound.Play();
		else if (InRect(press, btn_exit))
			m_btn_sound.Play();
	}
	// confirm selection (press down and up at the same button)
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		if (InRect(press, btn_menu) && InRect(mouse, btn_menu))
			SwitchTo<Title>();
		else if (InRect(press, btn_exit) && InRect(mouse, btn_exit))
			SwitchTo<Exit>();
	}
}

void LoseScreen::Draw()
{
	double part = WINDOW_WIDTH / 3.;
	double btnw = WINDOW_WIDTH / 6., btnh = WINDOW_HEIGHT / 4.;
	Color white(255, 255, 255);
	Color black(0, 0, 0);

	ClearToColor(black);
	m_font.SetSize(60);
	m_font.SetColor(white);
	m_font.Draw("DEFEAT", {WINDOW_WIDTH / 2., WINDOW_HEIGHT / 4.});
	DrawRect({part - btnw / 2, WINDOW_HEIGHT / 2.},     {btnw, btnh}, white, 5);
	DrawRect({part * 2 - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white, 5);
	m_font.SetSize(48);
	m_font.Draw("Menu", {part, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_font.Draw("Exit",  {part * 2, WINDOW_HEIGHT * 2.5 / 4 - 36});
	m_sad_face.DrawRotate({part / 3, WINDOW_HEIGHT / 8., 256, 256}, -0.5);
	m_sad_face.DrawRotate({part * 2 + 24, WINDOW_HEIGHT / 8., 256, 256}, 0.5);
	m_font.SetColor(black);
	switch (m_highlight) {
		case LoseScreen::HL::Menu:
			DrawRectFilled({part - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("Menu", {part, WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		case LoseScreen::HL::Exit:
			DrawRectFilled({part * 2 - btnw / 2, WINDOW_HEIGHT / 2.}, {btnw, btnh}, white);
			m_font.Draw("Exit", {part * 2., WINDOW_HEIGHT * 2.5 / 4 - 36});
			break;
		default:
			break;
	}
	Screen::ActiveScreen->m_update = false;
}

Exit::Exit() { m_state = Screen::Type::Exit; }

Test::Test()
{
	m_update = true;
}

Test::~Test()
{
	Log::Info("Leaving Test Screen");
}

void Test::Process(ALLEGRO_EVENT_QUEUE *&event_queue)
{
	ALLEGRO_EVENT event;

	al_wait_for_event(event_queue, &event);

	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		SwitchTo<Exit>();
		return ;
	}
	else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		SwitchTo<Title>();
		return ;
	}
}

void Test::Draw(void)
{
	ClearToColor({0, 0, 0});
	// API testing
	m_text.SetText("assets/fonts/pixel.ttf", 60, {255, 255, 255});
	m_text.Draw("Credit", {WINDOW_WIDTH / 2., WINDOW_HEIGHT / 16.});
	m_text.SetSize(36);
	m_text.DrawLeft("Zombie Pixel Art by Mirkan Caliskan.", {WINDOW_WIDTH / 10., WINDOW_HEIGHT / 8. + 50.});
	m_text.DrawLeft("Pixel Skeleton by PiskelKnight.", {WINDOW_WIDTH / 10., WINDOW_HEIGHT / 8. + 150});
	m_text.DrawLeft("Death Tyrant Ary by Katie Hulmes, Redrawn by me.", {WINDOW_WIDTH / 10., WINDOW_HEIGHT / 8. + 250});
	m_text.DrawLeft("Sound Effects and Font from Minecraft.",  {WINDOW_WIDTH / 10., WINDOW_HEIGHT / 8. + 350});
	m_text.DrawLeft("Song \"Welcome to Wildemount\" by Critical Role.", {WINDOW_WIDTH / 10., WINDOW_HEIGHT / 8. + 450});
	m_text.DrawLeft("Song \"Long May He Reign\" by The Rise Undaunted.", {WINDOW_WIDTH / 10., WINDOW_HEIGHT / 8. + 550});
}

About::About()
{
	m_highlight = false;
	m_update = true;
}

void About::Process(ALLEGRO_EVENT_QUEUE* &event_queue)
{
	ALLEGRO_EVENT event;
	Vec4 btn;

	btn.x = WINDOW_WIDTH / 2. - WINDOW_WIDTH / 12.;
	btn.y = WINDOW_HEIGHT / 2. + 150;
	btn.w = WINDOW_WIDTH / 6.;
	btn.h = WINDOW_HEIGHT / 5.;
	al_wait_for_event(event_queue, &event);
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		SwitchTo<Exit>();
		return ;
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		m_mouse.x = event.mouse.x;
		m_mouse.y = event.mouse.y;
		if (InRect(m_mouse, btn)) {
			if (m_highlight == false)
				m_update = true;
			m_highlight = true;
		}
		else {
			if (m_highlight == true)
				m_update = true;
			m_highlight = false;
		}
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		m_press.x = event.mouse.x;
		m_press.y = event.mouse.y;
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		if (InRect(m_mouse, btn) && InRect(m_press, btn)) {
			SwitchTo<Title>();
			return ;
		}
	}
}

void About::Draw(void)
{
	Bitmap bash, shield;
	Bitmap zomb, skele;
	Bitmap shop;
	Color white(255, 255, 255);
	Color black(0, 0, 0);
	Text font("assets/fonts/pixel.ttf", 60, white);

	bash.SetBitmap("assets/images/bash_card.png");
	shield.SetBitmap("assets/images/block_card.png");
	zomb.SetBitmap("assets/images/zombie.png");
	skele.SetBitmap("assets/images/skeleton.png");
	shop.SetBitmap("assets/images/shop.png");
	ClearToColor(black);
	font.SetSize(60);
	font.SetColor(white);
	font.Draw("About", {WINDOW_WIDTH / 2., WINDOW_HEIGHT / 9.});
	DrawRect({WINDOW_WIDTH / 2. - WINDOW_WIDTH / 12., WINDOW_HEIGHT / 2. + 150}, {WINDOW_WIDTH / 6., WINDOW_HEIGHT / 5.}, white, 5);
	font.SetSize(48);
	font.Draw("Menu", {WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2.5 / 4 - 50 + 150});
	shield.Draw({100, 200});
	bash.Draw({150, 250});
	shop.Draw({WINDOW_WIDTH / 2. - 64, WINDOW_HEIGHT / 3.5});
	zomb.Draw({850, WINDOW_HEIGHT / 3.});
	skele.Draw({1050, WINDOW_HEIGHT / 4.});
	font.Draw("Energy Cost", {200, 125});
	font.Draw("Play cards", {300, 525});
	font.Draw("Buy Cards", {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2.});
	font.Draw("Kill Monsters", {1050, 550});
	font.Draw("Overkill for Gold", {1050, 100});
	if (m_highlight) {
		DrawRectFilled({WINDOW_WIDTH / 2. - WINDOW_WIDTH / 12., WINDOW_HEIGHT / 2. + 150}, {WINDOW_WIDTH / 6., WINDOW_HEIGHT / 5.}, white);
		font.SetColor(black);
		font.Draw("Menu", {WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2.5 / 4 - 50 + 150});
	}
	m_update = false;
}
