#ifndef SIMPLEX_ARRAY_H
#define SIMPLEX_ARRAY_H

#include <utility>
#include <vector>

#include <fstream>
using std::ostream;

namespace
{
    typedef std::pair<int,int>  xy_pair;
    typedef double array_t;
    typedef std::vector<array_t> simplex_vector;
}

class simplex_array
{

private:

    array_t *array_;
    int num_rows_;
    int num_cols_;

    int pivot_row_;        // index of pivot
    int pivot_col_;

private:
    inline int index(const int row, const int col);
    void okay(const int row, const int col);


public:
    simplex_array(const int rows, const int cols, array_t * const data = 0);
    simplex_array(const int rows, const int cols, const simplex_vector &v);
    simplex_array(const simplex_array &x);
    simplex_array &operator =(const simplex_array &x);
    ~simplex_array(void);

    void set(const int row, const int col, const array_t data);
    array_t get(const int row, const int col);

    xy_pair pick_pivot(void);
    void perform_transform(void);


    simplex_array *clone(void) const;
    simplex_array *create(const int rows, const int cols) const;

    ostream &print(ostream &o) const;
};

ostream &operator <<(ostream &o, const simplex_array &a);
void print_array(const array_t *const p, const int rows, const int cols);

#endif  // SIMPLEX_ARRAY_H
