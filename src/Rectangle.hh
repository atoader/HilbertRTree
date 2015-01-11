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


class Rectangle
{
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
     * @return TRUE if the rectangles intersect, FALSE otherwise
     */
    bool intersects(const Rectangle& rect);

    /**
     * @brief contains Check if the current rectangle contains the second rectangle
     * @param rect
     * @return TRUE if the current rectangle contains the second rectangle, FALSE otherwise
     */
    bool contains(const Rectangle& rect);

    /**
     * @brief operator == Check if the two rectangles have the same coordinates
     * @param other
     * @return TRUE if this rectangle and the other rectangle have the same
     * lower and upper corners, FALSE otherwise
     */
    bool operator==(const Rectangle& other) const;

private:
    std::vector<boost::uint64_t> lower;/*!<Coordinates of the lower corner of the rectangle */
    std::vector<boost::uint64_t> upper;/*!<Coordinates of the upper corner of the rectangle */
};

#endif /* HRTREE_SRC_RECTANGLE_HH_ */
