// Copyright (c) Sushant 2026. All Rights Reserved.

/*
	Stores a list of listeners for gameplay messages
	Stores a default delegate to broadcast messages to all active listeners
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayMessageListenerData.h"
#include "GameplayMessageListenerList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCentralMessage);

UCLASS(BlueprintType)
class CENTRALCOMMUNICATIONSYSTEM_API UGameplayMessageListenerList : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FMessageData> Listeners;

	UPROPERTY(BlueprintAssignable)
	FCentralMessage CentralMessageDelegate;
};