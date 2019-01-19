// Author: Robert Crocombe
// Class: MAT479
// Professor: Dr. Thakur

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setfill;
using std::setw;

#include <values.h>         // integer constants for MININT, MAXINT

#include "game.h"

////////////////////////////////////////////////////////////////////////////////
// Utility
////////////////////////////////////////////////////////////////////////////////

void gprint(const std::string &text);

////////////////////////////////////////////////////////////////////////////////

game::game
(
    const unsigned int rows,
    const unsigned int cols,
    const std::vector<int> &v
):
    data_(0),
    num_rows_(rows),
    num_cols_(cols),
    plays_(0)
{
    if( (rows * cols) < v.size())
    {
        gprint("array dimensions smaller than game vector size");
        throw new int(0);
    }

    data_ = new int[rows * cols];
    memcpy( data_, &(v[0]), sizeof(int) * rows * cols);
}

game::~game(void)
{
    delete[] data_;
}

int
game::get_value(const unsigned int row, const unsigned int col) const
{
    return data_[index(row,col)];
}

// g++ crapped out when I had this in the header.  Weird.

int
game::operator[](const unsigned int dex)
{
    return data_[dex];
}

// Returns the minimum value in the row 'row', plus the index where it
// was found.  Index is p.first, value is p.second.

game_pair
game::row_minimum(const unsigned int row) const
{
    int min = MAXINT;
    unsigned int dex = 0;
    for(int i = 0; i < num_cols_; i++)
    {
        int d = data_[index(row, i)];
        if( d < min)
        {
            min = d;
            dex = i;
        }
    }
    return game_pair(dex,min);
}

// As above, but for column 'col'.

game_pair
game::col_minimum(const unsigned int col) const
{
    int min = MAXINT;
    unsigned int dex = 0;
    for(int i = 0; i < num_cols_; i++)
    {
        int d = data_[index(i, col)];
        if( d < min)
        {
            min = d;
            dex = i;
        }
    }
    return game_pair(dex,min);
}

// Returns the maximum value in row 'row': index is .first, value is
// .second.

game_pair
game::row_maximum(const unsigned int row) const
{
    int max = MININT;
    unsigned int dex = 0;
    for(int i = 0; i < num_cols_; i++)
    {
        int d = data_[index(row, i)];
        if( d > max)
        {
            max = d;
            dex = i;
        }
    }
    return game_pair(dex,max);
}

// As above, but for column 'col'.

game_pair
game::col_maximum(const unsigned int col) const
{
    int max = MININT;
    unsigned int dex = 0;
    for(int i = 0; i < num_cols_; i++)
    {
        int d = data_[index(i, col)];
        if( d > max)
        {
            max = d;
            dex = i;
        }
    }
    return game_pair(dex,max);
}

// Sum of the payoffs in the column 'col'.

int
game::col_sum(const unsigned int col) const
{
    int sum = 0;
    for(int i = 0; i < num_rows_; i++)
    {
        sum += data_[index(i,col)];
    }
    return sum;
}

// Sum of the payoffs in the row 'row'.

int
game::row_sum(const unsigned int row) const
{
    int sum = 0;
    for(int j = 0; j < num_cols_; j++)
    {
        sum += data_[index(row, j)];
    }
    return sum;
}

// This is kinda gross.  I use the prefix ++ operator to bump the iteration
// count for the game.

game &
game::operator ++(void)
{
    ++plays_;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
// Not in Class
////////////////////////////////////////////////////////////////////////////////

void
gprint(const std::string &text)
{
    cout << GAME << text << endl;
}

// Handy printout routine: displays the payoff values in proper
// rows and columns.  Might have to use setw() for large payoff
// values.

ostream &
operator <<(ostream &o, const game &g)
{
    o << "\n";
    for(int i = 0; i < g.rowsize(); i++)
    {
        o << "[ ";
        for(int j = 0; j < g.colsize(); j++)
        {
            o << setw(4) << setfill(' ') << g.get_value(i,j) << " ";
        }
        o << "]\n";
    }
    return o;
}

