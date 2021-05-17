// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PokemonCharacter.generated.h"

UCLASS()
class TEMPPKMN_API APokemonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APokemonCharacter();

	/** Getter for Max Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** Getter for Current Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	// Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	// Event for taking damage. Overridden from APawn.
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The player's maximum health. This is the highest that their health can be, and the value that their health starts at when spawned.
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	// The player's current health. When reduced to 0, they are considered dead.
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	// RepNotify for changes made to current health.
	UFUNCTION()
	void OnRep_CurrentHealth();

	// Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify
	void OnHealthUpdate();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AMovementActor> MovementClass;

	// Delay between movements in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float MovementRate;

	// If true, we are in the process of throwing movements
	bool bIsPerformingAMovement;

	// If true, the character can't receive damage
	UPROPERTY(Replicated)
	bool Defeated;

	// Function for beggining a movement
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartMovement();

	// Function for ending a movement
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopMovement();

	// Server function for spawning movements
	UFUNCTION(Server, Reliable)
	void HandleMovement();

	// Timer for Movement
	FTimerHandle MovementTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Property replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
