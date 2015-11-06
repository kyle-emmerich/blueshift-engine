#pragma once

namespace Blueshift {
	namespace Scene {

		class Object;
		class SceneGraph;
		class Component {
		protected:
			Object* object;
			Component(SceneGraph*, Object* obj = nullptr) : object(obj) {}
		public:
			virtual ~Component() {}
		};

	}
}