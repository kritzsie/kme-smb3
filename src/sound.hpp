#pragma once

#include <SFML/Audio.hpp>

#include <string>

#include <cstddef>

namespace kme {
class Sound {
public:
  static constexpr std::size_t MAX_VOICES = 32;

private:
  sf::Sound voices[MAX_VOICES];

public:
  bool play(std::size_t index, std::string name);
  std::size_t play(std::string name);
  std::size_t playLoop(std::string name);
  void setLoop(std::size_t index, bool loop);
  bool pause(std::size_t index);
  std::size_t pause();
  bool resume(std::size_t index);
  std::size_t resume();
  bool stop(std::size_t index);
  std::size_t stop();
};
}
