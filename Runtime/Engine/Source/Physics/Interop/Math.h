#pragma once
#include "Core/Math/Vector.h"
#include "btBulletDynamicsCommon.h"

namespace Blueshift {
	namespace Physics {
		namespace Interop {
			template<typename T>
			btVector3 ToBullet(const Core::Math::Vector<3, T>& vec) {
				btVector3 bt_vec;
				bt_vec.m_floats[0] = static_cast<btScalar>(vec.data[0]);
				bt_vec.m_floats[1] = static_cast<btScalar>(vec.data[1]);
				bt_vec.m_floats[2] = static_cast<btScalar>(vec.data[2]);
				return bt_vec;
			}

			template<typename T>
			btVector4 ToBullet(const Core::Math::Vector<4, T>& vec) {
				btVector4 bt_vec;
				bt_vec.m_floats[0] = static_cast<btScalar>(vec.data[0]);
				bt_vec.m_floats[1] = static_cast<btScalar>(vec.data[1]);
				bt_vec.m_floats[2] = static_cast<btScalar>(vec.data[2]);
				bt_vec.m_floats[3] = static_cast<btScalar>(vec.data[3]);
				return bt_vec;
			}

			template<typename T>
			Core::Math::Vector<3, T> FromBullet(const btVector3& vec) {
				Core::Math::Vector<3, T> bs_vec;
				bs_vec.data[0] = static_cast<T>(vec.m_floats[0]);
				bs_vec.data[1] = static_cast<T>(vec.m_floats[1]);
				bs_vec.data[2] = static_cast<T>(vec.m_floats[2]);
				return bs_vec;
			}

			template<typename T>
			Core::Math::Vector<4, T> FromBullet(const btVector4& vec) {
				Core::Math::Vector<4, T> bs_vec;
				bs_vec.data[0] = static_cast<T>(vec.m_floats[0]);
				bs_vec.data[1] = static_cast<T>(vec.m_floats[1]);
				bs_vec.data[2] = static_cast<T>(vec.m_floats[2]);
				bs_vec.data[3] = static_cast<T>(vec.m_floats[3]);
				return bs_vec;
			}
		}
	}
}