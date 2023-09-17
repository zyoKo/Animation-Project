#pragma once

#include <array>

namespace Animation::Math
{
	template <typename T>
	class Vector3
	{
	public:
		// Declare variables
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				T x, y, z;
			};

			std::array<T, 3> variables;
		};

		constexpr Vector3() noexcept;

		constexpr Vector3(T x, T y, T z) noexcept;

		constexpr Vector3(T value) noexcept;

		constexpr Vector3(std::initializer_list<T> data) noexcept;

		explicit constexpr Vector3(const Vector3& vector) noexcept;

		~Vector3() = default;

		// Operations
		Vector3& operator=(const Vector3& vector);

		Vector3 operator+(const Vector3& vector);

		Vector3& operator+=(const Vector3& vector);

		Vector3 operator-(const Vector3& vector);

		Vector3& operator-=(const Vector3& vector);

		Vector3 operator*(T value);

		Vector3& operator*=(T value);

		Vector3 operator/(T value);

		Vector3& operator/=(T value);

		bool operator==(const Vector3& vector);

		bool operator!=(const Vector3& vector);

		friend std::ostream& operator<<(std::ostream& os, const Vector3& vector)
		{
			os << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
			return os;
		}

		// Core Functions
		bool HasNaNs() const;

		bool HasInfinite() const;

		static bool IsZero(const Vector3& vector);

		T LengthSquared() const;

		T Length() const;

		Vector3& GetNormalize();

		static Vector3 Normalize(const Vector3& vector);

		static T Dot(const Vector3& lhs, const Vector3& rhs);

		static T AbsDot(const Vector3& lhs, const Vector3& rhs);
	};

	using Vec3f		= Vector3<float>;
	using Vec3F		= Vector3<float>;
	using Vector3f	= Vector3<float>;
	using Vector3F	= Vector3<float>;

	using Vec3d		= Vector3<double>;
	using Vec3D		= Vector3<double>;
	using Vector3d	= Vector3<double>;
	using Vector3D	= Vector3<double>;

	using Vec3U		= Vector3<unsigned int>;
	using Vec3UI	= Vector3<unsigned int>;
	using Vec3u		= Vector3<unsigned int>;
	using Vec3ui	= Vector3<unsigned int>;
	using Vector3U	= Vector3<unsigned int>;
	using Vector3UI = Vector3<unsigned int>;

	using Vec3I		= Vector3<int>;
	using Vec3i		= Vector3<int>;
	using Vector3i	= Vector3<int>;
	using Vector3I	= Vector3<int>;
}

#include "Vector3.inl"