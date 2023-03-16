#pragma once
#include "Prerequisites.h"

// Clase Device para ser usada en el cpp.
class
	Device;
// Clase Texture para ser usada en el cpp.
class
	Texture;

class 
RenderTargetView{
public:
	RenderTargetView() = default;
	~RenderTargetView() {};

	// Crear RenderTargetView --> (Vector,BackBuffer)
	void
		init(Device device, Texture backBuffer, DXGI_FORMAT Format);

	void
		update();

	void
		render();

	void
		destroy();

public:
	ID3D11RenderTargetView* m_renderTargetView = nullptr;

};