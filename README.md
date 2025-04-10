

EECS 280: TeenDoPanch
==========================


## Introduction
Teen Do Panch is a card game popular in several countries of South Asia. Several rules of the game were taken from `https://www.pagat.com/whist/3-2-5.html`

The learning goals of this project include Abstract Data Types in C++, Derived Classes, Inheritance, and Polymorphism.  You'll gain practice with C++-style Object Oriented Programming (OOP) with classes and virtual functions.

When you're done, you'll have a program that simulates a game of Teen Do Panch, supporting a AI player and a Human player.
```console
$ ./teendopanch.exe pack.in shuffle 10 A Simple B Simple C Simple
Hand 1
A deals 5 cards
B orders up Spades
A deals 3 cards
A deals 2 cards
Queen of Diamonds led by B
King of Diamonds played by C
Ace of Diamonds played by A
A takes the trick
...
```


## Setup
Use the files from the following git repo: `https://github.com/saquibrazak/teendopanch/tree/main/starter-files`


Create a new file `teendopanch.cpp`.  You should end up with a folder with starter files that looks like this. 
```console
$ ls
Card.cpp.starter        Pack_public_tests.cpp          correct_output.out
Card.hpp                Pack_tests.cpp.starter    
Card_public_tests.cpp   Player.hpp                
Card_tests.cpp.starter  Player_public_tests.cpp   
Makefile                Player_tests.cpp.starter       pack.in
Pack.hpp                teendopanch.cpp                unit_test_framework.hpp
```

Here's a short description of each starter file.

| File(s) | Description |
| ------- | ----------- |
| `Card.hpp` | Abstraction representing a playing card. |
| `Card.cpp.starter` | Starter code for the `Card`. |
| `Card_tests.cpp` | Your `Card` unit tests. |
| `Card_public_tests.cpp` | Compile check test for `Card.cpp`. |
| `Pack.hpp` | Abstraction representing a pack cards. |
| `Pack_tests.cpp` | Add your `Pack` unit tests to this file. |
| `Pack_public_tests.cpp` | Compile check test for `Pack.cpp`. |
| `Player.hpp` | Abstraction representing a euchre player. |
| `Player_tests.cpp` | Your `Player` unit tests. |
| `Player_public_tests.cpp` | Compile check test for `Player.cpp`. |
| `pack.in` | Input file containing a Euchre deck. |
| `Makefile` | Helper commands for building. |
| `euchre_test00.out.correct` <br>`euchre_test01.out.correct` | Correct output for system tests with Simple player.  |
| `euchre_test50.in` <br>`euchre_test50.out.correct` | Input and correct output for system tests with Human player. |
| `unit_test_framework.hpp` | A simple unit-testing framework. |


## Teen do panch Rules

There are many variants of of this game. Our particular version is based on
a variety commonly played in Pakistan and Inida with a few changes to make it
feasible as a coding project.

### Players

There are three players numbered 0-2. 

### The Cards


This game uses a deck of 24 *playing cards*, each of which has two
properties: a *rank* and a *suit*. The ranks are 7, 8, 9, 10, *Jack*,
*Queen*, *King*, and *Ace*, and the suits are *Spades*, *Hearts*,
*Clubs*, and *Diamonds*. Since we use 30 cards, the 7 rank is used only from *Spades* and *Hearts* suits. Each card is unique &mdash; there are no duplicates.
Throughout this document, we sometimes refer to ranks or suits using
only the first letter of their name. Farther below, we describe how to
determine the [ordering of the cards](#value-of-cards).

### Playing the Game

At a high level, a game of "Teen do panch" involves multiple rounds, which are
called *hands*. Each hand consists of the following phases.

Each hand:

1.  [Setup table](#setup-table)
    1.  [Shuffle](#shuffle)
    2.  [Deal](#deal)
2.  [Making Trump](#making-trump)
    1.  [Round One](#round-one)
    2.  [Round Two](#round-two)
3.  [Trick Taking](#trick-taking)
4.  [Scoring](#scoring)

We describe each in more detail below.

### Setup table

#### Shuffle

The dealer shuffles the deck at the beginning of each hand. The
algorithm you will implement for shuffling is a variant of a riffle
shuffle called an "in shuffle"
([https://en.wikipedia.org/wiki/In\_shuffle](https://en.wikipedia.org/wiki/In_shuffle)).
Cut the deck exactly in half and then interleave the two halves,
starting with the second half. Thus, the card originally at position
12 goes to position 0, the one originally at position 0 goes to
position 1, the one originally at position 13 goes to position 2, and
so on. Do this in-shuffle process 7 times.

You will also implement an option to run the game with shuffling
disabled - when this option is chosen, just reset the pack any time
shuffling would be called for. This may make for easier testing and
debugging.

#### Deal


In each hand, one player is designated as the *dealer* (if humans were
playing the game, the one who passes out the cards). In our game,
player 0 deals during the first hand. Each subsequent hand, the role
of dealer moves one player to the left (increasing player number).

The cards are dealth in three rounds. In the first round, each player gets 5 cards (one card to each player at a time), 3 cards in the second round, and 2 cards in the third.

### Making Trump

During this phase, the trump suit is picked by whichever player
is to the left of the dealer (*maker*).

#### Round One
After receiving the 5 cards during round 1, the player picks a trump after look at these five cards.
If the player *passes* calling trump during this round, the trump is determined during round 2.


#### Round Two

If the player *passes* during the first round, there is a second round
of *making*, where the suit of the seventh card (second card in this round) 
received by the player to the left of the dealer is
picked as trump.

### Trick Taking

Once the trump has been determined, ten *tricks* are played. For each
trick, players take turns laying down cards, and whoever played the
highest card *takes* the trick.

During each trick, the player who plays first is called the *leader*.
For the first trick, the player who calls *trump* leads.

At the beginning of each trick, the leader *leads* a card, which
affects which cards other players are allowed to play, as well as the
value of each card played (see below). Each other player must *follow
suit* (play a card with the same suit as the led card) if they are
able, and otherwise may play any card (it is removed from their hand).
Play moves to the left around the table, with each player playing one
card.

A trick is won by the player who played the highest valued card (see
below to determine comparative values). The winner of the trick
*leads* the next one.

During each hand, the *maker* has to win at least 5 tricks, the dealer 2 tricks, and the third player 
has to win three tricks. This is called the minimum *quota of tricks* that each player has to reach. 


### Scoring

The number of points for each player is determind by their *quota* for that hand. 
* The *maker* gets 2 points for reaching their quota - and 1 point for each trick above the quota
* All other player get 1 point for each trick above thier quota
Traditionally, the first player to reach 10 points wins the game. In
this project, the number of points needed to win is specified when the
program is run.

### Pulling Cards

Starting from the second deal onward, players who won more tricks than their assigned quota in the previous round get the opportunity to strengthen their hands by taking cards from players who fell short of their quotas.

However, in the first deal of a session—or in any deal where all players exactly met their quotas—no card exchanges take place. Each player simply plays with the 10 cards they were dealt.

From the second deal on, the card-pulling rule applies:

* A player who exceeded their quota may take one card for each trick they won above quota.

* Conversely, a player who fell short must give up one card for each trick they missed.

In a real-life game, the under-quota player holds out their 10 cards face down, and the over-quota player blindly picks one. The card is kept hidden from the third player. The over-quota player then selects one card from their hand to return—also face down—to the under-quota player.

In our project, instead of picking a random card, we’ll simplify this step: the over-quota player will always take the last card dealt to the under-quota player.

Card exchange rules:

* The over-quota player cannot return the same card they just took.

* The returned card must come from a suit in which the over-quota player holds at least three cards (i.e., they must retain at least two cards in that suit after returning one).

This process continues until:

* Each over-quota player has stolen one card for every extra trick, and

* Each under-quota player has lost one card for every missed trick.

If multiple players are over quota, the one with the lowest player number steals first. Similarly, when choosing which under-quota player to steal from, the over-quota player starts with the player with the lowest number.

### Value of cards

In order to determine which of two cards is better, you must pay
attention to the context in which they are being compared. There are
three separate contexts, which depend on whether or not a trump or led
suit is present.

In the simplest case, cards are ordered by rank (A \> K \> Q \> J \>
10 \> 9), with ties broken by suit (D \> C \> H \> S).

If a *trump suit* is present, all trump cards are more valuable than
non-trump cards. That means a 9 of the trump suit will beat an Ace of
a non-trump suit. 

If a *led suit* is present as well as a trump suit, the ordering is
the same except that all cards of the led suit are considered more
valuable than all non-trump-suit, non-led-suit cards. Note that it is
possible for the trump suit and led suit to be the same.


## Card

Write an abstract data type (ADT) for a playing card.  Refer back to the [Value of cards section](#value-of-cards) while you're working on the card ADT.


Write implementations in `Card.cpp` for the functions declared in
`Card.hpp`.

Run the public Card tests.
```console
$ make Card_public_tests.exe
$ ./Card_public_tests.exe
```

Write tests for `Card` in `Card_tests.cpp` using the [Unit Test Framework](https://eecs280staff.github.io/unit_test_framework/). See the [Unit Test Grading](#unit-test-grading) section.
```console
$ make Card_tests.exe
$ ./Card_tests.exe
```




### Setup
Rename these files ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#rename-files), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#rename-files), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#rename-files),  [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#rename-files), [CLI](https://eecs280staff.github.io/tutorials/cli.html#mv)):
-  `Card.cpp.starter` -> `Card.cpp`
-  `Card_tests.cpp.starter` -> `Card_tests.cpp`

Edit `Card.cpp`, adding a function stub for each of the Card member functions in `Card.hpp`.
```c++
Card::Card() {
  assert(false);
}
```

Edit `Card.cpp`, adding  function stubs for each of the non-member non-operator functions in `Card.hpp`.  Here are a few examples.
```c++
Suit Suit_next(Suit suit) {
  assert(false);
}

bool Card_less(const Card &a, const Card &b, Suit trump) {
  assert(false);
}
```

Edit `Card.cpp`, adding  function stubs for each of the overloaded operators in `Card.hpp`.  Here are a few examples.
```c++
std::ostream & operator<<(std::ostream &os, const Card &card) {
  assert(false);
}

bool operator<(const Card &lhs, const Card &rhs) {
  assert(false);
}

```

The Card tests should compile and run.  Expect them to fail at this point because the `Card.cpp` starter code contains function stubs.
```console
$ make Card_public_tests.exe
$ ./Card_public_tests.exe
$ make Card_tests.exe
$ ./Card_tests.exe
```

Configure your IDE to debug either the public tests or your own tests.


## Pack

Write an abstract data type (ADT) for a pack of playing cards.

Write implementations in `Pack.cpp` for the functions declared in
`Pack.hpp`.

Run the public Pack tests.
```console
$ make Pack_public_tests.exe
$ ./Pack_public_tests.exe
```

Write tests for `Pack` in `Pack_tests.cpp` using the unit test framework. While you should write your own tests for `Pack` to ensure that your implementation is correct, you do not have to submit your tests to the autograder.
```console
$ make Pack_tests.exe
$ ./Pack_tests.exe
```



### Default constructor

The default `Pack` constructor initializes a full pack with cards in order from lowest to highest.
```c++
Pack();
```

You can take advantage of an `enum`'s underlying integer representation to loop over all its values.
```c++
for (int s = SPADES; s <= DIAMONDS; ++s) {
  Suit suit = static_cast<Suit>(s);
}
```

### Stream input constructor


A second `Pack` constructor reads a pack from stream input. [Later](#command-line-arguments), your `main` function will open an input file and then call this constructor on the input stream.
### Setup

Create a new file `Pack.cpp`.  Add a function stub for every function prototype in `Pack.hpp`.  Remember to `#include "Pack.hpp"`.

For example:
```c++
#include "Pack.hpp"
void Pack::reset() {
  assert(false);
  assert(next); // DELETEME: avoid error "private field is not used"
}
```

The Pack tests should compile and run.  Expect them to fail at this point because the `Pack.cpp` contains function stubs.
```console
$ make Pack_public_tests.exe
$ ./Pack_public_tests.exe
$ make Pack_tests.exe
$ ./Pack_tests.exe
```


### Reading the Pack

One of the `Pack` constructors reads a pack from stream input. [Later](#interface), your `main` function will open an input file and then call this constructor on the input stream.
```c++
Pack(std::istream& pack_input);
```

The `pack.in` file provided with the project contains an example of the input format, with cards listed in "new pack" order:

```
Nine of Spades
Ten of Spades
Jack of Spades
...
Queen of Diamonds
King of Diamonds
Ace of Diamonds
```


Your implementation may assume any pack input is formatted correctly, with exactly 24
unique and correctly formatted cards.

## Player

We'll implement two players: a simple AI player and a human-controlled player that reads instructions from standard input (`cin`).  We have provided an abstract base class in `Player.hpp`.

### Setup
Create a new file `Player.cpp`.  Add function stubs for the two "vanilla" functions in `Player.hpp`. Remember to `#include "Player.hpp"`. Later, you'll add new classes and member functions using derived classes, which you can ignore for now.
```c++
#include "Player.hpp"
Player * Player_factory(const std::string &name, const std::string &strategy) {
  assert(false);
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
  assert(false);
}
```

The Player tests should compile and run.  Expect them to fail at this point because the `Player.cpp` contains function stubs.
```console
$ make Player_public_tests.exe
$ ./Player_public_tests.exe
$ make Player_tests.exe
$ ./Player_tests.exe
```


### Player factory

Since the specific types of Players are hidden inside `Player.cpp`, we
need to write a *factory function* that returns a pointer to a
`Player` with the correct dynamic type. We also need the pointed-to
objects to stick around after the factory function finishes, so we'll
create the players using *dynamically allocated memory*. The prototype
for `Player_factory` can be found in `Player.hpp`, and the
implementation will go in `Player.cpp`.

```c++
Player * Player_factory(const std::string &name, 
                        const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  // Repeat for each other type of Player
  ...
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}
```

### Simple Player

Implement the Simple Player in `Player.cpp` by creating a class that derives from our `Player` abstract base class.  Override each Player function in `Player.cpp` for the functions declared in `Player.hpp`. 

Much of the strategy for our Simple Player can be implemented using
the comparison functions provided by the Card interface.

The Simple Player should not print any output.

Run the public Simple Player tests.
```console
$ make Player_public_tests.exe
$ ./Player_public_tests.exe
```

Write tests for the Simple Player in `Player_tests.cpp` using the [Unit Test Framework](https://eecs280staff.github.io/unit_test_framework/).  See the [Unit Test Grading](#unit-test-grading) section.
```console
$ make Player_tests.exe
$ ./Player_tests.exe
```


#### Making

In making trump, a Simple Player considers the current cards it holds. 

During the first round, each player receives only 5 cards. A player considering whether to call trump follows this strategy:

*Identify the most common suit in the hand—the suit that appears most frequently among the 5 cards.

*Check for face cards (Jack, Queen, or King) within that suit.

*If the most frequent suit contains at least one face card, the player calls that suit as trump.

*If not, the player passes, choosing not to call trump.

This strategy balances the likelihood of having strong cards in a suit with the presence of high-ranking cards, increasing the chances of successful play in the round.



During round two, a Simple Player picks the suit of the seventh card it received as trump.

#### Pulling cards

When deciding which card to give up while pulling from another player, a Simple Player uses the following strategy:

* Make a list of the lowest card from each suit where there are three cards of the same suit
* Pick the lowest of all list of cards from the previous step.

#### Leading Tricks

When a Simple Player leads a trick, they play the highest non-trump
card in their hand. If they have only trump cards, they play the
highest trump card in their hand.

#### Playing Tricks

When playing a card, Simple Players use a simple strategy that
considers only the suit that was led. A more complex strategy would
also consider the cards on the table.

If a Simple Player can follow suit, they play the highest card that
follows suit. Otherwise, they play the lowest card in their hand.


### Human Player

The Human Player reads input from the human user. You may assume all
user input is correctly formatted and has correct values. You may also
assume the user will follow the rules of the game and not try to
cheat. See [Example With Human Players](#example-with-human-players) for
exact output for a game with a human player.

Implement the Human Player in `Player.cpp` by creating a class that derives from our `Player` abstract base class.  Override each Player function in `Player.cpp` for the functions declared in `Player.hpp`. 

The tests in `Player_tests.cpp` and `Player_public_tests.cpp` test only the Simple Player.  You'll have to wait until you have a working  game driver to test the Human player.



#### Making Trump
When making trump reaches a Human Player, first print the Player's
hand. Then, prompt the user for their decision to pass or order up. The user will
then enter one of the following: "Spades", "Hearts", "Clubs",
"Diamonds", or "pass" to either order up the specified suit or pass.
This procedure is the same for both rounds of making trump.

Sample output code:
```c++
print_hand();
cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
```


#### Playing and Leading Tricks

When it is the Human Player's turn to lead or play a trick, first
print the Player's hand. Then, prompt the user to select a card. The user
will then enter the number corresponding to the card they want to
play.

Sample output code:
```c++
print_hand();
cout << "Human player " << name << ", please select a card:\n";
```


## Teen Do Panch Game

Write the Game in `teendopanch.cpp` following the [EECS 280 Rules](#eecs-280-euchre-rules).

Run the game with three players.
```console
$ make teendopanch.exe
$ ./teendopanch.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple 
```

### Setup
To compile and run the game with three players.
```console
$ make teendopanch.exe
$ ./teendopanch.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple
```

### Interface

The game driver takes several command line arguments, for example:

```console
$ ./teendopanch.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple 
```


Here's a short description of each command line argument.

| `./teendopanch.exe` | Name of the executable |
| `pack.in` | Filename of the pack |
| `noshuffle` | Don't shuffle the deck, or use `shuffle` to turn on shuffling |
| `1` | Points to win the game |
| `Adi` | Name of player 0 |
| `Simple` | Type of player 0 |
| `Barbara` | Name of player 1 |
| `Simple` | Type of player 1 |
| `Chi-Chih` | Name of player 2 |
| `Simple` | Type of player 2 |


Check for each of these errors:
  - There are exactly 12 arguments, including the executable name.
  - Points to win the game is between 1 and 100, inclusive.
  - The shuffle argument is either `shuffle` or `noshuffle`.
  - The types of each of the players are either `Simple` or `Human`.

If there is an error print this message and no other output.  Exit returning a non-zero value from `main`.

```c++
cout << "Usage: teendopanch.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
```

You must also verify the pack input file opens successfully. If it does not,
print this error message and exit returning a non-zero value from `main`.

```c++
// Assume pack_filename is a variable containing
// the specified pack filename from argv
cout << "Error opening " << pack_filename << endl;
```

If the file opens successfully, you may assume it is formatted correctly.

### Design

The  game driver coordinates all the [actions in the game](#playing-the-game).  *Take some time* to design this complex piece of code before beginning.

We recommend writing a `Game` ADT. It should have a constructor that takes in details like the players, points to win, etc. and a public `play()` function.  The `Game` and its helper functions do the hard work, and the `main()` function is simple.

```c++
class Game {
 public:
  // primer-spec-highlight-start
  Game(/* game details */);
  void play();
  // primer-spec-highlight-end
};

int main(int argc, char **argv) {
  // Read command line args and check for errors
  // primer-spec-highlight-start
  Game game(/* game details */);
  game.play();
  // primer-spec-highlight-end
}
```

Next, consider which member data you'll need.  Examples include player pointers, pack of cards, etc.
```c++
class Game {
 public:
  Game(/* ... */);
  void play();

 private:
  // primer-spec-highlight-start
  std::vector<Player*> players;
  Pack pack;
  // ...
  // primer-spec-highlight-end
};
```

Which helper functions would help simplify the `play()` function?  Add these as private member functions.  Functions that shuffle, deal, make trump, and play a hand are a good starting point. 
```c++
class Game {
 public:
  Game(/* ... */);
  void play();

 private:
  std::vector<Player*> players;
  Pack pack;
  // ...

  // primer-spec-highlight-start
  void shuffle();
  void deal(/* ... */);
  void make_trump(/* ... */);
  void play_hand(/* ... */);
  // ...
  // primer-spec-highlight-end
};
```

#### End of game



## Acknowledgments

The original project for Euchre was written by Andrew DeOrio, Fall 2013. The
project was modified to use C++ style object oriented programming and
the specification updated by the Fall 2015 staff.
The project was then ported for the game "Teendopanch" by Saquib Razak in Winter 2025 while he had lost his voice and was not spending any time talking.
The rules for the game were mainly adapted from `https://www.pagat.com/whist/3-2-5.html`
