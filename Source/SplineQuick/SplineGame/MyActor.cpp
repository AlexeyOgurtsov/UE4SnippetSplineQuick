#include "MyActor.h"
#include "Util/Core/LogUtilLib.h"
#include "Movement/MySplineMovementComponent.h"

AMyActor::AMyActor()
{
	Movement = CreateDefaultSubobject<UMySplineMovementComponent>(TEXT("Movement"));
}

void AMyActor::BeginPlay()
{
	M_LOGFUNC();
	Super::BeginPlay();
}
