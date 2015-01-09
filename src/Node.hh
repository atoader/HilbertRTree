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

struct Node
{
    /**
     * @brief Node Initialize a new instance of the Node class
     * which can hold at most \i{capacity} items before overflowing
     * and at least \i{capacity/2} items before underflowing
     *
     * @param capacity
     */
    Node(boost::uint32_t capacity);

    bool isOverflowing();

    bool isUnderflowing();

    Node *getPrevSibling() const;
    void setPrevSibling(Node *value);

    Node *getNextSibling() const;
    void setNextSibling(Node *value);

    Node *getParent() const;
    void setParent(Node *value);

    bool isLeaf() const;
    void setLeaf(bool value);

    boost::shared_ptr<HilbertValue> getLHV();

    boost::shared_ptr<Rectangle> getMBR();

    /**
     * @brief insertLeafEntry Insert an entry into this node
     * if and only if this node is a leaf node and the entry is a leaf entry
     * @param entry
     */
    void insertLeafEntry(const boost::shared_ptr<NodeEntry>& entry);

    /**
     * @brief insertNonLeafEntry Insert an entry into this node
     * if and onlly if this node is a non-leaf node and the entry is a non-leaf entry
     * @param entry
     */
    void insertNonLeafEntry(const boost::shared_ptr<NodeEntry>& entry);

    void adjustMBR();

    void adjustLHV();

    /**
     * @brief getSiblings Get an ordered list of nodes.
     * Each node is a sibling of the node before and after it.
     * @param siblingsNo Maximum number of siblings to retrieve.
     * @return
     */
    std::list<Node*> getSiblings(boost::uint32_t siblingsNo);

    EntryMultiSet& getEntries();

private:
    boost::uint32_t capacity;
    boost::shared_ptr<Rectangle> mbr;
    boost::shared_ptr<HilbertValue> lhv;
    EntryMultiSet entries;
    bool leaf;
    Node *parent;
    Node *prevSibling;
    Node *nextSibling;
};

#endif /* HRTREE_SRC_NODE_HH_ */
