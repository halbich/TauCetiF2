

#pragma once


/**
 *
 */
class TAUCETIF2_API FMinMaxBox
{

public:
	FMinMaxBox(FVector min, FVector max);
	~FMinMaxBox();

	FVector Min;
	FVector Max;

	FMinMaxBox* B1;

	FMinMaxBox* B2;



	void DEBUGDrawContainingBox(UWorld* world);

};
