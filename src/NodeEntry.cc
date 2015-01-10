/*
 * NodeEntry.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */

#include <stdexcept>

#include "NodeEntry.hh"
#include "Node.hh"

NodeEntry::NodeEntry(boost::shared_ptr<HilbertValue> lhv,
                     boost::shared_ptr<Rectangle> mbr, Node *own_ptr,
                     char *own_data)
    : lhv(lhv), mbr(mbr), ptr(own_ptr), data(own_data)
{

    if (!this->lhv || !this->mbr)
    {
        throw std::runtime_error("The lhv and the mbr must be non-null");
    }
}

NodeEntry::~NodeEntry()
{
//    if(this->ptr !=NULL)
//    {
//        delete this->ptr;
//        this->ptr = NULL;
//    }

    if(this->data != NULL)
    {
        delete this->data;
        this->data = NULL;
    }
}
