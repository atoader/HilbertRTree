#include "gtest.h"
#include "../src/RTree.hh"
#include "../src/RTreeHelper.hh"
#include "../src/Constants.hh"

TEST(RTreeTest, Constructor)
{
    RTree* tree;

    ASSERT_NO_THROW(tree = new RTree());

    delete tree;
}

TEST(RTreeTest, insert)
{
    RTree tree;
    int nodeNo = 13;

    for(int i=0; i<nodeNo; i++)
    {
        std::vector<boost::uint64_t> lower(2, i);
        std::vector<boost::uint64_t> upper(2, i);
        boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
        if(i==nodeNo-1)
        {
            int a=2;
        }
        tree.insert(rect);

    }

    std::vector<boost::uint64_t> lower(2, 0);
    std::vector<boost::uint64_t> upper(2, nodeNo);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));

    std::list<boost::shared_ptr<NodeEntry> >result= tree.search(rect);
    ASSERT_EQ(nodeNo, result.size());
}

TEST(RTreeTest, remove_1)
{
    //Try to remove a rectangle that does not exist in the tree
    RTree tree;

    std::vector<boost::uint64_t> lower(2, 0);
    std::vector<boost::uint64_t> upper(2, 2);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));

    tree.remove(rect);
    ASSERT_TRUE(tree.getRoot()->isLeaf());
    ASSERT_EQ(0, tree.getRoot()->getEntries().size());
}

TEST(RTreeTest, remove_2)
{
    //Try to remove a rectangle that overlaps an existing entry
    RTree tree;

    std::vector<boost::uint64_t> lower(2, 4);
    std::vector<boost::uint64_t> upper(2, 8);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));

    tree.insert(rect);
    ASSERT_TRUE(tree.getRoot()->isLeaf());
    ASSERT_EQ(1, tree.getRoot()->getEntries().size());
    ASSERT_EQ(1, tree.search(rect).size());

    //This rectangle is inside the inserted rectangle
    std::vector<boost::uint64_t> lower1(2, 5);
    std::vector<boost::uint64_t> upper1(2, 7);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    tree.remove(rect1);

    ASSERT_TRUE(tree.getRoot()->isLeaf());
    ASSERT_EQ(1, tree.getRoot()->getEntries().size());
    ASSERT_EQ(1, tree.search(rect).size());

    //This rectangle is contains the inserted rectangle
    std::vector<boost::uint64_t> lower2(2, 2);
    std::vector<boost::uint64_t> upper2(2, 10);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    tree.remove(rect2);

    ASSERT_TRUE(tree.getRoot()->isLeaf());
    ASSERT_EQ(1, tree.getRoot()->getEntries().size());
    ASSERT_EQ(1, tree.search(rect).size());

    tree.remove(rect);

    ASSERT_TRUE(tree.getRoot()->isLeaf());
    ASSERT_EQ(0, tree.getRoot()->getEntries().size());
    ASSERT_EQ(0, tree.search(rect).size());
}

TEST(RTreeTest, remove_3)
{
    RTree tree;

    for(int i=0; i<MAX_NODE_ENTRIES+1; i++)
    {
        std::vector<boost::uint64_t> lower(2, i);
        std::vector<boost::uint64_t> upper(2, i);
        boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
        tree.insert(rect);
    }

    for(int i=0; i<MAX_NODE_ENTRIES - MIN_NODE_ENTRIES; i++)
    {
        std::vector<boost::uint64_t> lower(2, i);
        std::vector<boost::uint64_t> upper(2, i);
        boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
        tree.remove(rect);
    }

    ASSERT_TRUE(tree.getRoot()->isLeaf());
    ASSERT_EQ(MIN_NODE_ENTRIES+1, tree.getRoot()->getEntries().size());
}

TEST(RTreeTest, remove_4)
{
    RTree tree;
    //4
    int nodeNo = MAX_NODE_ENTRIES*4;

    for(int i=0; i<nodeNo; i++)
    {
        std::vector<boost::uint64_t> lower(2, i);
        std::vector<boost::uint64_t> upper(2, i);
        boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
        tree.insert(rect);
    }

    for(int i=0; i<nodeNo; i++)
    {
        std::vector<boost::uint64_t> lower(2, i);
        std::vector<boost::uint64_t> upper(2, i);
        boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
        tree.remove(rect);
    }

    ASSERT_TRUE(tree.getRoot()->isLeaf());
    ASSERT_EQ(0, tree.getRoot()->getEntries().size());
}
