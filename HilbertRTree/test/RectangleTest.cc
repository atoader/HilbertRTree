#include "gtest.h"
#include "../src/Rectangle.hh"

TEST(RectangleTest, ConstructorFail)
{
    Rectangle *rect;
    std::vector<boost::uint64_t> lower(2, 3);
    std::vector<boost::uint64_t> upper(3, 4);

    ASSERT_ANY_THROW(rect = new Rectangle(lower, upper));
}

TEST(RectangleTest, ConstructorSuccess)
{
    Rectangle *rect;
    std::vector<boost::uint64_t> lower(2, 3);
    std::vector<boost::uint64_t> upper(2, 4);

    ASSERT_NO_THROW(rect = new Rectangle(lower, upper));

    ASSERT_EQ(2,rect->getLower().size());
    ASSERT_EQ(2,rect->getUpper().size());

    delete rect;
}

TEST(RectangleTest, getCenter)
{
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);

    Rectangle rect(lower, upper);

    std::vector<boost::uint64_t> center = rect.getCenter();

    ASSERT_EQ(center[0], (lower[0] + upper[0]) / 2);
    ASSERT_EQ(center[1], (lower[1] + upper[1]) / 2);
}

TEST(RectangleTest, intersect)
{
    //TODO:Make a list of all the possible cases and test
    //each case.
    std::vector<boost::uint64_t> lower1;
    lower1.push_back(5);
    lower1.push_back(0);

    std::vector<boost::uint64_t> upper1;
    upper1.push_back(10);
    upper1.push_back(5);

    Rectangle main(lower1,upper1);

    std::vector<boost::uint64_t> lower2;
    lower2.push_back(7);
    lower2.push_back(2);

    std::vector<boost::uint64_t> upper2;
    upper2.push_back(9);
    upper2.push_back(4);

    Rectangle insideMain(lower2,upper2);

    ASSERT_TRUE(main.intersects(insideMain));
    ASSERT_TRUE(insideMain.intersects(main));

    std::vector<boost::uint64_t> lower3;
    lower3.push_back(11);
    lower3.push_back(2);

    std::vector<boost::uint64_t> upper3;
    upper3.push_back(14);
    upper3.push_back(4);

    Rectangle leftOfMain(lower3,upper3 );

    ASSERT_FALSE(main.intersects(leftOfMain));
    ASSERT_FALSE(leftOfMain.intersects(main));

    std::vector<boost::uint64_t> lower4;
    lower4.push_back(7);
    lower4.push_back(2);

    std::vector<boost::uint64_t> upper4;
    upper4.push_back(7);
    upper4.push_back(2);

    Rectangle pointRectangle(lower4,upper4);

    ASSERT_TRUE(main.intersects(pointRectangle));
    ASSERT_TRUE(pointRectangle.intersects(main));
}

TEST(RectangleTest, contains)
{
    std::vector<boost::uint64_t> lower1(2,0);
    std::vector<boost::uint64_t> upper1(2,2);
    Rectangle main(lower1,upper1);

    std::vector<boost::uint64_t> lower2(2,3);
    std::vector<boost::uint64_t> upper2(2,5);
    Rectangle outsideMain(lower2,upper2);

    ASSERT_FALSE(main.contains(outsideMain));
    ASSERT_FALSE(outsideMain.intersects(main));

    std::vector<boost::uint64_t> lower3;
    lower3.push_back(0);
    lower3.push_back(3);

    std::vector<boost::uint64_t> upper3;
    upper3.push_back(2);
    upper3.push_back(5);

    Rectangle outsideMain2(lower3,upper3);

    ASSERT_FALSE(main.contains(outsideMain2));
    ASSERT_FALSE(outsideMain2.intersects(main));

    ASSERT_FALSE(outsideMain.contains(outsideMain2));
    ASSERT_FALSE(outsideMain2.intersects(outsideMain));

    std::vector<boost::uint64_t> lower4;
    lower4.push_back(0);
    lower4.push_back(0);

    std::vector<boost::uint64_t> upper4;
    upper4.push_back(1);
    upper4.push_back(1);

    Rectangle insideMain(lower4,upper4);

    ASSERT_TRUE(main.contains(insideMain));
    ASSERT_FALSE(insideMain.contains(main));
}
