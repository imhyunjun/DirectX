#pragma once
#include "DrawableBase.h"

class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& _gfx, std::mt19937& _rng,
		std::uniform_real_distribution<float>& _adist,
		std::uniform_real_distribution<float>& _ddist,
		std::uniform_real_distribution<float>& _odist,
		std::uniform_real_distribution<float>& _rdist,
		std::uniform_real_distribution<float>& _bdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	DirectX::XMFLOAT3X3 mt;
};