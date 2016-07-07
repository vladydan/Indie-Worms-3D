#include "World.hh"

World::World(CollisionManager	*collisionMgr, Ogre::SceneManager *scene, int x, int y, float **map)
{
  Ogre::Plane			back(Ogre::Vector3::UNIT_Y, 0);
  Ogre::SceneNode		*node;

  this->_scene = scene;
  this->_collisionMgr = collisionMgr;
  this->_scene->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  if (map != nullptr)
    this->_map.LoadMap(map);
  this->_map.drawMapEntity(*this->_scene, *this->_collisionMgr, false);
  Ogre::MeshManager::getSingleton().createPlane("back", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, back, x, y, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
  Ogre::Entity *backMap = this->_scene->createEntity("Fond", "back");
  node = this->_scene->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(00, 0, 0));
  node->attachObject(backMap);
  node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(90));
  node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(180));
  node->translate(Ogre::Vector3(650, 200, -100));
  //node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(90));
  backMap->setMaterialName("Background/Sky");
  this->setWater();
  Ogre::ManualObject *circle = scene->createManualObject("CrossHair");
  float const radius = 2;
  float const accuracy = 55;
  circle->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

  unsigned point_index = 0;
  for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy) {
      circle->position(radius * cos(theta), radius * sin(theta), 0);
      circle->index(point_index++);
  }
  circle->index(0); // Rejoins the last point to the first.

  circle->end();

  Ogre::SceneNode *sceneNode = scene->getRootSceneNode()->createChildSceneNode("CrossHair");
  sceneNode->attachObject(circle);
  sceneNode->setPosition(10, 100, 0);
}

void		World::setWater()
{
  Ogre::Entity	*WaterEntity;
  Ogre::Plane 	WaterPlane;

  WaterPlane.normal = Ogre::Vector3::UNIT_Y;
  WaterPlane.d = -1.5;
  Ogre::MeshManager::getSingleton().createPlane(
				  "WaterPlane",
				  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				  WaterPlane,
				  2000, 2000,
				  20, 20,
				  true, 1,
				  10, 10,
				  Ogre::Vector3::UNIT_Z);
  WaterEntity = this->_scene->createEntity("water", "WaterPlane");
  WaterEntity->setMaterialName("Water/water1");
  Ogre::SceneNode *waterNode =
  this->_scene->getRootSceneNode()->createChildSceneNode("WaterNode");
  waterNode->attachObject(WaterEntity);
  waterNode->translate(300, 0, 0);

  Ogre::Light* WaterLight = this->_scene->createLight("WaterLight");
  WaterLight->setType( Ogre::Light::LT_DIRECTIONAL );
  WaterLight->setDirection( 0, -100, 0 );
  this->_waterLevel = 0;
}

void	World::riseWater()
{
  this->_waterLevel++;
}

unsigned int	World::getWaterLevel() const
{
  return this->_waterLevel;
}

const RandMap &World::getRandMap() const
{
  return this->_map;
}
