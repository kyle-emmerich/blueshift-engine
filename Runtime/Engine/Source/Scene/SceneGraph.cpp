#include "Scene/SceneGraph.h"

using namespace Blueshift;
using namespace Scene;

SceneGraph::SceneGraph(Core::Engine* engine) {

}

SceneGraph::~SceneGraph() {

}

void* SceneGraph::allocate_component(size_t type_size, const std::type_index& type, size_t& idx) {
	std::lock_guard<std::mutex>(this->mutex);
	if (components.find(type) == components.end()) {
		component_list list;
		list.capacity = 10;
		list.type_size = type_size;
		list.size = 0;
		list.ptr = malloc(type_size * list.capacity);
		components[type] = list;
	}
	component_list& comps = components[type];
	evaluate_capacity(comps);
	
	idx = comps.size++;
	uint8_t* ptr = static_cast<uint8_t*>(comps.ptr) + (type_size * idx);
	return static_cast<void*>(ptr);
}

void SceneGraph::delete_component(const std::type_index& type, void* component) {
	std::lock_guard<std::mutex>(mutex, std::adopt_lock);
	component_list& comps = components[type];
	uint8_t* comps_ptr = static_cast<uint8_t*>(comps.ptr);
	uint8_t* comp = reinterpret_cast<uint8_t*>(component);
	//don't try this at home, kids

	if (comp > comps_ptr && comp < comps_ptr + comps.type_size * comps.size) {
		size_t idx = (comp - comps_ptr) / comps.type_size;
		//this is valid
		//move everything after it down.
		//basically, we find the pointer to the end of the buffer by
		//taking the beginning and adding the size of the buffer,
		//then we find the remainder by subtracting the position of
		//the component being removed. This gives us the number of bytes
		//in the buffer AFTER the removed component.
		uint8_t* end_ptr = comps_ptr + (comps.size * comps.type_size);
		size_t remainder = static_cast<size_t>(end_ptr - comp) - comps.type_size;
		//if we have anything remaining after this, we can move it
		//with memcpy; destination being the position of the removed component,
		//source being the position of the next component after.
		//the number of bytes is given by the remainder.
		if (remainder > 0) {
			memcpy(comp, comp + comps.type_size, remainder);
		}
		//now we decrement out size counter.
		comps.size--;
		evaluate_capacity(comps);
		//let's also curate handles
		auto comp_it = comps.handles.end();
		for (auto it = comps.handles.begin(); it != comps.handles.end(); it++) {
			if (it->idx > idx) {
				it->idx--;
			} else if (it->idx == idx) {
				comp_it = it;
			}
		}
		if (comp_it != comps.handles.end()) {
			comps.handles.erase(comp_it);
		}
	} else {
		//TODO: add component not found in list exception
		throw 999;
	}
}

void SceneGraph::evaluate_capacity(component_list& list) {
	if (list.size <= list.capacity - 10) {
		//we can safely shrink
		list.capacity -= 10;
		list.ptr = realloc(list.ptr, list.capacity * list.type_size);
	}
	if (list.size == list.capacity) {
		list.capacity += 10;
		list.ptr = realloc(list.ptr, list.type_size * list.capacity);
	}
}

void* SceneGraph::GetComponents(std::type_index& type, size_t& out_size) {
	std::lock_guard<std::mutex>(mutex, std::adopt_lock);
	out_size = components[type].type_size;
	return components[type].ptr;
}

void SceneGraph::IntegrateTransforms() {
	for (std::type_index type : is_transformable) {
		component_list& list = components[type];
		//iterate over each one
		for (size_t i = 0; i < list.size; i++) {
			TransformableComponent* c = reinterpret_cast<TransformableComponent*>(static_cast<uint8_t*>(list.ptr) + (list.type_size * i));
			c->UpdateWorldTransform();
		}
	}
}