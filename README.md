# MonkeComputer

MonkeComputer is a library for Gorilla Tag for oculus quest, and replaces the in game pc with a custom pc, which allows mod makers to create their own custom UI in game for configuration or other actions
MonkeComputer also adds a wristwatch to the player, allowing for a mobile UI solution, making it easier than ever to configure things on the fly!

# Capabilities

There are multiple things that can be done using monkecomputer

## Creating Custom UI

### Creating Custom Views

For making Custom UI possible, You need to actually create a type that can be passed in to the computer for it to display!
Doing this is relatively easy, and just telling the computer or watch in the local object to redraw should be enough to update the screen.

Example of a header file for a custom view:
```c++
#pragma once
#include "custom-types/shared/macros.hpp"
#include "monkecomputer/shared/ViewLib/View.hpp"

DECLARE_CLASS_CUSTOM(namezpace, klass, GorillaUI::Components::View,
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, DidDeactivate);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
)    
```

Following that, you can then make text appear by writing to the string variable named `text` in your custom type (part of GorillaUI::View!)

```c++
void klass::DidActivate(bool firstActivation)
{
    if (firstActivation)
    {
        text = "Hello World For The First Time!";
    }
    else
    {
        text = "Hello World Again";
    }
    
    if (computer) computer->Redraw();
    else if (watch) watch->Redraw();
}
```

This type will write to text different messages based on if it is the first time it was activated, or not. Registering this view to the computer will make it show `Hello World For The First Time!` when you open the view the first time, and `Hello World Again` any other time, this distinction is useful if there are things you only want to initialize on your first opening  

The Method that also is of note here is the `OnKeyPressed` method. This method will let your view know a key was pressed, and thus let you do things with the keyboard keys.

An example below:

```c++
void klass::OnKeyPressed(int key)
{
    if (key == (int)EKeyboardKey::Enter)
    {
        text = "Enter Pressed!";
    }
    else
    {
        text = "Not Enter Pressed!";
    }
    
    if (computer) computer->Redraw();
    else if (watch) watch->Redraw();
}
```

This simple method shows that either the enter key was pressed, or a different key was pressed.

### Creating Custom View Managers

To Create a View Manager, you can inherit the View manager type in the view lib, an example of such a header is below:

```c++
#pragma once
#include "custom-types/shared/macros.hpp"
#include "monkecomputer/shared/ViewLib/ViewManager.hpp"
#include "monkecomputer/shared/ViewLib/View.hpp"

DECLARE_CLASS_CUSTOM(namezpace, klass, GorillaUI::Components::ViewManager,
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, DidDeactivate);
    DECLARE_INSTANCE_METHOD(void, NotifyOfKeyPress, int key);
    DECLARE_INSTANCE_METHOD(void, BackButtonWasPressed, GorillaUI::Components::View* view);
)
```

Regarding the DidActivate method, that part is the same, a more interesting method is the NotifyOfKeyPress method. This method will make it so that your viewmanager knows which button was pressed, and it can pass it along to it's current active view. An Example below:

```c++
void klass::NotifyOfKeyPress(int key)
{
    if (key == (int)EKeyboardKey::Back)
    {
        // we use these to run the method because it might not exist
        il2cpp_utils::RunMethod(this, "BackButtonWasPressed", activeView);
    }
    else
    {
        if (activeView) 
        {
            // we use these to run the method because the method might not exist 
            il2cpp_utils::RunMethod(activeView, "OnKeyPressed", key);
        }
        // log if there is no active view, that might be an issue
        else getLogger().error("No active view found");
    }
}
 
void klass::BackButtonWasPressed(GorillaUI::Components::View* view)
{
    // if the passed view (current active) is the same as the one we first see
    if (view == firstView)
    {
        // then this must mean the user wants to go back to the previous view manager, so dismiss this one
        if (parentViewManager)
            parentViewManager->DismissViewManager(this);
    }
    // if its a different one
    else 
    {
        // go back to our first view
        ReplaceTopView(firstView);
    }
}
    
```

### Advanced UI Capabilities

Within the monkecomputer shared header files, there are some helpers and input handlers, which mostly consist of being able to handle specific kinds of inputs. Most of their documentation can be found within their header files.
The input handlers are mostly designed for use within the OnKeyPressed method, while the helpers are designed for when you are creating the text that will be used by the computer to display

## Registering Custom UI elements

### Registering Views

You can register a view by including the header for it and calling the RegisterView template method, example below:

```c++
#include "monkecomputer/shared/Register.hpp"

extern "C" void load()
{
    // registering a main menu view
    GorillaUI::Register::RegisterView<Typename*>(ModID, Version);
    
    // registering a settings menu view
    GorillaUI::Register::RegisterSettingsView<Typename*>(ModID, Version);
    
    // registering a watch view
    GorillaUI::Register::RegisterWatchView<Typename*>(ModID, Version);
}
```

These methods will cause monkecomputer to automatically show your view as an option in their respective lists

### Registering ViewManagers

You can also register a ViewManager so you can have more control over what gets shown on the computer, this can for example be used for sub menus in the pc, example below:
```c++
#include "monkecomputer/shared/Register.hpp"

extern "C" void load()
{
    // registering a main menu viewmanager
    GorillaUI::Register::RegisterViewManager<Typename*>(ModID, Version);
    
    // registering a settings menu viewmanager
    GorillaUI::Register::RegisterSettingsViewManager<Typename*>(ModID, Version);
    
    // registering a watch viewmanager
    GorillaUI::Register::RegisterWatchViewManager<Typename*>(ModID, Version);
}
```

### Registering Callbacks

Lastly, you can register a callback, which is simply an option that executes the passed in std::function when the user presses enter on this option, example below:

```c++
#include "monkecomputer/shared/Register.hpp"

extern "C" void load()
{
    // registering a main menu callback
    GorillaUI::Register::RegisterCallback(ModID, Version, [](){
        getLogger().info("Callback Called!");
    });
    
    // registering a settings menu callback
    GorillaUI::Register::RegisterSettingsCallback(ModID, Version, [](){
        getLogger().info("Settings Callback Called!");
    });
    
    // registering a watch callback
    GorillaUI::Register::RegisterWatchCallback(ModID, Version, [](){
        getLogger().info("Watch Callback Called!");
    });
}
```


# Support Me!

For those who want to support me, I have multiple options available

-----

### [Patreon](https://www.patreon.com/Qosmetics)
Specifically for Qosmetics, there is a patreon that you can support (you can of course also donate here if you don't care about Qosmetics)

-----

### [Paypal](https://www.paypal.com/paypalme/RedBrumblerOfficial)
If you just want to donate one time, you can of course donate using paypal

-----

### [Ko-Fi](https://ko-fi.com/redbrumbler)
I do have a ko-fi, but it's never really been used nad I don't check it regularly

-----

