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

// returns index of other player's most popular strategy.
//

int
player::get_max(void)
{
    int index = 0;
    int size = statistics_.size();
    for( int i = 0; i < size; i++)
    {
        if( statistics_.count(i) && (statistics_[i] > index))
        {
            index = i;
        }
    }
    return index;
}

// from the strategies that the other player has used, pick one to
// defend against.  Mostly likely to defend against their most chosen
// strategy.

int
player::get_random(void)
{
    vector<int> v;
#undef DEBUG
#ifdef DEBUG
    cout << "Statistics vector is like so:\n";
    stat_print();
#endif

    // For each strategy that other player has used
    int size = statistics_.size();
    for(int i = 0; i < size; i++)
    {
        // Add the index of that strategy to vector v once for each
        // time it has been used
        int count = 0;
        if( statistics_.count(i) )
        {
            count = statistics_[i];
        }

        for(int j = 0; j < count; j++)
        {
            v.push_back(i);
        }
    }

#ifdef DEBUG
    cout << "Random choice vector looks like: " << v << endl;
#endif

    // now generate a random number that is an index into v.  Since
    // strategies that have been used more have more values in the
    // vector, it should be appropriately weighted.
    double rand = drand48();    // [0.0, 1.0)

#ifdef DEBUG
    cout << "Random value == " << setprecision(6) << rand << endl;
#endif

    // round: toward nearest integer, but 0.5 away from 0
    int dex = (int)(rand * v.size());

#ifdef DEBUG
    cout << "Resulted in index of " << dex << " which is strategy "
         << v[dex] << endl;
#endif

    return v[dex];
}

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
#ifdef DEBUG
    cout << "Incrementing strategy " << value << endl;
#endif

    statistics_[value] += 1;
}

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

// This doesn't work, because if a row has a value that is greater
// than all the values in another row, Player 1 will pretty much keep
// trying to get that value, despite the fact that that value may be
// very hard to obtain, like:
// 0 0 0 0 0 0 0 0 0 0 0 4
// 3 3 3 3 3 3 3 3 3 3 3 3
//
// Player 1 will often keep trying for the 4.

int
player::try1(const game &g)
{
    vector<payoff_pair> v;
    int strategy = 0;
    double rounds = g.get_rounds();
if( first_ ) {
    cout << "#rounds == " << rounds
         << " Statistics are like so: " << statistics_ << endl;
    int size = g.colsize();
    for( int j = 0; j < size; j++)
    {
        // probability that player 2 will play this column, based on
        // past play.
        double probability = double(statistics_[j]) / rounds;

        // what is the maximum payoff player 1 can get in this column?
        game_pair p = g.col_maximum(j);
        double max_payoff = p.second;

        cout << "Player I: max in column " << j << " is " << max_payoff << endl;

        // here is the expected payoff for this column
        double expected_payoff = probability * max_payoff;

        // put this value + what strategy we'd have to play to get
        // this in a vector.  If we use push_back(), then the values
        // are like this
        //  v[X] -- X is the column (Player II strategy)
        //  v[X].first -- Player 1 strategy which gives the best
        //  expected payoff against X
        //  v[X].second -- the expected payoff value given that
        //  strategy X is played with probability 'probability' and
        //  given that we counter with strategy v[X].first
        v.push_back(payoff_pair(p.first, expected_payoff));
        cout << "Player I: " << probability << " -> " << expected_payoff << endl;
    }

    // We have found expected payoffs for every strategy of Player II
    // given that he plays them with associated probabilities.  So
    // find the best payoff, and use the strategy that gives us that
    // value.
    double value = -1E6;
    size = v.size();
    for(int j = 0; j < size; j++)
    {
        double payoff = v[j].second;
        if( payoff > value )
        {
           value = payoff;
           strategy = v[j].first;
        }
    }
    cout << "Player I: best value @ " << strategy << " -> " << value << endl;

} else {

    cout << "#rounds == " << rounds << " Statistics are like so: " << statistics_ << endl;
    int size = g.rowsize();
    for( int i = 0; i < size; i++)
    {
        double probability = double(statistics_[i]) / rounds;

        game_pair p = g.row_minimum(i);
        double min_payoff = p.second;
        cout << "Player I: min in row " << i << " is " << min_payoff << endl;
        double expected_payoff = probability * min_payoff;
        v.push_back(payoff_pair(p.first, expected_payoff));

        cout << "Player II: " << probability << " -> " << expected_payoff << endl;
    }

    double value = 1E6;
    size = v.size();
    for(int i = 0; i < size; i++)
    {
        double payoff = v[i].second;
        if( payoff < value )
        {
           value = payoff;
           strategy = v[i].first;
        }
    }

    cout << "Player II: best value @ " << strategy << " -> " << value << endl;

}
    return strategy;
}

int
player::try2(const game &g)
{
#if 0
    return 0;
#else
    vector<payoff_pair> v;
    int strategy = 0;
    double rounds = g.get_rounds();

    if( first_ )
    {
        // Player I
        int size = g.colsize();
        for( int j = 0; j < size; j++)
        {
            int column_sum = g.col_sum(j);
            double probability = double(statistics_[j]) / rounds;
            double expected_column_payoff = probability * column_sum;

            v.push_back( payoff_pair(j, expected_column_payoff));
        }

        // figure out which column had best payoff
        double value = -1E6;
        size = v.size();
        int best_column = 0;
        for(int j = 0; j < size; j++)
        {
            double payoff = v[j].second;
            if( payoff > value )
            {
                value = payoff;
                best_column = v[j].first;
            }
        }

        // found column with best payoff, now pick best value in
        // column, and use strategy that gets us that value
        game_pair p = g.col_maximum(best_column);
        strategy = p.first;
    } else
    {
        // Player II
        int size = g.rowsize();
        // for each row, calculate expected payoff of entire row
        // assuming Player I played strategy that results in the row
        // 'probability' % of the time.
        for( int i = 0; i < size; i++)
        {
            int row_sum = g.row_sum(i);
            double probability = double(statistics_[i]) / rounds;
            double expected_row_payoff = probability * row_sum;

            v.push_back( payoff_pair(i, expected_row_payoff));
        }

        // figure out which row had best (SMALLEST) payoff
        double value = 1E6;
        size = v.size();
        int best_row = 0;
        for(int i = 0; i < size; i++)
        {
            double payoff = v[i].second;
            if( payoff < value )
            {
                value = payoff;
                best_row = v[i].first;
            }
        }

        // found column with best payoff, now pick best value in
        // column, and use strategy that gets us that value
        game_pair p = g.row_minimum(best_row);
        strategy = p.first;

    }

    return strategy;
#endif
}

int
player::try3(const game &g)
{
    payoff_map m;
    int strategy = 0;
    double rounds = g.get_rounds();
if( first_ ) {
    cout << "#rounds == " << rounds
         << " Statistics are like so: " << statistics_ << endl;
    int size = g.colsize();
    for( int j = 0; j < size; j++)
    {
        if( statistics_[j] == 0)
        {
            // no sense defending against strategy that isn't played
            continue;
        }

        // probability that player 2 will play this column, based on
        // past play.
        double probability = double(statistics_[j]) / rounds;

        // what is the maximum payoff player 1 can get in this column?
        game_pair p = g.col_maximum(j);
        double max_payoff = p.second;

        cout << "Player I: max in column is " << max_payoff << endl;

        // here is the expected payoff for this column
        double expected_payoff = probability * max_payoff;

        m[j] = payoff_pair(p.first, expected_payoff);
        cout << "Player I: for strategy " << j << " played with probability "
             << probability << " expected payoff == " << expected_payoff
             << endl;
    }

    // We have found expected payoffs for every strategy of Player II
    // that is played with non-zero probability.  So find the best payoff,
    // and use the strategy that gives us that value.
    double value = -1E6;
    payoff_map::const_iterator j = m.begin();
    for( ; j != m.end() ; j++)
    {
        double payoff = j->second.second;
        if( payoff > value )
        {
           value = payoff;
           strategy = j->second.first;
        }
    }
    cout << "Player I: best value @ " << strategy << " -> " << value << endl;

} else {

    cout << "#rounds == " << rounds << " Statistics are like so: " << statistics_ << endl;
    int size = g.rowsize();
    for( int i = 0; i < size; i++)
    {
        if( statistics_[i] == 0)
        {
            continue;
        }

        double probability = double(statistics_[i]) / rounds;

        game_pair p = g.row_minimum(i);
        double min_payoff = p.second;
        cout << "Player I: min in row is " << min_payoff << endl;
        double expected_payoff = probability * min_payoff;
        m[i] = payoff_pair(p.first, expected_payoff);
        cout << "Player II: for strategy " << i << " played with probability "
             << probability << " expected payoff == " << expected_payoff
             << endl;
    }

    double value = 1E6;
    payoff_map::const_iterator j = m.begin();
    for( ; j != m.end() ; j++)
    {
        double payoff = j->second.second;
        if( payoff < value )
        {
           value = payoff;
           strategy = j->second.first;
        }
    }
    cout << "Player II: best value @ " << strategy << " -> " << value << endl;

}
    return strategy;
}

int
player::try4(const game &g)
{
    payoff_map m;
    int strategy = 0;
if( first_ )
{
    // figure out what strategy other guy plays most often.
    int max_played_strategy = 0;
    int times_played = 0;
    int size = statistics_.size();
    for(int j = 0; j < size; j++)
    {
        if( statistics_[j] > times_played )
        {
            times_played = statistics_[j];
            max_played_strategy = j;
        }
    }

    // now know maximally played strategy.  Figure out best strategy
    // to play against that.
    game_pair p = g.col_maximum(max_played_strategy);
    strategy = p.first;

} else
{
    int max_played_strategy = 0;
    int times_played = 0;
    int size = statistics_.size();
    for(int i = 0; i < size; i++)
    {
        if( statistics_[i] > times_played )
        {
            times_played = statistics_[i];
            max_played_strategy = i;
        }
    }

    // now know maximally played strategy.  Figure out best strategy
    // to play against that.
    game_pair p = g.row_minimum(max_played_strategy);
    strategy = p.first;
}
    return strategy;
}

int
player::try5(const game &g)
{
    payoff_map m;
    int strategy = 0;
    double rounds = g.get_rounds();
if( first_ )
{
    int best_row = 0;
    //int best_col = 0;
    double best_value = -1E6;
    for( int i = 0; i < g.rowsize(); i++)
    {
        for( int j = 0; j < g.colsize(); j++)
        {
            // probability that this column will be played
            double probability = double(statistics_[j]) / rounds;

            double expected_value = probability * g.get_value(i,j);
            if( expected_value > best_value )
            {
                best_row = i;
                //best_col = j; // uhm, what's the point here?
                best_value = expected_value;
            }
        }
    }
    strategy = best_row;
} else
{
    int best_col = 0;
    double best_value = 1E6;

    // for each row
    for( int i = 0; i < g.rowsize(); i++)
    {
        // look at every value in that row
        for( int j = 0; j < g.colsize(); j++)
        {
            // probability that this row will be played
            double probability = double(statistics_[i]) / rounds;

            // find expected value of value at row,column
            double expected_value = probability * g.get_value(i,j);
            if( expected_value < best_value )
            {
                // best column is one with smallest value
                best_col = j;
                best_value = expected_value;
            }
        }
    }
    strategy = best_col;
}
    return strategy;
}

int
player::try6(const game &g)
{
    payoff_map m;
    int strategy = 0;
    double rounds = g.get_rounds();
if( first_ )
{
    // first time through, calculate per-column payoffs
    static vector<int> colsums;
    static bool first = true;
    if( first )
    {
        // for every column
        for(int j = 0; j < g.colsize(); j++)
        {
            // get E(aij)
            int sum = g.col_sum(j);
            colsums.push_back(sum);
        }
        first = false;
    }

    payoff_pair p(0,-1E6);

    // for every column
    for(int j = 0; j < g.colsize(); j++)
    {
        // this is p_i * E(aij)
        double value = double(colsums[j]) * (double(statistics_[j]) / rounds );

        // hang onto the best (biggest) value and the column number
        // that it came from
        if( value > p.second)
        {
            p.first = j;
            p.second = value;
        }
    }

    // use best
    strategy = p.first;

} else {
    // first time through, calculate per-row payoffs
    static vector<int> rowsums;
    static bool first = true;
    if( first )
    {
        for(int i = 0; i < g.rowsize(); i++)
        {
            int sum = g.row_sum(i);
            rowsums.push_back(sum);
        }
        first = false;
    }

    payoff_pair p(0,1E6);
    // for every row
    for(int i = 0; i < g.rowsize(); i++)
    {
        // value = sum of payoffs in column * probability of column
        double value = double(rowsums[i]) * (double(statistics_[i]) / rounds );

        // hang onto the best (smallest) value and the row number
        // that it came from
        if( value < p.second)
        {
            p.first = i;
            p.second = value;
        }
    }
    strategy = p.first;
}
    return strategy;
}

int
player::try7(const game &g)
{
    int strategy = 0;
    double rounds = g.get_rounds();
if( first_ )
{
    payoff_pair p(0,-1E6);
    for( int i = 0; i < g.rowsize(); i++)
    {
        double sum = 0;
        for( int j = 0; j < g.colsize(); j++)
        {
            // expected payoff value of a_ij given how often Player 2
            // plays strategy j
            sum += g.get_value(i,j) * ( double(statistics_[j]) / rounds );
        }

        if( sum > p.second)
        {
            p.first = i;
            p.second = sum;
        }
    }

    strategy = p.first;
} else {
    payoff_pair p(0,1E6);
    for( int j = 0; j < g.colsize(); j++)
    {
        double sum = 0;
        for( int i = 0; i < g.colsize(); i++)
        {
            // expected payoff value of a_ij given how often Player 1
            // plays strategy i
            sum += g.get_value(i,j) * ( double(statistics_[i]) / rounds );
        }

        if( sum < p.second)
        {
            p.first = j;
            p.second = sum;
        }
    }

    strategy = p.first;
}
    return strategy;
}


int
player::pick_strategy(const game &g)
{
    int strategy = -1;

    if( (statistics_.size() == 0) || (g.get_rounds() == 0) )
    {
        if( first_ ) {
            return int(round( drand48() * g.rowsize() ));
        } else {
            return int(round( drand48() * g.colsize() ));
        }
    }

#if 1
    return try7(g);
#else
    // pick which strategy to defend against
    int dex = get_random();
    if( first_ )
    {
        // now you know what strategy of theirs you are defending
        // against.  Maximize your value in this situation.
        game_pair p = g.col_maximum(dex);
#ifdef DEBUG
        cout << "Player 1 picked row " << p.first
             << " trying for value " << p.second << endl;
#endif
        strategy = p.first;
    } else {
        // minimize value of Player one's chosen
        game_pair p = g.row_minimum(dex);
#ifdef DEBUG
        cout << "Player 2 picked column " << p.first
             << " trying for value " << p.second << endl;
#endif
        strategy = p.first;
    }
#endif
    return strategy;
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
