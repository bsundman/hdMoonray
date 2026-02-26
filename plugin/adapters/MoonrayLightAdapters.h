// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Per-type light adapters that add ProcessPropertyChange / InvalidateImagingSubprim
// support for custom moonray:* attributes.  Registering a derived class for
// each primTypeName makes UsdImaging mark the sprim dirty whenever any
// moonray:* property changes, which in turn bumps HdChangeTracker's scene
// state version and causes RenderDelegate::CommitResources to re-sync the
// light into the MoonRay scene.

#include "pxr/pxr.h"
#include "pxr/usdImaging/usdImaging/sphereLightAdapter.h"
#include "pxr/usdImaging/usdImaging/rectLightAdapter.h"
#include "pxr/usdImaging/usdImaging/diskLightAdapter.h"
#include "pxr/usdImaging/usdImaging/cylinderLightAdapter.h"
#include "pxr/usdImaging/usdImaging/distantLightAdapter.h"
#include "pxr/usdImaging/usdImaging/domeLightAdapter.h"

PXR_NAMESPACE_OPEN_SCOPE

// Declare one adapter subclass per UsdLux light type handled by hdMoonray.
// Each overrides ProcessPropertyChange (classic delegate path) and
// InvalidateImagingSubprim (scene-index path) to mark lights dirty when
// any moonray:* attribute changes.  All other methods are inherited from
// the specific base adapter so Populate/_RemovePrim/GetImagingSubprims etc.
// continue to work correctly.

#define MOONRAY_LIGHT_ADAPTER_DECL(Name, Base)                              \
class Moonray##Name##LightAdapter : public Base {                           \
public:                                                                      \
    Moonray##Name##LightAdapter() : Base() {}                               \
    virtual ~Moonray##Name##LightAdapter();                                  \
    HdDirtyBits ProcessPropertyChange(                                       \
        UsdPrim const& prim,                                                 \
        SdfPath const& cachePath,                                            \
        TfToken const& propertyName) override;                               \
    HdDataSourceLocatorSet InvalidateImagingSubprim(                         \
        UsdPrim const& prim,                                                 \
        TfToken const& subprim,                                              \
        TfTokenVector const& properties,                                     \
        UsdImagingPropertyInvalidationType invalidationType) override;       \
};

MOONRAY_LIGHT_ADAPTER_DECL(Sphere,   UsdImagingSphereLightAdapter)
MOONRAY_LIGHT_ADAPTER_DECL(Rect,     UsdImagingRectLightAdapter)
MOONRAY_LIGHT_ADAPTER_DECL(Disk,     UsdImagingDiskLightAdapter)
MOONRAY_LIGHT_ADAPTER_DECL(Cylinder, UsdImagingCylinderLightAdapter)
MOONRAY_LIGHT_ADAPTER_DECL(Distant,  UsdImagingDistantLightAdapter)
MOONRAY_LIGHT_ADAPTER_DECL(Dome,     UsdImagingDomeLightAdapter)

#undef MOONRAY_LIGHT_ADAPTER_DECL

PXR_NAMESPACE_CLOSE_SCOPE
