#include "pch.h"
#include "scan/Scan.hpp"
#include "Core.hpp"

inline int GenerateDimensionCount(View& view)
{
	return (view.GetDepth() * view.GetWidth() * view.GetHeight());
}

TEST(ScanDimensions, TestScanDimensionsAxial)
{
    Scan scan;
    scan.CreateMockData();
	int atLeastThreeDims = GenerateDimensionCount(*scan.GetAxial());

    EXPECT_GT(atLeastThreeDims, 0);
}

TEST(ScanDimensions, TestScanDimensionsSagittal)
{
	Scan scan;
	scan.CreateMockData();
	int atLeastThreeDims = GenerateDimensionCount(*scan.GetSagittal());

	EXPECT_GT(atLeastThreeDims, 0);
}

TEST(ScanDimensions, TestScanDimensionsCoronal)
{
	Scan scan;
	scan.CreateMockData();
	int atLeastThreeDims = GenerateDimensionCount(*scan.GetCoronal());

	EXPECT_GT(atLeastThreeDims, 0);
}