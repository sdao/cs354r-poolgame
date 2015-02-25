#ifndef _gameobject_h_
#define _gameobject_h_

#include <vector>
#include <memory>
#include <OgreEntity.h>

#include "RootGameObject.h"
#include "Component.h"

class GameObject : public RootGameObject {

	protected:
		std::vector<std::shared_ptr<Component>> components; // List of generic components.
		Ogre::Entity* mesh; //Entity component

	public:
		/*GameObject(Ogre::SceneManager*,
			   const std::string&,
			   const std::string&);*/

		GameObject(Ogre::SceneManager*,
		           const std::string&,
		           const std::string&,
		           const Ogre::Vector3& = Ogre::Vector3::ZERO,
		           const Ogre::Vector3& = Ogre::Vector3::ZERO,
		           const Ogre::Vector3& = Ogre::Vector3::UNIT_SCALE);

		GameObject(Ogre::SceneManager*,
			   Ogre::Entity* entity,
			   const std::string&,
			   const Ogre::Vector3& = Ogre::Vector3::ZERO,
		           const Ogre::Vector3& = Ogre::Vector3::ZERO,
		           const Ogre::Vector3& = Ogre::Vector3::UNIT_SCALE);

		void update(float deltaTime);
		void setMaterial(const std::string&);
		void addComponent(std::shared_ptr<Component>);
};

#endif
