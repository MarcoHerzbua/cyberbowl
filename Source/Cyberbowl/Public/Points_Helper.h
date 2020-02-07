#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Points.h"
#include "Goal_Collider.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Points_Helper.generated.h"

UCLASS()
class CYBERBOWL_API UPoints_Helper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	//UFUNCTION(BlueprintCallable, Category="Ball")
	//static AActor* GetBall() { return ball; };
	//UFUNCTION(BlueprintCallable, Category="Ball")
	//static void SetBall(AActor* given_ball);
	//static AActor* ball;
};