// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultiplayerStudy/HUD/MyHUD.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERSTUDY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class AMyCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(class AWeapon* WeaponToEquip);


protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void FireButtonPressed(bool bPressed);

	void Fire();

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);


private:
	UPROPERTY()
	class AMyCharacter* Character;

	UPROPERTY()
	class AMyPlayerController* Controller;

	UPROPERTY()
	class AMyHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	// HUD and Crosshairs

	UPROPERTY(EditAnywhere, Category = Crosshair)
	float CrosshairVelocityFactor;

	UPROPERTY(EditAnywhere, Category = Crosshair)
	float CrosshairInAirFactor = 2.f;

	UPROPERTY(EditAnywhere, Category = Crosshair)
	float CrosshairAimFactor = 0.58f;

	UPROPERTY(EditAnywhere, Category = Crosshair)
	float CrosshairShootingFactor = 0.2f;

	UPROPERTY(EditAnywhere, Category = Crosshair)
	FLinearColor CrosshairColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = Crosshair)
	FLinearColor OnTargetCrosshairColor = FLinearColor::Red;



	FVector HitTarget;

	FHUDPackage HUDPackage;

	

	// Aiming and FOV

	// FOV when not aiming; set to the camera's base FOV in BeginPlay
	UPROPERTY(EditAnywhere, Category = FOV)
	float DefaultFOV = 103;

	UPROPERTY(EditAnywhere, Category = FOV)
	float ZoomedFOV = 70.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = FOV)
	float ZoomInterpSpeed = 12.5f;

	void InterpFOV(float DeltaTime);

	// Automatic Fire
	FTimerHandle FireTimer;
	bool bCanFire = true;
	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

public:	
		
};
