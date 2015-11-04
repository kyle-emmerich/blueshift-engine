#pragma once
#include "Core/Math/Math.h"

namespace Blueshift {
	namespace Scene {

		class Component {
		protected:
			Core::Math::Matrix4f parent_transform;
			Core::Math::Vector3f position;
			Core::Math::Quaternionf orientation;
			
		public:
			Component();
			virtual ~Component();
		};

	}
}