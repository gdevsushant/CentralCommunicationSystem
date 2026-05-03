// Copyright (c) Sushant 2026. All Rights Reserved.

#include "CentralCommunicationSubsystem.h"
#include "GameplayMessageListenerData.h"
#include "NativeGameplayTags.h"
#include "GameplayMessageListenerList.h"

void UCentralCommunicationSubsystem::BroadcastCentralCommunicationMessage(const UObject* WorldContextObject, FGameplayTag Channel)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (!World) return;
	UCentralCommunicationSubsystem* Subsystem = World->GetSubsystem<UCentralCommunicationSubsystem>();
	if (!Subsystem) return;
	if (!Channel.IsValid()) return;
	if (Subsystem->ListenersMap.Num() == 0) return;

	if (UGameplayMessageListenerList** MessageListPtrPtr = Subsystem->ListenersMap.Find(Channel)) {
		UGameplayMessageListenerList* MessageListPtr = *MessageListPtrPtr;
		if (!MessageListPtr) return;

		for (int32 i = MessageListPtr->Listeners.Num() - 1; i >= 0; --i) {
			if (!MessageListPtr->Listeners.IsValidIndex(i)) continue;
			if (!MessageListPtr->Listeners[i].ListenerObject.Get()) {
				MessageListPtr->Listeners.RemoveAt(i);
			}
		}

		MessageListPtr->CentralMessageDelegate.Broadcast();
	}
}

UGameplayMessageListenerList* UCentralCommunicationSubsystem::RegisterListener(const UObject* WorldContextObject, UObject* Listener, FGameplayTag Channel)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (!World) return nullptr;
	UCentralCommunicationSubsystem* Subsystem = World->GetSubsystem<UCentralCommunicationSubsystem>();
	if (!Subsystem) return nullptr;
	if (!Channel.IsValid()) return nullptr;
	if (!Listener) return nullptr;

	FMessageData Data;
	Data.ListenerObject = Listener;
	Data.HandleId = Subsystem->HandleIdCounter;

	if (UGameplayMessageListenerList** MessageListPtrPtr = Subsystem->ListenersMap.Find(Channel)) {
		if (UGameplayMessageListenerList* MessageListPtr = *MessageListPtrPtr) {
			MessageListPtr->Listeners.Add(Data);
			Subsystem->HandleIdCounter++;
			return MessageListPtr;
		}
		return nullptr;
	}
	else {
		UGameplayMessageListenerList* List = NewObject<UGameplayMessageListenerList>(Subsystem);
		List->Listeners.Add(Data);
		Subsystem->ListenersMap.Add(Channel, List);
		Subsystem->HandleIdCounter++;
		return List;
	}
}

void UCentralCommunicationSubsystem::UnRegisterListener(const UObject* WorldContextObject, FGameplayTag Channel)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (!World) return;
	UCentralCommunicationSubsystem* Subsystem = World->GetSubsystem<UCentralCommunicationSubsystem>();
	if (!Subsystem) return;
	if (!Channel.IsValid()) return;

	Subsystem->ListenersMap.Remove(Channel);
}

bool UCentralCommunicationSubsystem::HasListenerRegistered(const UObject* WorldContextObject, FGameplayTag Channel)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (!World) return false;
	UCentralCommunicationSubsystem* Subsystem = World->GetSubsystem<UCentralCommunicationSubsystem>();
	if (!Subsystem) return false;
	if (!Channel.IsValid()) return false;

	if (UGameplayMessageListenerList** FoundList = Subsystem->ListenersMap.Find(Channel)) {
		return (FoundList && *FoundList);
	}

	return false;
}
