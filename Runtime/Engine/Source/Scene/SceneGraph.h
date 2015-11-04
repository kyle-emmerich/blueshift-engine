#pragma once
#include "Core/Engine.h"
#include "Scene/Component.h"
#include <typeindex>
#include <map>
#include <vector>
#include <mutex>

namespace Blueshift {
	namespace Scene {

		class SceneGraph : public Core::ISubsystem {
		protected:
			std::mutex mutex;
			std::map<std::type_index&, std::vector<Component>> components;
			std::map<std::type_index&, Core::ISubsystem*> systems;
		public:
			SceneGraph(Core::Engine* engine);
			virtual ~SceneGraph();

			Component* AllocateComponent(std::type_index& type);
			template<typename T>
			inline T* AllocateComponent() {
				return dynamic_cast<T*>(AllocateComponent(typeid(T)));
			}

			void DeleteComponent(std::type_index& type, Component* component);
			template<typename T>
			inline void DeleteComponent(T* component) {
				DeleteComponent(typeid(T), component);
			}

			std::vector<Component>& GetComponents(std::type_index& type);
			template<typename T>
			inline std::vector<Component>& GetComponents() {
				return GetComponents(typeid(T));
			}
		};

	}
}