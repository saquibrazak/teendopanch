/* Player.cpp
 * Euchre player implementation
 *
 * Andrew DeOrio <awdeorio@umich.edu>
 * 2014-12-21
 */


#include "Player.hpp"
#include "Card.hpp"
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


class Simple : public Player {
 public:
  //EFFECTS initializes Player with no name
  Simple();

  //EFFECTS initializes Player with name and an empty hand.
  Simple(const std::string &name_in);

  //EFFECTS returns player's name
  virtual const std::string & get_name() const;

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  virtual void add_card(const Card &c);

  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.  See the spec for strategy.
  virtual bool make_trump(int round, Suit &order_up_suit) const;

  //REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  //  See spec for determining which.
  virtual Card add_and_discard(Card card_to_add, Suit upcard);

  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to the strategy in the
  //  project spec.  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  virtual Card lead_card(Suit trump);

  virtual Card remove_card();

  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to the strategy in the
  //  project spec.  The card is removed from the player's hand.
  virtual Card play_card(const Card &led_card, Suit trump);

 private:
  const std::string name; // This Player's name
  std::vector<Card> hand; // Cards in this Player's hand
  int count_cards(Suit s) const; // count how many cards do I have of this suit
  
};


////////////////////////////////////////////////////////////////////////////////
// helper functions
static Card remove(vector<Card> &hand, int i) {
  Card c = hand[i];
  hand.erase(hand.begin() + i);
  return c;
}

static int num_face_cards(const vector<Card> &hand, Suit suit) {
  int count=0;
  for (int i=0; i < int(hand.size()); ++i)
    if ((hand[i].get_suit() == suit) && hand[i].is_face_or_ace())
      ++count;
  return count;
}


////////////////////////////////////////////////////////////////////////////////
Simple::Simple() {}

Simple::Simple(const std::string &name_in)
  : name(name_in) {}

const std::string & Simple::get_name() const {
  return name;
}

void Simple::add_card(const Card &c) {
  assert(hand.size() < Player::MAX_HAND_SIZE);
  hand.push_back(c);
}

bool Simple::make_trump(int round, Suit &order_up_suit) const {
  assert(round == 1 || round == 2);

  // round 1 strategy: if you have two or more trump face cards, then order up
  if (round == 1) {
    // count how many cards I have for each suit
    vector <int> num_cards(4);
    num_cards[SPADES] = count_cards(SPADES);  // count spades 
    num_cards[HEARTS] = count_cards(HEARTS);  // count hearts
    num_cards[CLUBS] = count_cards(CLUBS);    // count clubs
    num_cards[DIAMONDS] = count_cards(DIAMONDS); // count diamonds
    
    // Make sure that at least one of the cards from the max suit is a face card
    Suit max_suit = static_cast<Suit>(std::distance(num_cards.begin(), std::max_element(num_cards.begin(), num_cards.end())));
    if (num_face_cards(hand, max_suit) >= 1)
    {
      order_up_suit = max_suit;
      return true;
    }    
  }

  // round 2 strategy: you are required to pick the suit of card number 7
  if (round == 2) {
      order_up_suit = hand[7].get_suit();
      return true;
  }

  

  // if you didn't order up, then pass
  return false;
}
Card Simple::remove_card()
{
  assert(hand.size() > 0);
  return remove(hand, hand.size()-1); // generate a random number between 0, hand.size()-1
  //return remove(hand, rand() % int(hand.size())); // generate a random number between 0, hand.size()-1
}   

Card Simple::add_and_discard(Card card_to_add, Suit upcard) {
  assert(hand.size() > 0);

  Suit trump = upcard;

  int min = 0;
  for (int i = 1; i < int(hand.size()); ++i)
      if (Card_less(hand[i], hand[min], trump))
        min = i;
  // pick a card such that
  // the player has at least 3 cards of the same suit
  // Find all the suits where we have at least 3 cards of this suit
  // and keep track of the lowest card of that suit
  // discard the lowest card of this list
  vector<Card> discard_options;
  for (Suit s = SPADES; s <= DIAMONDS; s = static_cast<Suit>(s + 1))
    if (count_cards(s) >= 3)
    {
      // find the lowest card with this suit
      int min = 0;
      for (int i = 1; i < int(hand.size()); ++i)
        if (hand[i].get_suit() == s)
          if (Card_less(hand[i], hand[min], trump))
            min = i;
      discard_options.push_back(hand[min]);
    }
  // now find the lowest card in the vector discard_options
  min = 0;
  for (int i = 1; i < int(discard_options.size()); ++i)
    if (Card_less(discard_options[i], discard_options[min], trump))
      min = i;
  // now discard the lowest card in the vector discard_options
  int min_from_hand = std::find(hand.begin(), hand.end(), discard_options[min]) - hand.begin();
  // discard the card
  remove(hand, min_from_hand); // discard the lowest card from hand
  // keep the upcard
  add_card(card_to_add); // keep upcard
  // return the removed card
  return discard_options[min];
}

Card Simple::lead_card(Suit trump) {
  assert(hand.size() > 0);

  int max=-1; // highest card, including trump
  int max_non_trump=-1; // highest card excluding trump
  for (int i = 0; i < int(hand.size()); ++i) {

    if(hand[i].is_trump(trump) ){
      // Card i is trump
      if ((max < 0) || !Card_less(hand[i], hand[max], trump))
        max = i;
    } else {
      // Card i is not trump
      if ((max_non_trump < 0) || !Card_less(hand[i], hand[max_non_trump], trump))
        max_non_trump = i;
    }

  }//for

  if (max_non_trump >= 0)
    return remove(hand, max_non_trump);
  else
    return remove(hand, max);
}


Card Simple::play_card(const Card &led_card, Suit trump) {
  assert(hand.size() > 0);

  int max=-1; // highest card that follows suit
  int min=-1; // lowest card that doesn't follow suit
  for (int i = 0; i < int(hand.size()); ++i) {

    if (hand[i].get_suit() == led_card.get_suit()) {
      // Card i matches led suit, update the max card
      if ((max < 0) || !Card_less(hand[i], hand[max], trump))
        max = i;

    } else {
      // Card i does not match led suit, update the min card
      if ((min < 0) || Card_less(hand[i], hand[min], trump))
        min = i;
    }

  }//for

  // if we found a high card that follows suit, then play it.  Otherwise,
  // play the low card
  if (max >= 0)
    return remove(hand, max);
  else
    return remove(hand, min);
}

int Simple::count_cards(Suit s) const { // implementation of count_cards
  return std::count_if(hand.begin(), hand.end(), [s](const Card &c) {
    return c.get_suit() == s;
  });
}

////////////////////////////////////////////////////////////////////////////////

std::ostream & operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}


////////////////////////////////////////////////////////////////////////////////
// Human player

class Human : public Player {
 public:
  //EFFECTS initializes Player with no name
  Human() {}

  //EFFECTS initializes Player with name and an empty hand.
  Human(const std::string &name_in) : name(name_in) {}

  //EFFECTS returns player's name
  virtual const std::string & get_name() const { return name; }

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  virtual void add_card(const Card &c) {
    assert(hand.size() < Player::MAX_HAND_SIZE);
    hand.push_back(c);
    std::sort(hand.begin(), hand.end());
  }

private:
  void print_hand() const {
    for (int i=0; i < int(hand.size()); ++i)
      cout << "Human player " << name << "'s hand: "
           << "[" << i << "] " << hand[i] << "\n";
  }

public:

  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  virtual bool make_trump(int round, Suit &order_up_suit) const {

    print_hand();

    cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
    string input;
    if (!(cin >> input)) {
      //FIXME: how do deal with bad input?
      cerr << "Bad input" << endl;
      exit(1);
    }
    //FIXME: this is a little ugly.  Should have an overloaded input operator
    //  for Suit that throws an exception.
    if (input == "pass") {
      return false;
    }

    order_up_suit = string_to_suit(input);
    return true;
  }

private:
  int ask_for_card() {
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";
    int i;
    if (!(cin >> i)) {
      cerr << "Bad input" << endl;
      exit(EXIT_FAILURE);
    }
    assert(i >= 0);
    assert(i < int(hand.size()));
    return i;
  }


public:
  //REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  //  See spec for determining which.
  virtual Card add_and_discard(Card card_to_add,  Suit upcard) {
    print_hand();
    cout << "Discard upcard: [-1]" << endl;
    cout << "Human player " << name << ", please select a card to discard:" << endl;
    int i;
    if (!(cin >> i)) {
      cerr << "Bad input" << endl;
      exit(EXIT_FAILURE);
    }

    if (i == -1) return Card(); // return a default Card if -1 is selected
    Card c = remove(hand, i); //discard from hand
    add_card(card_to_add); //keep upcard
    return c;
  }

  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to the strategy in the
  //  project spec.  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  virtual Card lead_card(Suit) {
    int i = ask_for_card();
    return remove(hand, i);
  }

  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to the strategy in the
  //  project spec.  The card is removed from the player's hand.
  virtual Card play_card(const Card &, Suit trump) {
    int i = ask_for_card();
    return remove(hand, i);
  }

  virtual Card remove_card() {
    assert(hand.size() > 0);
    return remove(hand, rand() % int(hand.size())); // generate a random number between 0, hand.size()-1
  }

 private:
  const std::string name; // This Player's name
  std::vector<Card> hand; // Cards in this Player's hand

};


////////////////////////////////////////////////////////////////////////////////

Player * Player_factory(const std::string &name, const std::string &strategy) {
  if (strategy == "Simple") {
    return new Simple(name);
  } else if (strategy == "Human") {
    return new Human(name);
  }
  cerr << "Unrecognized strategy: " << strategy << "" << endl;
  assert(false);
  return nullptr;
}
