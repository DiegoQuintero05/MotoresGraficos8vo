#include <Windows.h>
#pragma once

// Clase Tiempo, nos dara un tiempo para la ejecucion del programa.

class CTime
{
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
	float m_fDeltaTime;
private:
	// Variables para el cpp
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_lastTime;
	LARGE_INTEGER m_timerFrequency;
	
};