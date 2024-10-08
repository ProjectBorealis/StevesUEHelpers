// 

#pragma once

#include "CoreMinimal.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "StevesDynamicNavMeshVolume.generated.h"

/// NavMesh bounds that can be changed at runtime (requires that your NavMesh data Runtime Generation is set to Dynamic)
UCLASS()
class STEVESUEHELPERS_API AStevesDynamicNavMeshVolume : public ANavMeshBoundsVolume
{
	GENERATED_BODY()

public:
	AStevesDynamicNavMeshVolume();

	UFUNCTION(BlueprintCallable, Category=AI)
	void SetLocationAndDimensions(const FVector& Location, const FVector& NewDimensions);
	UFUNCTION(BlueprintCallable, Category=AI)
	void SetDimensions(const FVector& NewDimensions);

protected:
	void UpdateDimensions(const FVector& NewDimensions);
	void NotifyNavSystem();
};
