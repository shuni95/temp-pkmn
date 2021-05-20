// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEMPPKMN_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Sets default values for Battle Mode
	ABattleGameMode();

public:
	UFUNCTION()
	void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION()
	APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION()
	void PostLogin(APlayerController* NewPlayer) override;
};
