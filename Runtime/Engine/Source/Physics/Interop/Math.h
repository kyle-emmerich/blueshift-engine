#pragma once
#include "Core/Math/Vector.h"
#include "btBulletDynamicsCommon.h"

namespace Blueshift {
	namespace Physics {
		namespace Interop {

			inline btVector3 ToBullet(const Core::Math::Vector4& vec) {
				btVector4 bt_vec;
				bt_vec.m_floats[0] = static_cast<btScalar>(vec.data[0]);
				bt_vec.m_floats[1] = static_cast<btScalar>(vec.data[1]);
				bt_vec.m_floats[2] = static_cast<btScalar>(vec.data[2]);
				return bt_vec;
			}

			inline Core::Math::Vector4 FromBullet(const btVector3& vec) {
				Core::Math::Vector4 bs_vec;
				bs_vec.data[0] = static_cast<float>(vec.m_floats[0]);
				bs_vec.data[1] = static_cast<float>(vec.m_floats[1]);
				bs_vec.data[2] = static_cast<float>(vec.m_floats[2]);
				bs_vec.data[3] = 1.0f;
				return bs_vec;
			}

			inline btQuaternion ToBullet(const Core::Math::Quaternion& q) {
				btQuaternion bt_quat(
					static_cast<btScalar>(q.data[0]),
					static_cast<btScalar>(q.data[1]),
					static_cast<btScalar>(q.data[2]),
					static_cast<btScalar>(q.data[3])
				);
				return bt_quat;
			}
		}
	}
}