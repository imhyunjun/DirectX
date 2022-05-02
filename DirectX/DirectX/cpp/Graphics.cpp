#include "../header/Graphics.h"
#include <sstream>
#include "../dxerr.h"
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include "../header/Macros/GraphicThrowMacros.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")



Graphics::Graphics(HWND _hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;

	//프론트 백 버퍼, 디바이스 생성 후 swapchain
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	//스왑 체인의 텍스쳐 서브리소스에 접근( 백 버퍼)
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

	//depth stensil state 생성
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	//depth state 묶기
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	//depth state texture 생성
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800;
	descDepth.Height = 600;		//너비와 높이는 swapchain과 같아야 한다
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	//depth stensil texture view 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	//뷰포트
	D3D11_VIEWPORT vp;
	//뷰포트 크기
	vp.Width = 800.f;
	vp.Height = 600.f;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	//뷰포트 좌상단 좌표
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void Graphics::ClearBuffer(float _red, float _green, float _blue)
{
	const float color[] = { _red, _green, _blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT _count) noexcept(!IS_DEBUG)
{
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(_count, 0u, 0u));
}

void Graphics::SetProjection(DirectX::FXMMATRIX _proj) noexcept
{
	projection = _proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

//void Graphics::DrawTestTriangle(float _angle, float _x, float _z)
//{
//	//dx::XMVECTOR v = dx::XMVectorSet(3.0f, 3.0f, 0.0f, 0.0f);
//	//auto result = dx::XMVector4Dot(v, v);		//내적하면 스칼라가 아닌 벡터로 반환( 스칼라 * (1, 1, 1, 1))
//	//auto result = dx::XMVector4Transform(v, dx::XMMatrixScaling(1.5f, 2.0f, 0.f));
//	//xmvector은 윈도우 x64에선 16바이트 정렬이지만 윈도우 x86에선 8바이트 정렬
//	HRESULT hr;
//
//	struct Vertex
//	{
//		struct
//		{
//			float x;
//			float y;
//			float z;
//		}pos;
//		/*struct
//		{
//			unsigned char r;
//			unsigned char g;
//			unsigned char b;
//			unsigned char a;
//		}color;*/
//	};
//
//	const Vertex vertices[] =
//	{
//		//시계 방향(삼각형)
//		/*{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
//		{ 0.4f, -0.7f, 0.0f, 1.0f, 0.0f },
//		{ 0.3f, 0.8f, 0.0f, 0.0f, 1.0f },*/
//
//		/*{ 1.0f, -1.0f, 255, 0, 0, 255},
//		{ 0.4f, -0.7f, 0, 255, 0, 255 },
//		{ 0.3f, 0.8f, 0, 0, 255, 255 },*/
//
//
//		//선 하나에 무조건 점 두개씩(linelist)
//		/*{ 1.0f, -1.0f },
//		{ 0.4f, -0.7f },
//		{ 0.4f, -0.7f },
//		{ 0.3f, 0.8f },
//		{ 0.3f, 0.8f },
//		{ 1.0f, -1.0f },*/
//
//		//점 네개로 선을 긋는 것(linestrip)
//		//{ 1.0f, -1.0f },
//		//{ 0.4f, -0.7f },
//		//{ 0.3f, 0.8f },
//		//{ 1.0f, -1.0f },
//
//		//인덱스 사용 Draw 대신 DrawIndexed 사용
//		/*{0.5f, 1.0f, 255, 0, 0, 0},
//		{0.8f, 0.8f, 0, 255, 0, 0},
//		{0.8f, -0.8f, 0, 0, 255, 0},
//		{0.5f, -1.0f, 0, 0, 255, 0},
//		{0.2f, -0.8f, 0, 255, 0, 0},
//		{0.2f, 0.8f, 255, 0, 0, 0},*/
//
//		{-1.0f, -1.0f, -1.0f},
//		{1.0f, -1.0f, -1.0f},
//		{-1.0f, 1.0f, -1.0f},
//		{1.0f, 1.0f, -1.0f},
//		{-1.0f, -1.0f, 1.0f},
//		{1.0f, -1.0f, 1.0f},
//		{-1.0f, 1.0f, 1.0f},
//		{1.0f, 1.0f, 1.0f},
//
//	};
//
//	//인덱스 버퍼 만들기
//	const unsigned short indices[] =
//	{
//		/*0,1,2,
//		0,2,4,
//		2,3,4,
//		0,4,5*/
//		0,2,1, 2,3,1,
//		1,3,5, 3,7,5,
//		2,6,3, 3,6,7,
//		4,5,7, 4,7,6,
//		0,4,2, 2,4,6,
//		0,1,4, 1,5,4,
//	};
//	
//	//constant버퍼 만들기(변환)
//	struct ConstantBuffer
//	{
//		/*struct
//		{
//			float element[4][4];
//		}transformation;*/
//
//		dx::XMMATRIX transform;
//	};
//
//	const ConstantBuffer cb =
//	{
//		{
//			/*(3.0f / 4.0f) * std::cos(_angle), std::sin(_angle), 0.0f, 0.0f,
//			(3.0f / 4.0f) * -std::sin(_angle), std::cos(_angle), 0.0f, 0.0f,
//			0.0f, 0.0f, 1.0f, 0.0f,
//			0.0f, 0.0f, 0.0f, 1.0f*/
//			/*dx::XMMatrixMultiply(
//				dx::XMMatrixRotationZ(_angle),
//				dx::XMMatrixScaling(3.0f/4.0f, 1.0f, 1.0f))*/
//			dx::XMMatrixTranspose(
//			dx::XMMatrixRotationZ(_angle) *
//			dx::XMMatrixRotationX(_angle) *
//			//dx::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
//			dx::XMMatrixTranslation(_x, 0.0f, _z + 4.0f) *
//			dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
//			)
//		}
//	};
//
//	struct ConstantBuffer2
//	{
//		struct
//		{
//			float r;
//			float g;
//			float b;
//			float a;
//		}face_colors[6];
//	};
//	const ConstantBuffer2 cb2 =
//	{
//		{
//			{1.0f, 0.0f, 1.0f},
//			{1.0f, 0.0f, 0.0f},
//			{0.0f, 1.0f, 0.0f},
//			{0.0f, 0.0f, 1.0f},
//			{1.0f, 1.0f, 0.0f},
//			{0.0f, 1.0f, 1.0f},
//		}
//	};
//
//	//vertex layout 입력(2d)
//	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
//	const D3D11_INPUT_ELEMENT_DESC ied[] =
//	{
//		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0}
//		//{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0}
//	};//세번째 형식 중요!
//
//	//렌더 타겟
//	//pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
//
//	
//
//	//GFX_THROW_INFO_ONLY(pContext->Draw(std::size(vertices), 0u));		//그릴 점의 개수, 시작 점
//	
//}

Graphics::HrException::HrException(int _line, const char* _file, HRESULT _hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(_line, _file),
	hr(_hr)
{
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << "(" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description " << GetErrorDescription() << std::endl << GetOriginString();

	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "P Grphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const
{
	return DXGetErrorStringA(hr);
}

std::string Graphics::HrException::GetErrorDescription() const
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "P Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int _line, const char* _file, std::vector<std::string> infoMsgs)
	:
	Exception(_line, _file)
{
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();


	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const
{
	return info;
}
