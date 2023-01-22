#include "pch.h"
#include "scan/Scan.hpp"
#include "Core.hpp"

TEST(TestScanDimensions, ScanMockDataTests)
{
    Scan scan;
    scan.CreateMockData();
    int atLeastThreeDims = scan.GetAxial()->GetDepth() * scan.GetAxial()->GetWidth() * scan.GetAxial()->GetHeight();

    EXPECT_GT(atLeastThreeDims, 0);
}