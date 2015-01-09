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
    /**
     * @brief nodeEntryComparison Compare two node entries by using their hilbert
     * value and return true if the hilbert value of the first entry is smaller
     * than the hilbert value of the second entry, false othewise
     * @param first
     * @param second
     * @return
     */
    static bool nodeEntryComparison(const NodeEntry &first,
                                    const NodeEntry &second);

    struct nodeEntryComparisonObj
    {
        bool operator() (const boost::shared_ptr<NodeEntry> &first,
                         const boost::shared_ptr<NodeEntry> &second) const;
    };
};

typedef std::multiset<boost::shared_ptr<NodeEntry>, Common::nodeEntryComparisonObj> EntryMultiSet;

#endif /* HRTREE_SRC_COMMON_HH_ */
