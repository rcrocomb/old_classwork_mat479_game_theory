#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

#include <utility>

#include <fstream>
using std::ostream;

namespace
{
    const std::string GAME("Game: ");

    typedef std::pair<int,int> game_pair;
}

class game
{

private:

    int *data_;
    int num_rows_;
    int num_cols_;
    int plays_;

private:
    int index(const unsigned int row,
              const unsigned int col) const { return row * num_rows_ + col; }

public:
    game(const unsigned int rows,
         const unsigned int cols,
         const std::vector<int> &v);
    ~game(void);

    int get_value(const unsigned int row, const unsigned int col) const;
    int rowsize(void) const { return num_cols_; }
    int colsize(void) const { return num_rows_; }
    int operator[](const unsigned int index);

    // these are the *values*
    game_pair row_minimum(const unsigned int row) const;
    game_pair col_minimum(const unsigned int col) const;
    game_pair row_maximum(const unsigned int row) const;
    game_pair col_maximum(const unsigned int col) const;

    int col_sum(const unsigned int col) const;
    int row_sum(const unsigned int row) const;

    game &operator ++(void);

    int get_rounds(void) const { return plays_; }

};

ostream &operator <<(ostream &o, const game &g);

#endif  // GAME_H

