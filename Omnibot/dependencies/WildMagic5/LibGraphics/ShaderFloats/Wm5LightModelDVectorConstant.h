// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.1 (2010/10/01)

#ifndef WM5LIGHTMODELDVECTORCONSTANT_H
#define WM5LIGHTMODELDVECTORCONSTANT_H

#include "Wm5GraphicsLIB.h"
#include "Wm5ShaderFloat.h"
#include "Wm5Light.h"

namespace Wm5
{

class WM5_GRAPHICS_ITEM LightModelDVectorConstant : public ShaderFloat
{
    WM5_DECLARE_RTTI;
    WM5_DECLARE_NAMES;
    WM5_DECLARE_STREAM(LightModelDVectorConstant);

public:
    // Construction and destruction.
    LightModelDVectorConstant (Light* light);
    virtual ~LightModelDVectorConstant ();

    // Member access.
    Light* GetLight ();

    virtual void Update (const Visual* visual, const Camera* camera);

protected:
    LightPtr mLight;
};

WM5_REGISTER_STREAM(LightModelDVectorConstant);
typedef Pointer0<LightModelDVectorConstant> LightModelDVectorConstantPtr;

}

#endif
