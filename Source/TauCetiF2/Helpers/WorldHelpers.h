#pragma once

/**
 *
 */
namespace WorldHelpers
{
	static FORCEINLINE FVector BorderToWorld(const FVector& border)
	{
		return border - GameDefinitions::WorldCenterMapping;
	}
};
