#include <iostream>

#include "Core/Application/Application.h"
#include "Core/Logger/Log.h"

class Sandbox : public Animator::Application
{
public:
    Sandbox() = default;
};

#include "Math/Math.h"

void Vector2Test()
{
    using namespace Animation::Math;

    Vector2F vec0;
    std::cout << "vec0 without parameters: " << vec0 << std::endl;
    Vector2F vec1(1.0f, 2.0f);
    std::cout << "vec1(1.0f, 2.0f): " << vec1 << std::endl;
    Vector2F vec2(2.0f);
    std::cout << "vec2(2.0f): " << vec2 << std::endl;
    Vector2F vec3(vec2);
    std::cout << "vec3(vec2): " << vec3 << std::endl;
    Vector2F vec4({ 3.0f, 4.0f });
    std::cout << "vec4({ 0.0f, 1.0f }): " << vec4 << std::endl;

    Vector2F vec5 = static_cast<Vector2F>(vec0);
    std::cout << "vec5 = static_cast<Vector2F>(vec0)" << vec5 << std::endl;

    Vector2F vec6;
    vec6 = vec4;
    std::cout << "vec6 = vec4: " << vec6 << std::endl;

    std::cout << "Addition Test: vec3" << vec3 << " + " << vec4 << " = " << vec3 + vec4 << std::endl;
    std::cout << "Subtraction Test: vec3" << vec3 << " - " << vec4 << " = " << vec3 - vec4 << std::endl;
    std::cout << "Multiplication Test: vec3" << vec3 << " * " << 2 << " = " << vec3 * 2 << std::endl;
    std::cout << "Division Test: vec3" << vec3 << " / " << 2 << " = " << vec3 / 2 << std::endl;

    std::cout << "\n";

    std::cout << "vec4: " << vec3 << "\n";
    std::cout << "vec5: " << vec5 << "\n";
    vec3 += vec5;
    std::cout << "vec4 += vec5 -> " << vec3 << std::endl;

    std::cout << "\n";

    std::cout << "vec6: " << vec6 << "\n";
    std::cout << "vec1: " << vec1 << "\n";
    vec6 -= vec1;
    std::cout << "vec6 -= vec1 -> " << vec6 << std::endl;

    std::cout << "\n";

    std::cout << "vec3: " << vec3 << "\n";
    vec3 *= 8;
    std::cout << "vec3 *= 8 -> " << vec3 << std::endl;

    std::cout << "\n";

    std::cout << "vec3: " << vec3 << "\n";
    vec3 /= 3;
    std::cout << "vec3 /= 3 -> " << vec3 << std::endl;

    std::cout << "\n";

    (vec3 == vec2) ?
        std::cout << "Vec3 = Vec2" << std::endl :
        std::cout << "Vec3 != Vec2" << std::endl;

    std::cout << "\n";

    vec4 != vec1 ?
        std::cout << "Vec4 != Vec1" << std::endl :
        std::cout << "Vec4 == Vec1" << std::endl;

    std::cout << "\n";

    std::cout << "Core Functions Test\n";
    std::cout << "vec3 / sqrt(-3) has NaNs: " << ((vec3 / std::sqrt(-3.0f)).HasNaNs() ? "true" : "false") << std::endl;

    std::cout << "Is vec0 which is (0, 0) is Zero?: " << (Vec2F::IsZero(vec0) ? "Yes" : "No") << std::endl;

    std::cout << "Length Squared " << vec2 << " = " << vec2.LengthSquared() << std::endl;
    std::cout << "Length " << vec4 << " = " << vec4.Length() << std::endl;

    std::cout << "Normalized " << vec4 << " = " << vec4.GetNormalize() << std::endl;
    std::cout << "Normalized " << vec3 << " = " << Vector2F::Normalize(vec3) << std::endl;

    std::cout << "Dot Product of " << vec2 << ", " << vec3 << " = " << Vector2F::Dot(vec2, vec3) << std::endl;
    std::cout << "Dot Product of " << vec2 << ", " << vec3 << " = " << Vector2F::AbsDot(vec2, vec3) << std::endl;
}

void Vector3Test()
{
    using namespace Animation::Math;

    Vector3F vec0;
    std::cout << "vec0 without parameters: " << vec0 << std::endl;
    Vector3F vec1(1.0f, 2.0f, 3.0f);
    std::cout << "vec1(1.0f, 2.0f): " << vec1 << std::endl;
    Vector3F vec2(2.0f);
    std::cout << "vec2(2.0f): " << vec2 << std::endl;
    Vector3F vec3(vec2);
    std::cout << "vec3(vec2): " << vec3 << std::endl;
    Vector3F vec4({ 3.0f, 4.0f, 5.0f });
    std::cout << "vec4({ 0.0f, 1.0f }): " << vec4 << std::endl;

    Vector3F vec5 = static_cast<Vector3F>(vec0);
    std::cout << "vec5 = static_cast<Vector2F>(vec0)" << vec5 << std::endl;

    Vector3F vec6;
    vec6 = vec4;
    std::cout << "vec6 = vec4: " << vec6 << std::endl;

    std::cout << "Addition Test: vec3" << vec3 << " + " << vec4 << " = " << vec3 + vec4 << std::endl;
    std::cout << "Subtraction Test: vec3" << vec3 << " - " << vec4 << " = " << vec3 - vec4 << std::endl;
    std::cout << "Multiplication Test: vec3" << vec3 << " * " << 2 << " = " << vec3 * 2 << std::endl;
    std::cout << "Division Test: vec3" << vec3 << " / " << 2 << " = " << vec3 / 2 << std::endl;

    std::cout << "\n";

    std::cout << "vec4: " << vec3 << "\n";
    std::cout << "vec5: " << vec5 << "\n";
    vec3 += vec5;
    std::cout << "vec4 += vec5 -> " << vec3 << std::endl;

    std::cout << "\n";

    std::cout << "vec6: " << vec6 << "\n";
    std::cout << "vec1: " << vec1 << "\n";
    vec6 -= vec1;
    std::cout << "vec6 -= vec1 -> " << vec6 << std::endl;

    std::cout << "\n";

    std::cout << "vec3: " << vec3 << "\n";
    vec3 *= 8;
    std::cout << "vec3 *= 8 -> " << vec3 << std::endl;

    std::cout << "\n";

    std::cout << "vec3: " << vec3 << "\n";
    vec3 /= 3;
    std::cout << "vec3 /= 3 -> " << vec3 << std::endl;

    std::cout << "\n";

    (vec3 == vec2) ?
        std::cout << "Vec3 = Vec2" << std::endl :
        std::cout << "Vec3 != Vec2" << std::endl;

    std::cout << "\n";

    vec4 != vec1 ?
        std::cout << "Vec4 != Vec1" << std::endl :
        std::cout << "Vec4 == Vec1" << std::endl;

    std::cout << "\n";

    std::cout << "Core Functions Test\n";
    std::cout << "vec3 / sqrt(-3) has NaNs: " << ((vec3 / std::sqrt(-3.0f)).HasNaNs() ? "true" : "false") << std::endl;

    std::cout << "Is vec0 which is (0, 0) is Zero?: " << (Vector3F::IsZero(vec0) ? "Yes" : "No") << std::endl;

    std::cout << "Length Squared " << vec2 << " = " << vec2.LengthSquared() << std::endl;
    std::cout << "Length " << vec4 << " = " << vec4.Length() << std::endl;

    std::cout << "Normalized " << vec4 << " = " << vec4.GetNormalize() << std::endl;
    std::cout << "Normalized " << vec3 << " = " << Vector3F::Normalize(vec3) << std::endl;

    std::cout << "Dot Product of " << vec2 << ", " << vec3 << " = " << Vector3F::Dot(vec2, vec3) << std::endl;
    std::cout << "Dot Product of " << vec2 << ", " << vec3 << " = " << Vector3F::AbsDot(vec2, vec3) << std::endl;
}

int main()
{
    Sandbox application;
    application.Initialize();

	//Vector2Test();
	Vector3Test();

	application.Run();
    application.Shutdown();

    return 0;
}