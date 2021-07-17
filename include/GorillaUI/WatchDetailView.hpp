#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, WatchDetailView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawDetails);
)