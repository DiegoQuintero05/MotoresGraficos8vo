#include "Prerequisites.h"
#pragma once

class
	DeviceContext {
public:
	DeviceContext() = default;
	~DeviceContext() { SAFE_RELEASE(m_deviceContext) };

	void
		init();

	void
		update();

	void
		render();

	void
		destroy();

	void
		DrawIndexed(unsigned int IndexCount,
			unsigned int StartIndexLocation,
			unsigned int BaseVertexLocation);

	// Establecer array de SamplerStates para el pixel shader.
	void
		PSSetSamplers(unsigned int StartSlot,
			unsigned int NumSamplers,
			ID3D11SamplerState* const* ppSamplers);

	// ResourceShaders (recuros de textura), para el pixel shader.
	void
		PSSetShaderResources(unsigned int StartSlot,
			unsigned int NumViews,
			ID3D11ShaderResourceView* const* ppShaderResourceViews);

	// Constant buffers para el pixel shader.
	void
		PSSetConstantBuffers(unsigned int StartSlot,
			unsigned int NumBuffers,
			ID3D11Buffer* const* ppConstantBuffers);

	// Pixel shader para el/al device.
	void
		PSSetShader(ID3D11PixelShader* pPixelShader,
			ID3D11ClassInstance* const* ppClassInstances,
			unsigned int NumClassInstances);

	// Constant buffers para el vertex shader.
	void
		VSSetConstantBuffers(unsigned int StartSlot,
			unsigned int NumBuffers,
			ID3D11Buffer* const* ppConstantBuffers);

	// Vertex shader para el/al device.
	void
		VSSetShader(ID3D11VertexShader* pVertexShader,
			ID3D11ClassInstance* const* ppClassInstances,
			unsigned int NumClassInstances);

	// Limpiar DepthStencil.
	void
		ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
			unsigned int ClearFlags,
			float Depth,
			unsigned int Stencil);

	// Poner a un valor los valores del RenderTarget.
	void
		ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
			const float ColorRGBA[4]);

	// Render Target(s) y Depth Stencil Buffer.
	void
		OMSetRenderTargets(unsigned int NumViews,
			ID3D11RenderTargetView* const* ppRenderTargetViews,
			ID3D11DepthStencilView* pDepthStencilView);

	// Array de viewports para el rasterizado.
	void
		RSSetViewports(unsigned int NumViewports,
			const D3D11_VIEWPORT* pViewports);

	// Setear InputLayout
	void
		IASetInputLayout(ID3D11InputLayout* pInputLayout);

	// Array de VertexBuffers al InputAssembler.
	void
		IASetVertexBuffers(unsigned int StartSlot,
			unsigned int NumBuffers,
			ID3D11Buffer* const* ppVertexBuffers,
			const unsigned int* pStrides,
			const unsigned int* pOffsets);

	// IndexBuffer al input-assembler.
	void
		IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
			DXGI_FORMAT Format,
			unsigned int Offset);

	//  Informacion primitiva, y orden de datos del input data para el assembler stage.
	void
		IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

	// Actualizacion de recursos (datos en memoria). 
	void
		UpdateSubresource(ID3D11Resource* pDstResource,
			unsigned int DstSubresource,
			const D3D11_BOX* pDstBox,
			const void* pSrcData,
			unsigned int SrcRowPitch,
			unsigned int SrcDepthPitch);

public:

	ID3D11DeviceContext* m_deviceContext = nullptr;
};

