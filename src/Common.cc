/*
 * Common.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include "Common.hh"


bool Common::nodeEntryComparisonObj::operator()(const boost::shared_ptr<NodeEntry> &first,const boost::shared_ptr<NodeEntry> &second)  const
{
    boost::shared_ptr<HilbertValue> l = first->getLHV();
    boost::shared_ptr<HilbertValue> r = second->getLHV();
    return *l < *r;
}
