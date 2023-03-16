#include "Prerequisites.h"
#pragma once

// Declaracion directa a la Clase Device para ser usada en el cpp.
class Device;

class
InputLayout {
public:
	InputLayout() = default;
	~InputLayout() { SAFE_RELEASE(m_inputLayout); };

	// Crear InputLayout que describe el InputBuffer para input-assembler stage.
	void
		init(Device device,
			std::vector<D3D11_INPUT_ELEMENT_DESC> Layout,
			ID3DBlob* VertexShaderData);
	void
		update();

	void
		render();

	void
		destroy();

public:
	// InputLayout tiene la definicion de como proveer de datos de vertices que se presentan
	// en la memoria al InputAssembler.
	ID3D11InputLayout* m_inputLayout = nullptr;
};