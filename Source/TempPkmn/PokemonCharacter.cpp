// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MovementActor.h"

// Sets default values
APokemonCharacter::APokemonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the player's Health	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

    // Initialize movement class
    MovementClass = AMovementActor::StaticClass();
    // Initialize movement rate
    MovementRate = 0.5f; // Time in seconds
    bIsPerformingAMovement = false;
    Defeated = false;
}

// Called when the game starts or when spawned
void APokemonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APokemonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APokemonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    // Handle perfoming movements
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APokemonCharacter::StartMovement);
}

void APokemonCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate current health.
	DOREPLIFETIME(APokemonCharacter, CurrentHealth);
    DOREPLIFETIME(APokemonCharacter, Defeated);
}

void APokemonCharacter::OnHealthUpdate()
{
    if (Defeated) {
        return;
    }

    // Client-specific functionality
    if (IsLocallyControlled())
    {
        FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

        if (CurrentHealth <= 0)
        {
            FString deathMessage = FString::Printf(TEXT("You have been killed."));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
            Defeated = true;
        }
    }

    // Server-specific functionality
    if (GetLocalRole() == ROLE_Authority)
    {
        FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
    }

    // Functions that occur on all machines. 
    /*
        Any special functionality that should occur as a result of damage or death should be placed here.
    */
}

void APokemonCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void APokemonCharacter::SetCurrentHealth(float healthValue)
{
    if (GetLocalRole() == ROLE_Authority) {
        CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
        OnHealthUpdate();
    }
}

float APokemonCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (!Defeated) {
        float damageApplied = CurrentHealth - DamageTaken;
        SetCurrentHealth(damageApplied);
        return damageApplied;
    }

    return 0.0f;
}

void APokemonCharacter::StartMovement() 
{
    if (!bIsPerformingAMovement && !Defeated) {
        bIsPerformingAMovement = true; 
        UWorld* World = GetWorld();
        World->GetTimerManager().SetTimer(MovementTimer, this, &APokemonCharacter::StopMovement, MovementRate, false);
        HandleMovement();
    }
    else if (Defeated) {
        FString deathMessage = FString::Printf(TEXT("You cannot perform any movement. You are defeated."));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
    }
}

void APokemonCharacter::StopMovement()
{
    bIsPerformingAMovement = false;
}

void APokemonCharacter::HandleMovement_Implementation()
{
    FVector spawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
    FRotator spawnRotation = GetControlRotation();

    FActorSpawnParameters spawnParameters;
    spawnParameters.Instigator = GetInstigator();
    spawnParameters.Owner = this;

    AMovementActor* spawnedMovement = GetWorld()->SpawnActor<AMovementActor>(spawnLocation, spawnRotation, spawnParameters);
}
