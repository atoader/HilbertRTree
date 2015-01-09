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

void Node::insertLeafEntry(const boost::shared_ptr<NodeEntry> &entry)
{
    //Check if the entry we are inserting is a leaf entry
    //i.e. it does not have any children and that the current node
    //is a leaf
    if(entry->ptr != NULL)
    {
        throw std::runtime_error("The entry is not a leaf entry.");
    }

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

void Node::insertNonLeafEntry(const boost::shared_ptr<NodeEntry> &entry)
{
    if(entry->ptr == NULL)
    {
        throw std::runtime_error("The entry is a leaf entry.");
    }

    if(this->leaf)
    {
        throw std::runtime_error("The current node is a leaf node.");
    }

    if(this->isOverflowing())
    {
        throw std::runtime_error("The node is overflowing.");
    }

    entry->ptr->setParent(this);

    //Insert the entry in the correct position according
    //to its value and assign the appropriate siblings for the node
    EntryIterator it = this->entries.insert(entry);
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
            prevSib = (*prevIt)->ptr;
        }
    }
    else
    {
        EntryIterator prevIt =it;
        prevIt--;
        prevSib = (*prevIt)->ptr;
    }

    aux = this->entries.end();
    aux--;

    if(it==aux)
    {
        //The inserted element is the last in the list
        //try to find a sibling in the list of the sibling node.
        if(this->nextSibling!=NULL && this->nextSibling->entries.size()>0)
        {
            nextSib = (*this->nextSibling->entries.begin())->ptr;
        }
    }
    else
    {
        EntryIterator nextIt  = it;
        nextIt++;
        nextSib = (*nextIt)->ptr;
    }

    (*it)->ptr->prevSibling = prevSib;
    (*it)->ptr->nextSibling = nextSib;

    if(prevSib!=NULL)
    {
        prevSib->nextSibling = (*it)->ptr;
    }

    if(nextSib!=NULL)
    {
        nextSib->prevSibling = (*it)->ptr;
    }
}


void Node::adjustMBR()
{
    assert(this->entries.size() > 0);

    EntryIterator it = this->entries.begin();

    std::vector<boost::uint64_t> lower = (*it)->mbr->getLower();
    std::vector<boost::uint64_t> upper = (*it)->mbr->getUpper();

    ++it;

    for(; it!=this->entries.end(); ++it)
    {
        const std::vector<boost::uint64_t>& low = (*it)->mbr->getLower();
        const std::vector<boost::uint64_t>& up = (*it)->mbr->getUpper();

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

    this->lhv = (*it)->lhv;
    ++it;

    for(; it!=this->entries.end(); ++it)
    {
        if( (*(this->lhv.get())) < (*((*it)->lhv.get())))
        {
            this->lhv = (*it)->lhv;
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
        if(right!=NULL)
        {
            result.push_back(right);
            right=right->nextSibling;
        }

        if(left!=NULL && result.size() < siblingsNo)
        {
            result.push_front(left);
            left = left->prevSibling;
        }

        if(right==NULL && left == NULL)
        {
            break;
        }
    }

    return result;
}

EntryMultiSet &Node::getEntries()
{
    return this->entries;
}
