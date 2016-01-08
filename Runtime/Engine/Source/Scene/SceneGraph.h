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

		/*! \class SceneGraph
			\brief A subsystem that handles all object components in the engine and provides access to lists of components by type.

			The SceneGraph is responsible for managing all components, including memory allocation and access to lists sorted by component type.
			It functions via a liberal application of black magic, known as C++ memory management.

			Author is not responsible for any damages caused by use of this class.
		*/
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
			void  evaluate_capacity(component_list& list);

			friend class Component;
			friend struct Component::Handle;
		public:
			SceneGraph(Core::Engine* engine);
			virtual ~SceneGraph();

			/*! \brief Allocates a component of type T in the appropriate list, ensuring that the list exists in one contiguous block of memory for fast access.

				This method performs a placement new on memory allocated for the list ahead of time. If the type has never been allocated before,
				a list will be created, and if the list runs out of space, it will resize.

				Try not to call this too much each frame, since it carries the overhead of potentially resizing a component list.

				\param object A pointer to an object to attach the component to, can be nullptr for an unattached component.
			*/
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

			/*!	\brief Deallocates a component in the list for type T and shuffles everything after it to fill the gap.

				This method could potentially cause a reallocation, so use it sparingly whenever possible.
			*/
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

			/*! \brief Gets a pointer to the start of a list of the type given; be careful!

				This method is not type-safe. It's smarter to not use it whenever possible.

				\param type Call typeid(T) to get this. (#include <typeindex>)
				\param out_size This will be filled with the size of the list in bytes.
			*/
			void* GetComponents(std::type_index& type, size_t& out_size);

			/*!	\brief Gets a pointer to the start of a list of components of type T.
			*/
			template<typename T>
			inline T* GetComponents() {
				size_t size = 0;
				return static_cast<T*>(GetComponents(typeid(T), size));
			}

			/*! \brief Destroys an entire list of components at once. */
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