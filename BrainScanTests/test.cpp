#include "pch.h"

int xd(int a, int b)
{
	return a / b;
}

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestLoadsIndexBuffer, LoadIndexBuffer)
{
	int res = xd(6, 3);

	EXPECT_EQ(res, 3) << "dupa";
}