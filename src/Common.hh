/*
 * Common.hh
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#ifndef HRTREE_SRC_COMMON_HH_
#define HRTREE_SRC_COMMON_HH_

#include <set>

#include <boost/shared_ptr.hpp>

#include "NodeEntry.hh"

class Common
{
public:
    struct nodeEntryComparisonObj
    {
        bool operator() (const boost::shared_ptr<NodeEntry> &first,
                         const boost::shared_ptr<NodeEntry> &second) const;
    };
};

typedef std::multiset<boost::shared_ptr<NodeEntry>, Common::nodeEntryComparisonObj> EntryMultiSet;

#endif /* HRTREE_SRC_COMMON_HH_ */
