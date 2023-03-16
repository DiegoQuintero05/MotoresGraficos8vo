#pragma once
#include "Prerequisites.h"

// Declaracion directa a la Clase Device para ser usada en el cpp.
class
	Device;
// Declaracion directa a la Clase Texture para ser usada en el cpp.
class
	Texture;

class 
RenderTargetView{
public:
	RenderTargetView() = default;
	~RenderTargetView() {};

	// Crear RenderTargetView, recibe un device y un BackBuffer. Identifica los subrecursos
	// del RenderTarget que se pueden acceder durante el rendering.
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