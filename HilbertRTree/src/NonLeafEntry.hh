#ifndef HRTREE_SRC_NONLEAFENTRY_HH
#define HRTREE_SRC_NONLEAFENTRY_HH

#include "NodeEntry.hh"

class Node;

class NonLeafEntry : public NodeEntry
{
public:
    NonLeafEntry(Node* node);

    virtual ~NonLeafEntry();

    virtual boost::shared_ptr<HilbertValue> getLHV();

    virtual boost::shared_ptr<Rectangle> getMBR();

    virtual bool isLeafEntry();

    Node *getNode() const;

private:
    Node* node;
};

#endif // NONLEAFENTRY_HH
