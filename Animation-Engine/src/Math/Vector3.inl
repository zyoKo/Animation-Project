#pragma once

#include "Vector3.h"

#include "Core/Logger/Log.h"

namespace Animation::Math
{
	template <typename T>
	constexpr Vector3<T>::Vector3() noexcept
		:	x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0))
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(T x, T y, T z) noexcept
		:	x(x), y(y), z(z)
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(T value) noexcept
		:	x(value), y(value), z(value)
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(std::initializer_list<T> data) noexcept
	{
		ANIM_ASSERT(data.size() == 3, "Initializer List needs 3 arguments!");
		std::copy(data.begin(), data.end(), variables.begin());
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(const Vector3& vector) noexcept
		:	x(vector.x), y(vector.y), z(vector.z)
	{
	}

	// Operations
	template <typename T>
	Vector3<T>& Vector3<T>::operator=(const Vector3& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator*(T value)
	{
		ANIM_ASSERT(!std::isnan(value), "Operation '/' failed, the value is NaN!");
		return Vector3(x * value, y * value, z * value);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator*=(T value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator/(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		return Vector3(x * inverse, y * inverse, z * inverse);
	}

	template <typename T>
	Vector3<T>& Vector3<T>::operator/=(T value)
	{
		ANIM_ASSERT(value != 0, "[Division By Zero Error]: The result of operation '/' will be Infinite!");
		float inverse = static_cast<float>(1) / value;
		x *= inverse;
		y *= inverse;
		z *= inverse;
		return *this;
	}

	template <typename T>
	bool Vector3<T>::operator==(const Vector3& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x == vector.x && y == vector.y && z == vector.z;
	}

	template <typename T>
	bool Vector3<T>::operator!=(const Vector3& vector)
	{
		ANIM_ASSERT(!vector.HasNaNs(), "Operation '/' failed, the vector has NaNs!");
		return x != vector.x || y != vector.y || z != vector.z;
	}

	// Core Functions
	template <typename T>
	bool Vector3<T>::HasNaNs() const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	template <typename T>
	bool Vector3<T>::HasInfinite() const
	{
		return std::isinf(x) || std::isinf(y) || std::isinf(z);
	}

	template <typename T>
	bool Vector3<T>::IsZero(const Vector3& vector)
	{
		return vector.x == 0 && vector.y == 0 && vector.z == 0;
	}

	template <typename T>
	T Vector3<T>::LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	template <typename T>
	T Vector3<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <typename T>
	Vector3<T>& Vector3<T>::GetNormalize()
	{
		*this /= Length();
		return *this;
	}

	template <typename T>
	Vector3<T> Vector3<T>::Normalize(const Vector3& vector)
	{
		return Vector3(vector.x / vector.Length(), vector.y / vector.Length(), vector.z / vector.Length());
	}

	template <typename T>
	T Vector3<T>::Dot(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	template <typename T>
	T Vector3<T>::AbsDot(const Vector3& lhs, const Vector3& rhs)
	{
		return std::abs(Dot(lhs, rhs));
	}
}
