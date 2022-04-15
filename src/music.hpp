#pragma once

#include <SFML/Audio.hpp>

#include <gme/gme.h>

namespace kme {
class MusicStream final : public sf::SoundStream {
public:
  MusicStream(std::size_t buffer_size, unsigned int channels, unsigned int sample_rate);
  MusicStream();
  ~MusicStream();

  bool openFromFile(const char* name);

  double getTempo() const;
  void setTempo(double tempo);

protected:
  bool onGetData(Chunk& data);
  void onSeek(sf::Time time);

private:
  void onOpen();
  void destroy();

private:
  Music_Emu* gme = nullptr;
  double tempo = 1.0;
  std::vector<short> buffer;
};

class Music {
public:
  Music();
  ~Music();

  bool open(const char* path);
  bool open(const char* path, bool start_playing);

  void play();
  void pause();
  void stop();

  double getTempo() const;
  void setTempo(double tempo);

  double getVolume() const;
  void setVolume(double volume);

private:
  MusicStream stream;
};
}
