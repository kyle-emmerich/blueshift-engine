#pragma once
#include "Core/Engine.h"
#include "Scene/Component.h"
#include "Scene/TransformableComponent.h"
#include <typeindex>
#include <map>
#include <vector>
#include <mutex>

namespace Blueshift {
	namespace Scene {

		class SceneGraph : public Core::ISubsystem {
		protected:
			struct component_list {
				void* ptr = nullptr;
				size_t type_size = 0;
				size_t size = 0;
				size_t capacity = 0;

				std::vector<Component::Handle> handles;
			};

			std::mutex mutex;
			std::map<std::type_index, component_list> components;
			std::vector<std::type_index> is_transformable;

			void* allocate_component(size_t type_size, const std::type_index& type, size_t& idx);
			void  delete_component(const std::type_index& type, void* at);
			void evaluate_capacity(component_list& list);

			friend class Component;
			friend struct Component::Handle;
		public:
			SceneGraph(Core::Engine* engine);
			virtual ~SceneGraph();

			template<typename T>
			inline Component::Handle AllocateComponent(Object* object = nullptr) {
				//keep track of transformables
				const std::type_index& type = typeid(T);
				if (components.find(type) == components.end() &&
					std::is_base_of<TransformableComponent, T>::value) {
					is_transformable.push_back(type);
				}

				size_t idx;
				T* component = reinterpret_cast<T*>(allocate_component(sizeof(T), type, idx));
				component->end_type = typeid(T);
				component->object = object;
				Component::Handle handle(this, idx, type);
				components[typeid(T)].handles.push_back(handle);
				//this is not an initialized pointer, so we have to do that here.
				new(component) T(this, object);

				return handle;
			}

			template<typename T>
			inline void DeleteComponent(T* component) {
				//when placement new is used, the programmer is responsible for calling
				//the correct destructor
				//this means that to avoid VERY BAD THINGS from happening, we must 
				//check the type.
				//fortunately, we have a way to check this and throw an exception if
				//necessary because we store endpoint types.
				component_list& list = components[typeid(T)];
				//if we check the address of the component pointer against the list,
				//we can discover whether or not it's actually of that type.
				//however, we shouldn't use the type as is because the operators aren't
				//exactly defined by the C++ standard
				uint8_t* comp_ptr = reinterpret_cast<uint8_t*>(component);
				uint8_t* list_ptr = reinterpret_cast<uint8_t*>(list_ptr);
				size_t length = list.type_size * list.size;
				if (comp_ptr < list_ptr || comp_ptr > list_ptr + length) {
					//TODO: add exception for trying to destruct wrong type
					throw 0;
				}
				
				component->~T();
				//finally, kick it out of our list
				delete_component(typeid(T), static_cast<void*>(component));
			}

			void* GetComponents(std::type_index& type, size_t& out_size);
			template<typename T>
			inline T* GetComponents() {
				size_t size = 0;
				return static_cast<T*>(GetComponents(typeid(T), size));
			}

			template<typename T>
			inline void DestroyComponents() {
				auto it = Begin<T>();
				for (it; it != End<T>(); ++it) {
					it->~T();
				}
				free(components[typeid(T)].ptr);
				components.erase(typeid(T));
			}

			template<typename T>
			struct Iterator : std::iterator<std::bidirectional_iterator_tag, T> {
				inline T& operator*() {
					return *(reinterpret_cast<T*>(list->ptr) + pos);
				}
				inline T* operator->() {
					return reinterpret_cast<T*>(list->ptr) + pos;
				}

				friend bool operator== (Iterator<T> const& lhs, Iterator<T> const& rhs) {
					return lhs.pos == rhs.pos;
				}
				friend bool operator!= (Iterator<T> const& lhs, Iterator<T> const& rhs) {
					return lhs.pos != rhs.pos;
				}

				Iterator<T>& operator++() {
					pos++;
					return *this;
				}
				Iterator<T>& operator--() {
					pos--;
					return *this;
				}
			protected:
				component_list* list;
				size_t pos;

				friend class SceneGraph;
				Iterator(component_list* list, size_t pos) : list(list), pos(pos) {}
			};

			template<typename T>
			inline Iterator<T> Begin() {
				Iterator<T> begin(&components[typeid(T)], 0);
				return begin;
			}
			template<typename T>
			inline Iterator<T> End() {
				component_list& list = components[typeid(T)];
				Iterator<T> end(&list, list.size);
				return end;
			}

			void IntegrateTransforms();
		};

	}
}