#ifndef _rootgameobject_
#define _rootgameobject_

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <OgreMath.h>

class RootGameObject{

	protected:
		Ogre::SceneNode* node;

	public:
		RootGameObject(Ogre::SceneManager*, const std::string);

		void translate(const Ogre::Vector3&);
		void translate(float x, float y, float z);
		void setPosition(const Ogre::Vector3&);
		const Ogre::Vector3 getPosition() const;
		void setWorldPosition(const Ogre::Vector3&);
		const Ogre::Vector3 getWorldPosition() const;
		void rotate(const Ogre::Vector3&, const Ogre::Radian&);

		void scale (const Ogre::Vector3&);
		void setScale(const Ogre::Vector3&);
		

};

#endif
