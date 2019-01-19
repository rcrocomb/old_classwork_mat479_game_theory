#include <iostream>
using std::cout;
using std::endl;

#include <values.h>         // integer constants for MININT, MAXINT

#include "game.h"

////////////////////////////////////////////////////////////////////////////////
// Utility
////////////////////////////////////////////////////////////////////////////////

void gprint(const std::string &text);

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

int
game::operator[](const unsigned int dex)
{
    return data_[dex];
}

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

// Returns the maximum value in row 'row'

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

ostream &
operator <<(ostream &o, const game &g)
{
    o << "\n";
    for(int i = 0; i < g.rowsize(); i++)
    {
        o << "[ ";
        for(int j = 0; j < g.colsize(); j++)
        {
            o << g.get_value(i,j) << " ";
        }
        o << "]\n";
    }
    return o;
}

