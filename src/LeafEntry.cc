#include "LeafEntry.hh"

LeafEntry::LeafEntry(boost::shared_ptr<Rectangle> mbr, boost::shared_ptr<HilbertValue> lhv)
{
    this->mbr=mbr;
    this->lhv=lhv;
}

LeafEntry::~LeafEntry()
{

}

boost::shared_ptr<HilbertValue> LeafEntry::getLHV()
{
    assert(!!(this->lhv));
    return this->lhv;
}

boost::shared_ptr<Rectangle> LeafEntry::getMBR()
{
    assert(!!(this->mbr));
    return this->mbr;
}

bool LeafEntry::isLeafEntry()
{
    return true;
}

