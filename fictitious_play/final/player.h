#ifndef PLAYER_H
#define PLAYER_H

// Author: Robert Crocombe
// Class: MAT479
// Professor: Dr. Thakur
//
// Implements the notion of a "player" of a game.  Supports only 2
// player games (trivially fixable, but not done here).

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <vector>
using std::vector;

#include <fstream>
using std::ostream;

#include "game.h"

namespace
{
    typedef map<int,int> stat_map;
    typedef pair<int, double> payoff_pair;
    typedef map<int, payoff_pair> payoff_map;
}

class player
{

typedef int payoff_t;

private:    // data

    bool first_;
    payoff_t cumulative_payoff_;    // for *other* guy: assume integer
    stat_map statistics_;           // count of which strategies used by
                                    // other player
private:    // internal routines

    void stat_print(void);
    int decide_strategy(const game &g);

public:     // interface

    player(const int num_strategies, const bool is_first);
    ~player(void);

    void increment_strategy(const int value);
    void increment_payoff(const int value);
    int get_payoff(void) const { return cumulative_payoff_; }
    stat_map get_strats(void) const { return statistics_; }

    int pick_strategy(const game &g);
};

// handy printing routines
ostream &operator<<(ostream &o, const stat_map &m);
ostream &operator<<(ostream &o, const vector<int> &v);

#endif  // PLAYER_H

