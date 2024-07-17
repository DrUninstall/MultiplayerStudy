// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MultiplayerStudy/HUD/MyHUD.h"
#include "MultiplayerStudy/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MultiplayerStudy/Character/MyCharacter.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MyHUD = Cast<AMyHUD>(GetHUD());
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(InPawn);
	if (MyCharacter)
	{
		SetHUDHealth(MyCharacter->GetHealth(), MyCharacter->GetMaxHealth());
	}
}


void AMyPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	MyHUD = MyHUD == nullptr ? Cast<AMyHUD>(GetHUD()) : MyHUD;

	bool bHUDValid = MyHUD &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay->HealthBar &&
		MyHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		MyHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		MyHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));

	}
}

void AMyPlayerController::SetHUDScore(float Score)
{
	MyHUD = MyHUD == nullptr ? Cast<AMyHUD>(GetHUD()) : MyHUD;

	bool bHUDValid = MyHUD &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay->ScoreAmount;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		MyHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
}

void AMyPlayerController::SetHUDDefeats(int32 Defeats)
{
	MyHUD = MyHUD == nullptr ? Cast<AMyHUD>(GetHUD()) : MyHUD;

	bool bHUDValid = MyHUD &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		MyHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
}

void AMyPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	MyHUD = MyHUD == nullptr ? Cast<AMyHUD>(GetHUD()) : MyHUD;

	bool bHUDValid = MyHUD &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay->WeaponAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		MyHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AMyPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	MyHUD = MyHUD == nullptr ? Cast<AMyHUD>(GetHUD()) : MyHUD;

	bool bHUDValid = MyHUD &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay &&
		MyHUD->CharacterOverlay->CarriedAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		MyHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}
