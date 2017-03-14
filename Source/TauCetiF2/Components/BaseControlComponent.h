#pragma once

#include "Components/ActorComponent.h"
#include "Widgets/ObjectWidget.h"
#include "BaseControlComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UBaseControlComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UObjectWidget> BaseControl;

	UPROPERTY(BlueprintReadOnly, Transient, Category = BaseControlComponent)
		UObjectWidget* BaseControlWidget;
};
