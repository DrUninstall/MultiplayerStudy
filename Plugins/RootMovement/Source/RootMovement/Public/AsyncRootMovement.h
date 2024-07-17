// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncRootMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovementEvent);

/**
 * Enables root based movement with callbacks
 */
UCLASS()
class ROOTMOVEMENT_API UAsyncRootMovement : public UCancellableAsyncAction
{
  GENERATED_BODY()

  // The context world of this action
  TWeakObjectPtr<UWorld> ContextWorld = nullptr;

  // Timer handle for OnComplete management
  FTimerHandle OngoingDelay;

  // Character movement component to apply root movement to
  UCharacterMovementComponent* CharacterMovement;

  // World Direction of movement
  FVector WorldDirection;

  // Strength of movement
  float Strength;

  // Duration of movement
  float Duration;

  // If the movement is additive or not
  bool bIsAdditive;

  // Curve with strenght of movement over time
  UCurveFloat* StrengthOverTime;

  // Velocity Finish Mode of movement
  ERootMotionFinishVelocityMode VelocityOnFinishMode;

  // Velocity to set after movement finishes in SetVelocity mode; Ignored otherwise
  FVector SetVelocityOnFinish;

  // Clamp value to use after movement finishes in clamp mode; Ignored otherwise
  float ClampVelocityOnFinish;

  // Weather or not gravity should be enabled for movement
  bool bEnableGravity;

  // Place to save the active root motion source ID
  uint16 RootMotionSourceID;

  // A delegate called when the async action completes
  UPROPERTY(BlueprintAssignable)
    FMovementEvent OnComplete;

  // A delegate called when the async action fails
  UPROPERTY(BlueprintAssignable)
    FMovementEvent OnFail;

public:
  /**
   * Apply root motion movement to passed Character Movement Component
   */
  UFUNCTION(BlueprintCallable, DisplayName = "Apply Root Motion Constant Force with Callbacks", Category = "RootMovement", meta = (WorldContext = "WorldContext", BlueprintInternalUseOnly = "true", Keywords = "RootMovement RootMotion Root Motion Movement"))
    static UAsyncRootMovement* AsyncRootMovement(
      const UObject* WorldContext,
      UCharacterMovementComponent* CharacterMovement,
      FVector WorldDirection,
      float Strength,
      float Duration,
      bool bIsAdditive,
      UCurveFloat* StrengthOverTime,
      ERootMotionFinishVelocityMode VelocityOnFinishMode,
      FVector SetVelocityOnFinish,
      float ClampVelocityOnFinish,
      bool bEnableGravity
    );

  // Override functions from parent
  virtual void Activate() override;
  virtual void Cancel() override;

  virtual UWorld* GetWorld() const override
  {
    return ContextWorld.IsValid() ? ContextWorld.Get() : nullptr;
  }
};
