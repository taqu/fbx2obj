#include "fbx2obj.h"
#include <algorithm>

namespace fbx2obj
{
bool equals(float x0, float x1)
{
    return (std::abs)(x0 - x1) < Epsilon;
}

bool equals(const Vector2& x0, const Vector2& x1)
{
    return equals(x0.x_, x1.x_) && equals(x0.y_, x1.y_);
}

bool equals(const Vector3& x0, const Vector3& x1)
{
    return equals(x0.x_, x1.x_) && equals(x0.y_, x1.y_) && equals(x0.z_, x1.z_);
}

bool equals(const Vector4& x0, const Vector4& x1)
{
    return equals(x0.x_, x1.x_) && equals(x0.y_, x1.y_) && equals(x0.z_, x1.z_) && equals(x0.w_, x1.w_);
}
} // namespace fbx2obj
