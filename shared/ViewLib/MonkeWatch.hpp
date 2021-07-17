#pragma once

#include "custom-types/shared/macros.hpp"
#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"

#include "../ViewLib/CustomScreenInfo.hpp"
#include "../ViewLib/ViewManager.hpp"
#include "../ViewLib/View.hpp"
//#include "../ViewLib/MonkeWatchButton.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"

namespace GorillaUI::Components {
    class MonkeWatchButton;
}

DECLARE_CLASS_CODEGEN(GorillaUI, MonkeWatch, UnityEngine::MonoBehaviour, 
    DECLARE_STATIC_METHOD(void, SetupWatch, UnityEngine::GameObject* watchGO);
    DECLARE_STATIC_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, OnApplicationFocus, bool hasFocus);
    DECLARE_STATIC_METHOD(MonkeWatch*, get_instance);
    DECLARE_INSTANCE_METHOD(void, SetupButtons);
    DECLARE_INSTANCE_METHOD(void, SetActive, bool value);
    DECLARE_INSTANCE_FIELD(bool, isActive);

    public:
        GorillaUI::Components::ViewManager* activeViewManager = nullptr;
        void PressButton(GorillaUI::Components::MonkeWatchButton* button);
        void Init(GorillaUI::Components::View* initialView, UnityEngine::GameObject* watchGO);
        void Init(GorillaUI::Components::View* initialView);
    private:
        CustomScreenInfo screenInfo;
        static SafePtr<MonkeWatch> instance;
        CustomScreenInfo CreateWatch();
        CustomScreenInfo CreateWatch(UnityEngine::GameObject* watchGO);
)