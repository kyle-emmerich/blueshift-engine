#include "Scene/Component.h"

using namespace Blueshift;
using namespace Scene;

Component::Component(Object* object)
	: object(object), parent(0) {}

Component::~Component() {

}