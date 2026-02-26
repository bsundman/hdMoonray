// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "MoonrayLightAdapters.h"

#include <pxr/base/tf/registryManager.h>
#include <pxr/base/tf/type.h>
#include <pxr/base/tf/stringUtils.h>
#include <pxr/imaging/hd/light.h>
#include <pxr/imaging/hd/dataSourceLocator.h>
#include <pxr/usdImaging/usdImaging/primAdapter.h>

PXR_NAMESPACE_OPEN_SCOPE

namespace {

bool isMoonrayProperty(const TfToken& name)
{
    return TfStringStartsWith(name.GetString(), "moonray:");
}

bool hasMoonrayProperty(const TfTokenVector& properties)
{
    for (const TfToken& p : properties) {
        if (isMoonrayProperty(p)) return true;
    }
    return false;
}

} // namespace

// Register all Moonray light adapters in a single TfType registry function.
TF_REGISTRY_FUNCTION(TfType)
{
#define REGISTER_MOONRAY_LIGHT_ADAPTER(Name, Base)                           \
    {                                                                        \
        TfType t = TfType::Define<                                           \
            Moonray##Name##LightAdapter,                                     \
            TfType::Bases<Base>>();                                          \
        t.SetFactory<UsdImagingPrimAdapterFactory<                           \
            Moonray##Name##LightAdapter>>();                                  \
    }

    REGISTER_MOONRAY_LIGHT_ADAPTER(Sphere,   UsdImagingSphereLightAdapter)
    REGISTER_MOONRAY_LIGHT_ADAPTER(Rect,     UsdImagingRectLightAdapter)
    REGISTER_MOONRAY_LIGHT_ADAPTER(Disk,     UsdImagingDiskLightAdapter)
    REGISTER_MOONRAY_LIGHT_ADAPTER(Cylinder, UsdImagingCylinderLightAdapter)
    REGISTER_MOONRAY_LIGHT_ADAPTER(Distant,  UsdImagingDistantLightAdapter)
    REGISTER_MOONRAY_LIGHT_ADAPTER(Dome,     UsdImagingDomeLightAdapter)

#undef REGISTER_MOONRAY_LIGHT_ADAPTER
}

// Implement each adapter using a macro to avoid repetition.
#define MOONRAY_LIGHT_ADAPTER_IMPL(Name, Base)                               \
                                                                             \
Moonray##Name##LightAdapter::~Moonray##Name##LightAdapter() {}              \
                                                                             \
HdDirtyBits                                                                  \
Moonray##Name##LightAdapter::ProcessPropertyChange(                          \
    UsdPrim const& prim,                                                     \
    SdfPath const& cachePath,                                                \
    TfToken const& propertyName)                                             \
{                                                                             \
    if (isMoonrayProperty(propertyName)) {                                   \
        return HdLight::DirtyParams;                                         \
    }                                                                         \
    return Base::ProcessPropertyChange(prim, cachePath, propertyName);       \
}                                                                             \
                                                                             \
HdDataSourceLocatorSet                                                       \
Moonray##Name##LightAdapter::InvalidateImagingSubprim(                       \
    UsdPrim const& prim,                                                     \
    TfToken const& subprim,                                                  \
    TfTokenVector const& properties,                                         \
    UsdImagingPropertyInvalidationType invalidationType)                     \
{                                                                             \
    HdDataSourceLocatorSet result = Base::InvalidateImagingSubprim(          \
        prim, subprim, properties, invalidationType);                        \
    if (hasMoonrayProperty(properties)) {                                    \
        return HdDataSourceLocatorSet::UniversalSet();                       \
    }                                                                         \
    return result;                                                            \
}

MOONRAY_LIGHT_ADAPTER_IMPL(Sphere,   UsdImagingSphereLightAdapter)
MOONRAY_LIGHT_ADAPTER_IMPL(Rect,     UsdImagingRectLightAdapter)
MOONRAY_LIGHT_ADAPTER_IMPL(Disk,     UsdImagingDiskLightAdapter)
MOONRAY_LIGHT_ADAPTER_IMPL(Cylinder, UsdImagingCylinderLightAdapter)
MOONRAY_LIGHT_ADAPTER_IMPL(Distant,  UsdImagingDistantLightAdapter)
MOONRAY_LIGHT_ADAPTER_IMPL(Dome,     UsdImagingDomeLightAdapter)

#undef MOONRAY_LIGHT_ADAPTER_IMPL

PXR_NAMESPACE_CLOSE_SCOPE
