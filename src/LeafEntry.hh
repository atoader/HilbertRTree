#ifndef HRTREE_SRC_LEAFENTRY_HH
#define HRTREE_SRC_LEAFENTRY_HH

#include "NodeEntry.hh"

class LeafEntry : public NodeEntry
{
public:
    LeafEntry(boost::shared_ptr<Rectangle> mbr, boost::shared_ptr<HilbertValue> lhv);

    virtual ~LeafEntry();

    virtual boost::shared_ptr<HilbertValue> getLHV();

    virtual boost::shared_ptr<Rectangle> getMBR();

    virtual bool isLeafEntry();

private:
    boost::shared_ptr<Rectangle> mbr;
    boost::shared_ptr<HilbertValue> lhv;
};

#endif // LEAFENTRY_HH
