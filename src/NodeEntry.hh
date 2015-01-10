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

    virtual boost::shared_ptr<HilbertValue> getLHV()=0;

    virtual boost::shared_ptr<Rectangle> getMBR()=0;

    virtual bool isLeafEntry()=0;
};

#endif /* HRTREE_SRC_NODEENTRY_HH_ */
