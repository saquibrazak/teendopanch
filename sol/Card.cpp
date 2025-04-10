/* Card.cpp
 * Represents a single playing card
 *
 * Andrew DeOrio <awdeorio@umich.edu>
 * 2014-12-21
 */

#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////

Card::Card() : rank(TWO), suit(SPADES) {}

Card::Card(Rank rank_in, Suit suit_in)
  : rank(rank_in), suit(suit_in) {}

std::ostream & operator<<(std::ostream &os, const Card &card) {
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

//EFFECTS Reads a Card from a stream in the format "Two of Spades"
std::istream & operator>>(std::istream &is, Card &card) {
  string ignore;
  is >> card.rank >> ignore >> card.suit;
  return is;
}

Rank Card::get_rank() const {
  return rank;
}

Suit Card::get_suit() const {
  return suit;
}

bool Card::is_face_or_ace() const {
  return rank >= JACK;
}


bool Card::is_trump(Suit trump) const {
  return suit == trump;
}

bool operator<(const Card &lhs, const Card &rhs) {
  // To simplify debugging and ensure that every correct solution will get the
  // same output, we've added an additional rule here.  In addition to being
  // ordered by rank, cards are also ordered by suit.

  return lhs.get_rank() != rhs.get_rank()
    ? lhs.get_rank() < rhs.get_rank()  // differing ranks, compare those
    : lhs.get_suit() < rhs.get_suit(); // same ranks, compare suits
}

bool operator==(const Card &lhs, const Card &rhs) {
  // To simplify debugging and ensure that every correct solution will get the
  // same output, we've added an additional rule here.  In addition to being
  // ordered by rank, cards are also ordered by suit.

  return
    lhs.get_rank() == rhs.get_rank() &&
    lhs.get_suit() == rhs.get_suit();
}

bool operator<=(const Card &lhs, const Card &rhs) {
  return lhs < rhs || lhs == rhs;
}

bool operator>(const Card &lhs, const Card &rhs) {
  return !(lhs <= rhs);
}

bool operator>=(const Card &lhs, const Card &rhs) {
  return !(lhs < rhs);
}

bool operator!=(const Card &lhs, const Card &rhs){
  return !(lhs == rhs);
}

bool Card_less(const Card &a, const Card &b, Suit trump) {
  // one trump
  if (a.is_trump(trump) && !b.is_trump(trump)) return false;
  if (!a.is_trump(trump) &&  b.is_trump(trump)) return true;

  // both same suit
  return a < b;
}

bool Card_less(const Card &a, const Card &b, const Card &led_card,
               Suit trump) {
  // one or more cards is trump
  if (a.is_trump(trump) || b.is_trump(trump)) return Card_less(a,b,trump);

  // no trump, but exactly one card matches suit that was led
  if (a.get_suit() == led_card.get_suit() &&
      b.get_suit() != led_card.get_suit()) return false;

  if (a.get_suit() != led_card.get_suit() &&
      b.get_suit() == led_card.get_suit()) return true;

  // both cards match suit that was led or both cards don't
  return Card_less(a, b, trump);
}
