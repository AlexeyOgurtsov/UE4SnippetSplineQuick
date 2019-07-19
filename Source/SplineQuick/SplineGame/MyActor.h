#pragma once

/**
* To be copied to the destination and changed.
*/

#include "Util/TestUtil/TUVisibleActor.h"
#include "MyActor.generated.h"

class UMySplineMovementComponent;

UCLASS()
class AMyActor : public ATUVisibleActor
{
	GENERATED_BODY()

public:
	AMyActor();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess = true), Category = Movement)
	UMySplineMovementComponent* Movement = nullptr;
};
