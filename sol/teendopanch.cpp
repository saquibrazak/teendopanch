/* teendopanch.cpp
 * Teendopaanch card game simulator
 *
 * by Saquib Razak
 * razak @umich.edu
 * 2025-9-4
 */


 #include "Pack.hpp"
 #include "Player.hpp"
 #include "Card.hpp"
 #include <iostream>
 #include <fstream>
 #include <cassert>
 #include <cstdlib>
 #include <cstring>
#include <ctime>     // for time()
 using namespace std;
 
 
 
 ////////////////////////////////////////////////////////////////////////////////
 // Helper functions
 int usage() {
   cout << "Usage: teendopanch.exe PACK_FILENAME [shuffle|noshuffle] "
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
   void deal(const Player *dealer,int round);
   bool make_trump(Player *dealer, int round, Suit *trump);
   void play_hand(Player *dealer);
   void equalize_quotas(const Player *dealer, Suit trump);
   bool over() const;
   int Player_index(const Player *p) const;
   Player * Player_left(const Player *p);
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
   
   std::srand(std::time(0));
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
     cout << "Hand " << ++hand_counter << endl;
     
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
 
 
 
 bool Game::over() const {
   for (int i=0; i<NUM_PLAYERS; ++i)
     if (scores[i] >= points_to_win) return true;
   return false;
 }
 
 
 
 void Game::shuffle_pack() {
   if (shuffle)
     pack.shuffle();
   else
     pack.reset();
 }
 
 void Game::deal(const Player *dealer, int round) {
    int num_cards[] = {5,3,2};
   cout << *dealer << " deals "<<num_cards[round-1]<<" cards\n";
 
   // Player receiving cards
   Player *player = Player_left(dealer);
   
   for (int j=0; j<num_cards[round-1]; ++j) {
    for (int i=0; i<NUM_PLAYERS; ++i) {
    
       Card c = pack.deal_one();
       player->add_card(c);
       // move to next player
        player = Player_left(player);
     }
   }
 
 }
 
 bool Game::make_trump(Player *dealer, int round, Suit *trump) {
 
 
   // left of the dealer speaks first
   Player *player = Player_left(dealer);
 

   // ask the player if they want to order up
   bool r =  player->make_trump(round, *trump);
   if(r)
   {
       cout << *player << " orders up " << *trump << "\n";
       
   }
   else
   {
       cout << *player << " passes\n";
   }
    return r;

  
 }
 
 void Game::play_hand(Player *dealer) {
   shuffle_pack();
   deal(dealer,1//round 1
    );
 
   // Make trump, remembering make and trump suit for this round
   Suit trump;
   //Player *maker;
   bool trump_made = false;
   trump_made = make_trump(dealer, 1, &trump);

   deal(dealer,2); // round 2
    if(!trump_made)
    {
        trump_made = make_trump(dealer, 2, &trump);
    }
    deal(dealer,3); // round 3    
    // Pull cards based on quota
    equalize_quotas(dealer,trump);
   // number of tricks taken by each player
   int tricks_taken[3] = {0,0,0};
 
   // the first leader is left of the dealer
   Player *leader = Player_left(dealer);
 
   // play a trick for each card in the hand
   for (int i=0; i<10; ++i) {
 
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
 
     // increment tricks count of winning player
     tricks_taken[ Player_index(leader) ] += 1;
 
     cout << endl;
   }
   // announce tricks
   
   for(auto p:players)
   {
      int player_index = Player_index(p);
      cout << *p << " has " << tricks_taken[player_index] << " tricks\n";
      scores[player_index] += tricks_taken[player_index];
   }
   // set quotas
   // the dealer should have quota of 2, and the maker should have 5, and the third player has quota of 3
   int required_quotas[NUM_PLAYERS] = {2,5,3};
   int dealer_index = Player_index(dealer);
    for(int i=0; i<NUM_PLAYERS; i++)
    {
        quota[i] = tricks_taken[i] - required_quotas[(i+dealer_index) % NUM_PLAYERS];
    }   
 
 }

 void Game::equalize_quotas(const Player *dealer, Suit trump)
 {
    // This function will equalize the quotas for the players
    // Check quotas for each player, then for each player
    // if the quota is zero, do nothing
    // if the player has a negative quota, then give them cards from the player with positive quota until they are equal    
    // if the player has a positive quota, then take cards from the player with negative quota until they are equal
    // if the player has a zero quota, then do nothing
    // Go through all the players
    for(int i =0; i <NUM_PLAYERS; i++)
    {
        // check qouta of the next player
        while(quota[i] > 0)
        {
            // find the player with negative quota
            int p = quota[(i+1)%NUM_PLAYERS]<0?(i+1)%NUM_PLAYERS:(i+2)%NUM_PLAYERS;
            // take card from player[p] and add to player[i]
            Card c = players[p]->remove_card();
            c = players[i]->add_and_discard(c,trump);
            players[p]->add_card(c);
            quota[i]--;
            quota[p]++; 
        }
    }
 }
