/* Player_tests.cpp */

#include "Player.hpp"
#include "Card.hpp"

#include "unit_test_framework.hpp"

#include <array>
#include <vector>
#include <map>
#include <memory>

using namespace std;


namespace
{

void deal_cards(unique_ptr<Player> &player, const std::vector<Card>& cards);
bool player_has_card(unique_ptr<Player> &player, Card card);

}

// PLAYER TEST 01/02

TEST(test_player_construction)
{
    const char* const simple_name = "Steve";
    unique_ptr<Player> simple(Player_factory(simple_name, "Simple"));
    ASSERT_EQUAL(simple_name, simple->get_name())

    const char* const human_name = "Stove";
    unique_ptr<Player> human(Player_factory(human_name, "Human"));
    ASSERT_EQUAL(human_name, human->get_name())
}

TEST(test_player_add_card)
{
    unique_ptr<Player> simple(Player_factory("Stave", "Simple"));

    array<Card, Player::MAX_HAND_SIZE> cards = {{
        Card(JACK, DIAMONDS),
        Card(ACE, CLUBS),
        Card(JACK, HEARTS),
        Card(NINE, DIAMONDS),
        Card(TEN, DIAMONDS),
    }};

    for(const auto& card : cards)
    {
        simple->add_card(card);
    }

    for(const auto& card: cards)
    {
        ASSERT_TRUE(player_has_card(simple, card));
    }
}

//------------------------------------------------------------------------------
//----------------- Everything below this line needs updating ------------------
//------------------------------------------------------------------------------

// PLAYER TEST 03 START

const char* const PLAYER_NAME = "Waaaaaluigi";

TEST(test_no_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(KING, HEARTS));
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(NINE, HEARTS));
  cards.push_back(Card(TEN, HEARTS));

  deal_cards(simple, cards);

  Card upcard(TWO, SPADES);

  // ROUND 1
  upcard = Card(JACK, SPADES);
  Suit response_trump = SPADES;
  bool response = simple->make_trump( 1, response_trump);
  ASSERT_FALSE(response);
  ASSERT_EQUAL(response_trump, SPADES);

  // ROUND 2
  upcard = Card(JACK, HEARTS);
  response = simple->make_trump(2, response_trump);
  ASSERT_FALSE(response);
  ASSERT_EQUAL(response_trump, SPADES);
}

TEST(test_1_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(NINE, SPADES));
  cards.push_back(Card(TEN, SPADES));
  cards.push_back(Card(JACK, HEARTS));
  cards.push_back(Card(QUEEN, SPADES));
  cards.push_back(Card(TEN, SPADES));

  deal_cards(simple, cards);

  Card upcard(TWO, SPADES);

  // ROUND 1
  upcard = Card(JACK, DIAMONDS);
  Suit response_trump = DIAMONDS;
  bool response = simple->make_trump(1, response_trump);
  ASSERT_FALSE(response);
  ASSERT_EQUAL(response_trump, DIAMONDS);

  // ROUND 2
  upcard = Card(JACK, DIAMONDS);
  response = simple->make_trump(2, response_trump);
  ASSERT_TRUE(response);
  ASSERT_EQUAL(response_trump, HEARTS);
}

TEST(test_multiple_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(JACK, HEARTS));
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(NINE, DIAMONDS));
  cards.push_back(Card(TEN, DIAMONDS));

  deal_cards(simple, cards);

  Card upcard(TWO, SPADES);

  // ROUND 1
  Suit response_trump = DIAMONDS;
  upcard = Card(JACK, DIAMONDS);
  bool response = simple->make_trump(1, response_trump);
  ASSERT_FALSE(response);
  ASSERT_EQUAL(response_trump, DIAMONDS);

  // ROUND 2
  upcard = Card(JACK, DIAMONDS);
  response = simple->make_trump(2, response_trump);
  ASSERT_TRUE(response);
  ASSERT_EQUAL(response_trump, HEARTS);
}

TEST(test_screw_dealer)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(JACK, HEARTS));
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(ACE, DIAMONDS));
  cards.push_back(Card(NINE, DIAMONDS));
  cards.push_back(Card(TEN, DIAMONDS));

  deal_cards(simple, cards);

  Card upcard(TWO, SPADES);

  // ROUND 1
  upcard = Card(JACK, SPADES);
  Suit response_trump = SPADES;
  bool response = simple->make_trump(1, response_trump);
  ASSERT_FALSE(response);
  ASSERT_EQUAL(response_trump, SPADES);

  // ROUND 2
  upcard = Card(JACK, CLUBS);
  response = simple->make_trump(2, response_trump);
  ASSERT_TRUE(response);
  ASSERT_EQUAL(response_trump, SPADES);
}

//------------------------------------------------------------------------------

// CARD TEST 04

TEST(test_add_and_discard_no_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(TEN, CLUBS));
  cards.push_back(Card(TEN, DIAMONDS));
  cards.push_back(Card(TEN, SPADES));
  cards.push_back(Card(NINE, DIAMONDS));
  cards.push_back(Card(NINE, CLUBS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(KING, DIAMONDS));
  cards.push_back(Card(QUEEN, SPADES));
  cards.push_back(Card(JACK, DIAMONDS));
  cards.push_back(Card(KING, CLUBS));


  deal_cards(simple, cards);

  Card expected_card(TWO, SPADES);

  Card upcard(TWO, SPADES);

  upcard = Card(JACK, HEARTS);
  expected_card = Card(NINE, CLUBS);
  simple->add_and_discard(upcard, DIAMONDS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(QUEEN, HEARTS);
  expected_card = Card(NINE, DIAMONDS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(TEN, HEARTS);
  expected_card = Card(TEN, SPADES);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(ACE, HEARTS);
  expected_card = Card(TEN, CLUBS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(KING, HEARTS);
  expected_card = Card(TEN, DIAMONDS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));
}// test_no_trump

TEST(test_some_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(TEN, DIAMONDS));
  cards.push_back(Card(JACK, SPADES));
  cards.push_back(Card(NINE, CLUBS));
  cards.push_back(Card(TEN, HEARTS));
  cards.push_back(Card(TEN, SPADES));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(KING, DIAMONDS));
  cards.push_back(Card(QUEEN, SPADES));
  cards.push_back(Card(JACK, DIAMONDS));
  cards.push_back(Card(KING, CLUBS));

  deal_cards(simple, cards);

  Card expected_card(TWO, SPADES);

  Card upcard(TWO, SPADES);

  upcard = Card(ACE, CLUBS);
  expected_card = Card(TEN, SPADES);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(KING, CLUBS);
  expected_card = Card(TEN, HEARTS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(QUEEN, CLUBS);
  expected_card = Card(TEN, DIAMONDS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(TEN, CLUBS);
  expected_card = Card(NINE, CLUBS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(JACK, CLUBS);
  expected_card = Card(TEN, CLUBS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

}// test_some_trump

TEST(test_all_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(JACK, HEARTS));
  cards.push_back(Card(ACE, HEARTS));
  cards.push_back(Card(JACK, DIAMONDS));
  cards.push_back(Card(TEN, HEARTS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(KING, DIAMONDS));
  cards.push_back(Card(QUEEN, SPADES));
  cards.push_back(Card(JACK, DIAMONDS));
  cards.push_back(Card(KING, CLUBS));

  deal_cards(simple, cards);

  Card expected_card(TWO, SPADES);

  Card upcard(TWO, SPADES);

  upcard = Card(TEN, CLUBS);
  expected_card = Card(TEN, HEARTS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(NINE, CLUBS);
  expected_card = Card(JACK, HEARTS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(JACK, CLUBS);
  expected_card = Card(JACK, DIAMONDS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(KING, CLUBS);
  expected_card = Card(QUEEN, HEARTS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

  upcard = Card(ACE, CLUBS);
  expected_card = Card(ACE, HEARTS);
  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));

}// test_all_trump

//------------------------------------------------------------------------------

// PLAYER TEST 05

TEST(test_lead_card_no_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(KING, HEARTS));
  cards.push_back(Card(QUEEN, DIAMONDS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(TEN, HEARTS));
  cards.push_back(Card(TEN, CLUBS));
  

  deal_cards(simple, cards);

  Suit trump = SPADES;
  Card played_card(TWO, SPADES);
  Card expected_card(TWO, SPADES);

  expected_card = Card(ACE, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(KING, HEARTS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(QUEEN, DIAMONDS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, HEARTS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);
}

TEST(test_lead_card_multiple_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(JACK, HEARTS));
  cards.push_back(Card(JACK, DIAMONDS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(QUEEN, CLUBS));
  cards.push_back(Card(TEN, DIAMONDS));

  deal_cards(simple, cards);

  Suit trump = DIAMONDS;
  Card played_card(TWO, SPADES);
  Card expected_card(TWO, SPADES);

  expected_card = Card(ACE, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(QUEEN, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(JACK, DIAMONDS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(JACK, HEARTS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, DIAMONDS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);
}

TEST(test_lead_card_all_trump)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(NINE, SPADES));
  cards.push_back(Card(TEN, SPADES));
  cards.push_back(Card(QUEEN, SPADES));
  cards.push_back(Card(KING, SPADES));
  cards.push_back(Card(JACK, CLUBS));

  deal_cards(simple, cards);

  Suit trump = SPADES;
  Card played_card(TWO, SPADES);
  Card expected_card(TWO, SPADES);

  expected_card = Card(JACK, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(KING, SPADES);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(QUEEN, SPADES);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, SPADES);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(NINE, SPADES);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

}

TEST(test_left_bower)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(NINE, CLUBS));
  cards.push_back(Card(TEN, CLUBS));
  cards.push_back(Card(QUEEN, CLUBS));
  cards.push_back(Card(KING, CLUBS));
  cards.push_back(Card(JACK, HEARTS));

  deal_cards(simple, cards);

  Suit trump = DIAMONDS;
  Card played_card;
  Card expected_card;

  expected_card = Card(KING, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(QUEEN, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, CLUBS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(NINE, CLUBS);
  played_card = simple->lead_card(trump);
  // played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(JACK, HEARTS);
  played_card = simple->lead_card(trump);
  ASSERT_EQUAL(expected_card, played_card);
}

//------------------------------------------------------------------------------

// PLAYER TEST 06

TEST(test_no_suit_led)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(JACK, HEARTS));
  cards.push_back(Card(JACK, DIAMONDS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(NINE, DIAMONDS));
  cards.push_back(Card(TEN, DIAMONDS));

  deal_cards(simple, cards);

  Suit trump = DIAMONDS;
  Card led_card(NINE, HEARTS);
  Card played_card;
  Card expected_card;

  expected_card = Card(ACE, CLUBS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(NINE, DIAMONDS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, DIAMONDS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(JACK, HEARTS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(JACK, DIAMONDS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);
}

TEST(test_mixed_led_suit)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(TEN, CLUBS));
  cards.push_back(Card(TEN, DIAMONDS));
  cards.push_back(Card(NINE, CLUBS));
  cards.push_back(Card(TEN, HEARTS));
  cards.push_back(Card(TEN, SPADES));

  deal_cards(simple, cards);

  Suit trump = CLUBS;
  Card led_card(NINE, CLUBS);
  Card played_card;
  Card expected_card;

  expected_card = Card(TEN, CLUBS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(NINE, CLUBS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, SPADES);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, HEARTS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, DIAMONDS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);
}

TEST(test_has_suit_led)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(KING, HEARTS));
  cards.push_back(Card(ACE, HEARTS));
  cards.push_back(Card(NINE, HEARTS));
  cards.push_back(Card(TEN, HEARTS));

  deal_cards(simple, cards);

  Suit trump = DIAMONDS;
  Card led_card (NINE, HEARTS);
  Card played_card;
  Card expected_card;

  expected_card = Card(ACE, HEARTS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(KING, HEARTS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(QUEEN, HEARTS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(TEN, HEARTS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);

  expected_card = Card(NINE, HEARTS);
  played_card = simple->play_card(led_card, trump);
  ASSERT_EQUAL(expected_card, played_card);
}

//------------------------------------------------------------------------------

// PLAYER TEST 07

TEST(test_discard_upcard)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(JACK, HEARTS));
  cards.push_back(Card(ACE, HEARTS));
  cards.push_back(Card(TEN, HEARTS));
  cards.push_back(Card(JACK, DIAMONDS));

  deal_cards(simple, cards);

  Card expected_card(NINE, HEARTS);

  Card upcard(NINE, HEARTS);

  simple->add_and_discard(upcard, HEARTS);
  ASSERT_FALSE(player_has_card(simple, expected_card));
}

//------------------------------------------------------------------------------

// PLAYER TEST 08

TEST(test_trump_count_left_bower_round_1)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(KING, HEARTS));
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(ACE, CLUBS));
  cards.push_back(Card(JACK, SPADES));
  cards.push_back(Card(TEN, HEARTS));

  deal_cards(simple, cards);

  Card upcard;

  // ROUND 1
  upcard = Card(NINE, CLUBS);
  Suit response_trump = SPADES;
  bool response = simple->make_trump(1, response_trump);
  ASSERT_TRUE(response);
  ASSERT_EQUAL(response_trump, CLUBS);
}

TEST(test_trump_count_left_bower_round_2)
{
  unique_ptr<Player> simple(Player_factory(PLAYER_NAME, "Simple"));

  vector<Card> cards;
  cards.push_back(Card(KING, HEARTS));
  cards.push_back(Card(QUEEN, HEARTS));
  cards.push_back(Card(ACE, HEARTS));
  cards.push_back(Card(JACK, SPADES));
  cards.push_back(Card(TEN, HEARTS));

  deal_cards(simple, cards);

  Card upcard;

  // ROUND 2
  upcard = Card(NINE, SPADES);
  Suit response_trump = SPADES;
  bool response = simple->make_trump(2, response_trump);
  ASSERT_TRUE(response);
  ASSERT_EQUAL(response_trump, CLUBS);
}


namespace
{

void deal_cards(unique_ptr<Player> &player, const vector<Card>& cards)
{
  for (const auto& card : cards)
  {
    player->add_card(card);
  }
}

/**
 * REQUIRES: Player has full hand.
 * NOTE: This function will mutate the player by calling lead_card() 5 times
 * and then adding those cards back with add_card().
 */
bool player_has_card(unique_ptr<Player> &player, Card card)
{
  std::vector<Card> cards;
  auto found = false;
  for(int i = 0; i < Player::MAX_HAND_SIZE; ++i)
  {
    Card c = player->lead_card(SPADES);
    cards.push_back(c);
    if(c == card)
    {
        found = true;
        break;
    }
  }// for

  while(!cards.empty())
  {
    player->add_card(cards[cards.size() - 1]);
    cards.pop_back();
  }// while

  return found;
}


}

TEST_MAIN()
