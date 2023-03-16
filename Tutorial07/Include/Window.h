#pragma once
#include <Windows.h>

class
	Window{
public:
	Window();
	~Window();

	// Crear Ventana.
	// nCmdShow: Parametro para minimizar o maximizar la ventana.
	// WNDPROC: Puntero  a la funcion, define la mayoria del comportamiento.
	// windowName: String para la clase de la ventana. Cambio :D

	HRESULT
		init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc, LPCSTR windowName);

	void
		update();

	void
		render();

	void
		destroy();

public:
	// Window Hande devuelto por CreateWindowEx
	HWND m_hWnd = nullptr;
	// Especifica la instancia bajo la que la clase esta registrada.
	HINSTANCE m_hInst = nullptr;
	// Puntero a estructura que recibe las coordenadas del cliente.
	RECT m_rect;
	// Ancho.
	unsigned int m_width;
	// Altura.
	unsigned int m_height;
	// Nombre de la ventana.
	LPCSTR m_windowName = "None";
};