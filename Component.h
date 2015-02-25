#ifndef _component_h_
#define _component_h_

class Component {
public:
	virtual void update(float deltaTime) const = 0;
	virtual ~Component() {}
};

#endif
