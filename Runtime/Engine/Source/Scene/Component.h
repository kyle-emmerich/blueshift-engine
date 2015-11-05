#pragma once

namespace Blueshift {
	namespace Scene {

		class Object;
		class Component {
		protected:
			Object* object;
		public:
			Component();
			virtual ~Component();
		};

	}
}