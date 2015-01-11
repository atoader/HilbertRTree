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
    this->root = new Node(MAX_NODE_ENTRIES);
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

    this->root = RTreeHelper::adjustTree(this->root,L, NN, out_siblings);
}

void RTree::remove(const boost::shared_ptr<Rectangle> &rect)
{
    Node* node = RTreeHelper::exactSearch(this->root, rect);
    std::list<Node*> siblings;
    bool ok=true;

    if(node!=NULL)
    {
        if(node->removeEntry(rect))
        {
            node->adjustLHV();
            node->adjustMBR();

            while(ok)
            {
                if(node->isUnderflowing())
                {
                    Node* removed = RTreeHelper::handleUnderflow(node, siblings);
                    if(removed!=NULL)
                    {
                        Node* pr = removed->getParent();
                        if(pr!=NULL)
                        {
                            pr->removeEntry(removed);
                            node=pr;
                        }
                        else
                        {
                            ok=false;
                        }
                    }
                    else
                    {
                        ok=false;
                    }

                    std::set<Node*> s;
                    std::set<Node*> p;
                    s.insert(siblings.begin(), siblings.end());

                    for(std::set<Node*>::iterator it=s.begin(); it!=s.end(); ++it)
                    {
                        p.insert((*it)->getParent());
                    }


                    for(std::set<Node*>::iterator it=p.begin(); it!=p.end(); ++it)
                    {
                        (*it)->adjustLHV();
                        (*it)->adjustMBR();
                    }
                }
            }
        }
    }
}

Node *RTree::getRoot() const
{
    return root;
}
