#include "gtest.h"
#include "../src/HilbertValue.hh"

TEST(HilbertValueTest, ConstructorFail) {
  std::vector<boost::uint64_t> coordinates(1, 1);
  std::vector<boost::int32_t> ms(2, 1);
  HilbertValue *h;

  ASSERT_ANY_THROW(h = new HilbertValue(coordinates, ms));
}

TEST(HilbertValueTest, ConstructorSuccess) {
  std::vector<boost::uint64_t> coordinates(2, 1);
  std::vector<boost::int32_t> ms(2, 1);
  HilbertValue *h;

  ASSERT_NO_THROW(h = new HilbertValue(coordinates, ms));

  delete h;
}

TEST(HilbertValueTest, Equality) {
  std::vector<boost::uint64_t> coordinates(2, 1);
  std::vector<boost::int32_t> ms(2, 2);
  HilbertValue h1(coordinates, ms);

  std::vector<boost::uint64_t> coordinates2(2, 1);
  std::vector<boost::int32_t> ms2(2, 2);
  HilbertValue h2(coordinates2, ms2);

  std::vector<boost::uint64_t> coordinates3(3, 1);
  std::vector<boost::int32_t> ms3(3, 2);
  HilbertValue h3(coordinates3, ms3);

  ASSERT_TRUE(h1 == h2);
  ASSERT_FALSE(h1 == h3);

  std::vector<boost::uint64_t> coordinates4(3, 2);
  std::vector<boost::int32_t> ms4(3, 4);
  HilbertValue h4(coordinates4, ms4);

  std::vector<boost::uint64_t> coordinates5(3, 3);
  std::vector<boost::int32_t> ms5(3, 4);
  HilbertValue h5(coordinates5, ms5);

  ASSERT_FALSE(h4 == h5);
}

TEST(HilbertValueTest, Inequality) {
  std::vector<boost::uint64_t> coordinates(2, 1);
  std::vector<boost::int32_t> ms(2, 2);
  HilbertValue h1(coordinates, ms);

  std::vector<boost::uint64_t> coordinates2(2, 1);
  std::vector<boost::int32_t> ms2(2, 2);
  HilbertValue h2(coordinates2, ms2);

  std::vector<boost::uint64_t> coordinates3(3, 1);
  std::vector<boost::int32_t> ms3(3, 2);
  HilbertValue h3(coordinates3, ms3);

  ASSERT_FALSE(h1 != h2);
  ASSERT_TRUE(h1 != h3);

  std::vector<boost::uint64_t> coordinates4(3, 2);
  std::vector<boost::int32_t> ms4(3, 4);
  HilbertValue h4(coordinates4, ms4);

  std::vector<boost::uint64_t> coordinates5(3, 3);
  std::vector<boost::int32_t> ms5(3, 4);
  HilbertValue h5(coordinates5, ms5);

  ASSERT_TRUE(h4 != h5);
}

TEST(HilbertValueTest, LessThan) {
  std::vector<boost::uint64_t> coordinates(2, 0);
  std::vector<boost::int32_t> ms(2, 8);
  HilbertValue h1(coordinates, ms);

  std::vector<boost::uint64_t> coordinates2(2, 1);
  std::vector<boost::int32_t> ms2(2, 8);
  HilbertValue h2(coordinates2, ms2);

  ASSERT_TRUE(h1 < h2);
  ASSERT_FALSE(h2 < h1);
  ASSERT_FALSE(h1 < h1);
}

TEST(HilbertValueTest, LessOrEqual) {
  std::vector<boost::uint64_t> coordinates(2, 0);
  std::vector<boost::int32_t> ms(2, 8);
  HilbertValue h1(coordinates, ms);

  std::vector<boost::uint64_t> coordinates2(2, 1);
  std::vector<boost::int32_t> ms2(2, 8);
  HilbertValue h2(coordinates2, ms2);

  ASSERT_TRUE(h1 <= h2);
  ASSERT_FALSE(h2 <= h1);
  ASSERT_TRUE(h1 <= h1);
}

TEST(HilbertValueTest, GreaterThan) {
  std::vector<boost::uint64_t> coordinates(2, 0);
  std::vector<boost::int32_t> ms(2, 8);
  HilbertValue h1(coordinates, ms);

  std::vector<boost::uint64_t> coordinates2(2, 1);
  std::vector<boost::int32_t> ms2(2, 8);
  HilbertValue h2(coordinates2, ms2);

  ASSERT_FALSE(h1 > h2);
  ASSERT_TRUE(h2 > h1);
  ASSERT_FALSE(h1 > h1);
}

TEST(HilbertValueTest, GreaterOrEqual) {
  std::vector<boost::uint64_t> coordinates(2, 0);
  std::vector<boost::int32_t> ms(2, 8);
  HilbertValue h1(coordinates, ms);

  std::vector<boost::uint64_t> coordinates2(2, 1);
  std::vector<boost::int32_t> ms2(2, 8);
  HilbertValue h2(coordinates2, ms2);

  ASSERT_FALSE(h1 >= h2);
  ASSERT_TRUE(h2 >= h1);
  ASSERT_TRUE(h1 >= h1);
}
