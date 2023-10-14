#include <AnimationPch.h>

#include "VQS.h"

#include "Core/Utilities/Utilites.h"

namespace Animator::Math
{
	VQS::VQS()
		:	translationVector(),
			quatRotation(),
			scaleVector(1.0f)
	{
	}

	VQS::VQS(const glm::vec3& vector, const QuatF& quat)
		:	translationVector(vector),
			quatRotation(quat),
			scaleVector(1.0f)
	{
	}

	VQS::VQS(const glm::vec3& vector, const QuatF& quat, float scale)
		:	translationVector(vector),
			quatRotation(quat),
			scaleVector(scale)
	{
	}

	inline void VQS::SetVector(const glm::vec3& vector)
	{
		this->translationVector = vector;
	}

	inline const glm::vec3& VQS::GetVector() const
	{
		return translationVector;
	}

	inline void VQS::SetRotation(const QuatF& quat)
	{
		this->quatRotation = quat;
	}

	inline const QuatF& VQS::GetRotation() const
	{
		return quatRotation;
	}

	void VQS::SetScaleVector(float scale)
	{
		this->scaleVector.x = scale;
		this->scaleVector.y = scale;
		this->scaleVector.z = scale;
	}

	float VQS::GetScale() const
	{
		return this->scaleVector.x;
	}

	inline const glm::vec3& VQS::GetScaleVector() const
	{
		return scaleVector;
	}

	VQS& VQS::operator+(const VQS& vqs)
	{
		this->translationVector += vqs.translationVector;
		this->quatRotation += vqs.quatRotation;
		this->scaleVector += vqs.scaleVector;
		return *this;
	}

	VQS& VQS::operator*(float value)
	{
		this->translationVector *= value;
		this->quatRotation *= value;
		this->scaleVector *= value;
		return *this;
	}

	VQS& VQS::operator*=(const VQS& vqs)
	{
		this->translationVector *= ((*this) * vqs.translationVector);
		this->quatRotation *= vqs.quatRotation;
		this->scaleVector *= vqs.scaleVector;
		return *this;
	}

	VQS VQS::operator*(const VQS& vqs) const
	{
		return {
			(*this) * vqs.translationVector,
			this->quatRotation * vqs.quatRotation,
			(this->scaleVector * vqs.scaleVector).x
		};
	}

	void VQS::MakeInverse()
	{
		const auto scaledTranslationInverse = (1.0f / this->scaleVector) * -(this->translationVector);
		const auto scaledTranslationInverseQuat = QuatF(scaledTranslationInverse.x, scaledTranslationInverse.y, scaledTranslationInverse.z, 0.0f);

		const auto subResult = QuatF::Inverse(this->quatRotation) * (scaledTranslationInverseQuat) * this->quatRotation;

		this->translationVector = glm::vec3(subResult.x, subResult.y, subResult.z);
		QuatF::Inverse(this->quatRotation);
		this->scaleVector = 1.0f / this->scaleVector;
	}

	VQS VQS::Inverse() const
	{
		VQS inverse;

		const auto scaledTranslationInverse = (1.0f / this->scaleVector) * -(this->translationVector);
		const auto scaledTranslationInverseQuat = QuatF(scaledTranslationInverse.x, scaledTranslationInverse.y, scaledTranslationInverse.z, 0.0f);

		// [ q^-1 * (s^-1 * v^-1) * q, q^-1, s^-1 ]
		const auto subTranslationVectorInverse = QuatF::Inverse(this->quatRotation) * (scaledTranslationInverseQuat) * this->quatRotation;

		inverse.translationVector = glm::vec3(subTranslationVectorInverse.x, subTranslationVectorInverse.y, subTranslationVectorInverse.z);
		inverse.quatRotation = QuatF::Inverse(this->quatRotation);
		inverse.scaleVector = 1.0f / this->scaleVector;

		return inverse;
	}

	VQS VQS::Interpolate(const VQS& vqsOne, const VQS& vqsTwo, float t_Translation, float t_Rotation, float t_Scale)
	{
		return VQS();
	}

	VQS VQS::GetUnitVQS()
	{
		VQS result;

		result.translationVector = glm::vec3(0.0f);
		result.quatRotation = QuatF();
		result.scaleVector = glm::vec3(1.0f);

		return result;
	}
}
