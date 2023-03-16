#pragma once
#include "Prerequisites.h"

// Declaracion directa a la Clase Window para ser usada en el cpp.
class Window;

// Crear Viewport y definir Dimensiones (Width, Height)
class 
    Viewport{
public:
    
    Viewport() = default;
    ~Viewport() {};

    void
        init(Window window);

    void
        update();

    void
        render();

    void
        destroy();

public:
    //Dimensiones del Viewport.
    D3D11_VIEWPORT m_viewport;
};