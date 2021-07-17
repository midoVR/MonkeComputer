#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/GorillaTriggerBox.hpp"

DECLARE_CLASS_CODEGEN(GorillaUI::Components, WatchActivatorTrigger, GlobalNamespace::GorillaTriggerBox,
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(bool, IsUpright);
)