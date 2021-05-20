// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Engine/Engine.h"
#include "PokemonCharacter.h"

ABattleGameMode::ABattleGameMode() 
{
	//Super::ABattleGameMode();
	DefaultPawnClass = APokemonCharacter::StaticClass();
}

APlayerController* ABattleGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMessage);
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, TEXT("TEST"));
	


	//if (!ErrorMessage.IsEmpty()) {
	//	return nullptr;
	//}

	

	//FString message = FString::Printf(TEXT("Login."));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, message);

	//return nullptr;
	//return x;
}

void ABattleGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	FString message = FString::Printf(TEXT("Welcome new player."));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, message);
}

void ABattleGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) {
	ErrorMessage = TEXT("incompatible_unique_net_id");
	//FString eMessage = FString::Printf(TEXT("Message coming from PReLogin. %s"), ErrorMessage);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMessage);

	FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
}
