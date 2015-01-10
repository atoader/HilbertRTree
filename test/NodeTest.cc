#include <boost/shared_ptr.hpp>

#include "gtest.h"
#include "../src/Node.hh"
#include "../src/Constants.hh"
#include "../src/Rectangle.hh"
#include "../src/HilbertValue.hh"

TEST(NodeTest, Constructor)
{
    Node n(MAX_NODE_ENTRIES);

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

    boost::shared_ptr<NodeEntry> leafEntry( new NodeEntry(h, rect, NULL, NULL));
    Node leafNode(2);
    //The node becomes a leaf node
    leafNode.setLeaf(true);

    Node nonLeafNode(2);
    //This will throw an exception because the node is not a leaf node
    ASSERT_ANY_THROW(nonLeafNode.insertLeafEntry(leafEntry));

    Node* nodePtr = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry> nonLeafEntry( new NodeEntry(h, rect, nodePtr, NULL));

    //This will throw an exception because we are inserting a nonLeafEntry
    //into a leaf node
    ASSERT_ANY_THROW(leafNode.insertLeafEntry(nonLeafEntry));

    //This will succeed
    ASSERT_NO_THROW(leafNode.insertLeafEntry(leafEntry));

    ASSERT_EQ(1, leafNode.getEntries().size());

    ASSERT_NO_THROW(leafNode.insertLeafEntry(leafEntry));
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

    Node* firstNode = new Node(MAX_NODE_ENTRIES);
    Node* secondNode = new Node(MAX_NODE_ENTRIES);
    Node* nonLeafNode = new Node(MAX_NODE_ENTRIES);
    Node leafNode(MAX_NODE_ENTRIES);

    boost::shared_ptr<NodeEntry>  leafEntry( new NodeEntry(h, rect, NULL, NULL));
    boost::shared_ptr<NodeEntry>  nonLeafEntry( new NodeEntry(h, rect, firstNode, NULL));
    boost::shared_ptr<NodeEntry>  secondNonLeafEntry( new NodeEntry(h, rect, secondNode, NULL));

    //The node becomes a leaf node
    leafNode.setLeaf(true);

    ASSERT_ANY_THROW(leafNode.insertNonLeafEntry(nonLeafEntry));
    ASSERT_ANY_THROW(nonLeafNode->insertNonLeafEntry(leafEntry));

    ASSERT_NO_THROW(nonLeafNode->insertNonLeafEntry(secondNonLeafEntry));

    ASSERT_EQ(1, nonLeafNode->getEntries().size());
    ASSERT_EQ(nonLeafNode, secondNonLeafEntry->ptr->getParent());

    delete nonLeafNode;
}

//Test that the siblings are set for the newly
//inserted node
TEST(NodeTest, insertNonLeafEntrySiblings_1)
{
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));
    //No siblings
    Node* parent = new Node(MAX_NODE_ENTRIES);
    Node* childNode = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry>  nonLeafEntry( new NodeEntry(h, rect, childNode, NULL));

    parent->insertNonLeafEntry(nonLeafEntry);

    ASSERT_EQ(parent, childNode->getParent());
    ASSERT_EQ(parent, nonLeafEntry->ptr->getParent());
    ASSERT_EQ(NULL, childNode->getNextSibling());
    ASSERT_EQ(NULL, childNode->getPrevSibling());

    delete parent;
}

TEST(NodeTest, DISABLED_insertNonLeafEntrySiblings_2)
{
    Node* parent = new Node(MAX_NODE_ENTRIES);

    std::vector<boost::uint64_t> lower1(2, 0);
    std::vector<boost::uint64_t> upper1(2, 0);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));

    Node* childNode1 = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry>  firstEntry( new NodeEntry(h1, rect1, childNode1, NULL));


    std::vector<boost::uint64_t> lower(2, 1);
    std::vector<boost::uint64_t> upper(2, 1);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));

    Node* childNode = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry>  secondEntry( new NodeEntry(h, rect, childNode, NULL));

    parent->insertNonLeafEntry(firstEntry);
    parent->insertNonLeafEntry(secondEntry);

    ASSERT_EQ(childNode1, childNode->getPrevSibling());
    ASSERT_EQ(NULL, childNode->getNextSibling());

    std::vector<boost::uint64_t> lower2(2, 3);
    std::vector<boost::uint64_t> upper2(2, 3);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));

    Node* childNode2 = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry>  thirdEntry( new NodeEntry(h2, rect2, childNode2, NULL));

    parent->insertNonLeafEntry(thirdEntry);

    ASSERT_EQ(childNode, childNode2->getPrevSibling());
    ASSERT_EQ(childNode2, childNode->getNextSibling());

    delete parent;
}

TEST(NodeTest,insertNonLeafEntrySiblings_3)
{
    std::vector<boost::uint64_t> lower1(2, 0);
    std::vector<boost::uint64_t> upper1(2, 0);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));

    Node* parent1 = new Node(MAX_NODE_ENTRIES);
    Node* childNode1 = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry>  entry1( new NodeEntry(h1, rect1, childNode1, NULL));

    std::vector<boost::uint64_t> lower2(2, 1);
    std::vector<boost::uint64_t> upper2(2, 1);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));

    Node* parent2 = new Node(MAX_NODE_ENTRIES);
    Node* childNode2 = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry>  entry2( new NodeEntry(h2, rect2, childNode2, NULL));

    std::vector<boost::uint64_t> lower3(2, 1);
    std::vector<boost::uint64_t> upper3(2, 1);
    boost::shared_ptr<Rectangle> rect3(new Rectangle(lower3, upper3));
    boost::shared_ptr<HilbertValue> h3(new HilbertValue(rect3->getCenter()));

    Node* parent3 = new Node(MAX_NODE_ENTRIES);
    Node* childNode3 = new Node(MAX_NODE_ENTRIES);
    boost::shared_ptr<NodeEntry>  entry3( new NodeEntry(h3, rect3, childNode3, NULL));

    parent1->setNextSibling(parent2);
    parent2->setPrevSibling(parent1);

    parent2->setNextSibling(parent3);
    parent3->setPrevSibling(parent2);

    parent2->insertNonLeafEntry(entry2);
    ASSERT_EQ(NULL, childNode2->getPrevSibling());
    ASSERT_EQ(NULL, childNode2->getNextSibling());

    parent1->insertNonLeafEntry(entry1);

    ASSERT_EQ(childNode2, childNode1->getNextSibling());
    ASSERT_EQ(NULL, childNode1->getPrevSibling());
    ASSERT_EQ(childNode1, childNode2->getPrevSibling());
    ASSERT_EQ(NULL, childNode2->getNextSibling());

    parent3->insertNonLeafEntry(entry3);

    ASSERT_EQ(childNode2, childNode1->getNextSibling());
    ASSERT_EQ(NULL, childNode1->getPrevSibling());
    ASSERT_EQ(childNode1, childNode2->getPrevSibling());
    ASSERT_EQ(childNode3, childNode2->getNextSibling());
    ASSERT_EQ(NULL, childNode3->getNextSibling());
    ASSERT_EQ(childNode2, childNode3->getPrevSibling());

    delete parent1;
    delete parent2;
    delete parent3;
}

TEST(NodeTest, isOverflowing)
{
    std::vector<boost::uint64_t> lower(2, 2);
    std::vector<boost::uint64_t> upper(2, 4);
    boost::shared_ptr<Rectangle> rect(new Rectangle(lower, upper));
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));

    boost::shared_ptr<NodeEntry> leafEntry( new NodeEntry(h, rect, NULL, NULL));
    boost::shared_ptr<NodeEntry> leafEntry1( new NodeEntry(h, rect, NULL, NULL));

    Node n(2);
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

    boost::shared_ptr<NodeEntry> leafEntry( new NodeEntry(h, rect, NULL, NULL));

    Node n(2);
    n.setLeaf(true);

    ASSERT_TRUE(n.isUnderflowing());

    n.insertLeafEntry(leafEntry);

    //TODO:For some reason this is failing and crashing the program
    ASSERT_FALSE(n.isUnderflowing());
}

TEST(NodeTest, adjustMBR)
{
    std::vector<boost::uint64_t> lower1(2, 0);
    std::vector<boost::uint64_t> upper1(2, 4);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));
    boost::shared_ptr<NodeEntry> leafEntry1(new NodeEntry(h1, rect1, NULL, NULL));

    std::vector<boost::uint64_t> lower2(2, 1);
    std::vector<boost::uint64_t> upper2(2, 5);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));
    boost::shared_ptr<NodeEntry> leafEntry2(new NodeEntry(h2, rect2, NULL, NULL));

    std::vector<boost::uint64_t> lower3(2, 5);
    std::vector<boost::uint64_t> upper3(2, 10);
    boost::shared_ptr<Rectangle> rect3(new Rectangle(lower3, upper3));
    boost::shared_ptr<HilbertValue> h3(new HilbertValue(rect3->getCenter()));
    boost::shared_ptr<NodeEntry> leafEntry3(new NodeEntry(h3, rect3, NULL, NULL));

    Node n(4);
    n.setLeaf(true);
    n.insertLeafEntry(leafEntry1);
    n.insertLeafEntry(leafEntry2);

    ASSERT_NO_FATAL_FAILURE(n.adjustMBR());

    boost::shared_ptr<Rectangle> r = n.getMBR();

    ASSERT_EQ(0,r->getLower()[0]);
    ASSERT_EQ(0,r->getLower()[1]);

    ASSERT_EQ(5,r->getUpper()[0]);
    ASSERT_EQ(5,r->getUpper()[1]);

    Node n1(4);
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
    boost::shared_ptr<NodeEntry> leafEntry1(new NodeEntry(h1, rect1, NULL, NULL));

    std::vector<boost::uint64_t> lower2(2, 8);
    std::vector<boost::uint64_t> upper2(2, 8);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));
    boost::shared_ptr<NodeEntry> leafEntry2(new NodeEntry(h2, rect2, NULL, NULL));

    Node n(4);
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
    std::vector<boost::uint64_t> upper1(2, 4);
    boost::shared_ptr<Rectangle> rect1(new Rectangle(lower1, upper1));
    boost::shared_ptr<HilbertValue> h1(new HilbertValue(rect1->getCenter()));
    boost::shared_ptr<NodeEntry> leafEntry1(new NodeEntry(h1, rect1, NULL, NULL));

    std::vector<boost::uint64_t> lower2(2, 1);
    std::vector<boost::uint64_t> upper2(2, 5);
    boost::shared_ptr<Rectangle> rect2(new Rectangle(lower2, upper2));
    boost::shared_ptr<HilbertValue> h2(new HilbertValue(rect2->getCenter()));
    boost::shared_ptr<NodeEntry> leafEntry2(new NodeEntry(h2, rect2, NULL, NULL));

    Node n(4);
    n.setLeaf(true);
    n.insertLeafEntry(leafEntry1);
    n.insertLeafEntry(leafEntry2);

    ASSERT_NO_FATAL_FAILURE(n.adjustLHV());

    ASSERT_TRUE((*h1.get())<(*h2.get()));
    ASSERT_TRUE((*h2.get())== (*n.getLHV().get()));
}

TEST(NodeTest, getSiblings)
{
    Node* right =new Node(MAX_NODE_ENTRIES);
    right->setLeaf(true);

    Node* left =new Node(MAX_NODE_ENTRIES);
    left->setLeaf(true);

    Node* main =new Node(MAX_NODE_ENTRIES);
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

    ASSERT_TRUE(*it == left);
    ++it;
    ASSERT_TRUE(*it == main);
    ++it;
    ASSERT_TRUE(*it == right);

    delete left;
    delete right;
    delete main;
}
