/*
 * Common.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include "Common.hh"


bool Common::nodeEntryComparisonObj::operator()(const boost::shared_ptr<NodeEntry> &first,const boost::shared_ptr<NodeEntry> &second)  const
{
    HilbertValue& l =  (*first->getLHV().get());
    HilbertValue& r = (*second->getLHV().get());
    return l < r;
}
