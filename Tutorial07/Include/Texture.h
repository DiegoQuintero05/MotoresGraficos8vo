#pragma once
#include "Prerequisites.h"
#include "Commons.h"

// Declaracion directa a la Clase Device para ser usada en el cpp.
class
	Device;

class
	Texture{
public:
	Texture() = default;
	~Texture() {};

	// Inicializar textura desde archivo.
	void
		init(Device device, std::string textureName);

	// Inicializar textura desde recursos, crear array 2D de textura.
	void
		init(Device device,
			unsigned int width,
			unsigned int height,
			DXGI_FORMAT Format,
			unsigned int BindFlags);
	void
		update();

	void
		render();

	void
		destroy();

public:
	// Texture Resource (recursos de textura)  -> Datos.
	ID3D11Texture2D* m_texture = nullptr;

	// Texture Resource (recursos de textura)  -> Datos de imagen.
	ID3D11ShaderResourceView* m_textureFromImg = nullptr;

};
