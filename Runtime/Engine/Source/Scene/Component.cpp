#include "Scene/Component.h"
#include "Scene/SceneGraph.h"

using namespace Blueshift;
using namespace Scene;

Component* Component::Handle::operator->() const {
	uint8_t* bytes = static_cast<uint8_t*>(graph->components[type].ptr);
	bytes += idx * graph->components[type].type_size;
	return reinterpret_cast<Component*>(bytes);
}