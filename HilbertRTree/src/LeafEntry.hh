#ifndef HRTREE_SRC_LEAFENTRY_HH
#define HRTREE_SRC_LEAFENTRY_HH

#include "NodeEntry.hh"

class LeafEntry : public NodeEntry
{
public:
    LeafEntry(boost::shared_ptr<Rectangle> mbr, boost::shared_ptr<HilbertValue> lhv);

    virtual ~LeafEntry();

    /**
     * @brief getLHV
     * @return the Hilbert value of the current entry
     */
    virtual boost::shared_ptr<HilbertValue> getLHV();

    /**
     * @brief getMBR
     * @return the Minimum bounding rectangle of the current entry
     */
    virtual boost::shared_ptr<Rectangle> getMBR();

    /**
     * @brief isLeafEntry
     * @return FALSE
     */
    virtual bool isLeafEntry();

private:
    boost::shared_ptr<Rectangle> mbr;
    boost::shared_ptr<HilbertValue> lhv;
};

#endif // HRTREE_SRC_LEAFENTRY_HH
