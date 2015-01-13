#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "RTreeHelper.hh"
#include "Constants.hh"

using boost::shared_ptr;

int RTreeHelper::counter = 0;

std::list<boost::shared_ptr<NodeEntry> > RTreeHelper::search(Node *subtreeRoot, const boost::shared_ptr<Rectangle> &query)
{
    std::list<boost::shared_ptr<NodeEntry> > result;
    std::list<boost::shared_ptr<NodeEntry> > aux;

    const EntryMultiSet & entries =  subtreeRoot->getEntries();
    EntryMultiSet::iterator it;

    //If this is a leaf, return a list of entries that intersect the query rectangle.
    if(subtreeRoot->isLeaf())
    {
        for(it =entries.begin(); it!=entries.end(); ++it)
        {
            if((*it)->getMBR()->intersects(*query))
            {
                result.push_back(*it);
            }
        }
    }
    else
    {
        for(it = entries.begin(); it!=entries.end(); ++it)
        {
            //For each child node whose MBR intersects, retrieve its search result
            //and merge it into the final result
            if((*it)->getMBR()->intersects(*query))
            {
                aux = RTreeHelper::search(boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode(), query);
                result.insert(result.end(), aux.begin(), aux.end());
            }
        }
    }

    return result;
}

Node *RTreeHelper::chooseLeaf(Node *N, const boost::shared_ptr<HilbertValue> &h)
{
    if (N->isLeaf())
    {
        return N;
    }
    else
    {
        EntryMultiSet::iterator it;

        for(it = N->getEntries().begin(); it!=N->getEntries().end(); ++it)
        {
            //Choose the entry that has the LHV larger than the inserted value
            if( *((*it)->getLHV()) >= *h)
            {
                assert(!(*it)->isLeafEntry());
                return RTreeHelper::chooseLeaf(boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode(), h);
            }
        }

        //if h is larger than all the LHV already in the node,
        //choose the last of the node entries
        --it;

        return RTreeHelper::chooseLeaf( boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode(), h);
    }
}

Node *RTreeHelper::exactSearch(Node *subtreeRoot, boost::shared_ptr<Rectangle> rect)
{
    const EntryMultiSet & entries =  subtreeRoot->getEntries();
    EntryMultiSet::iterator it;

    //There are no duplicates in the tree
    if(subtreeRoot->isLeaf())
    {
        for(it =entries.begin(); it!=entries.end(); ++it)
        {
            if( *rect ==*((*it)->getMBR()))
            {
                return subtreeRoot;
            }
        }
    }
    else
    {
        for(it = entries.begin(); it!=entries.end(); ++it)
        {
            if((*it)->getMBR()->contains(*rect))
            {
                return exactSearch(boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode(), rect);
            }
        }
    }

    return NULL;
}

void RTreeHelper::redistributeEntries(EntryMultiSet &entries, std::list<Node *> siblings)
{
    unsigned long batchSize = (unsigned long)std::ceil( (double)entries.size()/(double)siblings.size());

    std::list<Node*>::iterator siblingsIt = siblings.begin();

    unsigned long currentBatch = 0;

    for(EntryMultiSet::iterator entryIt = entries.begin(); entryIt !=entries.end(); entryIt++)
    {
        if((*entryIt)->isLeafEntry())
        {
            (*siblingsIt)->insertLeafEntry(boost::dynamic_pointer_cast<LeafEntry>(*entryIt));
        }
        else
        {
            (*siblingsIt)->insertNonLeafEntry(boost::dynamic_pointer_cast<NonLeafEntry>(*entryIt));
        }

        currentBatch++;

        //If the current sibling has received its share,
        //adjust the MBR and LHV of the current node and
        //move to the next sibling and reset that batch counter
        if(currentBatch ==  batchSize)
        {
            currentBatch = 0;
            (*siblingsIt)->adjustLHV();
            (*siblingsIt)->adjustMBR();
            siblingsIt++;
        }
    }

    if(siblingsIt!=siblings.end())
    {
        (*siblingsIt)->adjustLHV();
        (*siblingsIt)->adjustMBR();
    }
}

Node *RTreeHelper::handleOverflow(Node *target, const boost::shared_ptr<NodeEntry> &entry,std::list<Node*>& out_siblings)
{
    //List of entries from the cooperating nodes
    EntryMultiSet entries;

    //Node that will be created if there is no room for redistribution
    Node* newNode=NULL;

    //Position of the target node in the siblings list
    std::list<Node*>::iterator targetPos;

    //The list will contain the siblings of the target
    out_siblings = target->getSiblings(SIBLINGS_NO);

    //Insert the new entry in the set of entries that have to be redistributed
    entries.insert(entry);

    //Copy the entries of the siblings node into the set of entries that must be redistributed
    for(std::list<Node*>::iterator it = out_siblings.begin(); it!=out_siblings.end(); ++it)
    {
        assert((*it)->isLeaf()==entry->isLeafEntry());
        entries.insert((*it)->getEntries().begin(), (*it)->getEntries().end());
        //Clear the entries in each node
        (*it)->resetEntriesSet();

        if(*it==target)
        {
            targetPos = it;
        }
    }

    //If there is not enough room, create e new node
    if (entries.size() > out_siblings.size() * MAX_NODE_ENTRIES)
    {
        //The new node is a sibling of the target.
        newNode = new Node(MIN_NODE_ENTRIES, MAX_NODE_ENTRIES);
        //The new node will be a leaf only if its entries are leaf entries
        newNode->setLeaf(entry->isLeafEntry());

        //The previous sibling of the new node will be the
        //previous sibling of the first node in the set of siblings
        Node* prevSib = target->getPrevSibling();
        newNode->setPrevSibling(prevSib);
        if(prevSib!=NULL)
        {
            assert(prevSib->isLeaf()==newNode->isLeaf());
            prevSib->setNextSibling(newNode);
        }

        //The next sibling of the new node will be the next sibling
        //of the first node
        newNode->setNextSibling(target);
        target->setPrevSibling(newNode);

        //Insert the new node in the list of cooperating siblings
        out_siblings.insert(targetPos, newNode);
    }

    //Redistribute the entries and adjust the nodes
    RTreeHelper::redistributeEntries(entries, out_siblings);

    for(std::list<Node*>::iterator it = out_siblings.begin(); it!=out_siblings.end(); ++it)
    {
        assert((*it)->isLeaf()==entry->isLeafEntry());
    }

    return newNode;
}

Node* RTreeHelper::handleUnderflow(Node *target, std::list<Node *> &out_siblings)
{
    //List of entries from the cooperating nodes
    EntryMultiSet entries;
    //Pointer to the deleted node if any
    Node* removed = NULL;

    //This will contain the target node and at most SIBLINGS_NO of its siblings
    out_siblings = target->getSiblings(SIBLINGS_NO+1);

    for(std::list<Node*>::iterator it = out_siblings.begin(); it!=out_siblings.end(); ++it)
    {
        entries.insert((*it)->getEntries().begin(), (*it)->getEntries().end());
        (*it)->resetEntriesSet();
    }

    //The nodes are underflowing and our node is not the root
    if(entries.size() < out_siblings.size()*MIN_NODE_ENTRIES && target->getParent()!=NULL)
    {
        removed = out_siblings.front();
        Node* prevSib = removed->getPrevSibling();
        Node* nextSib = removed->getNextSibling();

        if(prevSib!=NULL)
        {
            prevSib->setNextSibling(nextSib);
        }

        if(nextSib!=NULL)
        {
            nextSib->setPrevSibling(prevSib);
        }

        out_siblings.pop_front();
    }

    RTreeHelper::redistributeEntries(entries, out_siblings);

    return removed;
}

Node *RTreeHelper::adjustTreeForInsert(Node* root,Node *N, Node *NN, std::list<Node*>siblings)
{
    //Node that is created if the parent needs to be split
    Node* PP = NULL;

    //The new root
    Node* newRoot = root;

    std::list<Node*> newSiblings;

    //Set of parent nodes of the sibling nodes
    std::set<Node*> P;

    //Flag determining if a new node was created
    std::set<Node*> S;
    S.insert(siblings.begin(), siblings.end());

    bool ok = true;

    while (ok)
    {
        //The parent of the node being updated
        Node* Np = N->getParent();

        if (Np == NULL)
        {
            //N is the root, we must exit the loop
            ok = false;

            //If the root has overflown, a new root will be created
            if (NN != NULL)
            {
                shared_ptr<NonLeafEntry> n(new NonLeafEntry(N));
                shared_ptr<NonLeafEntry> nn(new NonLeafEntry(NN));

                newRoot = new Node(MIN_NODE_ENTRIES,MAX_NODE_ENTRIES);
                newRoot->insertNonLeafEntry(n);
                newRoot->insertNonLeafEntry(nn);
            }

            //Adjust the LHV and MBR of the root
            newRoot->adjustLHV();
            newRoot->adjustMBR();
        }
        else
        {
            if (NN != NULL)
            {
                //Insert the entry containing the newly created node into the tree
                shared_ptr<NonLeafEntry> nn(new NonLeafEntry(NN));
                //try to insert the new node into the parent
                if (!Np->isOverflowing())
                {
                    Np->insertNonLeafEntry(nn);

                    //After insertion, we must adjust the LHV and MBR
                    Np->adjustLHV();
                    Np->adjustMBR();
                    //The new siblings list will only contain the node into which
                    //the new entry was inserted
                    newSiblings.push_back(Np);
                }
                else
                {
                    //All the nodes in newsiblings will have their LHV and MBR adjusted
                    PP = RTreeHelper::handleOverflow(Np, nn, newSiblings);
                }
            }
            else
            {
                //the new entry was inserted
                newSiblings.push_back(Np);
            }

            for (std::set<Node*>::iterator node = S.begin(); node != S.end();
                    ++node)
            {
                P.insert((*node)->getParent());
            }

            for (std::set<Node*>::iterator node = P.begin(); node != P.end();
                    ++node)
            {
                (*node)->adjustLHV();
                (*node)->adjustMBR();
            }

            N = Np;
            NN = PP;

            S.clear();
            P.clear();

            S.insert(newSiblings.begin(), newSiblings.end());
        }
    }

    return newRoot;
}

void RTreeHelper::adjustTreeForRemove(Node *N, Node *DN, std::list<Node *> siblings)
{
    //This list will hold the sibling nodes that
    //have to handle an underflow at the parent level
    std::list<Node*> newSiblings;

    std::set<Node*> S;
    S.insert(siblings.begin(), siblings.end());

    //Set of parent nodes of the sibling nodes
    std::set<Node*> P;

    bool keepRunning =true;

    while(keepRunning)
    {
        //Parent of the node affected by the deletion
        Node* Np = N->getParent();

        //Node in the parent level that had to be removed due to the
        //propagation of an underflow.
        Node* DPParent = NULL;

        if(Np== NULL)
        {
            keepRunning = false;
            //If the root only has one child and the root is not a leaf,
            //The entries of the child node must be inserted in the leaf
            if(N->getEntries().size()==1 && !N->isLeaf())
            {
                Node* mainEntry = boost::dynamic_pointer_cast<NonLeafEntry>(*N->getEntries().begin())->getNode();
                if(mainEntry->isLeaf())
                {
                    //The root will become a leaf.
                    N->setLeaf(true);

                    //Copy the entries for redistribution
                    EntryMultiSet data = mainEntry->getEntries();

                    //Reset the entries contained in this set.
                    N->resetEntriesSet();

                    for(EntryMultiSet::iterator it = data.begin(); it!=data.end(); ++it)
                    {
                        N->insertLeafEntry(boost::dynamic_pointer_cast<LeafEntry>(*it));
                    }
                }
                else
                {
                    //If the node under the root is not a leaf, redistribute the entries
                    EntryMultiSet data = mainEntry->getEntries();
                    N->resetEntriesSet();

                    for(EntryMultiSet::iterator it = data.begin(); it!=data.end(); ++it)
                    {
                        N->insertNonLeafEntry(boost::dynamic_pointer_cast<NonLeafEntry>(*it));
                    }
                }
            }

            //Adjust the values of the root.
            N->adjustLHV();
            N->adjustMBR();
        }
        else
        {
            //If there was a node deleted, we must update its parent and remove its entry
            if(DN!=NULL)
            {
                //Remove the node's entry from its parent
                Node* DNParent = DN->getParent();
                DNParent->removeNonLeafEntry(DN);

                //If the parent node is underflowing, handle it
                if(DNParent->isUnderflowing())
                {
                    DPParent =RTreeHelper::handleUnderflow(DNParent,newSiblings);
                }
                else
                {
                    //otherwise add the DNParent to the list of nodes that must be updated
                    newSiblings.push_back(DNParent);
                }
            }

            //The parent of the node is always affected by the removal
            newSiblings.push_back(Np);

            for (std::set<Node*>::iterator node = S.begin(); node != S.end();
                    ++node)
            {
                P.insert((*node)->getParent());
            }

            for (std::set<Node*>::iterator node = P.begin(); node != P.end();
                    ++node)
            {
                (*node)->adjustLHV();
                (*node)->adjustMBR();
            }

            N = Np;
            DN =  DPParent;

            S.clear();
            P.clear();
            S.insert(newSiblings.begin(), newSiblings.end());
        }
    }
}

void RTreeHelper::debug(Node *root)
{
    std::ofstream of("graph.txt");

    of<<"digraph g { \n forcelabels=true;\n";
    of<<RTreeHelper::listNodes(root, of);
    of<<RTreeHelper::listNodeLinks(root, of);
    of<<"}"<<std::endl;

    of.close();
}


std::string RTreeHelper::listNodeLinks(Node* node, std::ofstream& ofs)
{
    std::stringstream ss;

    ss<<(boost::uint64_t)node << " -> "<<(boost::uint64_t)node->getNextSibling()<<"[ label=\"sibling\" color=\"red:blue\" ];\n";
    ss<<(boost::uint64_t)node->getPrevSibling()<< " -> "<<(boost::uint64_t)node <<"[ label=\"sibling\" color=\"green:blue\"];\n";

    for(EntryMultiSet::iterator it = node->getEntries().begin(); it!=node->getEntries().end(); ++it)
    {
        if(!(*it)->isLeafEntry())
        {
            ss<<(boost::uint64_t)node << " -> "<<(boost::uint64_t)(boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode())<<";\n";
            ss<<RTreeHelper::listNodeLinks(boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode(), ofs);
        }
        else
        {
            ss<<(boost::uint64_t)node << " -> "<<(boost::uint64_t)(boost::uint64_t)(*it).get()<<";\n";
        }
    }

    return ss.str();
}

std::string RTreeHelper::listNodes(Node* node, std::ofstream& ofs)
{
    std::stringstream ss;

    boost::shared_ptr<Rectangle> mbr = node->getMBR();
    if(!!mbr)
    {
        ss<<(boost::uint64_t)node<<" [label=\""<<mbr->getLower()[0]<<","<<mbr->getLower()[1]<<"\n"<<mbr->getUpper()[0]<<","<<mbr->getUpper()[1]<<"\"];\n";
    }


    for(EntryMultiSet::iterator it = node->getEntries().begin(); it!=node->getEntries().end(); ++it)
    {
        if(!(*it)->isLeafEntry())
        {
            ss<<RTreeHelper::listNodes(boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode(), ofs);
        }
        else
        {
            ss<<(boost::uint64_t)(*it).get()<<" [label=\""<<(*it)->getMBR()->getLower()[0]<<","<<(*it)->getMBR()->getLower()[1]<<"\n"<<(*it)->getMBR()->getUpper()[0]<<","<<(*it)->getMBR()->getUpper()[1]<<"\"];\n";
        }
    }

    return ss.str();
}

