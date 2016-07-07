#pragma once

#include "data.hh"
#include "RandMap.hh"
#include "CollisionManager.hh"



class World
{
  public:
    World(CollisionManager*, Ogre::SceneManager*, int, int, float **map);
    void	NewWind();
    void	riseWater();
    const RandMap &getRandMap() const;
    unsigned int	getWaterLevel() const;
    int		getWind()	const;

  private:
    void			setWater();

    CollisionManager	*_collisionMgr;
    int			_wind;
    unsigned int		_waterLevel;
    RandMap		_map;
    Ogre::SceneManager	*_scene;
};
