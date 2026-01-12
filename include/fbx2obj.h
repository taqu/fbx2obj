#ifndef INC_FBX2OBJ_H
#define INC_FBX2OBJ_H
#include <cstdint>
namespace fbx2obj
{
static constexpr float Epsilon = 1.0e-7f;

bool equals(float x0, float x1);

class Vector2
{
public:
    float x_;
    float y_;
};
bool equals(const Vector2& x0, const Vector2& x1);

class Vector3
{
public:
    float x_;
    float y_;
    float z_;
};
bool equals(const Vector3& x0, const Vector3& x1);

class Vector4
{
public:
    float x_;
    float y_;
    float z_;
    float w_;
};
bool equals(const Vector4& x0, const Vector4& x1);
} // namespace fbx2obj
#endif
