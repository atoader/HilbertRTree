#include <boost/shared_ptr.hpp>

#include "gtest.h"
#include "../src/Node.hh"
#include "../src/Constants.hh"
#include "../src/Rectangle.hh"
#include "../src/HilbertValue.hh"
#include "../src/LeafEntry.hh"
#include "../src/NonLeafEntry.hh"

TEST(NodeTest, Constructor)
{
    Node n(2,4);

    //Asserting defaults
    ASSERT_FALSE(n.isLeaf());
    ASSERT_EQ(NULL, n.getNextSibling());
    ASSERT_EQ(NULL, n.getPrevSibling());
    ASSERT_EQ(NULL, n.getParent());
}

TEST(NodeTest, insertLeafEntry)
{
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));

    boost::shared_ptr<LeafEntry> leafEntry( new LeafEntry(rect, h));

    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect->getCenter()));

    boost::shared_ptr<LeafEntry> leafEntry1( new LeafEntry(rect1, h1));
    Node leafNode(1,2);
    //The node becomes a leaf node
    leafNode.setLeaf(true);

    Node nonLeafNode(1,2);
    //This will throw an exception because the node is not a leaf node
    ASSERT_ANY_THROW(nonLeafNode.insertLeafEntry(leafEntry));

    //This will succeed
    ASSERT_NO_THROW(leafNode.insertLeafEntry(leafEntry));

    ASSERT_EQ(1, leafNode.getEntries().size());

    ASSERT_NO_THROW(leafNode.insertLeafEntry(leafEntry1));
    //The node will overflow
    ASSERT_TRUE(leafNode.isOverflowing());
    ASSERT_ANY_THROW(leafNode.insertLeafEntry(leafEntry));
}

TEST(NodeTest, insertNonLeafEntry)
{
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));


    Node* firstNode = new Node(2,4);
    Node* secondNode = new Node(2,4);
    Node* nonLeafNode = new Node(2,4);
    Node leafNode(2,4);

    boost::shared_ptr<NonLeafEntry>  nonLeafEntry( new NonLeafEntry(firstNode));
    boost::shared_ptr<NonLeafEntry>  secondNonLeafEntry( new NonLeafEntry(secondNode));

    //The node becomes a leaf node
    leafNode.setLeaf(true);

    //Inserting a non leaf entry into a leaf node will fail
    ASSERT_ANY_THROW(leafNode.insertNonLeafEntry(nonLeafEntry));

    ASSERT_NO_THROW(nonLeafNode->insertNonLeafEntry(secondNonLeafEntry));

    //One entry will be inserted
    ASSERT_EQ(1, nonLeafNode->getEntries().size());
    //The parent of the node in that entry is the current node
    ASSERT_EQ(nonLeafNode, boost::dynamic_pointer_cast<NonLeafEntry>(secondNonLeafEntry)->getNode()->getParent());

    delete nonLeafNode;
}

////Test that the siblings are set for the newly
////inserted node
TEST(NodeTest, insertNonLeafEntrySiblings_1)
{
    Node* childNode = new Node(2,4);
    childNode->setLeaf(true);
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry( new LeafEntry(rect, h));
    childNode->insertLeafEntry(leafEntry);

    boost::shared_ptr<NonLeafEntry>  nonLeafEntry( new NonLeafEntry(childNode));
    //No siblings
    Node* parent = new Node(2,4);
    parent->insertNonLeafEntry(nonLeafEntry);

    ASSERT_EQ(parent, childNode->getParent());
    ASSERT_EQ(parent, nonLeafEntry->getNode()->getParent());
    ASSERT_EQ(NULL, childNode->getNextSibling());
    ASSERT_EQ(NULL, childNode->getPrevSibling());

    delete parent;
}


TEST(NodeTest, isOverflowing)
{
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));

    boost::shared_ptr<LeafEntry> leafEntry( new LeafEntry(rect,h));
    boost::shared_ptr<LeafEntry> leafEntry1( new LeafEntry(rect, h));

    Node n(1,2);
    n.setLeaf(true);

    ASSERT_FALSE(n.isOverflowing());

    n.insertLeafEntry(leafEntry);
    n.insertLeafEntry(leafEntry1);

    ASSERT_TRUE(n.isOverflowing());
}

TEST(NodeTest, isUnderflowing)
{
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));

    boost::shared_ptr<LeafEntry> leafEntry( new LeafEntry(rect,h));

    Node n(1,2);
    n.setLeaf(true);

    ASSERT_TRUE(n.isUnderflowing());

    n.insertLeafEntry(leafEntry);
    n.insertLeafEntry(leafEntry);

    ASSERT_FALSE(n.isUnderflowing());
}

TEST(NodeTest, adjustMBR)
{
    std::vector<boost::uint64_t> lower1(2, 0);
    std::vector<boost::uint64_t> upper1(2, 4);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry1(new LeafEntry(rect1, h1));

    std::vector<boost::uint64_t> lower2(2, 1);
    std::vector<boost::uint64_t> upper2(2, 5);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry2(new LeafEntry(rect2, h2));

    std::vector<boost::uint64_t> lower3(2, 5);
    std::vector<boost::uint64_t> upper3(2, 10);
    boost::shared_ptr<Rectangle> rect3(new Rectangle(lower3, upper3));
    boost::shared_ptr<HilbertValue> h3(new HilbertValue(rect3->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry3(new LeafEntry(rect3, h3));

    Node n(2,4);
    n.setLeaf(true);
    n.insertLeafEntry(leafEntry1);
    n.insertLeafEntry(leafEntry2);

    ASSERT_NO_FATAL_FAILURE(n.adjustMBR());

    boost::shared_ptr<Rectangle> r = n.getMBR();

    ASSERT_EQ(0,r->getLower()[0]);
    ASSERT_EQ(0,r->getLower()[1]);

    ASSERT_EQ(5,r->getUpper()[0]);
    ASSERT_EQ(5,r->getUpper()[1]);

    Node n1(2,4);
    n1.setLeaf(true);
    n1.insertLeafEntry(leafEntry1);
    n1.insertLeafEntry(leafEntry3);

    ASSERT_NO_FATAL_FAILURE(n1.adjustMBR());

    boost::shared_ptr<Rectangle> r1 = n1.getMBR();

    ASSERT_EQ(0,r1->getLower()[0]);
    ASSERT_EQ(0,r1->getLower()[1]);

    ASSERT_EQ(10,r1->getUpper()[0]);
    ASSERT_EQ(10,r1->getUpper()[1]);
}

TEST(NodeTest, adjustMBR_1)
{
    std::vector<boost::uint64_t> lower1(2, 2);
    std::vector<boost::uint64_t> upper1(2, 3);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry1(new LeafEntry(rect1, h1));

    std::vector<boost::uint64_t> lower2(2, 8);
    std::vector<boost::uint64_t> upper2(2, 8);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry2(new LeafEntry(rect2, h2));

    Node n(2,4);
    n.setLeaf(true);
    n.insertLeafEntry(leafEntry1);
    n.insertLeafEntry(leafEntry2);

    ASSERT_NO_FATAL_FAILURE(n.adjustMBR());

    boost::shared_ptr<Rectangle> r = n.getMBR();

    ASSERT_EQ(2,r->getLower()[0]);
    ASSERT_EQ(2,r->getLower()[1]);

    ASSERT_EQ(8,r->getUpper()[0]);
    ASSERT_EQ(8,r->getUpper()[1]);
}

TEST(NodeTest, adjustLHV)
{
    std::vector<boost::uint64_t> lower1(2, 0);
    std::vector<boost::uint64_t> upper1(2, 0);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry1(new LeafEntry(rect1, h1));

    std::vector<boost::uint64_t> lower2(2, 0);
    std::vector<boost::uint64_t> upper2(2, 2);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));
    boost::shared_ptr<LeafEntry> leafEntry2(new LeafEntry(rect2, h2));

    Node n(2,4);
    n.setLeaf(true);
    n.insertLeafEntry(leafEntry1);
    n.insertLeafEntry(leafEntry2);

    ASSERT_NO_FATAL_FAILURE(n.adjustLHV());

    ASSERT_TRUE(*h1 < *h2);
    ASSERT_TRUE(*h2 == *(n.getLHV()));
}

TEST(NodeTest, getSiblings)
{
    Node* right =new Node(2,4);
    right->setLeaf(true);

    Node* left =new Node(2,4);
    left->setLeaf(true);

    Node* main =new Node(2,4);
    main->setLeaf(true);

    ASSERT_EQ(1, main->getSiblings(2).size());

    main->setNextSibling(right);
    right->setPrevSibling(main);

    ASSERT_EQ(2, main->getSiblings(2).size());

    main->setPrevSibling(left);
    left->setNextSibling(main);

    ASSERT_EQ(2, main->getSiblings(2).size());

    ASSERT_EQ(1, main->getSiblings(1).size());

    std::list<Node *> siblings = main->getSiblings(3);
    std::list<Node *>::iterator it = siblings.begin();


    ASSERT_TRUE(*it == main);
    ++it;
    ASSERT_TRUE(*it == right);

    delete left;
    delete right;
    delete main;
}
