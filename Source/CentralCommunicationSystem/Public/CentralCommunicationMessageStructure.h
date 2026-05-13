/*
	Central communication data structure

	Base message payload for communication message
	 -Sender
	 -Tag/Channel
	 -Payload(InputActionValue)
*/

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "EnhancedPlayerInput.h"
#include "CentralCommunicationMessageStructure.generated.h"

USTRUCT(BlueprintType)
struct FUniversalCommunicationMessage
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UObject> Sender = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Tag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadWrite)
	ETriggerEvent EventType = ETriggerEvent::None;

	UPROPERTY(BlueprintReadWrite)
	bool BoolInputActionValue = false;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D Vector2InputActionValue = FVector2D(0.0f, 0.0f);
	
	UPROPERTY(BlueprintReadWrite)
	float FloatInputActionValue = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector VectorInputActionValue = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(BlueprintReadWrite)
	bool bInputComplete = false;
};