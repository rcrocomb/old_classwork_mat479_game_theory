#include <iostream>
using std::cout;
using std::endl;

#include <stdio.h>

#include <vector>

#include <string.h>     // memset
#include <values.h>     // MAXDOUBLE

#include "simplex_array.h"

simplex_array::simplex_array
(
    const int rows,
    const int cols,
    array_t * const data
):
    num_rows_(rows),
    num_cols_(cols)
{
    array_ = new array_t[rows * cols];

    const size_t array_bytes = rows * cols * sizeof(array_t);
    if ( data != 0 )
    {
        memcpy(array_, data, array_bytes);
    } else {
        memset(array_, 0, array_bytes);
    }
}

simplex_array::~simplex_array(void)
{
    delete[] array_;
}

////////////////////////////////////////////////////////////////////////////////
// Const Constructor and Assignment Operator
////////////////////////////////////////////////////////////////////////////////

simplex_array::simplex_array(const simplex_array &x):
    array_(0),
    num_rows_(x.num_rows_),
    num_cols_(x.num_cols_),
    pivot_row_(x.pivot_row_),
    pivot_col_(x.pivot_col_)
{
    array_ = new array_t[ num_rows_ * num_cols_];
    memcpy(array_, x.array_, num_rows_ * num_cols_ * sizeof(array_t));
}

simplex_array &
simplex_array::operator =(const simplex_array &x)
{
    if (this != &x)
    {
        num_rows_   = x.num_rows_;
        num_cols_   = x.num_cols_;
        pivot_row_  = x.pivot_row_;
        pivot_col_  = x.pivot_col_;

        array_t *temp = new array_t[ num_rows_ * num_cols_];
        delete[] array_;

        array_ = temp;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////////

int
simplex_array::index(const int row, const int col)
{
    okay(row,col);
    return (row * num_cols_ + col);
}

// Slows us down, if that's a concern.  Like using at() for vectors.

void
simplex_array::okay(const int row, const int col)
{
    if ( (row > num_rows_) || (col > num_cols_) || (row < 0) || (col <
0))
    {
        cout << "Out of bounds @ " << row << ": " << col << endl;
        throw new int(0);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////////////////////////

void
simplex_array::set(const int row, const int col, const array_t data)
{
    array_[index(row,col)] = data;
}

array_t
simplex_array::get(const int row, const int col)
{
    return array_[index(row,col)];
}

xy_pair
simplex_array::pick_pivot(void)
{
    int i = 0;  // this will be our pivot column

    // Case I (page 50, 3.5.1)
    //
    // starting with bottom row, check for a positive entry in
    // rightmost column, working up the last column until such a value
    // is found
    int k = num_rows_ - 1;
    array_t b_k;
    for ( ; k >= 0; --k)
    {
        b_k = get(k, num_cols_ - 1);
        if (b_k > 0)
        {
            // found value -b_k
            break;
        }
    }

    if (k >= 0)
    {
        cout << "\n\n\nb_k is in row " << k << " with value " << b_k << endl;
        // Case I
        //
        // choose any negative entry in kth row, a_i0k
        //
        // I pick the rightmost value for now: could allow user choice
        i = num_cols_ - 1;
        for( ; i >= 0; --i)
        {
            if ( get(k, i) < 0)
            {
                // found a_i0k
                break;
            }
        }

        cout << "a_i0k is in column " << i << " with value "
             << get(k,i) << endl;

        if( i == num_cols_ )
        {
            cout << "No negative entry in kth (" << i << ") row!" << endl;
            throw new int(0);
        }

    } else {
        // no negative values in right-most column
        //
        // Case II (p 50, 3.5.2)

        // look for positive entry from the right (why not)
        //
        // It's okay if "w" argument is positive?
        i = num_cols_ - 2;
        array_t c_i0;
        for ( ; i >= 0; --i)
        {
            c_i0 = get( num_rows_ - 1, i);
            if  (c_i0 > 0)
            {
                // found value c_i0
                break;
            }
        }

        if (i < 0)
        {
            cout << "Case II: no positive values in bottom row: "
                 << "Phase 1 COMPLETED.\n";
            // use this special value to signal completion
            return xy_pair(-1,-1);
        }

        cout << "c_i0 is in column " << i << " with value " << c_i0 << endl;
    }

    // pivot in column containing this entry.
    pivot_col_ = i;

    // pivot will be in this column.  For all rows >= k (smaller row indices)
    // for which -b_j / a_i0j is negative (values of b_j are already
    // negative: I don't think adding a negation if necessary), find
    // value closest to 0 (least negative).  This is the pivot: a_i0j0.
    // If there's a tie, any will do.

    int j = 0;
    double smallest = -1E6;
    for ( ; j <= k; j++)
    {
                        // -b_j                 // a_i0j
        double value = get(j, num_cols_ - 1) / get(j, i);
        cout << "At [" << j << "," << i << "] == " << value << endl;
        if ( (value < 0) && (value > smallest))   // closest to 0
        {
            smallest = value;
            pivot_row_ = j;
        }
    }

    // okay, pivot_row_ is now set at correct a_i0j0
    cout << "pivoting @ row " << pivot_row_ << " and column " << pivot_col_
         << " which has value " << get(pivot_row_, pivot_col_) << endl;

    // now have both pivot row and column set internally.  Return
    // them, as well.
    return xy_pair(pivot_row_, pivot_col_);
}

//
// [ p q ]
// [ r s ]
//
// becomes
//
// [ 1/p        q/p]
// [-r/p   s - qr/p]
//
// where "p" is the pivot point (so p -> 1/p)
// where "r" is everything in the same column with "p"
// where "q" is everything in the same row with "p"
// where s is everything else
//
// I've decided not to do the obvious and precalculate 1/p and then
// multiply through, since even though that would save time, it might
// "weird" the floating point results.  Probably not for typical
// values, but with floating point, who knows?  Not me, that's who.
//
// I write some of the value a number of times because I'm too damn
// lazy to make the logic for skipping the "r" and "q" parts (even
// though it's pretty easy).

void
simplex_array::perform_transform(void)
{
    // make duplicate array
    array_t *dupe = new array_t[ num_rows_ * num_cols_];

    const double p = get(pivot_row_, pivot_col_);

    // everything else, "s": just do the whole damn array and fix it
    // up afterward.
    for(int i = 0; i < num_rows_; i++)
    {
        // get r for this row
        const double r = get(i, pivot_col_);
        for(int j = 0; j < num_cols_; j++)
        {
            const double q = get(pivot_row_, j);
            dupe[index(i,j)] = get(i,j) - ((q * r) / p);
        }
    }

    // transform "r" column: transform over "p" since it doesn't matter
    for(int i = 0; i < num_rows_; i++)
    {
        dupe[index(i, pivot_col_)] = -get(i, pivot_col_) / p;
    }

    // transform "q" row: transform over "p" since it doesn't matter
    for(int j = 0; j < num_cols_; j++)
    {
        dupe[index(pivot_row_, j)] = get(pivot_row_, j) / p;
    }

// transform "p": done after "s", "r", and "q" transforms so that it "sticks"
    dupe[index(pivot_row_, pivot_col_)] = 1.0 / p;

    print_array(dupe, num_rows_, num_cols_);

// here's the commit
#if 1
    delete[]array_;
    array_ = dupe;
#else
    delete[] dupe;
#endif
}

simplex_array *
simplex_array::clone(void) const
{
    return new simplex_array(*this);
}

simplex_array *
simplex_array::create(const int rows, const int cols) const
{
    return new simplex_array(rows, cols);
}

ostream &
simplex_array::print(ostream &o) const
{
    char buffer[7];
    for(int i = 0; i < num_rows_; i++)
    {
        for(int j = 0; j < num_cols_; j++)
        {
            sprintf(buffer, "%+5.3f ", array_[ i * num_rows_ + j]);
            o << buffer;
        }
        o << "\n";
    }
    return o;
}

////////////////////////////////////////////////////////////////////////////////
// Utility functions
////////////////////////////////////////////////////////////////////////////////

ostream &
operator <<(ostream &o, const simplex_array &a)
{
    return a.print(o);
}

void
print_array
(
    const array_t *const p,
    const int rows,
    const int cols
)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%+5.3f ", p[ i * rows + j]);
        }
        printf("\n");
    }
    fflush(stdout);
}

