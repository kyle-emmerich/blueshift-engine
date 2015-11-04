#include "SceneGraph.h"

using namespace Blueshift;
using namespace Scene;

SceneGraph::SceneGraph(Core::Engine* engine) {

}

SceneGraph::~SceneGraph() {

}

Component* SceneGraph::AllocateComponent(std::type_index& type) {
	if (components.find(type) == components.end()) {
		//gotta create a new vector and give it a try
		components[type] = std::vector<Component>();
	}
	std::vector<Component>& comps = components[type];
	size_t idx = comps.size();
	comps.resize(comps.size() + 1);
	return comps.data() + idx;
}

void SceneGraph::DeleteComponent(std::type_index& type, Component* component) {
	if (components.find(type) == components.end()) {
		return;
	}
	std::vector<Component>& comps = components[type];

	//Less than obvious: std::vector is guaranteed to be contiguous
	//so x past the begin() iterator is an iterator pointing at vec[x].
	//Since x is the offset of the given component past the beginning
	//of the vec (comps.data()), we can add x to the beginning to get
	//an iterator to our component.
	//All just so we can tell std::vector where to erase. It'll handle
	//the rest.
	comps.erase(comps.begin() + (component - comps.data()));
}

std::vector<Component>& SceneGraph::GetComponents(std::type_index& type) {
	std::lock_guard<std::mutex>(mutex, std::adopt_lock);
	return components[type];
}