#pragma once
#include "Prerequisites.h"

// Declaracion directa a la Clase Device para ser usada en el cpp.
class Device;

class 
SamplerState{
public:
    SamplerState() = default;
    ~SamplerState() {};


    // Crear SamplerState, para modificar dimensiones de la textura. 
   
    void
        init(Device device);

    void
        update();

    void
        render();

    void
        destroy();

public:
    // Sampler State interface mantiene una descripcion del SamplerState para poder acceder
    // a ella por operaciones de muestreo de textura
    ID3D11SamplerState* m_sampler = nullptr;
};