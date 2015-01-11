/*
 * Common.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include "Common.hh"


bool Common::nodeEntryComparisonObj::operator()(const boost::shared_ptr<NodeEntry> &first,const boost::shared_ptr<NodeEntry> &second)  const
{
    return *(first->getLHV()) < *(second->getLHV());
}
