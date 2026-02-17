// NobiqNation2025/Source/NobiqNation2025/Terrain/TerrainPatch.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainPatch.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class NOBIQNATION01_API
    ATerrainPatch : public AActor
{
    GENERATED_BODY()

public:
    ATerrainPatch();

    virtual void BeginPlay() override;

    // クリック地点を中心に盛る
    UFUNCTION(BlueprintCallable, Category = "Terrain")
    void RaiseAtWorldPoint(const FVector& WorldPoint, float RadiusCm = 200.f, float AmountCm = 50.f);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Terrain")
    UProceduralMeshComponent* ProcMesh;

    // Details から指定できる（ここに好きな Material を入れる）
    UPROPERTY(EditAnywhere, Category = "Terrain")
    UMaterialInterface* TerrainMaterial = nullptr;

    // 地形のサイズと密度（Detailsから変更OK）
    UPROPERTY(EditAnywhere, Category = "Terrain")
    float PatchSizeCm = 3000.f;

    UPROPERTY(EditAnywhere, Category = "Terrain")
    float GridStepCm = 50.f;

    UPROPERTY(EditAnywhere, Category = "Terrain")
    float BaseZ = 0.f;

private:
    int32 NumX = 0;
    int32 NumY = 0;

    TArray<float> Heights; // (NumX+1)*(NumY+1)

    void InitGrid();
    void RebuildMesh();

    FORCEINLINE int32 Idx(int32 X, int32 Y) const
    {
        return Y * (NumX + 1) + X;
    }
};
