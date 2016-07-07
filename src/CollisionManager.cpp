#include "CollisionManager.hh"

CollisionManager::CollisionManager(int chunkSize, Ogre::SceneManager *scene)
{
  this->_chunkSize = chunkSize;
  this->_maxChunk = 300 / chunkSize;
  this->_StaticElems.resize(this->_maxChunk);
  this->_scene = scene;
}

void	CollisionManager::addEntity(PhysBody *body, const std::string &name)
{
  this->_EntityList[name] = body;
}

void	CollisionManager::addStaticEntity(PhysBody *body, Ogre::Vector3 pos)
{
  while (static_cast<int>(pos.x / (300/this->_chunkSize)) >= this->_maxChunk)
    {
      std::cerr << "Added" << std::endl;
      std::cerr << " Before ===MaxChunck " << this->_maxChunk << " Pos :" << static_cast<int>(pos.x / (300/this->_chunkSize)) << std::endl;
      this->_maxChunk++;
      this->_StaticElems.resize(this->_maxChunk);
    }
  this->_StaticElems[static_cast<int>(pos.x / (300/this->_chunkSize))].push_back(std::make_pair(pos, body));
  if (static_cast<int>(pos.x / (300/this->_chunkSize)) > 1)
    this->_StaticElems[static_cast<int>(pos.x / (300/this->_chunkSize)) -1].push_back(std::make_pair(pos, body));
}

bool	CollisionManager::isCollidedToStatic(const std::string &name, CollisionManager::Direction dir)
{
  PhysBody	*tmp = NULL;
  Ogre::Node	*node;
  float		posX, posY, sizeX, sizeY;

  for (entityMap::iterator it = this->_EntityList.begin(); it != this->_EntityList.end(); it++)
    {
      if (name.compare(it->first) == 0)
        tmp = it->second;
    }
  if (!tmp)
    {
      std::cerr << "Can't find Entity " << name << " PhysBody" << std::endl;
      return (false);
    }
  node = this->_scene->getRootSceneNode()->getChild(name);

  if (dir == CollisionManager::DOWN)
    {
      posX = node->getPosition().x + 1;
      posY = node->getPosition().y - (tmp->getSizeY()/2 - 2);
      sizeX = tmp->getSizeX() - 2;
      sizeY = 2;
    }
  else if (dir == CollisionManager::UP)
    {
      posX = node->getPosition().x + 1;
      posY = node->getPosition().y + (tmp->getSizeY()/2 - 2);
      sizeX = tmp->getSizeX() - 2;
      sizeY = 2;
    }
  else if (dir == CollisionManager::LEFT)
    {
      posX = node->getPosition().x - (tmp->getSizeX()/2 - 2);
      posY = node->getPosition().y + (tmp->getSizeY()/2 - 1);
      sizeX = 2;
      sizeY = tmp->getSizeY() - 1;
    }
  else if (dir == CollisionManager::RIGHT)
    {
      posX = node->getPosition().x + (tmp->getSizeX()/2 - 2);
      posY = node->getPosition().y + (tmp->getSizeY()/2 - 1);
      sizeX = 2;
      sizeY = tmp->getSizeY() - 1;
    }
  else
    {
      posX = node->getPosition().x;
      posY = node->getPosition().y;
      sizeX = tmp->getSizeX();
      sizeY = tmp->getSizeY();
    }
    /*if ((it2->first.x - it2->second->getSizeX()/2) < (node->getPosition().x - tmp->getSizeX()/2)+ tmp->getSizeX() &&
        (it2->first.x - it2->second->getSizeX()/2) + it2->second->getSizeY() > node->getPosition().x &&
        (it2->first.y - it2->second->getSizeY()/2) < node->getPosition().y + tmp->getSizeY() &&
        (it2->first.y - it2->second->getSizeY()/2) + it2->second->getSizeY() > (node->getPosition().y - tmp->getSizeY()/2))*/
  if (static_cast<int>(node->getPosition().x / (300/this->_chunkSize)) <= 0 ||
    static_cast<int>(node->getPosition().x / (300/this->_chunkSize)) >= this->_maxChunk)
    return false;
  for (std::vector<std::pair<Ogre::Vector3, PhysBody*>>::iterator it2 = this->_StaticElems[static_cast<int>(node->getPosition().x / (300/this->_chunkSize))].begin(); it2 != this->_StaticElems[static_cast<int>(node->getPosition().x / (300/this->_chunkSize))].end(); it2++)
    {
      if (this->collide(it2->first.x, it2->second->getSizeX(), it2->first.y, it2->second->getSizeY(), posX, sizeX, posY, sizeY))
        return true;
    }
  return(false);
}

bool	CollisionManager::isCollided(const std::string &name, const std::string &name2)
{
  PhysBody	*tmp = NULL;
  PhysBody	*tmp2 = NULL;
  Ogre::Node	*node;
  Ogre::Node	*node2;

  for (entityMap::iterator it = this->_EntityList.begin(); it != this->_EntityList.end(); it++)
    {
      if (name.compare(it->first) == 0)
	tmp = it->second;
      if (name2.compare(it->first) == 0)
	tmp2 = it->second;
    }
  if (!tmp || !tmp2)
    {
      std::cerr << "Can't find Entity " << name << " PhysBody" << std::endl;
      return (false);
    }
  node = this->_scene->getRootSceneNode()->getChild(name);
  node2 = this->_scene->getRootSceneNode()->getChild(name2);
  return (collide(node->getPosition().x, tmp->getSizeX(), node->getPosition().y, tmp->getSizeY(), node2->getPosition().x, tmp2->getSizeX(), node2->getPosition().y, tmp2->getSizeY()));
}


bool	CollisionManager::collide(float x1, float sizeX1, float y1, float sizeY1, float x2, float sizeX2, float y2, float sizeY2)
{
  return ((x1 - sizeX1/2) < (x2 - sizeX2/2)+ sizeX2 && (x1 - sizeX1/2) + sizeY1 > x2 &&
	  (y1 - sizeY1/2) < y2 + sizeY2 && (y1 - sizeY1/2) + sizeY1 > (y2 - sizeY2/2));
}

void  CollisionManager::clear()
{
  _EntityList.clear();

  for (StaticBodyList::iterator it = this->_StaticElems.begin(); it != this->_StaticElems.end(); it++)
    {
      it->clear();
    }
}
