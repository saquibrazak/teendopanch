/* Card_tests.cpp */

#include "Card.hpp"
#include "unit_test_framework.hpp"

#include <string>

using namespace std;


TEST(test_Card_default_ctor)
{
    Card two_spades;
    ASSERT_EQUAL(TWO, two_spades.get_rank());
    ASSERT_EQUAL(SPADES, two_spades.get_suit());
}

TEST(test_Card_custom_ctor)
{
    Card two_spades(TWO, SPADES);
    ASSERT_EQUAL(TWO, two_spades.get_rank());
    ASSERT_EQUAL(SPADES, two_spades.get_suit());

    Card queen_diamonds(QUEEN, DIAMONDS);
    ASSERT_EQUAL(QUEEN, queen_diamonds.get_rank());
    ASSERT_EQUAL(DIAMONDS, queen_diamonds.get_suit());
}



TEST(test_is_face_or_ace)
{
    ASSERT_TRUE(Card(JACK, SPADES).is_face_or_ace());
    ASSERT_TRUE(Card(QUEEN, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(KING, DIAMONDS).is_face_or_ace());
    ASSERT_TRUE(Card(ACE, HEARTS).is_face_or_ace());

    ASSERT_FALSE(Card(TWO, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(THREE, DIAMONDS).is_face_or_ace());
    ASSERT_FALSE(Card(FOUR, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(FIVE, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(SIX, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(SEVEN, DIAMONDS).is_face_or_ace());
    ASSERT_FALSE(Card(EIGHT, HEARTS).is_face_or_ace());
    ASSERT_FALSE(Card(NINE, CLUBS).is_face_or_ace());
    ASSERT_FALSE(Card(TEN, DIAMONDS).is_face_or_ace());
}


TEST(test_is_trump)
{
    ASSERT_TRUE(
        Card(FOUR, CLUBS).is_trump(CLUBS));
    ASSERT_FALSE(
        Card(ACE, DIAMONDS).is_trump(HEARTS));

    ASSERT_TRUE(
        Card(JACK, SPADES).is_trump(SPADES));
    ASSERT_FALSE(
        Card(JACK, SPADES).is_trump(HEARTS));

    
}


TEST(test_Card_lt_operator)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_TRUE(ace_hearts < ace_diamonds);
    ASSERT_FALSE(ace_diamonds < ace_hearts);
    ASSERT_FALSE(ace_diamonds < jack_diamonds);
}

TEST(test_Card_lt_eq_operator)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_TRUE(ace_hearts <= ace_diamonds);
    ASSERT_FALSE(ace_diamonds <= ace_hearts);
    ASSERT_FALSE(ace_diamonds <= jack_diamonds);
    ASSERT_TRUE(ace_hearts <= ace_hearts);
    ASSERT_TRUE(ace_diamonds <= ace_diamonds);
    ASSERT_TRUE(jack_diamonds <= jack_diamonds);
}

TEST(test_Card_gt_operator)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_TRUE(ace_diamonds > ace_hearts);
    ASSERT_FALSE(ace_hearts > ace_diamonds);
    ASSERT_FALSE(jack_diamonds > ace_diamonds);
}

TEST(test_Card_gt_eq_operator)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_TRUE(ace_diamonds >= ace_hearts);
    ASSERT_FALSE(ace_hearts >= ace_diamonds);
    ASSERT_FALSE(jack_diamonds >= ace_diamonds);
    ASSERT_TRUE(ace_hearts >= ace_hearts);
    ASSERT_TRUE(ace_diamonds >= ace_diamonds);
    ASSERT_TRUE(jack_diamonds >= jack_diamonds);
}

TEST(test_Card_eq_operator)
{
    ASSERT_EQUAL(Card(JACK, HEARTS),
                 Card(JACK, HEARTS));
}

TEST(test_Card_neq_operator)
{
    ASSERT_NOT_EQUAL(Card(JACK, SPADES),
                     Card(JACK, HEARTS));

    ASSERT_NOT_EQUAL(Card(JACK, HEARTS),
                     Card(ACE, HEARTS));
}

TEST(test_Card_ostream_operator)
{
    stringstream expected;
    stringstream actual;
    for (int r = TWO; r <= ACE; r++) {
        for (int s = SPADES; s <= DIAMONDS; s++) {
            expected.str("");
            actual.str("");
            Rank rank = static_cast<Rank>(r);
            Suit suit = static_cast<Suit>(s);

            // Check output to verify <<
            expected << rank << " of " << suit;
            actual << Card(rank, suit);
            ASSERT_EQUAL(expected.str(), actual.str());

            // Read expected output back in to check >>
            istringstream iss(expected.str());
            Card c;
            iss >> c;
            ASSERT_EQUAL(c, Card(rank, suit));
        }
    }
}


TEST(test_Card_less_with_trump_neither_trump)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card jack_diamonds(JACK, DIAMONDS);

    ASSERT_FALSE(Card_less(ace_diamonds, jack_diamonds, HEARTS));
    ASSERT_TRUE(Card_less(jack_diamonds, ace_diamonds, HEARTS));
}

TEST(test_Card_less_with_trump_both_trump)
{
    Card jack_clubs(JACK, CLUBS);
    Card nine_spades(NINE, SPADES);
    Card ten_spades(TEN, SPADES);

    ASSERT_FALSE(Card_less(nine_spades, jack_clubs, SPADES));
    ASSERT_TRUE(Card_less(nine_spades, ten_spades, SPADES));

    ASSERT_FALSE(Card_less(ten_spades, nine_spades, SPADES));
}

TEST(test_Card_less_with_trump_one_trump)
{
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    Card nine_spades(NINE, SPADES);

    // Trump as second argument
    ASSERT_TRUE(Card_less(jack_diamonds, nine_spades, SPADES));
    ASSERT_TRUE(Card_less(ace_hearts, nine_spades, SPADES));

    ASSERT_FALSE(Card_less(nine_spades, jack_diamonds, SPADES));
    ASSERT_FALSE(Card_less(nine_spades, ace_hearts, SPADES));

    // Trump as first argument
    ASSERT_FALSE(Card_less(jack_diamonds, nine_spades, DIAMONDS));
    ASSERT_FALSE(Card_less(ace_hearts, nine_spades, HEARTS));

    ASSERT_TRUE(Card_less(nine_spades, jack_diamonds, DIAMONDS));
    ASSERT_TRUE(Card_less(nine_spades, ace_hearts, HEARTS));
}

TEST(test_Card_less_with_trump_left_bower)
{
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);

    ASSERT_FALSE(Card_less(ace_hearts, jack_diamonds, HEARTS));
}




TEST(test_Card_less_with_led_card_both_follow_no_trump)
{
    Card nine_diamonds(NINE, DIAMONDS);
    Card ten_diamonds(TEN, DIAMONDS);
    Card ace_diamonds(ACE, DIAMONDS);

    ASSERT_TRUE(Card_less(ten_diamonds, ace_diamonds,
                          nine_diamonds, SPADES));
    ASSERT_FALSE(Card_less(ace_diamonds, ten_diamonds,
                          nine_diamonds, SPADES));
}

TEST(test_Card_less_with_led_card_one_follows_no_trump)
{
    Card nine_diamonds(NINE, DIAMONDS);
    Card ace_diamonds(ACE, DIAMONDS);
    Card nine_hearts(NINE, HEARTS);
    Card ace_hearts(ACE, HEARTS);

    ASSERT_TRUE(Card_less(ace_diamonds, ace_hearts,
                          nine_hearts, CLUBS));
    ASSERT_FALSE(Card_less(ace_diamonds, ace_hearts,
                          nine_diamonds, CLUBS));
}

TEST(test_Card_less_with_led_card_one_follows_one_trump)
{
    Card nine_diamonds(NINE, DIAMONDS);
    Card ace_diamonds(ACE, DIAMONDS);
    Card nine_hearts(NINE, HEARTS);
    Card ace_hearts(ACE, HEARTS);

    ASSERT_FALSE(Card_less(ace_diamonds, ace_hearts,
                           nine_hearts, DIAMONDS));
    ASSERT_TRUE(Card_less(ace_diamonds, ace_hearts,
                           nine_diamonds, HEARTS));
}

TEST(test_Card_less_with_led_card_neither_follows_one_trump)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card ace_hearts(ACE, HEARTS);
    Card nine_clubs(NINE, CLUBS);

    ASSERT_FALSE(Card_less(ace_diamonds, ace_hearts,
                           nine_clubs, DIAMONDS));
    ASSERT_TRUE(Card_less(ace_diamonds, ace_hearts,
                           nine_clubs, HEARTS));
    ASSERT_TRUE(Card_less(ace_hearts, ace_diamonds,
                           nine_clubs, DIAMONDS));
}

TEST(test_Card_less_with_led_card_neither_follows_no_trump)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card jack_diamonds(JACK, DIAMONDS);
    Card nine_clubs(NINE, CLUBS);

    ASSERT_FALSE(Card_less(ace_diamonds, jack_diamonds,
                           nine_clubs, SPADES));
}

TEST(test_Card_less_with_led_card_both_trump)
{
    Card ace_diamonds(ACE, DIAMONDS);
    Card jack_diamonds(JACK, DIAMONDS);
    Card jack_spades(JACK, SPADES);
    Card jack_clubs(JACK, CLUBS);
    Card nine_clubs(NINE, CLUBS);
    Card nine_diamonds(NINE, DIAMONDS);

    ASSERT_TRUE(Card_less(jack_diamonds,ace_diamonds, 
                          nine_clubs, DIAMONDS));
    // Right and left bower - make sure this is not creeping in from Euchre
    ASSERT_TRUE(Card_less(jack_spades,jack_clubs, 
                           nine_diamonds, CLUBS));
    ASSERT_FALSE(Card_less(jack_spades,jack_clubs, 
                           nine_diamonds, SPADES));
}

TEST(test_Card_less_with_led_card_left_bower)
{
    Card ace_hearts(ACE, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    // Check to see if there is any left over code from Euchre
    ASSERT_FALSE(Card_less(ace_hearts, jack_diamonds, ace_hearts, HEARTS));
}

TEST(test_Card_less_with_led_card_right_bower)
{
    Card ace_hearts(ACE, HEARTS);
    Card jack_hearts(JACK, HEARTS);
    // Check to see if there is any left over code from Euchre
    ASSERT_FALSE(Card_less(ace_hearts, jack_hearts, ace_hearts, HEARTS));
}

TEST_MAIN()
