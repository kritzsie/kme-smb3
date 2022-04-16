#include "music.hpp"

#include "util/file.hpp"

#include <string>
#include <vector>

namespace kme {
MusicStream::MusicStream(std::size_t buffer_size, unsigned int channels, unsigned int sample_rate)
: sf::SoundStream(), buffer(buffer_size) {
  initialize(channels, sample_rate);
}

MusicStream::MusicStream() : MusicStream(2048, 2, 44100) {}

MusicStream::~MusicStream() {
  destroy();
}

void MusicStream::destroy() {
  if (gme != nullptr) {
    gme_delete(gme);
    gme = nullptr;
  }
}

bool MusicStream::openFromFile(std::string name) {
  using namespace std::literals;

  destroy();

  std::string path = "/music/"s + name;
  std::vector<char> buffer = util::readFile(path);
  if (gme_err_t error = gme_open_data(buffer.data(), buffer.size(), &gme, getSampleRate())) {
    return false;
  }

  try {
    onOpen();
  }
  catch (std::runtime_error ex) {
    return false;
  }

  return true;
}

void MusicStream::onOpen() {
#if GME_VERSION >= 0x000602
  gme_set_autoload_playback_limit(gme, 0);
#endif
  setTempo(1.0);

  if (gme_err_t error = gme_start_track(gme, 0)) {
    throw std::runtime_error(error);
  }
}

double MusicStream::getTempo() const {
  return tempo;
}

void MusicStream::setTempo(double tempo) {
  this->tempo = tempo;
  gme_set_tempo(gme, tempo);
}

bool MusicStream::onGetData(Chunk& data) {
  if (gme == nullptr) {
    return false;
  }

  if (gme_err_t error = gme_play(gme, buffer.size(), buffer.data())) {
    return false;
  }

  data.samples = buffer.data();
  data.sampleCount = buffer.size();
  return true;
}

void MusicStream::onSeek(sf::Time time) {
  gme_seek(gme, time.asMilliseconds());
}

Music::Music() : stream() {}

Music::~Music() {}

bool Music::open(std::string name, bool start) {
  bool success = stream.openFromFile(name);

  if (success and start) {
    play();
  }

  return success;
}

bool Music::open(std::string name) {
  return open(name, true);
}

void Music::play() {
  stream.play();
}

void Music::pause() {
  stream.pause();
}

void Music::stop() {
  stream.stop();
}

double Music::getTempo() const {
  return stream.getTempo();
}

void Music::setTempo(double tempo) {
  stream.setTempo(tempo);
}

double Music::getVolume() const {
  return stream.getVolume() / 100.0;
}

void Music::setVolume(double volume) {
  stream.setVolume(volume * 100.0);
}
}
