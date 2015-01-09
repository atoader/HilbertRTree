#include "gtest.h"
#include "../src/RTree.hh"

TEST(RTreeTest, Constructor)
{
    RTree* tree;

    ASSERT_NO_THROW(tree = new RTree());

    delete tree;
}
