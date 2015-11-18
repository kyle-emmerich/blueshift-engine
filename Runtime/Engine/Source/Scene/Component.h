#pragma once
#include <typeindex>

namespace Blueshift {
	namespace Scene {

		class Object;
		class SceneGraph;
		class Component {
		protected:
			Object* object;
			std::type_index end_type;
			Component(SceneGraph*, Object* obj = nullptr) : object(obj), end_type(typeid(Component)) {}

#pragma pack(push, 8)
			struct serialized {};
#pragma pack(pop)
		public:
			virtual ~Component() {}

			inline const std::type_index& GetEndType() const {
				return end_type;
			}

			struct Handle {
				size_t idx;
				std::type_index type;
				SceneGraph* graph;

				Component* operator->() const;
				template<typename T>
				T* Get() const {
					return dynamic_cast<T*>(operator->());
				}
				template<typename T>
				operator T*() const {
					return dynamic_cast<T*>(operator->());
				}

				Handle(SceneGraph* graph, size_t idx, const std::type_index& type)
					: graph(graph), idx(idx), type(type) {}
				Handle()
					: graph(nullptr), idx(999999999), type(typeid(Component)) {}
			};
		};

	}
}