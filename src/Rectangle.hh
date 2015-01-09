/*
 * Rectangle.hh
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#ifndef HRTREE_SRC_RECTANGLE_HH_
#define HRTREE_SRC_RECTANGLE_HH_

#include <vector>

#include <boost/cstdint.hpp>


class Rectangle {
public:
    /**
     * @brief Rectangle Initialize a new instance of a n-dimensional rectangle using the provided
     * lower and upper bounds of the rectange.
     * The lower and upper vectors must have the same length.
     * @param lower
     * @param upper
     */
    Rectangle(std::vector<boost::uint64_t> lower, std::vector<boost::uint64_t> upper);

    /**
     * @brief getCenter Get the coordinates of the center of the rectangle
     * @return
     */
    std::vector<boost::uint64_t> getCenter() const;

    /**
     * @brief getLower Get the coordinates of the lower corner of the rectangle
     * @return
     */
    const std::vector<boost::uint64_t>& getLower() const;

    /**
     * @brief getLower Get the coordinates of the upper corner of the rectangle
     * @return
     */
    const std::vector<boost::uint64_t>& getUpper() const;

    /**
     * @brief intersects Check if the current rectangle and the one passed as
     * parameter intersect.
     * @param rect
     * @return
     */
    bool intersects(const Rectangle& rect);
private:
    std::vector<boost::uint64_t> lower;
    std::vector<boost::uint64_t> upper;
};

#endif /* HRTREE_SRC_RECTANGLE_HH_ */
