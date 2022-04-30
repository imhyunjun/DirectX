#include "../header/Graphics.h"
#include <sstream>
#include "../dxerr.h"
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) { throw Graphics::InfoException(__LINE__,__FILE__, v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

Graphics::Graphics(HWND _hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
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
}

void Graphics::DrawTestTriangle(float _angle)
{
	namespace wrl = Microsoft::WRL;

	HRESULT hr;

	struct Vertex
	{
		struct
		{
			float x;
			float y;
		}pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}color;
	};

	const Vertex vertices[] =
	{
		//시계 방향(삼각형)
		/*{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
		{ 0.4f, -0.7f, 0.0f, 1.0f, 0.0f },
		{ 0.3f, 0.8f, 0.0f, 0.0f, 1.0f },*/

		/*{ 1.0f, -1.0f, 255, 0, 0, 255},
		{ 0.4f, -0.7f, 0, 255, 0, 255 },
		{ 0.3f, 0.8f, 0, 0, 255, 255 },*/


		//선 하나에 무조건 점 두개씩(linelist)
		/*{ 1.0f, -1.0f },
		{ 0.4f, -0.7f },
		{ 0.4f, -0.7f },
		{ 0.3f, 0.8f },
		{ 0.3f, 0.8f },
		{ 1.0f, -1.0f },*/

		//점 네개로 선을 긋는 것(linestrip)
		//{ 1.0f, -1.0f },
		//{ 0.4f, -0.7f },
		//{ 0.3f, 0.8f },
		//{ 1.0f, -1.0f },

		//인덱스 사용 Draw 대신 DrawIndexed 사용
		{0.5f, 1.0f, 255, 0, 0, 0},
		{0.8f, 0.8f, 0, 255, 0, 0},
		{0.8f, -0.8f, 0, 0, 255, 0},
		{0.5f, -1.0f, 0, 0, 255, 0},
		{0.2f, -0.8f, 0, 255, 0, 0},
		{0.2f, 0.8f, 255, 0, 0, 0},
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	//VertexShader를 파이프라인에 주입
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);		//IA -> input assembly 렌더링 파이프라인 시작

	wrl::ComPtr<ID3DBlob> pBlob;

	//인덱스 버퍼 만들기
	const unsigned short indices[] =
	{
		0,1,2,
		0,2,4,
		2,3,4,
		0,4,5
	};
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	//인덱스 버퍼 묶기
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//constant버퍼 만들기(변환)
	struct ConstantBuffer
	{
		struct
		{
			float element[4][4];
		}transformation;
	};

	const ConstantBuffer cb =
	{
		{
			(3.0f / 4.0f) * std::cos(_angle), std::sin(_angle), 0.0f, 0.0f,
			(3.0f / 4.0f) * -std::sin(_angle), std::cos(_angle), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.ByteWidth = sizeof(cb);
	cbd.Usage = D3D11_USAGE_DYNAMIC;	//Lock function 사용 가능,  non-dynamic에 UpdateSubresource로 업데이트 가능
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	//constant 버퍼 묶기(vs에 묶음)
	pContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	//PS 만들기(픽셀 셰이더)
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//VS 만들기 - 아래 레이아웃에서 vs 정보가 필요해서 순서 여기
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//vertex layout 입력(2d)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0}
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};//세번째 형식 중요!

	GFX_THROW_INFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	pContext->IASetInputLayout(pInputLayout.Get());

	//렌더 타겟
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	//기본 도형을 삼각형으로
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	//뷰포트
	D3D11_VIEWPORT vp;
	//뷰포트 크기
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	//뷰포트 좌상단 좌표
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	//GFX_THROW_INFO_ONLY(pContext->Draw(std::size(vertices), 0u));		//그릴 점의 개수, 시작 점
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(std::size(indices), 0u, 0u));		
}

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
