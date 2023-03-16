#pragma once
#include "Prerequisites.h"

// Declaracion directa a la Clase Device para ser usada en el cpp.
class
	Device;

// Declaracion directa a la Clase DeviceContext para ser usada en el cpp.
class
	DeviceContext;

// Declaracion directa a la Clase Window para ser usada en el cpp.
class
	Window;

// Declaracion directa a la Clase Texture para ser usada en el cpp.
class
	Texture;

// El Swap Chain controla la rotacion del back buffer. 
class 
	SwapChain{
public:
	SwapChain() = default;
	~SwapChain() {};

	// Crear Direct3D Device. 
	// Window: Dimensiones de la ventana (widht,height)
	
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
	// Representa el tipo de driver que se creara
	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	// Devuelve la direccion del puntero al IDXGISwapChain. 
	IDXGISwapChain* m_swapChain = nullptr;

};
