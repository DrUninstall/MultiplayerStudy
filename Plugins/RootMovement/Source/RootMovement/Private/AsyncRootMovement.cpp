// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncRootMovement.h"
#include "Engine.h"

// This function is just a factory that creates a UAsyncRootMovement instance.
UAsyncRootMovement* UAsyncRootMovement::AsyncRootMovement(
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
  bool bEnableGravity)
{
  UWorld* ContextWorld = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
  if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
  {
    return nullptr;
  }

  // Create node and store variables
  UAsyncRootMovement* Node = NewObject<UAsyncRootMovement>();
  Node->ContextWorld = ContextWorld;
  Node->CharacterMovement = CharacterMovement;
  Node->WorldDirection = WorldDirection;
  Node->Strength = Strength;
  Node->Duration = Duration;
  Node->bIsAdditive = bIsAdditive;
  Node->StrengthOverTime = StrengthOverTime;
  Node->VelocityOnFinishMode = VelocityOnFinishMode;
  Node->SetVelocityOnFinish = SetVelocityOnFinish;
  Node->ClampVelocityOnFinish = ClampVelocityOnFinish;
  Node->bEnableGravity = bEnableGravity;
  Node->RegisterWithGameInstance(ContextWorld->GetGameInstance());
  return Node;
}

void UAsyncRootMovement::Activate()
{
  if (const UWorld* World = GetWorld())
  {
    // Create a root motion source
    TSharedPtr<FRootMotionSource_ConstantForce> ConstantForce = MakeShared<FRootMotionSource_ConstantForce>();
    ConstantForce->InstanceName = FName("ConstantForce");
    ConstantForce->AccumulateMode = bIsAdditive ? ERootMotionAccumulateMode::Additive : ERootMotionAccumulateMode::Override;
    ConstantForce->Priority = 5;
    ConstantForce->Force = WorldDirection * Strength;
    ConstantForce->Duration = Duration;
    ConstantForce->StrengthOverTime = StrengthOverTime;
    ConstantForce->FinishVelocityParams.Mode = VelocityOnFinishMode;
    ConstantForce->FinishVelocityParams.SetVelocity = SetVelocityOnFinish;
    ConstantForce->FinishVelocityParams.ClampVelocity = ClampVelocityOnFinish;

    if (bEnableGravity)
    {
      ConstantForce->Settings.SetFlag(ERootMotionSourceSettingsFlags::IgnoreZAccumulate);
    }

    if (CharacterMovement) {
      // Apply root motion source to the character's movement component and save ID
      RootMotionSourceID = CharacterMovement->ApplyRootMotionSource(ConstantForce);

      // Set up timer for OnComplete delay
      FTimerManager& TimerManager = World->GetTimerManager();

      // We set the timer for DelayTime, and we pass in the callback function as a lambda.
      TimerManager.SetTimer(
        OngoingDelay,
        FTimerDelegate::CreateLambda([WeakThis = TWeakObjectPtr<UAsyncRootMovement>(this)]()
      {
        // We're passing "this" as a weak pointer, because there is no guarantee that "this" will
        // exist by the time this lambda callback executes.
        if (WeakThis.IsValid() && WeakThis->IsActive())
        {
          // If everything went well, broadcast OnComplete (fire the On Complete pin), and wrap up.
          WeakThis->OnComplete.Broadcast();
          WeakThis->Cancel();
        }
      }),
        Duration,
        false
        );

      return;
    }
  }

  // If something failed, we can broadcast OnFail, and then wrap up.
  OnFail.Broadcast();
  Cancel();
}

void UAsyncRootMovement::Cancel()
{
  Super::Cancel();

  // Cancel the timer if it's ongoing, so OnComplete never broadcasts.
  if (OngoingDelay.IsValid())
  {
    if (const UWorld* World = GetWorld())
    {
      // Cancel Root Movement
      CharacterMovement->RemoveRootMotionSourceByID(RootMotionSourceID);

      // Clear up timers
      FTimerManager& TimerManager = World->GetTimerManager();
      TimerManager.ClearTimer(OngoingDelay);
    }
  }
}