//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// 
// 
// 
//DiegoQu --- IDVMI --- 8vo Cuatrimestre --- Arquitectura de Motores Gráficos 
//
//1er Parcial --- Sistema de Inputs para manipulacion de la posicion de un objeto. 
//
//Inputs: 
//W -> Arriba
//S -> Abajo
//A -> Izquierda
//D -> Derecha
//R -> Profundidad contrario a la pantalla (Atras)
//F -> Profundidad hacia la pantalla (Adelante)
// 
// 2do Parcial --- Abstracción de clases. 
// 
// Clases Abstraidas:
// - CTime
// - Window
// - DeviceContext
// - Device
// - DepthStencilView
// - Texture
// - InputLayout
// - SwapChain
// - RenderTargetView
// - SamplerState
// - Viewport
//
//--------------------------------------------------------------------------------------

//Mejor estructura, organizacion de librerias, se guardan los prerequisitos y ya no se 
//cargan por cada script.
#include "Prerequisites.h"     

// Libreria Time
#include "CTime.h"

// Abstracciones
#include "Window.h"
#include "DeviceContext.h"
#include "Device.h"
#include "DepthStencilView.h"
#include "Texture.h"
#include "InputLayout.h"
#include "SwapChain.h"
#include "RenderTargetView.h"
#include "SamplerState.h"
#include "Viewport.h"
#include "Transform.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------




//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
Window                              g_window;
DeviceContext                       g_deviceContext;
Device                              g_device;
DepthStencilView                    g_depthStencilView;
Texture                             g_ModelTexture;
Texture                             g_depthStencil;
Texture                             g_backBuffer;
InputLayout                         g_inputLayout;
SwapChain                           g_swapChain;
RenderTargetView                    g_renderTargetView;
SamplerState                        g_samplerState;
Viewport                            g_viewport;
Transform                           g_transform;
Camera                              cam;

D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11VertexShader*                 g_pVertexShader = nullptr;
ID3D11PixelShader*                  g_pPixelShader = nullptr;
ID3D11Buffer*                       g_pVertexBuffer = nullptr;
ID3D11Buffer*                       g_pIndexBuffer = nullptr;
ID3D11Buffer*                       g_Camera = nullptr;
ID3D11Buffer*                       g_pCBChangesEveryFrame = nullptr;

XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);




// Variables para P1.
CTime g_Time;

// Velocidad de movimiento


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();
void update();
//Destruir/limpiar los recursos del programa, para asi no llenar la RAM.
void destroy();








//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int 
WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, 
  int nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(g_window.init(hInstance, nCmdShow, WndProc, "DQWindow")))
        return 0;

    if (FAILED(InitDevice())){
        destroy();
        return 0;
    }

    //Init -> Tiempo (Se inicializa el tiempo)
    g_Time.init();
    g_transform.init();

    // Main message loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message){
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else{
            g_Time.update();
            update();
            Render();
        }
    }

    destroy();

    return (int)msg.wParam;
}





//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT 
CompileShaderFromFile(char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
  ID3DBlob** ppBlobOut){
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
    if (FAILED(hr)){
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain 
// **
//--------------------------------------------------------------------------------------
HRESULT 
InitDevice(){
    HRESULT hr = S_OK;

    // Crear SwapChain
    g_swapChain.init(g_device, g_deviceContext, g_backBuffer, g_window);

    // Crear RenderTargetView
    g_renderTargetView.init(g_device, g_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);

    g_backBuffer.destroy();
    if (FAILED(hr))
        return hr;

    g_depthStencil.init(g_device,
        g_window.m_width,
        g_window.m_height,
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        D3D11_BIND_DEPTH_STENCIL);

    // Crear DepthStencilView
    g_depthStencilView.init(g_device, g_depthStencil.m_texture, DXGI_FORMAT_D24_UNORM_S8_UINT);

    // Init -> VierPort
    g_viewport.init(g_window);


    // Compilar Shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile("Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr)){
        MessageBox(nullptr,
            "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
        return hr;
    }
 
    // Crear vertex shader
    hr = g_device.CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr)){
        pVSBlob->Release();
        return hr;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , 
      D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    unsigned int numElements = ARRAYSIZE(layout);
     

    std::vector <D3D11_INPUT_ELEMENT_DESC> Layout;

    D3D11_INPUT_ELEMENT_DESC position;
    position.SemanticName = "POSITION";
    position.SemanticIndex = 0;
    position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    position.InputSlot = 0;
    position.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    position.InstanceDataStepRate = 0;
    Layout.push_back(position);

    D3D11_INPUT_ELEMENT_DESC texcoord;
    texcoord.SemanticName = "TEXCOORD";
    texcoord.SemanticIndex = 0;
    texcoord.Format = DXGI_FORMAT_R32G32_FLOAT;
    texcoord.InputSlot = 0;
    texcoord.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    texcoord.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    texcoord.InstanceDataStepRate = 0;
    Layout.push_back(texcoord);

    
    // Create the input layout
    g_inputLayout.init(g_device, Layout, pVSBlob);

    pVSBlob->Release();
    if (FAILED(hr))
        return hr;

    // Compile the pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile("Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.",
          "Error", MB_OK);
        return hr;
    }

    // Create the pixel shader
    hr = g_device.CreatePixelShader(pPSBlob->GetBufferPointer(),
      pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;
     
    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = vertices;
    hr = g_device.CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return hr;
    
    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    g_deviceContext.IASetVertexBuffers(0, 1, &g_pVertexBuffer,
      &stride, &offset);

    // Create index buffer
    // Create vertex buffer
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };
     
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = g_device.CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
        return hr;

    // Set index buffer
    g_deviceContext.IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    // 
    // Set primitive topology
    g_deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Camera);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_device.CreateBuffer(&bd, nullptr, &g_Camera);
    if (FAILED(hr))
        return hr;

    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_device.CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);
    if (FAILED(hr))
        return hr;

    // Load the Texture
    g_ModelTexture.init(g_device, "seafloor.dds");

    if (FAILED(hr))
        return hr;
    // 
    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_device.CreateSamplerState(&sampDesc, &g_samplerState.m_sampler);
    if (FAILED(hr))
        return hr;

    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    g_View = XMMatrixLookAtLH(Eye, At, Up);



    // Initialize the projection matrix (global)
    g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
      g_window.m_width / (FLOAT)g_window.m_height, 0.01f, 100.0f);

    cam.mView = XMMatrixTranspose(g_View);
    cam.mProjection = XMMatrixTranspose(g_Projection);

    return S_OK;
}





void 
  update(){

  g_transform.m_fRotateNum += 0.0002f;


  // Rotate cube around the origin
  g_World = XMMatrixScaling(g_transform.m_fScaleNum,
    g_transform.m_fScaleNum,
    g_transform.m_fScaleNum) *

    XMMatrixRotationY(g_transform.m_fRotateNum) *

    XMMatrixTranslation(g_transform.m_v3Position.x,
      g_transform.m_v3Position.y,
      g_transform.m_v3Position.z);
  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixTranspose(g_World);
  cb.vMeshColor = g_vMeshColor;

  g_deviceContext.UpdateSubresource(g_pCBChangesEveryFrame, 0,
    nullptr, &cb, 0, 0);

  g_deviceContext.UpdateSubresource(g_Camera, 0, nullptr,
    &cam, 0, 0);
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void 
  destroy(){
    g_deviceContext.destroy();
    g_samplerState.destroy();
    g_ModelTexture.destroy();

    if (g_Camera) g_Camera->Release();
    if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pIndexBuffer) g_pIndexBuffer->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShader) g_pPixelShader->Release();

    g_depthStencil.destroy();
    g_depthStencilView.destroy();
    g_renderTargetView.destroy();
    g_swapChain.destroy();
    g_device.destroy();

}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------

// Se llama a esta funcion cada vez que recibe un mensaje. Por ejemplo utiliza wParam ya 
// que establece o obtiene el campo wParam del mensaje. 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message){
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

        //WM_KEYDOWN para las Nonsystem Keys que son las que son presionadas sin ALT, esto
        //nos nos permite usar W,A,S,D u otras teclas como inputs.
    case WM_KEYDOWN:

        //wParam para el campo del mensaje. 
        switch (wParam)
        {

            //Input tecla W.
        case 'W':
            // Se modifica el Vector3 de la posicion del objeto en su eje "y" (el cual es
            //un float), asi le sumamos a su valor actual la velocidad por el tiempo.
            g_transform.m_v3Position.y += g_transform.m_fSpeed * g_Time.m_fDeltaTime;
            break;

            //Input tecla A
        case 'A':
            // Se modifica el Vector3 de la posicion del objeto en su eje "x" (el cual es 
            //un float), asi le restamos a su valor actual la velocidad por el tiempo.
            g_transform.m_v3Position.x -= g_transform.m_fSpeed * g_Time.m_fDeltaTime;
            break;

            //Input tecla S
        case 'S':
            // Se modifica el Vector3 de la posicion del objeto en su eje "y" (el cual es 
            //un float), asi le restamos a su valor actual la velocidad por el tiempo.
            g_transform.m_v3Position.y -= g_transform.m_fSpeed * g_Time.m_fDeltaTime;
            break;

            //Input tecla D
        case 'D':
            // Se modifica el Vector3 de la posicion del objeto en su eje "x" (el cual es 
            //un float), asi le sumamos a su valor actual la velocidad por el tiempo.
            g_transform.m_v3Position.x += g_transform.m_fSpeed * g_Time.m_fDeltaTime;
            break;

            //Input tecla R
        case 'R':
            // Se modifica el Vector3 de la posicion del objeto en su eje "z" (el cual es 
            //un float), asi le sumamos a su valor actual la velocidad por el tiempo.
            g_transform.m_v3Position.z += g_transform.m_fSpeed * g_Time.m_fDeltaTime;
            break;

            //Input tecla F
        case 'F':
            // Se modifica el Vector3 de la posicion del objeto en su eje "z" (el cual es 
            //un float), asi le restamos a su valor actual la velocidad por el tiempo.
            g_transform.m_v3Position.z -= g_transform.m_fSpeed * g_Time.m_fDeltaTime;
            break;
            // Color Inicial
        case '1':
            g_vMeshColor = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
            break;
            // Color rojo
        case '2':
            g_vMeshColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
            // Color naranja
        case '3':
            g_vMeshColor = XMFLOAT4(1.0f, 0.7f, 0.3f, 0.0f);
            break;
            // Color verde 
        case '4':
            g_vMeshColor = XMFLOAT4(0.3f, 0.5f, 0.0f, 0.5f);
            break;
        }

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void 
Render(){
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_deviceContext.ClearRenderTargetView(g_renderTargetView.m_renderTargetView,
      ClearColor);
    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    g_deviceContext.ClearDepthStencilView(g_depthStencilView.m_pDepthStencilView
      , D3D11_CLEAR_DEPTH, 1.0f, 0);

    g_deviceContext.OMSetRenderTargets(1,
      &g_renderTargetView.m_renderTargetView, 
      g_depthStencilView.m_pDepthStencilView);

    g_deviceContext.RSSetViewports(1, &g_viewport.m_viewport);

    //Set inputlayout
    g_deviceContext.IASetInputLayout(g_inputLayout.m_inputLayout);

    //
    // Render the cube
    //
    g_deviceContext.VSSetShader(g_pVertexShader, nullptr, 0);
    g_deviceContext.VSSetConstantBuffers(0, 1, &g_Camera);

    g_deviceContext.VSSetConstantBuffers(1, 1, &g_pCBChangesEveryFrame);
    g_deviceContext.PSSetShader(g_pPixelShader, nullptr, 0);

    g_deviceContext.PSSetConstantBuffers(1, 1, &g_pCBChangesEveryFrame);
    g_deviceContext.PSSetShaderResources(0, 1, &g_ModelTexture.m_textureFromImg);

    g_deviceContext.PSSetSamplers(0, 1, &g_samplerState.m_sampler);
    g_deviceContext.DrawIndexed(36, 0, 0);

    //
    // Present our back buffer to our front buffer
    //
    g_swapChain.present();
}