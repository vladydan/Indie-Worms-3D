#pragma once

#include "PhysBody.hh"

typedef	std::map<std::string, PhysBody*> 	entityMap;
typedef std::vector<std::vector<std::pair<Ogre::Vector3, PhysBody*>	> > StaticBodyList;

class	CollisionManager{
public:
    typedef enum {
      UP,
      DOWN,
      LEFT,
      RIGHT,
      NONE
    }		Direction;

  CollisionManager(int chunkSize, Ogre::SceneManager *scene);

  void	addEntity(PhysBody *, const std::string &name);
  void	addStaticEntity(PhysBody *, Ogre::Vector3 position);

  bool	isCollidedToStatic(const std::string &name, CollisionManager::Direction = CollisionManager::NONE);
  bool	isCollided(const std::string &name, const std::string &name2);
  void  clear();
private:
  bool			collide(float x1, float sizeX1, float y1, float sizeY1,
				float x2, float sizeX2, float y2, float sizeY2);
  Ogre::SceneManager	*_scene;
  int			_maxChunk;
  int			_chunkSize;
  entityMap		_EntityList;
  StaticBodyList	_StaticElems;
};
