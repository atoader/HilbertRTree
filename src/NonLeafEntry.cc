#include "NonLeafEntry.hh"

#include "Node.hh"

NonLeafEntry::NonLeafEntry(Node* node)
{
    this->node=node;
}

NonLeafEntry::~NonLeafEntry()
{
    if(this->node!=NULL)
    {
        delete this->node;
    }
}

boost::shared_ptr<HilbertValue> NonLeafEntry::getLHV()
{
    assert(!!(this->node->getLHV()));
    return this->node->getLHV();
}

boost::shared_ptr<Rectangle> NonLeafEntry::getMBR()
{
    assert(!!(this->node->getLHV()));
    return this->node->getMBR();
}

bool NonLeafEntry::isLeafEntry()
{
    return false;
}

Node *NonLeafEntry::getNode() const
{
    return node;
}


