#include "../../header/Drawable/Box.h"
#include "../../header/Bindable/BindableBase.h"
#include "../../header/Macros/GraphicThrowMacros.h"
//#include "../../header/Figure/Sphere.h"
#include "../../header/Figure/Cube.h"

Box::Box(Graphics& _gfx,
	std::mt19937& _rng,
	std::uniform_real_distribution<float>& _adist,
	std::uniform_real_distribution<float>& _ddist,
	std::uniform_real_distribution<float>& _odist,
	std::uniform_real_distribution<float>& _rdist,
	std::uniform_real_distribution<float>& _bdist)
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
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		/*const std::vector<Vertex> vertices =
		{
			{ -1.0f,-1.0f,-1.0f },
			{ 1.0f,-1.0f,-1.0f },
			{ -1.0f,1.0f,-1.0f },
			{ 1.0f,1.0f,-1.0f },
			{ -1.0f,-1.0f,1.0f },
			{ 1.0f,-1.0f,1.0f },
			{ -1.0f,1.0f,1.0f },
			{ 1.0f,1.0f,1.0f },
		};*/
		const auto model = Cube::Make<Vertex>();
		//model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));
		AddStaticBind(std::make_unique<VertexBuffer>(_gfx, model.vertices));

		//auto pvs = std::make_unique<VertexShader>(_gfx, L"VertexShader.cso");
		auto pvs = std::make_unique<VertexShader>(_gfx, L"ColorIndexVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(_gfx, L"ColorIndexPS.cso"));

		/*const std::vector<unsigned short> indices =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};*/
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(_gfx, model.indices));

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants cb2 =
		{
			{
				{ 1.0f,1.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
				{ 0.0f,0.0f,0.0f }
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(_gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(_gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(_gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(_gfx, *this));

	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, _bdist(_rng)));
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
	/*return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);*/
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}

