// Copyright (c) Sushant 2026. All Rights Reserved.

/*
	Central message listener data structure

	Contains the listener object
	Contains the handle id of the listener
*/

#pragma once

#include "CoreMinimal.h"
#include "GameplayMessageListenerData.generated.h"

USTRUCT()
struct FMessageData
{
	GENERATED_BODY()

public:

	TWeakObjectPtr<UObject> ListenerObject;
	int32 HandleId = 0;
};