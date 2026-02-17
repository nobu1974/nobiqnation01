
// NobiqNation2025/Source/NobiqNation2025/Terrain/TerrainPatch.cpp
#include "TerrainPatch.h"

#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATerrainPatch::ATerrainPatch()
{
    PrimaryActorTick.bCanEverTick = false;

    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    SetRootComponent(ProcMesh);

    // 当たり判定を有効化（クリックレイが当たるように）
    ProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ProcMesh->SetCollisionObjectType(ECC_WorldStatic);
    ProcMesh->SetCollisionResponseToAllChannels(ECR_Block);
    ProcMesh->bUseAsyncCooking = true;
}

void ATerrainPatch::BeginPlay()
{
    Super::BeginPlay();

    InitGrid();
    RebuildMesh();
}

void ATerrainPatch::InitGrid()
{
    // 分割数（最低1）
    NumX = FMath::Max(1, FMath::RoundToInt(PatchSizeCm / GridStepCm));
    NumY = FMath::Max(1, FMath::RoundToInt(PatchSizeCm / GridStepCm));

    Heights.SetNumZeroed((NumX + 1) * (NumY + 1));
}

void ATerrainPatch::RaiseAtWorldPoint(const FVector& WorldPoint, float RadiusCm, float AmountCm)
{
    if (Heights.Num() == 0)
    {
        InitGrid();
    }

    // TerrainPatch のローカル座標へ
    const FTransform& T = GetActorTransform();
    const FVector Local = T.InverseTransformPosition(WorldPoint);

    const float Half = PatchSizeCm * 0.5f;

    // グリッド上の中心インデックス（0..Num）
    const int32 Cx = FMath::Clamp(FMath::RoundToInt((Local.X + Half) / GridStepCm), 0, NumX);
    const int32 Cy = FMath::Clamp(FMath::RoundToInt((Local.Y + Half) / GridStepCm), 0, NumY);

    const int32 R = FMath::Max(1, FMath::CeilToInt(RadiusCm / GridStepCm));
    const float Radius2 = RadiusCm * RadiusCm;

    for (int32 y = Cy - R; y <= Cy + R; y++)
    {
        if (y < 0 || y > NumY)
            continue;

        for (int32 x = Cx - R; x <= Cx + R; x++)
        {
            if (x < 0 || x > NumX)
                continue;

            const float Px = (float)x * GridStepCm - Half;
            const float Py = (float)y * GridStepCm - Half;

            const float Dx = Px - Local.X;
            const float Dy = Py - Local.Y;
            const float D2 = Dx * Dx + Dy * Dy;

            if (D2 > Radius2)
                continue;

            // なめらかに盛る（中心=1, 外側=0）
            const float Dist = FMath::Sqrt(D2);
            const float W = 1.0f - (Dist / RadiusCm);
            Heights[Idx(x, y)] += AmountCm * (W * W);
        }
    }

    RebuildMesh();
}

void ATerrainPatch::RebuildMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    const float Half = PatchSizeCm * 0.5f;

    const int32 VCount = (NumX + 1) * (NumY + 1);
    Vertices.Reserve(VCount);
    Normals.SetNum(VCount);
    UV0.Reserve(VCount);
    VertexColors.Reserve(VCount);
    Tangents.SetNum(VCount);

    // 頂点
    for (int32 y = 0; y <= NumY; y++)
    {
        for (int32 x = 0; x <= NumX; x++)
        {
            const float Px = (float)x * GridStepCm - Half;
            const float Py = (float)y * GridStepCm - Half;
            const float Pz = BaseZ + Heights[Idx(x, y)];

            Vertices.Add(FVector(Px, Py, Pz));
            UV0.Add(FVector2D((float)x / (float)NumX, (float)y / (float)NumY));
            VertexColors.Add(FLinearColor::White);
        }
    }

    // 三角形（セルごとに2枚）
    Triangles.Reserve(NumX * NumY * 6);
    for (int32 y = 0; y < NumY; y++)
    {
        for (int32 x = 0; x < NumX; x++)
        {
            const int32 I0 = Idx(x, y);
            const int32 I1 = Idx(x + 1, y);
            const int32 I2 = Idx(x, y + 1);
            const int32 I3 = Idx(x + 1, y + 1);

            // 反時計回り
            Triangles.Add(I0);
            Triangles.Add(I3);
            Triangles.Add(I1);
            Triangles.Add(I0);
            Triangles.Add(I2);
            Triangles.Add(I3);
        }
    }

    // 簡易法線（周辺差分）
    for (int32 y = 0; y <= NumY; y++)
    {
        for (int32 x = 0; x <= NumX; x++)
        {
            const float HL = Heights[Idx(FMath::Max(0, x - 1), y)];
            const float HR = Heights[Idx(FMath::Min(NumX, x + 1), y)];
            const float HD = Heights[Idx(x, FMath::Max(0, y - 1))];
            const float HU = Heights[Idx(x, FMath::Min(NumY, y + 1))];

            // X/Y の傾きから法線
            const FVector N = FVector(-(HR - HL), -(HU - HD), 2.0f * GridStepCm).GetSafeNormal();
            Normals[Idx(x, y)] = N;

            Tangents[Idx(x, y)] = FProcMeshTangent(FVector(1, 0, 0), false);
        }
    }

    ProcMesh->CreateMeshSection_LinearColor(
        0,
        Vertices,
        Triangles,
        Normals,
        UV0,
        VertexColors,
        Tangents,
        true // collision
    );

    if (TerrainMaterial)
    {
        ProcMesh->SetMaterial(0, TerrainMaterial);
    }
}
