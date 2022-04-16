#include "sound.hpp"

#include "assetmanager.hpp"

#include <SFML/Audio.hpp>

#include <cstddef>

namespace kme {
bool Sound::play(std::size_t voice_id, std::string name) {
  const sf::SoundBuffer& sound = sfx.getSound(name);
  sf::Sound& voice = voices[voice_id];

  if (voice.getBuffer() == &sound) {
    voice.play();
    return true;
  }
  else if (voice.getStatus() == sf::SoundSource::Status::Stopped) {
    voice.setBuffer(sound);
    voice.play();
    return true;
  }

  return false;
}

std::size_t Sound::play(std::string name) {
  const sf::SoundBuffer& sound = sfx.getSound(name);

  for (std::size_t i = 0; i < MAX_VOICES - 1; i++) {
    if (play(i, name)) {
      setLoop(i, false);
      return i;
    }
  }

  if (voices[MAX_VOICES - 1].getBuffer() != &sound) {
    voices[MAX_VOICES - 1].setBuffer(sound);
  }
  voices[MAX_VOICES - 1].play();
  return MAX_VOICES - 1;
}

std::size_t Sound::playLoop(std::string name) {
  std::size_t index = play(name);
  setLoop(index, true);
  return index;
}

void Sound::setLoop(std::size_t index, bool loop) {
  voices[index].setLoop(loop);
}

bool Sound::pause(std::size_t voice_id) {
  sf::Sound& voice = voices[voice_id];
  if (voice.getStatus() != sf::SoundSource::Status::Paused) {
    voice.pause();
    return true;
  }
  return false;
}

std::size_t Sound::pause() {
  std::size_t count = 0;
  for (std::size_t i = 0; i < MAX_VOICES; i++) {
    if (pause(i)) {
      count++;
    }
  }
  return count;
}

bool Sound::resume(std::size_t voice_id) {
  sf::Sound& voice = voices[voice_id];
  if (voice.getStatus() == sf::SoundSource::Status::Paused) {
    voice.play();
    return true;
  }
  return false;
}

std::size_t Sound::resume() {
  std::size_t count = 0;
  for (std::size_t i = 0; i < MAX_VOICES; i++) {
    if (resume(i)) {
      count++;
    }
  }
  return count;
}

bool Sound::stop(std::size_t voice_id) {
  sf::Sound& voice = voices[voice_id];
  if (voice.getStatus() != sf::SoundSource::Status::Stopped) {
    voice.stop();
    return true;
  }
  return false;
}

std::size_t Sound::stop() {
  std::size_t count = 0;
  for (std::size_t i = 0; i < MAX_VOICES; i++) {
    if (stop(i)) {
      count++;
    }
  }
  return count;
}
}
