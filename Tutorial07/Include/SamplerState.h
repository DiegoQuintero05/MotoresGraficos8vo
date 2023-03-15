#pragma once
#include "Prerequisites.h"

// Clase Device para ser usada en el cpp.
class Device;

class SamplerState
{
public:
    SamplerState() = default;
    ~SamplerState() {};


    // Crear SamplerState, dimensiones de la textura. 
    void
        init(Device device);

    void
        update();

    void
        render();

    void
        destroy();

public:
    ID3D11SamplerState* m_sampler = nullptr;
};