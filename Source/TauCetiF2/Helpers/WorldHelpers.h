#pragma once

/**
 *
 */
struct TAUCETIF2_API WorldHelpers
{
	static FORCEINLINE FVector BorderToWorld(const FVector& border)
	{
		return border - GameDefinitions::WorldCenterMapping;
	}
};
