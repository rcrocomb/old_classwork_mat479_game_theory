// Author: Robert Crocombe
// Class: MAT479
// Professor: Dr. Thakur

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;
using std::dec;
using std::hex;
using std::setprecision;

#include <vector>
using std::vector;

#include <stdlib.h>         // lrand48()
#include <math.h>           // round()

#include "player.h"

////////////////////////////////////////////////////////////////////////////////

player::player(const int num_strategies, const bool is_first):
    first_(is_first),
    cumulative_payoff_(0),
    statistics_()
{
}

player::~player(void)
{

}

////////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////////

// Prints information about what strategies the other player is using.
// First value is strategy number (row or column index), second value
// is count of how many times strategy has been used.

void
player::stat_print(void)
{
    int size = statistics_.size();
    cout << "vector size == " << size << "\n";
    stat_map::iterator i = statistics_.begin();
    for( ; i != statistics_.end(); i++)
    {
        cout << "[" << setw(4) << setfill(' ') << i->first << "] == "
             << i->second << "\n";
    }
    cout << endl;
}

////////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////////

void
player::increment_strategy(const int value)
{
    statistics_[value] += 1;
}

// Here is part of why this only supports 2 players: assumes payoffs
// are symmetrical.

void
player::increment_payoff(const int value)
{
    if( first_ )
    {
        cumulative_payoff_ += value;
    } else {
        cumulative_payoff_ -= value;
    }
}

// Am I stupid, or are the summation notations in the book on page 29
// backwards?  Maybe I just misunderstand them.

int
player::decide_strategy(const game &g)
{
    // adjust if payoffs can legitimately be less than this
    const double BIGNUM = 1E6;
    int strategy = 0;
    double rounds = g.get_rounds();
    if( first_ )    // Player 1
    {
        payoff_pair p(0, -BIGNUM);
        for( int i = 0; i < g.rowsize(); i++)
        {
            double sum = 0;
            for( int j = 0; j < g.colsize(); j++)
            {
                // expected payoff value of a_ij given how often Player 2
                // plays strategy j
                sum += g.get_value(i,j) * ( double(statistics_[j]) / rounds );
            }

            if( sum > p.second) // Find largest value
            {
                p.first = i;
                p.second = sum;
            }
        }
        strategy = p.first;

    } else          // Player 2
    {

        payoff_pair p(0, BIGNUM);
        for( int j = 0; j < g.colsize(); j++)
        {
            double sum = 0;
            for( int i = 0; i < g.colsize(); i++)
            {
                // expected payoff value of a_ij given how often Player 1
                // plays strategy i
                sum += g.get_value(i,j) * ( double(statistics_[i]) / rounds );
            }

            if( sum < p.second) // Find smallest value
            {
                p.first = j;
                p.second = sum;
            }
        }

        strategy = p.first;
    }
    return strategy;
}

// It's in two stages, main guts is above.

int
player::pick_strategy(const game &g)
{
    // randomly pick first strategy
    if( (statistics_.size() == 0) || (g.get_rounds() == 0) )
    {
        if( first_ ) {
            return int(round( drand48() * g.rowsize() ));
        } else {
            return int(round( drand48() * g.colsize() ));
        }
    }

    return decide_strategy(g);
}

////////////////////////////////////////////////////////////////////////////////
// Utility
////////////////////////////////////////////////////////////////////////////////

ostream &
operator<<(ostream &o, const stat_map &m)
{
    stat_map::const_iterator i = m.begin();
    for( ; i != m.end(); i++)
    {
        o << i->first << " == " << i->second << " ";
    }
    return o;
}
ostream &
operator<<(ostream &o, const vector<int> &v)
{
    const int size = v.size();
    for(int i = 0; i < size; i++)
    {
        o << v[i] << " ";
    }
    return o;
}

