#pragma once

#include "PerlinNoise.hh"
#include "data.hh"
#include "CollisionManager.hh"

class	RandMap{
private:
  PerlinNoise	*_baseNoise;
  float		**_map;
  float		_sizeX;
  float		_sizeY;
  bool		_loaded;
  void		DestructIsland(int x, int  y, float target, float t2);
  void	newIsland(Ogre::Vector3 lastPoint, Ogre::Vector3 nextPoint, Ogre::Vector3 startPoint, Ogre::ManualObject *object, float **tab, int y, int x);

  public:
  RandMap();
  RandMap(int sizeX, int sizeY);
  void		regenMap();
  float		**getMap() const;
  float		getSizeX() const;
  float		getSizeY() const;
  void		optimise(float **tab);
  void		emptyBlocks(float **tab);
  void		drawMapEntity(Ogre::SceneManager &SceneMgr, CollisionManager &CollisionMgr, bool empty = true);
  void		LoadMap(float **map);

};
