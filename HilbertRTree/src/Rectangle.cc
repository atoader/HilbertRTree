/*
 * Rectangle.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include <stdexcept>
#include "Rectangle.hh"

Rectangle::Rectangle(std::vector<boost::uint64_t> lower, std::vector<boost::uint64_t>upper)
{
    if (lower.size() != upper.size())
    {
        throw std::runtime_error(
            "Lower and upper bounds must have the same dimension.");
    }

    this->lower = lower;
    this->upper = upper;
}

std::vector<boost::uint64_t>Rectangle::getCenter() const
{
    std::vector<boost::uint64_t> center;

    for (size_t i = 0; i < this->lower.size(); i++)
    {
        center.push_back((this->lower[i] + this->upper[i]) / 2);
    }

    return center;
}

const std::vector<boost::uint64_t>& Rectangle::getLower() const
{
    return lower;
}

const std::vector<boost::uint64_t>& Rectangle::getUpper() const
{
    return upper;
}

bool Rectangle::intersects(const Rectangle &rect)
{
    bool intersect = true;

    if(this->lower.size()!=rect.lower.size())
    {
        throw std::runtime_error("The two rectangles do not have the same dimension.");
    }

    for(size_t i=0; intersect && i<this->lower.size(); ++i)
    {
        intersect = (this->lower[i]<=rect.upper[i] && this->upper[i] >=rect.lower[i]);
    }

    return intersect;
}

bool Rectangle::contains(const Rectangle &rect)
{
    bool contains = true;

    if(this->lower.size()!=rect.lower.size())
    {
        throw std::runtime_error("The two rectangles do not have the same dimension.");
    }

    for(size_t i=0; i<this->lower.size() && contains == true; ++i)
    {
        contains = (this->lower[i]<=rect.lower[i] && rect.lower[i]<=this->upper[i]
                    && this->lower[i]<= rect.upper[i] && rect.upper[i]<=this->upper[i]);
    }

    return contains;
}

bool Rectangle::operator==(const Rectangle &other) const
{
    bool equal = true;

    if(this->lower.size()!= other.lower.size())
    {
        throw std::runtime_error("The two rectangles do not have the same dimension.");
    }

    for(size_t i=0; equal && i<this->lower.size(); ++i)
    {
        equal = (this->lower[i]==other.lower[i] && this->upper[i]==other.upper[i]);
    }

    return equal;
}
