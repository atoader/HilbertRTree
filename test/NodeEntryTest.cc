#include "gtest.h"
#include "../src/NodeEntry.hh"

TEST(NodeEntryTest, ConstructorFail) {
  boost::shared_ptr<HilbertValue> lhv;
  boost::shared_ptr<Rectangle> mbr;
  NodeEntry *entry;

  ASSERT_ANY_THROW(entry = new NodeEntry(lhv, mbr, NULL, NULL));
}

TEST(NodeEntryTest, ConstructorSuccess) {
  std::vector<boost::uint64_t> lower(2, 2);
  std::vector<boost::uint64_t> upper(2, 4);

  boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
  boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));

  NodeEntry *entry;

  ASSERT_NO_THROW(entry = new NodeEntry(h, rect, NULL, NULL));

  delete entry;
}
