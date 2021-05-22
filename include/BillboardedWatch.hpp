#pragma once

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"

DECLARE_CLASS_CODEGEN(GorillaUI::Components, BillboardedWatch, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, LateUpdate);
    DECLARE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, camTransform);

    REGISTER_FUNCTION(
        REGISTER_METHOD(LateUpdate);
        REGISTER_METHOD(OnEnable);
        REGISTER_FIELD(camTransform);
    )   
)