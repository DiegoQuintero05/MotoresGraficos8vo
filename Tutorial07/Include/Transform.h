#pragma once
#include "Prerequisites.h"
#include "Commons.h"

class
	Transform {
public:
	Transform() = default;
	~Transform() {};

	//Inicializamos el transform 
	//(variables: escala, rotación, velocidad y posición)
	void
		init();

	void
		update();

	void
		render();

	void
		destroy();

public:
	// Vector Posicion
	Vector3 m_v3Position;
	// Vector de escala
	float		m_fScaleNum;
	// Vector de rotacion
	float		m_fRotateNum;
	// Vector de velocidad
	float		m_fSpeed;
};