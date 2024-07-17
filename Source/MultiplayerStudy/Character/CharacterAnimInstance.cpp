// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MultiplayerStudy/Weapon/Weapon.h"


void UCharacterAnimInstance::NativeInitializeAnimation()
{
  Super::NativeInitializeAnimation();

  MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
}


void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
  Super::NativeUpdateAnimation(DeltaTime);

  if (MyCharacter == nullptr)
  {
    MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
  }
  if (MyCharacter == nullptr) return;

  FVector Velocity = MyCharacter->GetVelocity();
  Velocity.Z = 0.f;
  Speed = Velocity.Size();

  bIsInAir = MyCharacter->GetCharacterMovement()->IsFalling();
  bIsAccelerating = MyCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
  bWeaponEquipped = MyCharacter->IsWeaponEquipped();
  EquippedWeapon = MyCharacter->GetEquippedWeapon();
  bIsCrouched = MyCharacter->bIsCrouched;
  bAiming = MyCharacter->IsAiming();
  TurningInPlace = MyCharacter->GetTurningInPlace();
  bRotateRootBone = MyCharacter->ShouldRotateRootBone();
  bElimmed = MyCharacter->IsElimmed();


  // Offset Yaw for Strafing
  FRotator AimRotation = MyCharacter->GetBaseAimRotation();
  FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MyCharacter->GetVelocity());
  FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
  DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
  YawOffset = DeltaRotation.Yaw;






  CharacterRotationLastFrame = CharacterRotation;
  CharacterRotation = MyCharacter->GetActorRotation();
  const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
  const float Target = Delta.Yaw / DeltaTime;
  const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
  Lean = FMath::Clamp(Interp, -90.f, 90.f);

  AO_Yaw = MyCharacter->GetAO_Yaw();
  AO_Pitch = MyCharacter->GetAO_Pitch();

  if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && MyCharacter->GetMesh())
  {
    LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
    FVector OutPosition;
    FRotator OutRotation;
    MyCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
    LeftHandTransform.SetLocation(OutPosition);
    LeftHandTransform.SetRotation(FQuat(OutRotation));


    if (MyCharacter->IsLocallyControlled())
    {
      bLocallyControlled = true;
      FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
      FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - MyCharacter->GetHitTarget()));
      RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 30.f);
    }
  }
}
