#ifndef HRTREE_SRC_RTREEHELPER_HH
#define HRTREE_SRC_RTREEHELPER_HH

#include <list>
#include <boost/shared_ptr.hpp>

#include "Node.hh"
#include "HilbertValue.hh"
#include "Common.hh"

class RTreeHelper
{
public:
    /**
     * @brief chooseLeaf Choose a leaf for the hilbert value starting from the node N
     * @param N Node to start the search from
     * @param h Hilbert value of the item we want to insert
     * @return Pointer to a leaf node in which the given hilbert value can be inserted.
     */
    static Node* chooseLeaf(Node* N, const boost::shared_ptr<HilbertValue>& h);

    /**
     * @brief search
     * @param subtreeRoot
     * @param rect
     * @return
     */
    static std::list<boost::shared_ptr<NodeEntry> > search(Node* subtreeRoot, const boost::shared_ptr<Rectangle>& rect);

    /**
     * @brief redistributeEntries Redistribute the given set of NodeEntries which are assumed to be sorted
     * according to their hilbert value over the given list of nodes which are assumed to be in the correct
     * sibling order i.e. the next node in the list is also the next sibling of the previous node in the list.
     * @param entries
     * @param siblings
     */
    static void redistributeEntries(EntryMultiSet& entries, std::list<Node*> siblings);

    /**
     * @brief handleOverflow
     * @param target
     * @param entry
     * @param out_siblings Will contain the target node, its siblings, and the new node if one was created
     * @return
     */
    static Node* handleOverflow(Node* target, const boost::shared_ptr<NodeEntry>& entry, std::list<Node*>& out_siblings);

    /**
     * @brief adjustTree Adjust the tree following an insertion or a deletion.
     * @param root The current root of the tree.
     * @param N The node that has been directly modified.
     * @param NN The node that was created if there was no room for insertion in N.
     * This is NULL if no node was created.
     * @param overflowed True if N reached capacity.
     * @return The new root of the tree.
     */
    static Node* adjustTree(Node* root, Node*N, Node* NN, bool overflowed,std::list<Node*>siblings);

    static void debug(Node* root);

    static std::string listNodeLinks(Node* node, std::ofstream& ofs);
    static std::string listNodes(Node* node, std::ofstream& ofs);

};

#endif // HRTREE_SRC_RTREEHELPER_HH
