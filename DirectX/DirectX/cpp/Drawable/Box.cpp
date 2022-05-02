#include "../../header/Drawable/Box.h"
#include "../../header/Bindable/BindableBase.h"
#include "../../header/Macros/GraphicThrowMacros.h"

Box::Box(Graphics& _gfx,
	std::mt19937& _rng,
	std::uniform_real_distribution<float>& _adist,
	std::uniform_real_distribution<float>& _ddist,
	std::uniform_real_distribution<float>& _odist,
	std::uniform_real_distribution<float>& _rdist)
	:
	r(_rdist(_rng)),
	droll(_ddist(_rng)),
	dpitch(_ddist(_rng)),
	dyaw(_ddist(_rng)),
	dphi(_odist(_rng)),
	dtheta(_odist(_rng)),
	dchi(_odist(_rng)),
	chi(_adist(_rng)),
	theta(_adist(_rng)),
	phi(_adist(_rng))
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};
	const std::vector<Vertex> vertices =
	{
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f,-1.0f,-1.0f },
		{ -1.0f,1.0f,-1.0f },
		{ 1.0f,1.0f,-1.0f },
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f,-1.0f,1.0f },
		{ -1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
	};
	AddBind(std::make_unique<VertexBuffer>(_gfx, vertices));

	auto pvs = std::make_unique<VertexShader>(_gfx, L"VertexShader.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(_gfx, L"PixelShader.cso"));

	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	AddIndexBuffer(std::make_unique<IndexBuffer>(_gfx, indices));

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{ 1.0f,0.0f,1.0f },
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,1.0f,1.0f },
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(_gfx, cb2));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(_gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransformCbuf>(_gfx, *this));
}

void Box::Update(float _dt) noexcept
{
	roll += droll * _dt;
	pitch += dpitch * _dt;
	yaw += dyaw * _dt;
	theta += dtheta * _dt;
	phi += dphi * _dt;
	chi += dchi * _dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}