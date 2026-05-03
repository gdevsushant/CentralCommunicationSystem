# Central Communication System

**Central Communication System** is an Unreal Engine runtime plugin for decoupled gameplay communication.  
It provides a central message hub where gameplay systems can broadcast and listen to events through **Gameplay Tags** instead of direct object references.

The plugin is useful for systems such as input, weapons, abilities, UI, interactions, quests, audio triggers, and gameplay state changes where multiple systems need to communicate without tightly depending on each other.

---

## Features

- Gameplay Tag based communication channels
- Centralized `UWorldSubsystem` message hub
- Blueprint-callable broadcast, register, unregister, and listener-check functions
- Dynamic multicast delegate support for Blueprint event binding
- Weak listener object references for safer runtime object tracking
- Runtime plugin module for Unreal Engine
- Designed for modular gameplay architectures

---

## Why Use This Plugin?

In many gameplay systems, one component often needs to notify another component when something happens.

For example:

- Input system tells weapon system to fire
- Weapon system tells UI to update ammo
- Health system tells audio system to play damage sound
- Interaction system tells quest system that an object was used

A direct-reference design can become tightly coupled:

```txt
Input System  --->  Weapon System  --->  UI System
```

Central Communication System changes this into a message-channel design:

```txt
Input System
     |
     v
Gameplay Tag Channel: Input.Fire.Started
     |
     v
Weapon System / UI System / Audio System / Any Listener
```

The broadcaster does not need to know who is listening.  
The listener only needs to register to the same Gameplay Tag channel.

---

## Core Concept

The plugin uses `FGameplayTag` as the message channel.

Example channel names:

```txt
Input.Fire.Started
Input.Fire.Completed
Input.Fire.Cancelled
Weapon.Reload.Started
Weapon.Reload.Completed
Interaction.Object.Used
UI.Inventory.Opened
```

Any system can broadcast to a channel.  
Any object can register as a listener to that channel.

---

## Main Classes

### `UCentralCommunicationSubsystem`

`UCentralCommunicationSubsystem` is the main communication hub.

It derives from:

```cpp
UWorldSubsystem
```

It stores registered listeners using a Gameplay Tag keyed map and exposes static Blueprint-callable helper functions.

Main responsibilities:

- Broadcast messages on Gameplay Tag channels
- Register listener objects to a channel
- Unregister a channel
- Check if a channel has an active listener list

---

### `UGameplayMessageListenerList`

`UGameplayMessageListenerList` stores listeners for a single Gameplay Tag channel.

It contains:

```cpp
TArray<FMessageData> Listeners;
FCentralMessage CentralMessageDelegate;
```

`CentralMessageDelegate` is a Blueprint-assignable multicast delegate.  
Blueprints can bind custom events to this delegate after registering a listener.

---

### `FMessageData`

`FMessageData` stores listener metadata.

It contains:

```cpp
TWeakObjectPtr<UObject> ListenerObject;
int32 HandleId;
```

The weak object pointer helps avoid keeping destroyed listener objects alive.

---

## Blueprint Usage

### 1. Register a listener

Use:

```txt
Register Listener
```

Inputs:

| Input | Description |
|---|---|
| World Context Object | Usually `self` |
| Listener | The object that wants to listen |
| Channel | Gameplay Tag message channel |

Output:

```txt
Gameplay Message Listener List
```

After registration, bind a custom event to:

```txt
Central Message Delegate
```

---

### 2. Broadcast a message

Use:

```txt
Broadcast Central Communication Message
```

Inputs:

| Input | Description |
|---|---|
| World Context Object | Usually `self` |
| Channel | Gameplay Tag message channel |

Every active listener registered on that channel receives the delegate broadcast.

---

### 3. Check if a listener exists

Use:

```txt
Has Listener Registered
```

This returns `true` if the channel currently has a listener list.

---

### 4. Unregister a channel

Use:

```txt
UnRegister Listener
```

Current behavior:

```txt
Removes the full listener list for the Gameplay Tag channel.
```

Important: if multiple listeners are registered to the same channel, this removes the channel entry rather than removing only one specific listener object.

---

## C++ Usage Example

### Registering a listener

```cpp
#include "CentralCommunicationSubsystem.h"
#include "GameplayMessageListenerList.h"
#include "GameplayTagContainer.h"

void UMyWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    const FGameplayTag FireStartedTag =
        FGameplayTag::RequestGameplayTag(TEXT("Input.Fire.Started"));

    UGameplayMessageListenerList* ListenerList =
        UCentralCommunicationSubsystem::RegisterListener(
            this,
            this,
            FireStartedTag
        );

    if (ListenerList)
    {
        ListenerList->CentralMessageDelegate.AddDynamic(
            this,
            &UMyWeaponComponent::OnFireStarted
        );
    }
}
```

### Listener callback

```cpp
UFUNCTION()
void OnFireStarted();
```

```cpp
void UMyWeaponComponent::OnFireStarted()
{
    // Fire weapon logic here
}
```

---

## Broadcasting from C++

```cpp
#include "CentralCommunicationSubsystem.h"
#include "GameplayTagContainer.h"

void UMyInputComponent::HandleFireStarted()
{
    const FGameplayTag FireStartedTag =
        FGameplayTag::RequestGameplayTag(TEXT("Input.Fire.Started"));

    UCentralCommunicationSubsystem::BroadcastCentralCommunicationMessage(
        this,
        FireStartedTag
    );
}
```

---

## Example: Input System to Weapon System

### Input data asset

```txt
Input Action: IA_Fire
Gameplay Tag: Input.Fire.Started
Trigger State: Started
```

### Input system

The input system receives the input action and broadcasts:

```txt
Input.Fire.Started
```

### Weapon system

The weapon system registers to:

```txt
Input.Fire.Started
```

When the input system broadcasts, the weapon system receives the event without needing a direct reference to the input system.

This creates a low-coupling communication boundary between input and weapon logic.

---

## Build Dependencies

The module depends on:

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core",
    "CoreUObject",
    "Engine",
    "GameplayTags"
});
```

Make sure Gameplay Tags are enabled and configured in your project.

---

## Installation

### Option 1: Project Plugin

Copy the plugin folder into your Unreal project:

```txt
YourProject/
└── Plugins/
    └── CentralCommunicationSystem/
```

Then regenerate project files and rebuild the project.

---

### Option 2: Engine Plugin

Copy the plugin folder into your Unreal Engine plugins directory:

```txt
UnrealEngine/
└── Engine/
    └── Plugins/
        └── Marketplace/
            └── CentralCommunicationSystem/
```

Then rebuild the editor.

---

## Usage Recommendations

### Good use cases

Use this plugin for:

- Input event routing
- Weapon action dispatching
- Ability activation events
- UI update notifications
- Interaction events
- Gameplay state changes
- Audio or VFX triggers
- Modular component communication

---

### Avoid using it for

Avoid using this plugin as a replacement for every function call.

Do not use a central message system when:

- One object clearly owns another object
- A direct method call is simpler and safe
- You need return values
- You need strongly typed payload data
- You need guaranteed execution order

Central communication is best used for event notification, not for tightly controlled logic flow.

---

## Current Limitations

- The default delegate currently does not pass payload data.
- `UnRegisterListener` removes the full channel listener list.
- Listener callbacks must bind to the returned listener list delegate.
- Message channels depend on valid Gameplay Tags.
- This is an event-broadcasting system, not a full gameplay message router with typed message payloads.

---

## Suggested Future Improvements

Possible improvements for future versions:

- Add payload/message data support
- Add typed message structs
- Add listener handles for unregistering a single listener
- Add priority-based listener execution
- Add automatic cleanup on listener destruction
- Add debug logging and editor visualization
- Add Blueprint examples and sample maps
- Add unit tests or functional tests

---

## Example Architecture

```txt
+------------------+
|   Input System   |
+------------------+
         |
         | Broadcast: Input.Fire.Started
         v
+-------------------------------+
| Central Communication System  |
| Gameplay Tag Message Channel  |
+-------------------------------+
         |
         | Notify listeners
         v
+------------------+      +------------------+      +------------------+
|  Weapon System   |      |    UI System     |      |   Audio System   |
+------------------+      +------------------+      +------------------+
```

---
