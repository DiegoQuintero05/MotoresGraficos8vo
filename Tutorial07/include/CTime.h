#include <Windows.h>
#pragma once

// Declaracion directa a la  Clase Tiempo, su funcion es generar un tiempo dentro de la 
// ejecucion del programa.

class 
CTime{
public:
	CTime();
	~CTime();
	
	void 
	init();

	void 
	update();

	void
	render();

	void 
	destroy();

public: 
	// Variable tiempo para la ejecucion del programa. 
	float m_fDeltaTime;
private:
	// Tiempo actual.
	LARGE_INTEGER m_currentTime;
	// Guardar el ultimo tiempo.
	LARGE_INTEGER m_lastTime;
	// Frecuencia del contador. 
	LARGE_INTEGER m_timerFrequency;
	
};