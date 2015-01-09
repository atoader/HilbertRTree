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
    //Distribute the entries evenly
    int batchSize = entries.size() / siblings.size() + 1;

    int currentBatchSize = 0;
    std::list<Node*>::iterator siblingIt = siblings.begin();
    EntryMultiSet::iterator entriesIt = entries.begin();
    EntryMultiSet::iterator entriesBeginCopy = entries.begin();

    while (entriesIt != entries.end() && siblingIt != siblings.end())
    {
        currentBatchSize++;
        entriesIt++;

        if (currentBatchSize == batchSize)
        {
            //Replace the current entries of the set with the new ones
            (*siblingIt)->getEntries().clear();
            (*siblingIt)->getEntries().insert(entriesBeginCopy, entriesIt);
            currentBatchSize = 0;
            siblingIt++;
            entriesBeginCopy = entriesIt;
        }
    }

    if(siblingIt!=siblings.end())
    {
        (*siblingIt)->getEntries().clear();
        (*siblingIt)->getEntries().insert(entriesBeginCopy, entriesIt);
    }
}

Node *RTreeHelper::handleOverflow(Node *target, const boost::shared_ptr<NodeEntry> &entry)
{
    //List of entries from the cooperating nodes
    EntryMultiSet entries;

    //Node that will be created if there is no room for redistribution
    Node* newNode=NULL;

    std::list<Node*> siblings = target->getSiblings(SIBLINGS_NO);

    entries.insert(entry);

    for(std::list<Node*>::iterator it = siblings.begin(); it!=siblings.end(); ++it)
    {
        entries.insert((*it)->getEntries().begin(), (*it)->getEntries().end());
    }

    //If there is not enough room, create e new node
    if (entries.size() > siblings.size() * MAX_NODE_ENTRIES)
    {
        //TODO:Recheck logic
        newNode = new Node(MAX_NODE_ENTRIES);
        Node* prevSib = siblings.front()->getPrevSibling();
        newNode->setPrevSibling( prevSib);

        if(prevSib!=NULL)
        {
            prevSib->setNextSibling(newNode);
        }
        newNode->setNextSibling(siblings.front());

        siblings.push_front(newNode);
    }

    RTreeHelper::redistributeEntries(entries, siblings);

    return newNode;
}

Node *RTreeHelper::adjustTree(Node* root,Node *N, Node *NN, bool overflowed)
{
    //The parent of the node being updated
    Node* Np = N->getParent();
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
        if (Np == NULL)
        {
            //We are at the root and we must exit the loop
            ok = false;
            if (NN != NULL)
            {
                shared_ptr<NodeEntry> n1(new NodeEntry(N->getLHV(), N->getMBR(), N, NULL));
                shared_ptr<NodeEntry> n2(new NodeEntry(NN->getLHV(), NN->getMBR(), NN, NULL));
                newRoot = new Node(MAX_NODE_ENTRIES);
                newRoot->insertNonLeafEntry(n1);
                newRoot->insertNonLeafEntry(n2);
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
                std::list<Node*> siblings =  N->getSiblings(SIBLINGS_NO);
                S.insert(siblings.begin(), siblings.end());
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
        }
    }

    return newRoot;
}


