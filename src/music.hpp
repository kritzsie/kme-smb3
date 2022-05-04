#pragma once

#include <SFML/Audio.hpp>

#include <gme/gme.h>

#include <string>

namespace kme {
class MusicStream final : public sf::SoundStream {
public:
  MusicStream(std::size_t buffer_size, unsigned int channels, unsigned int sample_rate);
  MusicStream();
  ~MusicStream();

  bool openFromFile(std::string name);

  double getTempo() const;
  void setTempo(double tempo);

protected:
  bool onGetData(Chunk& data);
  void onSeek(sf::Time time);

private:
  void onOpen();

private:
  Music_Emu* gme = nullptr;
  double tempo = 1.0;
  std::vector<short> buffer;
};

class Music {
public:
  Music();
  ~Music();

  bool open(std::string path);
  bool open(std::string path, bool start);

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
