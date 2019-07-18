#pragma once

#include "Util/TestUtil/TUMovementComponent.h"
#include "MySplineMovementComponent.generated.h"

class USplineComponent;

UENUM(BlueprintType)
enum class EMySplineComponentTransformMode : uint8
{
	KeepWorld           UMETA(DisplayName = "Keep World"),
	KeepRelative        UMETA(DisplayName = "Keep Relative")
};

UCLASS(ClassGroup=(Movement), Meta=(BlueprintSpawnableComponent))
class UMySplineMovementComponent : public UTUMovementComponent
{
	GENERATED_BODY()

public:
	UMySplineMovementComponent();

	virtual void TickComponent(float InDeltaTime, ELevelTick InLevelTick, FActorComponentTickFunction* InThisTickFunction) override;

	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintPure, Category = Spline)
	USplineComponent* GetSplineComponent() const { return SplineComponent; }

	UFUNCTION(BlueprintPure, Category = Velocity)
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintPure, Category = Transform)
	float GetLocationAlongSpline() const { return LocationAlongSpline; }

protected:
	UFUNCTION(BlueprintCallable, Category = Movement)
	bool TestMoveTick(float InDeltaTime);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess = true), Category = "Path")
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true), Category = "Velocity")
	float Speed;

	/**
	* Current location along spline:
	* Distance along spline from the start spline point.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true), Category = "Location")
	float LocationAlongSpline;

private:
	// ~Config Begin
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true), Category = "Transform")
	EMySplineComponentTransformMode TransformMode = EMySplineComponentTransformMode::KeepWorld;
	// ~Config End

	UFUNCTION()
	void SetupInitialTransform();

	/**
	* Compute transform of the updated component along spline at the given distance along spline.
	*/
	UFUNCTION()
	FTransform GetWorldTransformAlongSpline(float InDistance);

	UFUNCTION()
	void MoveAlongSpline(float InDistance, ETeleportType InTeleportType);

	UPROPERTY()
	bool bSweepWhenMove;
};
