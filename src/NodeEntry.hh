/*
 * NodeEntry.hh
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#ifndef HRTREE_SRC_NODEENTRY_HH_
#define HRTREE_SRC_NODEENTRY_HH_

#include <boost/shared_ptr.hpp>

#include "HilbertValue.hh"
#include "Rectangle.hh"

// Forward declaration to prevent compile-time error
struct Node;

struct NodeEntry {
  /**
   * Initialize a new instance of the NodeEntry class
   * @param lhv The Largest hilbert value of this entry
   * (or of the node to which this entry is pointing)
   * @param mbr The minimum bounding rectangle
   * @param ptr Pointer to a node of the tree
   * (If this entry is a non-leaf entry).
   * @param data Pointer to the data this entry will store
   * (If this entry is a leaf entry). The created object becomes the owner of
   * the data
   */
  NodeEntry(boost::shared_ptr<HilbertValue> lhv,
            boost::shared_ptr<Rectangle> mbr, Node *own_ptr, char *own_data);

  ~NodeEntry();

  /**
   * Largest hilbert value of corresponding to the MBR if this is a leaf node,
   * or the largest hilbert
   * value among the entries of the node to which ptr is pointing.
   */
  boost::shared_ptr<HilbertValue> lhv;

  /**
   * Minimum bounding rectangle
   */
  boost::shared_ptr<Rectangle> mbr;

  /**
   *	Pointer to the node if this is a non-leaf entry, otherwise NULL.
   */
  Node *ptr;

  /**
   * Pointer to the data this entry is holding, if this is a leaf node,
   * otherwise NULL.
   */
  char *data;
};

#endif /* HRTREE_SRC_NODEENTRY_HH_ */
