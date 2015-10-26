#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <string>
#include "Core/Math/Matrix.h"

namespace Blueshift {
	namespace Scene {

		class Object;
		class Component {
		private:
			Object* object;
			std::unordered_map<std::type_index, std::vector<Component*>> children;
			Component* parent;

			std::string name;
		public:
			Component(Object* object);
			Component(Component&) = delete;
			Component(Component&&) = delete;
			virtual ~Component();

			inline std::string GetName() { return name; }

			inline virtual std::type_index GetType() { return typeid(Component); }
			inline virtual bool IsTransformable() { return false; }

			inline virtual Core::Math::Matrix4 GetTransform() const {
				if (parent) {
					return parent->GetTransform();
				}
				return Core::Math::IdentityMatrix4;
			}
			
			template<typename T>
			inline T* GetParent() { return reinterpret_cast<T*>(parent); }
			inline const Component* GetParent() const { return parent; }
			inline bool HasParent() const { return parent != nullptr; }

			template<typename T>
			inline std::vector<T*> GetChildrenOfType() {
				std::vector<T*> out;
				const std::vector<Component*>& untyped_vec = children[typeid(T)];
				for (auto component : untyped_vec) {
					out.push_back(reinterpret_cast<T*>(component));
				}
				return out;
			}

			template<typename T>
			inline bool AddChild(T* child) {
				if (children.find(typeid(T)) == children.end()) {
					children[typeid(T)] = std::vector<Component*>();
				}
				children[typeid(T)].push_back(child);
				return true;
			}

			template<typename T>
			inline bool RemoveChild(T* child) {
				if (children.find(typeid(T)) == children.end())
					return false;

				for (auto it = children[typeid(T)].begin(); it != children[typeid(T)].end(); it++) {
					if (comp == child) {
						children[typeid(T)].erase(comp);
						break;
					}
				}
			}
		};

	}
}