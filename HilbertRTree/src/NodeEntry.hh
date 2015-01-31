/*
 * NodeEntry.hh
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#ifndef HRTREE_SRC_NODEENTRY_HH_
#define HRTREE_SRC_NODEENTRY_HH_

#include <boost/shared_ptr.hpp>

#include "HilbertValue.hh"
#include "Rectangle.hh"

class NodeEntry
{
public:
    virtual ~NodeEntry();

    /**
     * @brief getLHV Get the largest hilbert value of this entry
     * @return
     */
    virtual boost::shared_ptr<HilbertValue> getLHV()=0;

    /**
     * @brief getMBR Get the minimum bounding rectangle of this entry
     * @return
     */
    virtual boost::shared_ptr<Rectangle> getMBR()=0;

    /**
     * @brief isLeafEntry Returns true if this is a leaf entry
     * @return
     */
    virtual bool isLeafEntry()=0;
};

#endif /* HRTREE_SRC_NODEENTRY_HH_ */
