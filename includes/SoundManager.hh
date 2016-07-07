#pragma once

#include <SFML/Audio.hpp>

#define SOUND_DIRECTORY "ressources/sounds/"

class SoundManager
{
  public:
    SoundManager();
    ~SoundManager();
    bool addFile(std::string const &musicName);
    void play(std::string const &musicName);
    std::string truncateName(std::string const &name) const;
    void stop(std::string const &musicName);

  private:
    std::map<std::string, sf::Music> _musicList;
};
