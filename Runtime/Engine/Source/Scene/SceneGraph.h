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
				void* ptr;
				size_t type_size;
				size_t size;
				size_t capacity;
			};

			std::mutex mutex;
			std::map<std::type_index, component_list> components;
			std::vector<std::type_index> is_transformable;

			void* allocate_component(size_t type_size, std::type_index& type);
			void  delete_component(std::type_index& type, void* at);
			void evaluate_capacity(component_list& list);
		public:
			SceneGraph(Core::Engine* engine);
			virtual ~SceneGraph();

			template<typename T>
			inline T* AllocateComponent(Object* object = nullptr) {
				//keep track of transformables
				if (components.find(type) == components.end() &&
					std::is_base_of<TransformableComponent, T>::value) {
					is_transformable.push_back(type);
				}

				T* component = allocate_component(sizeof(T), typeid(T));
				//this is not an initialized pointer, so we have to do that here.
				return new(component) T(this, object);
			}

			template<typename T>
			inline void DeleteComponent(T* component) {
				//call destructor to ensure that the user code doesn't have to get
				//its dainty little hands dirty
				component->~T(); //holy shit why does this work
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
			struct Iterator : std::iterator<std::bidirectional_iterator_tag, T> {
				inline T& operator*() {
					return *(static_cast<T*>(list->ptr) + pos);
				}
				inline T* operator->() {
					return static_cast<T*>(list->ptr) + pos;
				}

				friend bool operator== (Iterator<T> const& lhs, Iterator<T> const& rhs) {
					return lhs.pos == rhs.pos;
				}
				friend bool operator!= (Iterator<T> const& lhs, Iterator<T> const& rhs) {
					return lhs.pos != rhs.pos;
				}

				Iterator& operator++() {
					pos++;
					return *this;
				}
				Iterator& operator--() {
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
				Iterator begin(&components[typeid(T)], 0);
				return begin;
			}
			template<typename T>
			inline Iterator<T> End() {
				component_list& list = components[typeid(T)];
				Iterator end(&list, list.size * list.type_size);
				return end;
			}

			void IntegrateTransforms();
		};

	}
}