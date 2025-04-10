/* Pack.cpp
 * Represents a pack of playing cards
 *
 * Andrew DeOrio <awdeorio@umich.edu>
 * 2014-12-21
 */


#include "Pack.hpp"
#include "Card.hpp"
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


static const int PACK_SHUFFLE_NUM_RIFFLE = 7;

Pack::Pack()
  : next(0) {
  int num_cards = 0;
  for (int s = SPADES; s <= DIAMONDS; ++s) {
    for (int r = s==SPADES || s==HEARTS? SEVEN:EIGHT; r <= ACE; ++r) {
      cards[num_cards++] = Card(static_cast<Rank>(r), static_cast<Suit>(s));
    }
  }
}

// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
Pack::Pack(std::istream& pack_input) : next(0) {
  Card card;
  while(pack_input >> card) {
    cards[next++] = card;
  };
  next = 0;
}

Card Pack::deal_one() {
  assert(next < PACK_SIZE);
  return cards[next++];
}

void Pack::reset() {
  // reset next pointer to first Card
  next = 0;
}

void Pack::shuffle() {
  for (int i=0; i<PACK_SHUFFLE_NUM_RIFFLE; ++i) {

    // copy pack to tmp variable
    array<Card, PACK_SIZE> tmp = cards;

    // do in shuffle
    int src = 0;
    for (int offset = 1; offset >= 0; --offset) {
      for (int count = 0; count < PACK_SIZE / 2; ++count) {
        cards[offset + 2*count] = tmp[src++];
      }
    }
  }

  reset();
}

bool Pack::empty() const {
  return next >= PACK_SIZE;
}
