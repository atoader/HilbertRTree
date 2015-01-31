/*
 * Node.hh
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#ifndef HRTREE_SRC_NODE_HH_
#define HRTREE_SRC_NODE_HH_

#include <set>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include "HilbertValue.hh"
#include "Rectangle.hh"
#include "NodeEntry.hh"
#include "Common.hh"
#include "NonLeafEntry.hh"
#include "LeafEntry.hh"

class Node
{
public:

    /**
     * @brief Node Initialize a new instance of the Node class.
     * Each node can hold at least m <= M/2 and at most M entries(if it is not the root)
     * @param m
     * @param M
     */
    Node(boost::uint32_t m, boost::uint32_t M);

    /**
     * @brief isOverflowing
     * @return TRUE if there are M entries in this node, FALSE otherwise
     */
    bool isOverflowing();

    /**
     * @brief isUnderflowing
     * @return TRUE if there are less than m entries in this node.
     */
    bool isUnderflowing();

    Node *getPrevSibling() const;
    void setPrevSibling(Node *value);

    Node *getNextSibling() const;
    void setNextSibling(Node *value);

    Node *getParent() const;
    void setParent(Node *value);

    bool isLeaf() const;
    void setLeaf(bool value);

    /**
     * @brief getLHV Get the largest Hilbert Value of the subtree rooted in this node.
     * A shared_ptr containing NULL is returned if this node does not have any children
     * @return
     */
    boost::shared_ptr<HilbertValue> getLHV();

    /**
     * @brief getMBR Get the minimum bounding rectangle for the subtree rooted in this node
     * A shared_ptr containing NULL is returned if this node does not have any children
     * @return
     */
    boost::shared_ptr<Rectangle> getMBR();

    /**
     * @brief insertLeafEntry Insert an entry into this node
     * if and only if this node is a leaf node and the entry is a leaf entry
     * @param entry
     */
    void insertLeafEntry(const boost::shared_ptr<LeafEntry>& entry);

    /**
     * @brief insertNonLeafEntry Insert an entry into this node
     * if and onlly if this node is a non-leaf node and the entry is a non-leaf entry
     * @param entry
     */
    void insertNonLeafEntry(const boost::shared_ptr<NonLeafEntry>& entry);

    /**
     * @brief removeEntry Remove the leaf entry that has a MBR equal to the one
     * passed in as a parameter
     * @param rect
     */
    void removeLeafEntry(const boost::shared_ptr<Rectangle>& rect);

    /**
     * @brief removeNonLeafEntry Remove the non-leaf entry that has a ptr to the node
     * passed in as parameter
     * @param child
     */
    void removeNonLeafEntry(Node* child);

    /**
     * @brief adjustMBR Adjust the value of the MBR
     * of this node depending on the value of the entries
     * If this node is empty, the value of the MBR should be NULL.
     */
    void adjustMBR();

    /**
     * @brief adjustLHV Adjust the value of the MBR of this node.
     * If this node is empty, the value of the LHV will be NULL.
     */
    void adjustLHV();

    /**
     * @brief getSiblings Get an ordered list of nodes.
     * Each node is a sibling of the node before and after it.
     * @param siblingsNo Maximum number of siblings to retrieve.
     * @return
     */
    std::list<Node*> getSiblings(boost::uint32_t siblingsNo);

    const EntryMultiSet& getEntries() const;

    /**
     * @brief resetEntriesSet Remove all the entries of this node.
     */
    void resetEntriesSet();

private:
    //These values can be factored out, but they are great for testing.
    boost::uint32_t m;
    boost::uint32_t M;

    boost::shared_ptr<Rectangle> mbr;
    boost::shared_ptr<HilbertValue> lhv;
    EntryMultiSet entries;
    bool leaf;
    Node *parent;
    Node *prevSibling;
    Node *nextSibling;
};

#endif /* HRTREE_SRC_NODE_HH_ */
