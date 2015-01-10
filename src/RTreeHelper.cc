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
            HilbertValue& current = *(it->get()->lhv.get());
            if( current > searchedValue)
            {
                return RTreeHelper::chooseLeaf((*it)->ptr, h);
            }
        }

        //if h is larger than all the LHV already in the node,
        //choose the last of the node entries
        --it;

        return RTreeHelper::chooseLeaf( (*it)->ptr, h);
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
            if((*it)->mbr->intersects(*rect.get()))
            {
                result.push_back(*it);
            }
        }
    }
    else
    {
        for(it = entries.begin(); it!=entries.end(); ++it)
        {
            if((*it)->mbr->intersects(*rect.get()))
            {
                aux = RTreeHelper::search((*it)->ptr, rect);
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

    (*siblingsIt)->getEntries().clear();

    unsigned long currentBatch = 0;

    for(EntryMultiSet::iterator entryIt = entries.begin(); entryIt !=entries.end(); entryIt++)
    {
        if((*entryIt)->ptr==NULL)
        {
            (*siblingsIt)->insertLeafEntry((*entryIt));
        }
        else
        {
            (*siblingsIt)->insertNonLeafEntry((*entryIt));
        }

        currentBatch++;

        if(currentBatch ==  batchSize && (++siblingsIt)!=siblings.end())
        {
            (*siblingsIt)->getEntries().clear();
            currentBatch = 0;
        }
    }

    for(siblingsIt=siblings.begin(); siblingsIt!=siblings.end(); ++siblingsIt)
    {
        (*siblingsIt)->adjustLHV();
        (*siblingsIt)->adjustLHV();
    }
}

Node *RTreeHelper::handleOverflow(Node *target, const boost::shared_ptr<NodeEntry> &entry, bool leaf)
{
    //List of entries from the cooperating nodes
    EntryMultiSet entries;

    //Node that will be created if there is no room for redistribution
    Node* newNode=NULL;

    std::list<Node*> siblings = target->getSiblings(SIBLINGS_NO);

    entries.insert(entry);

    for(std::list<Node*>::iterator it = siblings.begin(); it!=siblings.end(); ++it)
    {
        assert((*it)->isLeaf()==leaf);
        entries.insert((*it)->getEntries().begin(), (*it)->getEntries().end());
    }

    //If there is not enough room, create e new node
    if (entries.size() > siblings.size() * MAX_NODE_ENTRIES)
    {
        //The new node is a sibling of the target.
        newNode = new Node(MAX_NODE_ENTRIES);

        newNode->setLeaf(leaf);

        Node* prevSib = siblings.front()->getPrevSibling();

        newNode->setPrevSibling(prevSib);
        if(prevSib!=NULL)
        {
            assert(prevSib->isLeaf()==newNode->isLeaf());
            prevSib->setNextSibling(newNode);
        }

        newNode->setNextSibling(siblings.front());
        siblings.front()->setPrevSibling(newNode);

        assert(siblings.front()->isLeaf()==newNode->isLeaf());
        siblings.push_front(newNode);
    }

    RTreeHelper::redistributeEntries(entries, siblings);

    for(std::list<Node*>::iterator it = siblings.begin(); it!=siblings.end(); ++it)
    {
        assert((*it)->isLeaf()==leaf);
    }

    return newNode;
}

Node *RTreeHelper::adjustTree(Node* root,Node *N, Node *NN, bool overflowed)
{
    //Node that is created if the parent needs to be split
    Node* PP = NULL;
    //Flag determining if a new node was created
    bool newOverflow = false;
    //Set of sibling nodes
    std::set<Node*> S;
    //Set of parent nodes of the sibling nodes
    std::set<Node*> P;
    //The new root
    Node* newRoot = root;

    bool ok = true;

    while (ok)
    {
        N->adjustLHV();
        N->adjustMBR();

        if(NN)
        {
            NN->adjustLHV();
            NN->adjustMBR();
        }

        //The parent of the node being updated
        Node* Np = N->getParent();

        if (Np == NULL)
        {
            //We are at the root and we must exit the loop
            ok = false;
            if (NN != NULL)
            {
                shared_ptr<NodeEntry> n(new NodeEntry(N->getLHV(), N->getMBR(), N, NULL));
                shared_ptr<NodeEntry> nn(new NodeEntry(NN->getLHV(), NN->getMBR(), NN, NULL));

                newRoot = new Node(MAX_NODE_ENTRIES);
                //This will not be a leaf
                newRoot->setLeaf(false);
                newRoot->insertNonLeafEntry(n);
                newRoot->insertNonLeafEntry(nn);

                newRoot->adjustLHV();
                newRoot->adjustMBR();
            }
        }
        else
        {
            if (NN != NULL)
            {
                //Insert the entry containing the newly created node into the tree
                shared_ptr<NodeEntry> nn(new NodeEntry(NN->getLHV(), NN->getMBR(), NN, NULL));
                //try to insert the new node into the parent
                if (!Np->isOverflowing())
                {
                    Np->insertNonLeafEntry(nn);
                }
                else
                {
                    newOverflow = true;
                    PP = RTreeHelper::handleOverflow(Np, nn);
                }
            }

            if (NN!=NULL)
            {
                S.insert(NN);
            }

            if(overflowed)
            {
                //The siblings only need to be updated if an overflow took place
                std::list<Node*> siblings =  N->getSiblings(SIBLINGS_NO);
                S.insert(siblings.begin(), siblings.end());
            }
            else
            {
                //If no overflow happened, we need to have at least the modified node
                S.insert(N);
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
            overflowed = newOverflow;
            S.clear();
            P.clear();
        }
    }

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

    for(EntryMultiSet::iterator it = node->getEntries().begin(); it!=node->getEntries().end(); ++it)
    {
        if((*it)->ptr!=NULL)
        {
            ss<<(boost::uint64_t)node << " -> "<<(boost::uint64_t)(*it)->ptr<<";\n";
            ss<<RTreeHelper::listNodeLinks((*it)->ptr, ofs);
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

    ss<<(boost::uint64_t)node<<" [label=\""<<node->getMBR()->getLower()[0]<<","<<node->getMBR()->getLower()[1]<<"\n"<<node->getMBR()->getUpper()[0]<<","<<node->getMBR()->getUpper()[1]<<"\"];\n";

    for(EntryMultiSet::iterator it = node->getEntries().begin(); it!=node->getEntries().end(); ++it)
    {
        if((*it)->ptr!=NULL)
        {
            ss<<RTreeHelper::listNodes((*it)->ptr, ofs);
        }
        else
        {
            ss<<(boost::uint64_t)(*it).get()<<" [label=\""<<(*it).get()->mbr->getLower()[0]<<","<<(*it).get()->mbr->getLower()[1]<<"\n"<<(*it).get()->mbr->getUpper()[0]<<","<<(*it).get()->mbr->getUpper()[1]<<"\"];\n";
        }
    }

    return ss.str();
}

