// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MultiplayerStudy/Character/MyCharacter.h"
#include "MultiplayerStudy/PlayerController/MyPlayerController.h"
#include "Net/UnrealNetwork.h"

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, Defeats);
}

void AMyPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	Character = Character == nullptr ? Cast<AMyCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller == nullptr ? Cast<AMyPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}


void AMyPlayerState::OnRep_Score()
	{
		Super::OnRep_Score();
		Character = Character == nullptr ? Cast<AMyCharacter>(GetPawn()) : Character;
		if (Character && Character->Controller)
		{
			Controller = Controller == nullptr ? Cast<AMyPlayerController>(Character->Controller) : Controller;
			if (Controller)
			{
				Controller->SetHUDScore(GetScore());
			}
		}
	}

void AMyPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character == nullptr ? Cast<AMyCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller = nullptr ? Cast<AMyPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void AMyPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<AMyCharacter>(GetPawn()) : Character;
	if (Character && Character->Controller)
	{
		Controller = Controller = nullptr ? Cast<AMyPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}


