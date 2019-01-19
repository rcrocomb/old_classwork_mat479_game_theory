#ifndef PLAYER_H
#define PLAYER_H

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

private:
    bool first_;
    payoff_t cumulative_payoff_;    // for *other* guy: assume integer
    stat_map statistics_;           // count of which strategies used by
                                    // other player
private:
    int get_max(void);
    int get_random(void);

    void stat_print(void);

    int try1(const game &g);    // max in column
    int try2(const game &g);    // average of column
    int try3(const game &g);    // max in non-zero probability column
    int try4(const game &g);    // play against max probability
    int try5(const game &g);
    int try6(const game &g);
    int try7(const game &g);    // THIS IS IT RIGHT HERE!!!!!!!!11!!!!!
public:
    player(const int num_strategies, const bool is_first);
    ~player(void);

    void increment_strategy(const int value);
    void increment_payoff(const int value);
    int get_payoff(void) const { return cumulative_payoff_; }
    stat_map get_strats(void) const { return statistics_; }

    int pick_strategy(const game &g);
};

ostream &operator<<(ostream &o, const stat_map &m);
ostream &operator<<(ostream &o, const vector<int> &v);

#endif  // PLAYER_H
