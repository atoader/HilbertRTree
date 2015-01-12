/*
 * RTree.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include <cstddef>

#include <set>

#include "RTree.hh"
#include "RTreeHelper.hh"
#include "HilbertValue.hh"
#include "Constants.hh"
#include "Common.hh"
#include "NodeEntry.hh"

RTree::RTree()
{
    this->root = new Node(MIN_NODE_ENTRIES, MAX_NODE_ENTRIES);
    this->root->setLeaf(true);
}

RTree::~RTree()
{
    delete this->root;
}

std::list<boost::shared_ptr<NodeEntry> > RTree::search(const boost::shared_ptr<Rectangle> &rect)
{
    return RTreeHelper::search(this->root, rect);
}

void RTree::insert(const boost::shared_ptr<Rectangle> &rect)
{
    //Compute the hilbert value of the center of the rectangle
    boost::shared_ptr<HilbertValue> h(new HilbertValue(rect->getCenter()));

    std::list<Node*> out_siblings;

    //Create a new entry to insert
    boost::shared_ptr<LeafEntry> newEntry(new LeafEntry(rect, h));

    //Node that is created if an overflow occurs.
    Node* NN = NULL;

    //Find a leaf into which we can insert this value
    Node* L = RTreeHelper::chooseLeaf(this->root, h);

    //if the leaf is full
    if(!L->isOverflowing())
    {
        //If there is room in the leaf, insert the entry
        L->insertLeafEntry(newEntry);
        out_siblings.push_back(L);

    }
    else
    {
        //Handle the overflow of the new node.
        NN =  RTreeHelper::handleOverflow(L, newEntry, out_siblings);
    }

    this->root = RTreeHelper::adjustTreeForInsert(this->root,L, NN, out_siblings);
}

void RTree::remove(const boost::shared_ptr<Rectangle> &rect)
{
    std::list<Node*> out_siblings;

    //Find the node containing the entry
    Node* L = RTreeHelper::exactSearch(this->root, rect);
    Node* DL = NULL;

    //If there was a match
    if(L!=NULL)
    {
        //Remove the entry from the node
        L->removeLeafEntry(rect);

        if(L->isUnderflowing())
        {
            //DL is the node that was deleted, NULL if no node was deleted
            DL=RTreeHelper::handleUnderflow(L, out_siblings);
        }

        RTreeHelper::adjustTreeForRemove(L, DL, out_siblings);
    }
}

Node *RTree::getRoot() const
{
    return root;
}
