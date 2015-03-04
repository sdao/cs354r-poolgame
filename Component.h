#ifndef _component_h_
#define _component_h_

#include <memory>
#include "UpdateInfo.h"

// Forward declarations.
class GameObject;
class PhysicsCollider;

class Component {
protected:
	std::weak_ptr<GameObject> gameObject;

public:
	Component() : gameObject() {}
	Component(std::weak_ptr<GameObject> go) : gameObject(go) {}
	virtual void update(const UpdateInfo& info) {}
	virtual ~Component() {}
	std::weak_ptr<GameObject> getGameObject() const {
		return gameObject;
	}
	virtual void didCollide(PhysicsCollider& collider) {}
};

#endif
