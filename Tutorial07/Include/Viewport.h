#pragma once
#include "Prerequisites.h"

// Clase Window para ser usada en el cpp.
class Window;

// Dimensiones (Width, Height)
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
    D3D11_VIEWPORT m_viewport;
};