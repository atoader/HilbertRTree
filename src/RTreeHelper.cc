#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "RTreeHelper.hh"
#include "Constants.hh"

using boost::shared_ptr;

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
            //Choose the entry that has the LHV at least equal to the value we have
            HilbertValue& searchedValue = *h.get();
            HilbertValue& current = *((*it)->getLHV().get());
            if( current > searchedValue)
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

std::list<boost::shared_ptr<NodeEntry> > RTreeHelper::search(Node *subtreeRoot, const boost::shared_ptr<Rectangle> &rect)
{
    std::list<boost::shared_ptr<NodeEntry> > result;
    std::list<boost::shared_ptr<NodeEntry> > aux;

    const EntryMultiSet & entries =  subtreeRoot->getEntries();
    EntryMultiSet::iterator it;

    if(subtreeRoot->isLeaf())
    {
        for(it =entries.begin(); it!=entries.end(); ++it)
        {
            if((*it)->getMBR()->intersects(*rect.get()))
            {
                result.push_back(*it);
            }
        }
    }
    else
    {
        for(it = entries.begin(); it!=entries.end(); ++it)
        {
            if((*it)->getMBR()->intersects(*rect.get()))
            {
                aux = RTreeHelper::search(boost::dynamic_pointer_cast<NonLeafEntry>(*it)->getNode(), rect);
                result.insert(result.end(), aux.begin(), aux.end());
            }
        }
    }

    return result;
}

void RTreeHelper::redistributeEntries(EntryMultiSet &entries, std::list<Node *> siblings)
{
    unsigned long batchSize = std::min(MAX_NODE_ENTRIES, entries.size()/siblings.size()+1);

    std::list<Node*>::iterator siblingsIt = siblings.begin();

    (*siblingsIt)->resetEntriesSet();

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

        if(currentBatch ==  batchSize && (++siblingsIt)!=siblings.end())
        {
            (*siblingsIt)->resetEntriesSet();
            currentBatch = 0;
        }
    }

    for(siblingsIt=siblings.begin(); siblingsIt!=siblings.end(); ++siblingsIt)
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

    out_siblings = target->getSiblings(SIBLINGS_NO);

    entries.insert(entry);

    for(std::list<Node*>::iterator it = out_siblings.begin(); it!=out_siblings.end(); ++it)
    {
        assert((*it)->isLeaf()==entry->isLeafEntry());
        entries.insert((*it)->getEntries().begin(), (*it)->getEntries().end());
    }

    //If there is not enough room, create e new node
    if (entries.size() > out_siblings.size() * MAX_NODE_ENTRIES)
    {
        //The new node is a sibling of the target.
        newNode = new Node(MAX_NODE_ENTRIES);

        newNode->setLeaf(entry->isLeafEntry());

        Node* prevSib = out_siblings.front()->getPrevSibling();

        newNode->setPrevSibling(prevSib);
        if(prevSib!=NULL)
        {
            assert(prevSib->isLeaf()==newNode->isLeaf());
            prevSib->setNextSibling(newNode);
        }

        newNode->setNextSibling(out_siblings.front());
        out_siblings.front()->setPrevSibling(newNode);

        assert(out_siblings.front()->isLeaf()==newNode->isLeaf());
        out_siblings.push_front(newNode);
    }

    RTreeHelper::redistributeEntries(entries, out_siblings);

    for(std::list<Node*>::iterator it = out_siblings.begin(); it!=out_siblings.end(); ++it)
    {
        assert((*it)->isLeaf()==entry->isLeafEntry());
    }

    return newNode;
}

Node *RTreeHelper::adjustTree(Node* root,Node *N, Node *NN, std::list<Node*>siblings)
{
    //Node that is created if the parent needs to be split
    Node* PP = NULL;
    //Flag determining if a new node was created
    std::set<Node*> S;
    S.insert(siblings.begin(), siblings.end());
    //Set of parent nodes of the sibling nodes
    std::set<Node*> P;

    std::list<Node*> newSiblings;
    //The new root
    Node* newRoot = root;

    bool ok = true;

    while (ok)
    {
        //The parent of the node being updated
        Node* Np = N->getParent();

        if (Np == NULL)
        {
            //We are at the root and we must exit the loop
            ok = false;
            if (NN != NULL)
            {
                N->adjustLHV();
                N->adjustMBR();

                NN->adjustLHV();
                NN->adjustMBR();

                shared_ptr<NonLeafEntry> n(new NonLeafEntry(N));
                shared_ptr<NonLeafEntry> nn(new NonLeafEntry(NN));

                newRoot = new Node(MAX_NODE_ENTRIES);
                newRoot->insertNonLeafEntry(n);
                newRoot->insertNonLeafEntry(nn);
            }
        }
        else
        {
            if (NN != NULL)
            {
                NN->adjustLHV();
                NN->adjustMBR();

                //Insert the entry containing the newly created node into the tree
                shared_ptr<NonLeafEntry> nn(new NonLeafEntry(NN));
                //try to insert the new node into the parent
                if (!Np->isOverflowing())
                {
                    Np->insertNonLeafEntry(nn);
                }
                else
                {
                    newSiblings.clear();
                    PP = RTreeHelper::handleOverflow(Np, nn, newSiblings);
                }
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


    newRoot->adjustLHV();
    newRoot->adjustMBR();

    return newRoot;
}

void RTreeHelper::debug(Node *root)
{
    if(root->getParent()==NULL)
    {
        std::ofstream of("graph.txt");

        of<<"digraph g { \n forcelabels=true;\n";
        of<<RTreeHelper::listNodes(root, of);
        of<<RTreeHelper::listNodeLinks(root, of);
        of<<"}";

        of.close();
    }
}

std::string RTreeHelper::listNodeLinks(Node* node, std::ofstream& ofs)
{
    std::stringstream ss;

    // ss<<(boost::uint64_t)node << " -> "<<(boost::uint64_t)node->getNextSibling()<<";\n";
    //ss<<(boost::uint64_t)node->getPrevSibling()<< " -> "<<(boost::uint64_t)node <<";\n";

    if(node->getMBR()->getLower()[0]==0 && node->getMBR()->getUpper()[0]==49)
    {
        int a=2;
    }

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

