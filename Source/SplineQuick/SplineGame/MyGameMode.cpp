#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyPawn.h"
#include "MyConfig.h"
#include "Util/Core/LogUtilLib.h"

MyGameModeType::AMyGameMode()
{
	DefaultPawnClass = MyPawnType::StaticClass();
	PlayerControllerClass = MyPCType::StaticClass();
}
