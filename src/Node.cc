/*
 * Node.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include <cstddef>
#include <stdexcept>

#include "Constants.hh"
#include "Node.hh"
#include "Common.hh"


typedef std::multiset<boost::shared_ptr<NodeEntry>, Common::nodeEntryComparisonObj>::iterator EntryIterator;

Node::Node(boost::uint32_t capacity)
{
    this->capacity = capacity;

    this->leaf = false;
    this->nextSibling = NULL;
    this->prevSibling = NULL;
    this->parent = NULL;
}

bool Node::isOverflowing()
{
    return this->entries.size() == this->capacity;
}

bool Node::isUnderflowing()
{
    return (this->entries.size()) < (this->capacity / 2);
}

Node *Node::getPrevSibling() const
{
    return prevSibling;
}

void Node::setPrevSibling(Node *value)
{
    prevSibling = value;
}
Node *Node::getNextSibling() const
{
    return nextSibling;
}

void Node::setNextSibling(Node *value)
{
    nextSibling = value;
}
Node *Node::getParent() const
{
    return parent;
}

void Node::setParent(Node *value)
{
    parent = value;
}
bool Node::isLeaf() const
{
    return leaf;
}

void Node::setLeaf(bool value)
{
    leaf = value;
}

boost::shared_ptr<HilbertValue> Node::getLHV()
{
    return this->lhv;
}

boost::shared_ptr<Rectangle> Node::getMBR()
{
    return this->mbr;
}

void Node::insertLeafEntry(const boost::shared_ptr<LeafEntry> &entry)
{
    if(!this->leaf)
    {
        throw std::runtime_error("The current node is not a leaf node.");
    }

    if(this->isOverflowing())
    {
        throw std::runtime_error("The node is overflowing.");
    }

    this->entries.insert(entry);
}

void Node::insertNonLeafEntry(const boost::shared_ptr<NonLeafEntry> &entry)
{
    if(this->leaf)
    {
        throw std::runtime_error("The current node is a leaf node.");
    }

    if(this->isOverflowing())
    {
        throw std::runtime_error("The node is overflowing.");
    }

    //Insert the entry in the correct position according
    //to its value and assign the appropriate siblings for the node
    EntryIterator it = this->entries.insert(entry);

    //The parent of the node the entry is pointing to will be the current node.
    entry->getNode()->setParent(this);

    assert(!this->nextSibling || (this->nextSibling && this->leaf == this->nextSibling->leaf));
    assert(!this->prevSibling || (this->prevSibling && this->leaf == this->prevSibling->leaf));

    EntryIterator aux;
    Node* nextSib=NULL;
    Node* prevSib=NULL;

    //If the element was inserted in the beginning, we have to search for the sibling
    //in the previous node
    if(it==this->entries.begin())
    {
        if(this->prevSibling!=NULL && this->prevSibling->entries.size()>0)
        {
            EntryIterator prevIt = this->prevSibling->entries.end();
            prevIt--;
            prevSib = boost::dynamic_pointer_cast<NonLeafEntry>(*prevIt)->getNode();
        }
    }
    else
    {
        EntryIterator prevIt =it;
        prevIt--;
        prevSib = boost::dynamic_pointer_cast<NonLeafEntry>(*prevIt)->getNode();
    }

    entry->getNode()->prevSibling = prevSib;
    if(prevSib!=NULL)
    {
        prevSib->nextSibling = boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode();
        assert(entry->getNode()->leaf == prevSib->leaf);
    }

    aux = this->entries.end();
    aux--;

    if(it==aux)
    {
        //The inserted element is the last in the list
        //try to find a sibling in the list of the sibling node.
        if(this->nextSibling!=NULL && this->nextSibling->entries.size()>0)
        {
            nextSib = boost::dynamic_pointer_cast<NonLeafEntry>(*this->nextSibling->entries.begin())->getNode();
        }
    }
    else
    {
        EntryIterator nextIt  = it;
        nextIt++;
        nextSib = boost::dynamic_pointer_cast<NonLeafEntry>(*nextIt)->getNode();
    }

    entry->getNode()->nextSibling = nextSib;
    if(nextSib!=NULL)
    {
        nextSib->prevSibling = boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode();
        assert(entry->getNode()->leaf == nextSib->leaf);
    }
}


void Node::adjustMBR()
{
    assert(this->entries.size() > 0);
    EntryIterator it = this->entries.begin();

    std::vector<boost::uint64_t> lower = (*it)->getMBR()->getLower();
    std::vector<boost::uint64_t> upper = (*it)->getMBR()->getUpper();
    ++it;

    for(; it!=this->entries.end(); ++it)
    {
        const std::vector<boost::uint64_t>& low = (*it)->getMBR()->getLower();
        const std::vector<boost::uint64_t>& up = (*it)->getMBR()->getUpper();

        for (size_t i = 0; i < low.size(); ++i)
        {
            if (lower[i] > low[i])
            {
                lower[i] = low[i];
            }

            if (upper[i] < up[i])
            {
                upper[i] = up[i];
            }
        }
    }

    this->mbr.reset(new Rectangle(lower, upper));
}

void Node::adjustLHV()
{
    assert(this->entries.size() > 0);
    EntryIterator it = this->entries.begin();

    this->lhv = (*it)->getLHV();
    ++it;

    for(; it!=this->entries.end(); ++it)
    {
        if( (*(this->getLHV().get())) < (*((*it)->getLHV().get())))
        {
            this->lhv = (*it)->getLHV();
        }
    }
}

std::list<Node *> Node::getSiblings(boost::uint32_t siblingsNo)
{
    std::list<Node*> result;

    Node* right = this->nextSibling;
    Node* left = this->prevSibling;

    //The node is already part of the list of cooperating siblings
    result.push_back(this);

    while (result.size() < siblingsNo)
    {
        if(left!=NULL )
        {
            result.push_front(left);
            left = left->prevSibling;
        }

        if(right!=NULL && result.size() < siblingsNo)
        {
            result.push_back(right);
            right=right->nextSibling;
        }

        if(right==NULL && left == NULL)
        {
            break;
        }
    }

    return result;
}

const EntryMultiSet &Node::getEntries() const
{
    return this->entries;
}

void Node::resetEntriesSet()
{
    this->entries.clear();
}
