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
	virtual void PostLoad() override;

	UFUNCTION(BlueprintPure, Category = Spline)
	AActor* GetSplineProvider() const { return SplineProvider; }

	UFUNCTION(BlueprintPure, Category = Spline)
	USplineComponent* GetSplineComponent() const { return SplineComponent; }

	UFUNCTION(BlueprintPure, Category = Velocity)
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintPure, Category = Transform)
	float GetLocationAlongSpline() const { return LocationAlongSpline; }

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	UFUNCTION(BlueprintCallable, Category = Movement)
	bool TestMoveTick(float InDeltaTime);

	// ~Spline Begin
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess = true), Category = "Path")
	AActor* SplineProvider = nullptr;

	/**
	* Spline component we are using now.
	* Because we cannot choose existing components in the scene, 
	* we should use the spline provider actor to find the spline component.
	*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Path")
	USplineComponent* SplineComponent = nullptr;
	// ~Spline End

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

	// ~Spline component Begin
	/**
	* Updates spline component property from the provider actor
	*/
	void UpdateSplineComponentPropertyFromProvider();
	// ~Spline component End

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
