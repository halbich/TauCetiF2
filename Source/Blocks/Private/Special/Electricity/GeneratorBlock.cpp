#include "Blocks.h"
#include "GeneratorBlock.h"

#define RED 2
#define GREEN 1
#define BLUE 0
#define ALPHA 3

AGeneratorBlock::AGeneratorBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));
	GeneratorMesh->SetupAttachment(GetRootComponent());

	dynamicColors = nullptr;
	dynamicColorsFloat = nullptr;
	updateTextureRegion = nullptr;
}

void AGeneratorBlock::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}

void AGeneratorBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	delete[] dynamicColors; dynamicColors = nullptr;
	delete[] dynamicColorsFloat; dynamicColorsFloat = nullptr;
	delete updateTextureRegion; updateTextureRegion = nullptr;
	Super::EndPlay(EndPlayReason);
}

void  AGeneratorBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if (dynamicColors) { delete[] dynamicColors; dynamicColors = nullptr; }
	if (dynamicColorsFloat) { delete[] dynamicColorsFloat; dynamicColorsFloat = nullptr; }
	if (updateTextureRegion) { delete updateTextureRegion; updateTextureRegion = nullptr; }


	pixelsPerBaseBlock = 64;

	auto currentScale = GetBlockScale();
	int32 w = FMath::RoundToInt(currentScale.X) * pixelsPerBaseBlock, h = FMath::RoundToInt(currentScale.Y) * pixelsPerBaseBlock;

	dynamicMaterials.Empty();

	auto meshComp = Cast<IGenericBlock>(this)->Execute_GetMeshStructureComponent(this, 0);

	if (!meshComp)
		return;

	dynamicMaterials.Add(meshComp->CreateAndSetMaterialInstanceDynamic(0));
	DynamicTexture = UTexture2D::CreateTransient(w, h);
	DynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	DynamicTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	DynamicTexture->SRGB = false;
	DynamicTexture->AddToRoot();
	DynamicTexture->UpdateResource();

	updateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);

	dynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", DynamicTexture);

	dataSize = w * h * 4;
	dataSqrtSize = w * 4;
	arraySize = w * h;
	rowSize = w;
	dynamicColors = new uint8[dataSize];

	for (uint32 i = 0; i < dataSize; ++i)
		dynamicColors[i] = 0;

}

#pragma optimize("", off)

void AGeneratorBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int32 sec;
	float partSec;
	UGameplayStatics::GetAccurateRealTime(GetWorld(), sec, partSec);

	TArray<int32> toDel;

	for (int32 i = 0; i < spots.Num(); ++i)
	{
		auto elem = &spots[i];

		elem->ActualTime += DeltaTime;

		uint8 setValue = (uint8)FMath::Lerp(256, 0, FMath::Min(1.0f, elem->ActualTime));
		for (int32 kx = 0; kx < pixelsPerBaseBlock; kx++)
		{

			for (int32 ky = 0; ky < pixelsPerBaseBlock; ky++)
			{
				uint32 baseIndex = ((elem->X * pixelsPerBaseBlock + kx) + (elem->Y * pixelsPerBaseBlock + ky) * rowSize) * 4;

				ensure(baseIndex + RED >= 0 && baseIndex + RED < dataSize);
				ensure(baseIndex + GREEN >= 0 && baseIndex + GREEN < dataSize);
				ensure(baseIndex + BLUE >= 0 && baseIndex + BLUE < dataSize);

				dynamicColors[baseIndex + RED] = dynamicColors[baseIndex + GREEN] = dynamicColors[baseIndex + BLUE] = setValue;
			}

		}



		if (setValue == 0)
			toDel.Push(i);
	}

	while (toDel.Num() > 0)
		spots.RemoveAt(toDel.Pop());

	UpdateCustomTexture();
}

void AGeneratorBlock::UpdateCustomTexture()
{
	UpdateTextureRegions(DynamicTexture, 0, 1, updateTextureRegion, dataSqrtSize, (uint32)4, dynamicColors, false);
	dynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", DynamicTexture);
}

UStaticMeshComponent* AGeneratorBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return GeneratorMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}



void  AGeneratorBlock::WasHitByStorm(const FVector& blockLocation)
{
	print(*blockLocation.ToString());

	FHittedSpot hitted;

	auto currentScale = GetBlockScale();

	hitted.X = (uint8)((FMath::RoundToInt(blockLocation.X + currentScale.X) % FMath::RoundToInt(currentScale.X)));
	ensure(hitted.X >= 0 && hitted.X < currentScale.X);

	hitted.Y = (uint8)((FMath::RoundToInt(blockLocation.Y + currentScale.Y) % FMath::RoundToInt(currentScale.Y)));
	ensure(hitted.Y >= 0 && hitted.Y < currentScale.Y);

	hitted.ActualTime = 0;

	//hitted.X = hitted.Y = 0;

	print(*FVector2D(hitted.X, hitted.Y).ToString());

	auto existing = spots.IndexOfByPredicate([hitted](const FHittedSpot& spot) {
		return hitted.X == spot.X && hitted.Y == spot.Y;
	});

	if (existing != INDEX_NONE)
		spots[existing].ActualTime = 0;
	else
		spots.Insert(hitted, 0);
}


#pragma optimize("", on)