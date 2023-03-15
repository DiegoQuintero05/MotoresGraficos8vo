#pragma once
#include "Prerequisites.h"

// Clase Device para ser usada en el cpp.
class
	Device;

// Clase DeviceContext para ser usada en el cpp.
class
	DeviceContext;

// Clase Window para ser usada en el cpp.
class
	Window;

// Clase Texture para ser usada en el cpp.
class
	Texture;

class SwapChain
{
public:
	SwapChain() = default;
	~SwapChain() {};

	// Crear Direct3D Device. Nos ayudara con lo que mostremos en pantalla.
	void
		init(Device& device,
			DeviceContext& deviceContext,
			Texture& backBuffer,
			Window window); 
	void
		update();
	void
		render();

	void
		destroy();

	void
		present();


private:
	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain* m_swapChain = nullptr;

};
