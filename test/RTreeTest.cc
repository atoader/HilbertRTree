#include "gtest.h"
#include "../src/RTree.hh"

TEST(RTreeTest, Constructor)
{
    RTree* tree;

    ASSERT_NO_THROW(tree = new RTree());

    delete tree;
}

TEST(RTreeTest, insert)
{
//    RTree tree;

//    for(int i=0; i<10; i++)
//    {
//        std::vector<boost::uint64_t> lower(2, i);
//        std::vector<boost::uint64_t> upper(2, i);
//        boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
//        tree.insert(rect, NULL);
//    }

//    std::vector<boost::uint64_t> lower(2, 0);
//    std::vector<boost::uint64_t> upper(2, 10);
//    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));

//    std::list<boost::shared_ptr<NodeEntry> >result= tree.search(rect);
//    ASSERT_EQ(10, result.size());

}
