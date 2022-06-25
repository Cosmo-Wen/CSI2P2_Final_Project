#ifndef SCREEN_H
#define SCREEN_H

#include "Draw.h"
#include "Audio.h"

class Screen
{
	public:
		enum class Type
		{
			None,
			Init,
			Title,
			Win,
			Lose,
			Exit,
			Test,
		};
		virtual ~Screen();
		static Screen *ActiveScreen;
		virtual void Process(ALLEGRO_EVENT_QUEUE* &event_queue) = 0;
		virtual void Draw(void) = 0;
		virtual void Pause(ALLEGRO_EVENT_QUEUE* &event_queue);
		bool m_update = false;
		Screen::Type m_state = Screen::Type::Init;
};

template<typename ScreenT>
void SwitchTo(void)
{
	if (Screen::ActiveScreen)
		delete Screen::ActiveScreen;
	Screen::ActiveScreen = new ScreenT;
}


class Init: public Screen
{
	public:
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override {}
};

class Title: public Screen
{
	public:
		enum class HL
		{
			None,
			NewGame,
			Continue,
			About,
			Exit
		};
		Title();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override;
	private:
		Text m_font;
		Audio m_btn_sound;
		Title::HL m_highlight = Title::HL::None;
		int m_press_x;
		int m_press_y;
};

class WinScreen: public Screen
{
	public:
		enum class HL
		{
			None,
			Endless,
			SaveMenu,
			SaveExit
		};
		WinScreen();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override;
	private:
		Text m_font;
		Bitmap m_trophy;
		Audio m_btn_sound;
		Audio m_win_bgm;
		WinScreen::HL m_highlight = WinScreen::HL::None;
		int m_press_x;
		int m_press_y;
};

class LoseScreen: public Screen
{
	public:
		enum class HL
		{
			None,
			Menu,
			Exit
		};
		LoseScreen();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override;
	private:
		Text m_font;
		Bitmap m_sad_face;
		Audio m_btn_sound;
		Audio m_lose_bgm;
		LoseScreen::HL m_highlight = LoseScreen::HL::None;
		int m_press_x;
		int m_press_y;
};


class About: public Screen
{
   	public:
   		About();
   		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
   		void Draw(void) override;
	private:
		Vec2 m_mouse, m_press;
		bool m_highlight;
};

class Exit: public Screen
{
	public:
		Exit();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override {}
		void Draw() override {}
};

class Test: public Screen
{
	public:
		Test();
		~Test();
		void Process(ALLEGRO_EVENT_QUEUE* &event_queue) override;
		void Draw(void) override;
	private:
		Bitmap m_pog, m_red;
		Audio m_amogus;
		Text m_text;
		float m_volume = 1; // Note: I imagine there would be an global volume instead of member variables
};

#endif
