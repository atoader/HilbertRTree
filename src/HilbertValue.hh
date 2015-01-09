/*
 * HilbertValue.hh
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#ifndef HRTREE_SRC_HILBERTVALUE_HH_
#define HRTREE_SRC_HILBERTVALUE_HH_

#include <vector>

#include <boost/cstdint.hpp>

#include <Hilbert.hpp>
#include <Hilbert/BigBitVec.hpp>

class HilbertValue {
public:
    /**
     * @brief HilbertValue Compute the Hilbert value from the coordinates
     * @param coordinates Coordinates from which we want to compute the hilbert value
     * @param ms Array of precision values, one per dimension
     */
    HilbertValue(const std::vector<boost::uint64_t>& coordinates, const std::vector<boost::int32_t>& ms);

    /**
     * @brief HilbertValue Compute the Hilbert value from the coordinates
     * @param coordinates Coordinates from which we want to compute the hilbert value.
     */
    HilbertValue(const std::vector<boost::uint64_t>& coordinates);

	virtual ~HilbertValue();

	friend bool operator <(const HilbertValue& left, const HilbertValue& right);
	friend bool operator <=(const HilbertValue& left,
			const HilbertValue& right);

	friend bool operator >(const HilbertValue& left, const HilbertValue& right);
	friend bool operator >=(const HilbertValue& left,
			const HilbertValue& right);

	friend bool operator ==(const HilbertValue& left,
			const HilbertValue& right);
	friend bool operator !=(const HilbertValue& left,
			const HilbertValue& right);

private:
	CBigBitVec* hilbertIndex;

    void init(const std::vector<boost::uint64_t>& coordinates, const std::vector<boost::int32_t>& ms);
};

#endif /* HRTREE_SRC_HILBERTVALUE_HH_ */
