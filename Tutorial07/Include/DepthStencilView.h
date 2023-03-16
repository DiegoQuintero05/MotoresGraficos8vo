#include "Prerequisites.h"
#pragma once

// Declaracion directa a la  Clase Device para ser usada en el cpp.
class 
Device;

class
DepthStencilView{

public:
	DepthStencilView() = default;
	~DepthStencilView() { SAFE_RELEASE(m_pDepthStencilView) };

	// Crear DepthStencil View, accesa a los recursos de textura durante el DepthStencil Testing.
	void
	init(Device device, ID3D11Resource* depthStencil, DXGI_FORMAT Format);

	void
	update();

	void
	render();

	void
	destroy();

public:
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

};