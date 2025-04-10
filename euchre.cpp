/* euchre.cpp
 * Euchre card game simulator
 *
 * by Andrew DeOrio
 * awdeorio@umich.edu
 * 2014-12-21
 */


#include "Pack.hpp"
#include "Player.hpp"
#include "Card.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cstring>
using namespace std;



////////////////////////////////////////////////////////////////////////////////
// Helper functions
int usage() {
  cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
       << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4"
       << endl;
  return EXIT_FAILURE;
}


// A game keeps track of a Pack, and whether it should be shuffled, the score
// and how many points it takes to win, and the Players in the game
class Game {
 public:
  //REQUIRES is stream contains a well-formed configuration and pack
  //MODIFIES stdout
  //EFFECTS Initializes Game including initializing the Pack and each Player.
  Game
    (
     std::istream &pack_stream,
     bool shuffle_in,
     int points_to_win_in,
     const std::vector<std::string> &player_names_in
     );

  //MODIFIES stdout
  //EFFECTS  Plays one game of Euchre
  void play();

 private:
  static const int NUM_PLAYERS = 3;
  Pack pack;                    // Pack of cards
  bool shuffle;                 // Shuffle the pack after each hand
  int points_to_win;
  int hand_counter;
  std::vector<Player*> players;  // Players in this game, in order around table
  std::array<int,3> quota;
  std::array<int,3> scores;
  void shuffle_pack();
  void deal(const Player *dealer);
  void make_trump(Player *dealer, Suit *trump, Player **maker);
  void play_hand(Player *dealer);
  bool over() const;
  int Player_index(const Player *p) const;
  Player * Player_left(const Player *p);
  int get_partnership(const Player *p);
  void print_partnership(const int partnership) const;
};


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {

  if (argc != 12)
    return usage();

  // get pack filename and whether or not to shuffle
  bool shuffle = false;
  if (strcmp(argv[2], "shuffle") == 0)
    shuffle = true;
  else if (strcmp(argv[2], "noshuffle") == 0)
    shuffle = false;
  else
    return usage();

  // check bounds on points to win game
  int points_to_win = atoi(argv[3]);
  if (points_to_win < 1 || points_to_win > 100)
    return usage();

  // print command line arguments
  for (char **s = argv; s<argv+argc; ++s)
    cout << *s << " ";
  cout << endl;

  ifstream pack_stream(argv[1]);
  if (!pack_stream.is_open()) {
    cout << "Error opening " << argv[1] << endl;
    return EXIT_FAILURE;
  }
  vector<string> player_names(argv + 4, argv + 12);

  // play a game
  Game game(pack_stream, shuffle, points_to_win, player_names);
  game.play();

  return EXIT_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
Game::Game
(
 std::istream &pack_stream,
 bool shuffle_in,
 int points_to_win_in,
 const std::vector<std::string> &player_names_in
 ) {

  // initialize pack
  pack = Pack(pack_stream);
  shuffle = shuffle_in;
  points_to_win = points_to_win_in;
  hand_counter = 0;
  for (int i=0; i<NUM_PLAYERS; ++i)
  {
    quota[i] = 0;
    scores[i] = 0;
  }
  // initialize three players
  for (int i=0; i<NUM_PLAYERS*2; i+=2)
    players.push_back(Player_factory(player_names_in[i], player_names_in[i+1]));
}

void Game::play() {

  Player *dealer = players[0]; //first dealer is player 0
  while (!over()) {
    cout << "Hand " << hand_counter++ << endl;
    // Pull cards based on quota
    // pull_cards(dealer);
    play_hand(dealer);
    dealer = Player_left(dealer);
    cout << endl;
  }

  //int winners = score[0] > score[1] ? 0 : 1;
  //SPEC: which order to print names in a partnership?
  //print_partnership(winners);
  cout << " win!\n";

  for (int i=0; i<NUM_PLAYERS; ++i) {
    delete players[i];
  }
}


int Game::Player_index(const Player *p) const {

  // search for p
  int i = 0;
  for (i=0; i<NUM_PLAYERS; ++i)
    if (players[i] == p)
      break;

  assert(players[i] == p); //make sure that p was found

  return i;
}

Player * Game::Player_left(const Player *p) {
  int i = Player_index(p);
  return players[(i+1) % NUM_PLAYERS];
}

int Game::get_partnership(const Player *p) {
  int i = Player_index(p);
  return (i%2);  // even players are team 0, odd players are team 1
}

bool Game::over() const {
  for (int i=0; i<NUM_PLAYERS; ++i)
    if (scores[i] >= points_to_win) return true;
  return false;
}

void Game::print_partnership(int partnership) const {
  assert(partnership >= 0 && partnership < NUM_PARTNERSHIPS);

  cout << *players[partnership] << " and "
       << *players[partnership+NUM_PARTNERSHIPS];
}

void Game::shuffle_pack() {
  if (shuffle)
    pack.shuffle();
  else
    pack.reset();
}

void Game::deal(const Player *dealer) {
  cout << *dealer << " deals\n";

  // Player receiving cards
  Player *player = Player_left(dealer);

  const int DEAL_ROUNDS = 2; //number of times around the table
  for (int i=0; i<NUM_PLAYERS * DEAL_ROUNDS; ++i) {

    int num_cards = 0;
    if (i < NUM_PLAYERS && !(i % 2))
      num_cards = 3;
    else if (i < NUM_PLAYERS && (i % 2))
      num_cards = 2;
    else if (i >= NUM_PLAYERS && !(i % 2))
      num_cards = 2;
    else if (i >= NUM_PLAYERS && (i % 2))
      num_cards = 3;
    else
      assert(0);

    for (int i=0; i<num_cards; ++i) {
      Card c = pack.deal_one();
      player->add_card(c);
    }

    // move to next player
    player = Player_left(player);
  }

}

void Game::make_trump(Player *dealer, Suit *trump, Player **maker) {

  // turn up one card to propose trump suit
  Card upcard = pack.deal_one();
  cout << upcard << " turned up" << endl;

  // left of the dealer speaks first
  Player *player = Player_left(dealer);

  // go around the table, and each player can "pass" or "order up"
  for (int round=1; round<=2; ++round) {
    for (int i=0; i<NUM_PLAYERS; ++i) {

      // ask one player if they want to order up
      bool orderup = player->make_trump(upcard, dealer == player, round, *trump);

      // nobody ordered up, that's a pass, move to the next player on the left
      if (!orderup) {
        cout << *player << " passes\n";
        player = Player_left(player);
        continue;
      }

      // a player orders up, announce
      cout << *player << " orders up " << *trump << "\n";
      *maker = player;

      // if player orders up on the first round, dealer gets the upcard
      if (round == 1) {
        dealer->add_and_discard(upcard);
      }

      // A player orders up, we're done
      return;
    }
  }

  assert(0); //should never get here
}

void Game::play_hand(Player *dealer) {
  shuffle_pack();
  deal(dealer);

  // Make trump, remembering make and trump suit for this round
  Suit trump;
  Player *maker;
  make_trump(dealer, &trump, &maker);
  cout << endl;

  // number of tricks taken by each partnership
  int tricks_taken[2] = {0,0};

  // the first leader is left of the dealer
  Player *leader = Player_left(dealer);

  // play a trick for each card in the hand
  for (int i=0; i<5; ++i) {

    // get first card
    Card led_card = leader->lead_card(trump);
    Card high_card = led_card;
    cout << high_card << " led by " << *leader << "\n";

    // each player plays one card
    Player *player = Player_left(leader);
    for (int j=0; j<NUM_PLAYERS-1; ++j) {

      // ask player for a card
      Card c = player->play_card(led_card, trump);

      // announce card played
      cout << c << " played by " << *player << "\n";

      // update the high card
      if (Card_less(high_card, c, led_card, trump)) {
        leader = player;
        high_card = c;
      }

      // advance to next player, clockwise around table
      player = Player_left(player);
    }

    // announce leader
    cout << *leader << " takes the trick\n";

    // increment tricks count of winning partnership
    tricks_taken[ get_partnership(leader) ] += 1;

    cout << endl;
  }

  // announce winners
  assert(tricks_taken[0] != tricks_taken[1]);
  int trick_winners = tricks_taken[0] > tricks_taken[1] ? 0 : 1;
  print_partnership(trick_winners);
  cout << " win the hand\n";

  // increment score
  //SPEC: be careful about order of messages
  if (trick_winners == get_partnership(maker)) {
    // making side wins

    if (tricks_taken[trick_winners] == 5) {
      cout << "march!\n";
      score[trick_winners] += 2;
    } else {
      score[trick_winners] += 1;
    }

  } else {
    // makin side loses (euchred)
    cout << "euchred!\n";
    score[trick_winners] += 2;
  }

  // announce score
  for (int i=0; i<NUM_PARTNERSHIPS; ++i) {
    print_partnership(i);
    cout  << " have " << score[i] << " points\n";
  }

}
