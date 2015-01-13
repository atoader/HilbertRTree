/*
 * RTree.hh
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#ifndef RTREE_HH_
#define RTREE_HH_

#include <set>

#include "Node.hh"
#include "Rectangle.hh"
#include "HilbertValue.hh"

class RTree
{
public:
    /**
     * @brief RTree Initialize a new instance of the RTree class
     */
    RTree();

    virtual ~RTree();

    /**
     * @brief search Search the R-tree for entries that are intersected by the
     * given rectangle.
     * This is just a sample implementation of the search method that only retrieves the
     * list of nodes entries instead of a list of data items.
     * In a real implementation, this method would return a list of data objects
     * @param query
     * @return A list of NodeEntry items that contain rectangles that are intersected
     * by the query rectangle
     */
    std::list<boost::shared_ptr<NodeEntry> >  search(const boost::shared_ptr<Rectangle>& query);

    /**
     * @brief insert Insert a new data item with the given bounding box into the tree.
     * In this implementation, only the rectangle is inserted, but in a real implementation,
     * additional data (pointer to disk page etc) should be inserted.
     * @param rect The bounding box to insert
     */
    void insert(const boost::shared_ptr<Rectangle>& rect);

    /**
     * @brief remove Remove the entry containing the rectangle from the tree.
     * @param rect
     */
    void remove(const boost::shared_ptr<Rectangle>& rect);

    /**
     * @brief getRoot
     * @return Pointer to the Node that is the root of the tree.
     */
    Node *getRoot() const;

private:
    //Root of this RTree
    Node* root;
};

#endif /* RTREE_HH_ */
