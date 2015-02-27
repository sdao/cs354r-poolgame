#ifndef _rootgameobject_
#define _rootgameobject_

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <OgreMath.h>

class RootGameObject{

	protected:
		Ogre::SceneNode* node;
		int tag;

	public:
		RootGameObject(Ogre::SceneManager*, const std::string);

		void translate(const Ogre::Vector3&, 
					   const Ogre::Node::TransformSpace = Ogre::Node::TransformSpace::TS_PARENT);
		void translate(float x, float y, float z,
					   const Ogre::Node::TransformSpace = Ogre::Node::TransformSpace::TS_PARENT);

		void setPosition(const Ogre::Vector3&);
		const Ogre::Vector3 getPosition() const;
		void setWorldPosition(const Ogre::Vector3&);
		const Ogre::Vector3 getWorldPosition() const;

		void rotate(const Ogre::Vector3&, const Ogre::Radian&, const Ogre::Node::TransformSpace = Ogre::Node::TransformSpace::TS_PARENT);
		void yaw(const Ogre::Degree&);
		void pitch(const Ogre::Degree&);
		void roll(const Ogre::Degree&);

		void scale (const Ogre::Vector3&);
		void setScale(const Ogre::Vector3&);
		
		Ogre::Vector3 localToWorldPosition(const Ogre::Vector3& v)
			const;
		Ogre::Vector3 worldToLocalPosition(const Ogre::Vector3& v)
			const;

		void setTag(int newTag);
		int getTag() const;
};

#endif
