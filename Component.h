#ifndef _component_h_
#define _component_h_

#include <memory>
#include "UpdateInfo.h"

// Forward declarations.
class GameObject;

class Component {
protected:
	std::weak_ptr<GameObject> gameObject;

public:
	Component() : gameObject() {}
	Component(std::weak_ptr<GameObject> go) : gameObject(go) {}
	virtual void update(const UpdateInfo& info) const = 0;
	virtual ~Component() {}
};

#endif
