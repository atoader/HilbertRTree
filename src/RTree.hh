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
     * @brief search Search the R-tree for a entries that are intersected by the
     * given rectangle.
     * @param rect
     * @return A list of NodeEntry items that contain rectangles that are intersected
     * by the query rectangle
     */
    std::list<boost::shared_ptr<NodeEntry> >  search(const boost::shared_ptr<Rectangle>& rect);

    /**
     * @brief insert Insert a new data item with the given bounding box into the tree.
     * @param rect The bounding box to insert
     * @param data The data item to insert. The RTree becomes the owner of this item.
     */
    void insert(const boost::shared_ptr<Rectangle>& rect, char* data);

    Node *getRoot() const;

private:
    //Root of this RTree
    Node* root;

//  Node* handleLeafOverflow(Node* leaf, const Rectangle& rect,
//            const HilbertValue& h, char* data);

//  Node* handleNonLeafOverflow(Node*target, Node* toInsert);

//  void adjustTree(Node* N, Node*NN, bool overflowed);

};

#endif /* RTREE_HH_ */
