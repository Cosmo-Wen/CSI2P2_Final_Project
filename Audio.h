#ifndef AUDIO_H
#define AUDIO_H

#include <algorithm>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Main.h"
#include "Util.h"

// Allegro5 Audio API Abstraction
// NOTES:
//   call Audio::Init() before creating any Audio class instances
//   even with addons, Allegro can not load .mp3 files
//   default mixer is initialized after al_reserve_samples() is called

class Audio
{
public:
  Audio() // default constructor
    : m_sample(nullptr), m_sample_instance(nullptr)
  {
   // should call SetAudio explicitly before Play
  }
  Audio(const char *sample_file)
    : m_sample_file(sample_file)
  {
    m_sample = al_load_sample(m_sample_file);
    if (!m_sample)
      Log::Warn("failed to load sample file: %s", m_sample_file);
    // configure sample and attach an instance to mixer
    m_sample_instance = al_create_sample_instance(m_sample);
    al_set_sample_instance_playmode(m_sample_instance, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(m_sample_instance, al_get_default_mixer());
  }
  ~Audio()
  {
    if (m_sample_instance) { // detach before destroying instance
      if (al_get_sample_instance_attached(m_sample_instance))
        al_detach_sample_instance(m_sample_instance);
      al_destroy_sample_instance(m_sample_instance);
    }
    if (m_sample)
      al_destroy_sample(m_sample);
  }
  void Play(void)
  {
    al_stop_sample_instance(m_sample_instance);
    if (!al_play_sample_instance(m_sample_instance))
      Log::Warn("failed to play audio: %s", m_sample_file);
    else
      Log::Info("playing audio:  %s", m_sample_file);
  }
  void Stop(void)
  {
    if (m_sample_instance) {
      Log::Info("stopping audio: %s", m_sample_file);
      al_stop_sample_instance(m_sample_instance);
    }
  }
  void SetAudioOnce(const char *sample_file)
  {
    m_sample_file = sample_file;
    // reload sample
    if (m_sample)
      al_destroy_sample(m_sample);
    m_sample = al_load_sample(m_sample_file);
    if (!m_sample)
      Log::Warn("failed to load sample file: %s", m_sample_file);
    // reload instance
    if (m_sample_instance) { // detach before destroying instance
      if (al_get_sample_instance_attached(m_sample_instance))
        al_detach_sample_instance(m_sample_instance);
      al_destroy_sample_instance(m_sample_instance);
    }
    m_sample_instance = al_create_sample_instance(m_sample);
    al_set_sample_instance_playmode(m_sample_instance, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(m_sample_instance, al_get_default_mixer());
  }

  void SetAudioLoop(const char *sample_file)
  {
    m_sample_file = sample_file;
    // reload sample
    if (m_sample)
      al_destroy_sample(m_sample);
    m_sample = al_load_sample(m_sample_file);
    if (!m_sample)
      Log::Warn("failed to load sample file: %s", m_sample_file);
    // reload instance
    if (m_sample_instance) { // detach before destroying instance
      if (al_get_sample_instance_attached(m_sample_instance))
        al_detach_sample_instance(m_sample_instance);
      al_destroy_sample_instance(m_sample_instance);
    }
    m_sample_instance = al_create_sample_instance(m_sample);
    al_set_sample_instance_playmode(m_sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(m_sample_instance, al_get_default_mixer());
  }

  void SetVolume(double volume)
  {
    volume = std::min(volume, 1.);
    volume = std::max(volume, 0.);
    if (m_sample_instance)
      al_set_sample_instance_gain(m_sample_instance, volume);
  }
private:
  const char *m_sample_file;
  ALLEGRO_SAMPLE *m_sample;
  ALLEGRO_SAMPLE_INSTANCE *m_sample_instance;
public:
  static void Init(void)
  {
    if (!al_install_audio())
      Log::Fatal("failed to install audio addon");
    // maximum number of sample instances allowed to be played at once
    al_reserve_samples(4);
    // default mixer is initialized after reserving samples
    if (!al_get_default_mixer())
      Log::Fatal("default mixer does not exist");
  }
};

#endif
