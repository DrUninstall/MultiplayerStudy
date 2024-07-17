// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class MULTIPLAYERSTUDY_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACasing();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Impulse applied to the casing when it is ejected"))
	float ShellEjectionImpulse;


	UPROPERTY(EditAnywhere, meta = (ToolTip = "Sound played when the casing hits something"))
	class USoundCue* ShellSound;

	UPROPERTY(EditAnywhere, meta = (Tooltip = "The lifespan of the casing in seconds"))
	float CasingLifeSpan = 4.0f;

	// bool bHasPlayedSound = false;

	int32 SoundPlayCount = 0; // To Track the number of times the sound has been played

	UPROPERTY(EditAnywhere, meta = (Tooltip = "Minimum time interval between sound plays"))
	float SoundPlayInterval = 0.25f; // Minimum time interval between sound plays

	float LastSoundPlayTime = -SoundPlayInterval; // To track the last time the sound was played



};
