/*
 * HilbertValue.cc
 *
 *  Created on: Jan 7, 2015
 *      Author: rasdaman
 */
#include <stdexcept>
#include <numeric>

#include "../lib/easylogging/easylogging++.hh"

#include "HilbertValue.hh"

HilbertValue::HilbertValue(const std::vector<boost::uint64_t> &coordinates,
                           const std::vector<boost::int32_t> &ms) {
  this->init(coordinates, ms);
}

HilbertValue::HilbertValue(const std::vector<boost::uint64_t> &coordinates) {

  std::vector<boost::int32_t> ms(coordinates.size(),
                                 sizeof(boost::uint64_t) * 8);
  this->init(coordinates, ms);
}

HilbertValue::~HilbertValue() { delete this->hilbertIndex; }

void HilbertValue::init(const std::vector<boost::uint64_t> &coordinates,
                        const std::vector<boost::int32_t> &ms) {
  if (coordinates.size() != ms.size()) {
    throw std::runtime_error("The coordinates vector and the precision vector "
                             "must have the same size.");
  }

  // The precision of the compact hilbert code
  uint32_t M = std::accumulate(ms.begin(), ms.end(), 0);
  int n = coordinates.size();

  this->hilbertIndex = new CBigBitVec(M);
  CBigBitVec coords[n];

  for (int i = 0; i < coordinates.size(); ++i) {
    coords[i].setSize(ms[i]);
    coords[i] = coordinates[i];
  }

  Hilbert::coordsToCompactIndex(coords, &ms[0], n, *this->hilbertIndex);
}

bool operator<(const HilbertValue &left, const HilbertValue &right) {
  bool result = false;
  int lmsb = left.hilbertIndex->msb();
  int rmsb = right.hilbertIndex->msb();

  if (lmsb < rmsb) {
    result = true;
  } else if (lmsb == rmsb) {
    for (int i = lmsb; i >= 1; i--) {
      bool lbit = left.hilbertIndex->getBit(i);
      bool rbit = right.hilbertIndex->getBit(i);
      if (lbit != rbit) {
        if (lbit == false) {
          result = true;
        } else {
          result = false;
        }
        break;
      }
    }
  } else {
    result = false;
  }
  return result;
}

bool operator<=(const HilbertValue &left, const HilbertValue &right) {
  return left < right || left == right;
}

bool operator>(const HilbertValue &left, const HilbertValue &right) {
  bool result = false;
  int lmsb = left.hilbertIndex->msb();
  int rmsb = right.hilbertIndex->msb();

  if (lmsb > rmsb) {
    result = true;
  } else if (lmsb == rmsb) {
    for (int i = lmsb; i >= 1; i--) {
      bool lbit = left.hilbertIndex->getBit(i);
      bool rbit = right.hilbertIndex->getBit(i);

      if (lbit != rbit) {
        if (lbit == true) {
          result = true;
        } else {
          result = false;
        }
        break;
      }
    }
  } else {
    result = false;
  }

  return result;
}

bool operator>=(const HilbertValue &left, const HilbertValue &right) {
  return left > right || left == right;
}

bool operator==(const HilbertValue &left, const HilbertValue &right) {
  bool result = true;

  if (left.hilbertIndex->getSize() != right.hilbertIndex->getSize()) {
    result = false;
  } else {
    for (int i = 0; i < left.hilbertIndex->rackCount(); i++) {
      if (left.hilbertIndex->racks()[i] != right.hilbertIndex->racks()[i]) {
        result = false;
        break;
      }
    }
  }

  return result;
}

bool operator!=(const HilbertValue &left, const HilbertValue &right) {
  return !(left == right);
}
