#include "SoundManager.hh"
#include "UIManager.hh"

SoundManager::SoundManager()
{
  std::list<std::string> *listMusic = UIManager::readDirectory(SOUND_DIRECTORY);
  for (auto &music : *listMusic)
  {
    std::string truncName = this->truncateName(music);
    this->_musicList[truncName].openFromFile(SOUND_DIRECTORY + music);
  }
}

SoundManager::~SoundManager()
{
}

std::string SoundManager::truncateName(std::string const &name) const
{
  std::string truncName;

  size_t dotPosition = name.find_last_of(".");
  if (dotPosition == std::string::npos)
    truncName = name;
  else
    truncName = name.substr(0, dotPosition);
  return truncName;
}

bool SoundManager::addFile(std::string const &musicName)
{
  std::string truncName = this->truncateName(musicName);
  this->_musicList[truncName].openFromFile(musicName);
}

void SoundManager::play(std::string const &musicName)
{
  if (this->_musicList.find(musicName) != this->_musicList.end())
    this->_musicList[musicName].play();
}

void SoundManager::stop(std::string const &musicName)
{
  if (this->_musicList.find(musicName) != this->_musicList.end())
    this->_musicList[musicName].stop();
}
