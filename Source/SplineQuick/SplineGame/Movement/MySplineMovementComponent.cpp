#include "MySplineMovementComponent.h"
#include "Util/Core/LogUtilLib.h"
#include "Util/Core/SystemUtils.h"

#include "Components/SplineComponent.h"

/**
* TODO Property validation:
* 1.1. SplineComponent must never be nullptr
*
* TODO Initialization:
* 1. Where to call UpdateComponentVelocity()?
* 2. Initial position: 
* 2.1. Should we check whether we using PIE?
*
* TODO Movement:
* 1. Teleport actor to initial location / rotation.
* 2. Move along the given spline
*
* TODO Misc:
* 1. Honor MaxSpeed
*/

namespace
{
	using MyComponentType = UMySplineMovementComponent;

	constexpr float TEST_SPEED = 100.0F; // UU / S (Cm / S)
	FVector const TEST_VELOCITY { TEST_SPEED, 0.0F, 0.0F };
} // Anonymous

MyComponentType::UMySplineMovementComponent()
:	Speed{ TEST_SPEED }
,	LocationAlongSpline { 0.0F }
,	bSweepWhenMove{ false }
{
	Velocity = TEST_VELOCITY;
}

void MyComponentType::BeginPlay()
{
	// @Note: BeginPlay of component always called AFTER the owner actor's BeginPlay is called

	M_LOGFUNC();
	Super::BeginPlay();

	if( nullptr == SplineComponent)
	{
		return;
	}

	if( nullptr == UpdatedComponent )
	{
		return;
	}

	SetupInitialTransform();
}

void MyComponentType::SetupInitialTransform()
{
	M_LOGFUNC();

	checkf(UpdatedComponent, TEXT("%s to be called only if SplineComponent is non-NULL"), TEXT(__FUNCTION__));
	checkf(UpdatedComponent, TEXT("%s to be called only if UpdatedComponent is non-NULL"), TEXT(__FUNCTION__));

	M_SWITCH_BEGIN(TransformMode)
		case EMySplineComponentTransformMode::KeepWorld:
		{
			M_LOG(TEXT("KeepWorld transform chosen, teleporting updated component to initial location/rotation"));
		}
		/*Fallthrough*/ M_TO_BE_IMPL(TEXT("Keeping world transform is NOT yet implemented, using Relative Transform"));

		case EMySplineComponentTransformMode::KeepRelative:
		{
			M_LOG(TEXT("KeepRelative transform chosen, teleporting updated component to initial location/rotation"));

			MoveAlongSpline(LocationAlongSpline, ETeleportType::ResetPhysics);
			break;
		}
	M_SWITCH_END()
}

FTransform MyComponentType::GetWorldTransformAlongSpline(float InDistance)
{
	FVector const NewWorldLocation = SplineComponent->GetLocationAtDistanceAlongSpline(InDistance, ESplineCoordinateSpace::World);
	FQuat const NewQuat = SplineComponent->GetQuaternionAtDistanceAlongSpline(InDistance, ESplineCoordinateSpace::World);
	return FTransform { NewQuat, NewWorldLocation };
}
void MyComponentType::MoveAlongSpline(float InDistance, ETeleportType InTeleportType)
{
	FTransform const NewWorldTransform = GetWorldTransformAlongSpline(LocationAlongSpline);
	MoveUpdatedComponent
	(
			NewWorldTransform.GetLocation() - UpdatedComponent->GetComponentLocation(), NewWorldTransform.GetRotation(), bSweepWhenMove, /*OutHitResult*/nullptr, InTeleportType
	);
}

void MyComponentType::PostInitProperties()
{
	M_LOGFUNC();

	M_LOG_WARN_IF(SplineComponent == nullptr, TEXT("SplineComponent pointer must NEVER be nullptr"));
}

void MyComponentType::TickComponent(float const InDeltaTime, ELevelTick const InLevelTick, FActorComponentTickFunction* const InThisTickFunction)
{
	if(ShouldSkipUpdate(InDeltaTime))
	{
		return;
	}

	Super::TickComponent(InDeltaTime, InLevelTick, InThisTickFunction);

	if(SplineComponent == nullptr)
	{
		return;
	}

	MoveAlongSpline(LocationAlongSpline, ETeleportType::None);
	
	//M_TO_BE_IMPL(TEXT("Use correct integration (verlet etc.)"));
	LocationAlongSpline = LocationAlongSpline + InDeltaTime * Speed;
}

bool MyComponentType::TestMoveTick(float const InDeltaTime)
{
	checkf(UpdatedComponent, TEXT("%s to be called only when UpdatedComponent is non-NULL"), TEXT(__FUNCTION__));

	FVector const DeltaLocation = InDeltaTime * Velocity;

	FHitResult Hit;
	bool const bCollisionEnabled = true;
	bool const bMoved = MoveUpdatedComponent(DeltaLocation, UpdatedComponent->GetComponentQuat(), bCollisionEnabled, &Hit/*, ETeleportType::None*/);

	return bMoved;
}
