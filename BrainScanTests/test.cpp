#include "pch.h"
#include "scan/Scan.hpp"
#include "Core.hpp"

inline int GenerateDimensionCount(View& view)
{
	return (view.GetDepth() * view.GetWidth() * view.GetHeight());
}

TEST(ScanDimensionsTest, TestScanDimensionsAxial)
{
    Scan scan;
    scan.CreateMockData();
	int atLeastThreeDims = GenerateDimensionCount(*scan.GetAxial());

    EXPECT_GT(atLeastThreeDims, 0);
}

TEST(ScanDimensionsTest, TestScanDimensionsSagittal)
{
	Scan scan;
	scan.CreateMockData();
	int atLeastThreeDims = GenerateDimensionCount(*scan.GetSagittal());

	EXPECT_GT(atLeastThreeDims, 0);
}

TEST(ScanDimensionsTest, TestScanDimensionsCoronal)
{
	Scan scan;
	scan.CreateMockData();
	int atLeastThreeDims = GenerateDimensionCount(*scan.GetCoronal());

	EXPECT_GT(atLeastThreeDims, 0);
}

::testing::AssertionResult AreAllValuesBetweenInclusive(View* view, int from, int to)
{
	std::vector<float*> data = view->GetData();
	for (int i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < view->GetDepth(); j++)
		{
			if(!(data[j][i] >= from) && (data[j][i] <= to))
				return ::testing::AssertionFailure() << data[j][i] << " is outside the range " << from << " to " << to << " (Layer: "<<j<<" Buffer index: "<<i;
		}
	}
	return ::testing::AssertionSuccess();
}

TEST(PixelValueTest, CheckPixelValuesRangeFrom0To1Axial)
{
	Scan scan;
	scan.CreateMockData();
	View* axial = scan.GetAxial();		

	EXPECT_TRUE(AreAllValuesBetweenInclusive(axial, 0, 1));
}

TEST(PixelValueTest, CheckPixelValuesRangeFrom0To1Sagittal)
{
	Scan scan;
	scan.CreateMockData();
	View* axial = scan.GetSagittal();

	EXPECT_TRUE(AreAllValuesBetweenInclusive(axial, 0, 1));
}

TEST(PixelValueTest, CheckPixelValuesRangeFrom0To1Coronal)
{
	Scan scan;
	scan.CreateMockData();
	View* axial = scan.GetCoronal();

	EXPECT_TRUE(AreAllValuesBetweenInclusive(axial, 0, 1));
}