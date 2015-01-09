/*
 * Common.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include "Common.hh"

bool Common::nodeEntryComparison(const NodeEntry &first,
                                 const NodeEntry &second)
{
    return (*first.lhv.get()) < (*second.lhv.get());
}

bool Common::nodeEntryComparisonObj::operator()(const boost::shared_ptr<NodeEntry> &first,const boost::shared_ptr<NodeEntry> &second) const
{
    return (*first->lhv.get()) < (*second->lhv.get());
}
