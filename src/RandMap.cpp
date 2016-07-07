#include "RandMap.hh"

RandMap::RandMap(int sizeY, int sizeX)
{
  this->_baseNoise = new PerlinNoise(static_cast<int>(sizeX), static_cast<int>(sizeY));
  this->_sizeX = sizeX;
  this->_sizeY = sizeY;
  this->_loaded = false;
  this->regenMap();
}

RandMap::RandMap()
{
  this->_baseNoise = new PerlinNoise(static_cast<int>(300), static_cast<int>(250));
  this->_sizeX = 300;
  this->_sizeY = 250;
  this->_loaded = false;
  this->regenMap();
}


void	RandMap::DestructIsland(int x, int y, float target, float t2)
{

  this->_map[y][x] = target;
  if (x < this->_sizeX - 1 && this->_map[y][x + 1] == t2)
    this->DestructIsland(x + 1, y, target, t2);
  if (x > 0 && this->_map[y][x - 1] == t2)
    this->DestructIsland(x - 1, y, target, t2);
  if (y < this->_sizeY - 1 && this->_map[y + 1][x] == t2)
    this->DestructIsland(x, y + 1, target, t2);
  if (y > 0 && this->_map[y - 1][x] == t2)
    this->DestructIsland(x, y - 1, target, t2);
}

void	RandMap::regenMap()
{
  int	a, b = 0;
  float	**tmp = new float*[static_cast<int>(this->_sizeY) + 2];
  for (int i = 0; i < this->_sizeY + 2; i++)
    tmp[i] = new float[static_cast<int>(this->_sizeX) + 2];
  this->_baseNoise->newNoise();
  this->_map = (this->_baseNoise->getMap());
  for (int y = 0; y < this->_sizeY; y++)
    {
      for (int x = 0; x < this->_sizeX; x++)
	{
	  if (this->_map[y][x] < 120)
	    this->_map[y][x] = 1;
	  else
	    this->_map[y][x] = 0;
	}
    }
  for (int y = 0; y < this->_sizeY; y++)
    {
	for (int x = 0; x < this->_sizeX; x++)
	{
	  if (y < 150)
	    {
	      if (this->_map[y][x] == 1)
		this->DestructIsland(x, y, 0, 1);
	    }
            if (x < 20 || x > this->_sizeX - 20)
              {
  	      if (this->_map[y][x] == 1)
  		this->DestructIsland(x, y, 0, 1);
              }
	}
    }
  std::reverse(&this->_map[0], &this->_map[static_cast<int>(this->_sizeY)]);
}



void	RandMap::drawMapEntity(Ogre::SceneManager &SceneMgr, CollisionManager &CollisionMgr, bool empty)
{
  int	mapnb;
  Ogre::SceneNode	*node;
  if (!this->_loaded)
    {
      RandMap	map2(this->_sizeY, this->_sizeX);
      float	**tab2	= map2.getMap();

      for (int i = 0; i < this->_sizeY; i++)
	{
	  for (int y = 0; y < this->_sizeX; y++)
	    {
	      if (tab2[i][y] ==  1)
		this->_map[i][y] = 1;
	    }
	}
    }
  float	**tab = this->_map;

  if (empty)
    this->emptyBlocks(tab);
  this->optimise(tab);
  Ogre::StaticGeometry	*geo = new Ogre::StaticGeometry(&SceneMgr, "Map");
  geo->setRegionDimensions(Ogre::Vector3(this->_sizeY, this->_sizeX, 100));
  geo->setOrigin(Ogre::Vector3(-this->_sizeY/2, 0, -this->_sizeX/2));
  PhysBody	*tmpBody;
  for (int i = 0; i < this->_sizeY; i++)
    {
      for (int y = 0; y < this->_sizeX; y++)
	{

              //if (i < 2)
              if (tab[i][y] > 0)
		{
		  mapnb++;
		  Ogre::Entity	*head = SceneMgr.createEntity("block_map" + patch::to_string(i) + patch::to_string(y) + patch::to_string(rand()), "cube.mesh");
      head->setMaterialName("Grass/GrassFloor");
		  if (i == 0 && y == 0)
		    node = SceneMgr.getRootSceneNode()->createChildSceneNode("nodeTete" + patch::to_string(i), Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
		  else
		    node = SceneMgr.getRootSceneNode()->createChildSceneNode(Ogre::Vector3(y , i , 0));
		  geo->addEntity(head, Ogre::Vector3( y * 4  + (tab[i][y] - 1) * 2, i * 4  + (tab[i][y] - 1) * 2, 0), Ogre::Quaternion::IDENTITY, Ogre::Vector3(0.04 * tab[i][y], 0.04 * tab[i][y], 0.5));
		  tmpBody = new PhysBody(PhysBody::RECTANGLE, 0.04 * tab[i][y] * 100, 0.04 * tab[i][y] * 100, 0, "nodeTete" + patch::to_string(i));
		  CollisionMgr.addStaticEntity(tmpBody, Ogre::Vector3( y * 4  + (tab[i][y] - 1) * 2, i * 4  + (tab[i][y] - 1) * 2, 0));
		  //	  	  geo->addEntity(head, Ogre::Vector3( y * 4  + (1 - 1) * 2, i * 4  + (1 - 1) * 2, 0), Ogre::Quaternion::IDENTITY, Ogre::Vector3(0.04 * 1, 0.04 * 1, 0.5));
		  //	  	  tmpBody = new PhysBody(PhysBody::RECTANGLE, 0.04 * 1 * 100, 0.04 * 1 * 100, 0, "nodeTete" + patch::to_string(i));
		  //	  	  CollisionMgr.addStaticEntity(tmpBody, Ogre::Vector3( y * 4 , i * 4 , 0));
		}
	}
    }
  geo->build();
}

void	RandMap::LoadMap(float **tab)
{
  this->_map = tab;
  this->_loaded = true;
}


void	RandMap::emptyBlocks(float **tab)
{
    for (int y = 0; y < this->_sizeY; y++) {
	for (int x = 0; x < this->_sizeX; x++) {
	  if (tab[y][x] == 1 && (x > 0 && (tab[y][x - 1] == 1 || tab[y][x - 1] == 2)) && (x < this->_sizeX - 1&& (tab[y][x + 1] == 1 || tab[y][x + 1] == 2)) && (y > 0 && (tab[y - 1][x] == 1 || tab[y - 1][x] == 2)) && (y < this->_sizeY - 1 && (tab[y + 1][x] == 1 || tab[y + 1][x] == 2)))
	    tab[y][x] = 2;
	}
    }
    for (int y = 0; y < this->_sizeY; y++) {
	for (int x = 0; x < this->_sizeX; x++) {
	  if (tab[y][x] == 2)
	    tab[y][x] = 0;
	}
    }

}

void	RandMap::optimise(float **tab)
{
    for (int y = 0; y < this->_sizeY; y++) {
	for (int x = 0; x < this->_sizeX; x++) {
	  if (tab[y][x] == 1 && tab[y + 1][x] == 1 && tab[y + 1][x + 1] == 1 && tab[y][x + 1] == 1)
	    {
	      tab[y][x] = 2;
	      tab[y + 1][x] = 0;
	      tab[y + 1][x + 1] = 0;
	      tab[y][x + 1] = 0;
	    }
	}
    }
}

float**	RandMap::getMap() const
{
  return this->_map;
}

float	RandMap::getSizeY() const
{
  return this->_sizeY;
}

float	RandMap::getSizeX() const
{
  return this->_sizeX;
}
