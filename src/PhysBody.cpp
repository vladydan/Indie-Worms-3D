#include "PhysBody.hh"

PhysBody::PhysBody(PhysBody::Shape shape, float sizeX, float sizeY, float weight, const std::string EntityName)
{
  this->_EntityName = EntityName;
  this->_Weight = weight;
  this->_Shape = shape;
  this->_sizeX = sizeX;
  this->_sizeY = sizeY;
}

PhysBody::PhysBody(PhysBody::Shape shape, float radius, float weight, const std::string EntityName)
{
  this->_EntityName = EntityName;
  this->_Weight = weight;
  this->_Shape = shape;
  this->_Radius = radius;
}

PhysBody::~PhysBody()
{

}

PhysBody::Shape	PhysBody::getShapeType() const
{
  return this->_Shape;
}

float			PhysBody::getSizeX() const
{
  return this->_sizeX;
}

float			PhysBody::getSizeY() const
{
  return this->_sizeY;
}

std::string		PhysBody::getName() const
{
  return this->_EntityName;
}

float			PhysBody::getRadius() const
{
  return this->_Radius;
}

void			PhysBody::setRadius(float radius)
{
  this->_Radius = radius;
}

float			PhysBody::getWeight() const
{
  return this->_Weight;
}

bool	PhysBody::isCollided(PhysBody &target, Ogre::SceneManager *scene)
{
  if (target.getShapeType() == PhysBody::RECTANGLE)
    return (RectOnlyCollision(target, scene));
  return (false);
}

bool	PhysBody::RectOnlyCollision(PhysBody &target, Ogre::SceneManager *scene)
{
  Ogre::Node	*me = scene->getRootSceneNode()->getChild(this->_EntityName);
  Ogre::Node	*targetNode = scene->getRootSceneNode()->getChild(target.getName());

  return ((me->getPosition().x - this->getSizeX()/2) < (targetNode->getPosition().x - target.getSizeX()/2)+ target.getSizeX() &&
	  (me->getPosition().x - this->getSizeX()/2) + this->getSizeY() > targetNode->getPosition().x &&
	  (me->getPosition().y - this->getSizeY()/2)< targetNode->getPosition().y + target.getSizeY() &&
	  (me->getPosition().y - this->getSizeY()/2) + this->getSizeY() > (targetNode->getPosition().y - target.getSizeY()/2));
}
