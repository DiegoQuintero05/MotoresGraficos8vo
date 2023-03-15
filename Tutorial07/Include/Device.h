#include "Prerequisites.h" 
#pragma once
// Clase Device para ser usada en el cpp.
class
	Device
{
public:
	Device() = default;
	~Device() { SAFE_RELEASE(m_device) };

	void
		init();
	void
		update();

	void
		render();

	void
		destroy();

	// Crear DepthStencil view. 
	HRESULT
		CreateDepthStencilView(ID3D11Resource* pResource,
			const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
			ID3D11DepthStencilView** ppDepthStencilView);

	// Crear RenderTarget view.
	HRESULT
		CreateRenderTargetView(ID3D11Resource* pResource,
			const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
			ID3D11RenderTargetView** ppRTView);

	// Crear array de texturas 2D.
	HRESULT
		CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
			const D3D11_SUBRESOURCE_DATA* pInitialData,
			ID3D11Texture2D** ppTexture2D);

	// Crear VertexShader a partir de un shader compilado.
	HRESULT
		CreateVertexShader(const void* pShaderBytecode,
			unsigned int BytecodeLength,
			ID3D11ClassLinkage* pClassLinkage,
			ID3D11VertexShader** ppVertexShader);

	// Crear  InputLayout que describe los datos del InputBuffer .
	HRESULT
		CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
			unsigned int NumElements,
			const void* pShaderBytecodeWithInputSignature,
			unsigned int BytecodeLength,
			ID3D11InputLayout** ppInputLayout);

	// Crear PixelShader.
	HRESULT
		CreatePixelShader(const void* pShaderBytecode,
			unsigned int BytecodeLength,
			ID3D11ClassLinkage* pClassLinkage,
			ID3D11PixelShader** ppPixelShader);

	// Crear Buffer puede ser vertex buffer, index buffer, o ShaderConstant buffer).
	HRESULT
		CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
			const D3D11_SUBRESOURCE_DATA* pInitialData,
			ID3D11Buffer** ppBuffer);
	
	// Crear SamplerState, informacion del sampling para textura.
	HRESULT
		CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
			ID3D11SamplerState** ppSamplerState);

public:
	ID3D11Device* m_device = nullptr;

};