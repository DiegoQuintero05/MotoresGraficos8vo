#pragma once

// std lib
#include <iostream>
#include <sstream>
#include <vector>

// Librerias externas 
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

// Windows
#include <windows.h>

// Internal Includes (Nuestros)
#include "Resource.h"

// Defines
#define WINDOWS
#define WARNING( s )						            \
{											                      \
   std::wostringstream os_;					        \
   os_ << s;								                \
   OutputDebugStringW( os_.str().c_str() ); \
}

// Release seguro de los recursos.
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;


#define OutputLOG(_ClassName, _FunctionName, _OutputMessage)           \
OutputDebugStringA(_ClassName);                                        \
OutputDebugStringA(" : In Function : ");                               \
OutputDebugStringA(_FunctionName);                                     \
OutputDebugStringA(" : ");                                             \
OutputDebugStringA(_OutputMessage);                                    \
OutputDebugStringA("\n");
