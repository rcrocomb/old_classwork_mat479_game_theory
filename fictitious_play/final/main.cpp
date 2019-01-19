// Author: Robert Crocombe
// Class: MAT479
// Professor: Dr. Thakur
//
// Demonstrate how "Solution by Ficticious Play" can work.

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;
using std::setprecision;

#include <fstream>
using std::ofstream;
using std::ifstream;

#include <vector>

#include <unistd.h>             // getopt
#include <values.h>

#include "game.h"
#include "player.h"

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

void compute_probabilities(const stat_map &m, const int c);

////////////////////////////////////////////////////////////////////////////////

int
main
(
    int argc,
    char *argv[]
)
{
    extern char *optarg;

    int iterations = 200;
    double threshold = 0.0;
    char *filename_p = 0;

    // Don't use -n and -t at the same time.
    while(1)
    {
        int c = getopt(argc, argv, "f:n:t:");
        if( c == -1)
        {
            break;
        }

        switch(c) {
        case 'f':               // -f for filename
            filename_p = optarg;
            break;

        case 'n':               // -n to change iterations
            iterations = atoi(optarg);
            break;

        case 't':               // -t to change convergence threshold
            threshold = atof(optarg);
            iterations = MAXINT;// better hope it converges!
            break;

        case '?':
            return 0;

        default:
            cout << "Uh-oh: weirdness with getopt: '" << char(c) << "'" << endl;
            return 0;
        }
    }

    std::vector<int> v;
    unsigned int rows = 0, cols = 0;
    if( filename_p != 0)
    {
        ifstream in(filename_p);
        if( !in.good()) {
            cout << "Error opening input file " << filename_p << endl;
            return 0;
        }

        in >> rows;
        in >> cols;

        cout << "#rows == " << rows << " #cols == " << cols << endl;

        while(1)
        {
            int data;
            in >> data;

            if( in.eof())
            {
                //cout << "Hit EOF" << endl;
                break;
            }

            v.push_back(data);
        }

        if( v.size() < (rows*cols) )
        {
            cout << "Insufficient data" << endl;
            return 0;
        } else if( v.size() > (rows*cols)) {
            cout << "Ignoring additional data" << endl;
        }
    } else {
        // our default test array
        rows = 2;
        cols = 2;
        v.push_back(0);
        v.push_back(4);
        v.push_back(2);
        v.push_back(1);
    }

    if( threshold == 0)
    {
        cout << "Running for " << iterations << " iterations" << endl;
    } else {
        cout << "Threshold for convergence is at " << threshold << endl;
    }

    game g(rows, cols, v);

    cout << "Game payoffs look like this:" << g << endl;

    cout << "\n\n\n";

    player p1(2,true);
    player p2(2,false);

    ofstream out("data.dat");
    double old_value = 0.0;
    for( int i = 0 ; i < iterations; i++)
    {
        int s1 = p1.pick_strategy(g);
        int s2 = p2.pick_strategy(g);

        p1.increment_strategy(s2);
        p2.increment_strategy(s1);

        int payout = g.get_value(s1, s2);
        p1.increment_payoff(payout);
        p2.increment_payoff(payout);
        ++g;

        double value = double(p1.get_payoff()) / double(g.get_rounds());
#define GNUPLOT_FORMAT
#ifdef GNUPLOT_FORMAT
        if( i == 0) { continue; }
        out << setw(10)<< i << " "
            << setw(12) << setprecision(10) << value << "\n";
#else
        stat_map m1 = p2.get_strats();
        compute_probabilities(m1,g.get_rounds());
        cout << " | ";
        stat_map m2 = p1.get_strats();
        compute_probabilities(m2,g.get_rounds());
        cout << " value == " << setw(8) << setprecision(6) << value << " ";
        if( filename_p == 0) {
            // kinda cheating here: only for the 0 4 2 1 array
            const double TRUE_VALUE = 1.6;
            double oscillation = TRUE_VALUE - value;
            char data[50];
            sprintf(data, "%+12.8f", oscillation);
            cout << data << "\n";
        } else {
            cout << "\n";
        }
#endif

        if( (threshold != 0) && (fabs(value - old_value) < threshold))
        {
            double diff = fabs(value - old_value);
            cout << "\nCrossed threshold of convergence @ " << threshold
                 << "\nIteration == " << i
                 << "\nAbsolute difference == "
                 << setprecision(8) << diff << endl;
            break;
        }

        old_value = value;
    }

    cout << "\n\n\nCUMULATIVE RESULTS for " << g.get_rounds()
         << " iterations\n\n\n" << endl;

    double value = double(p1.get_payoff()) / double(g.get_rounds());
    cout << "Value of the game == " << value << endl;

    cout << "\n\nStrategy results are:\n\n";

    cout << "Player 1:\n";
    cout << "raw strategy counts: ";
    stat_map m1 = p2.get_strats();
    cout << "\n" << m1 << endl;
    compute_probabilities(m1,g.get_rounds());
    cout << endl;

    cout << "\n\nPlayer 2:\n";
    cout << "raw strategy counts: ";
    stat_map m2 = p1.get_strats();
    cout << "\n" << m2 << endl;
    compute_probabilities(m2,g.get_rounds());
    cout << endl;

    return 0;
}

void
compute_probabilities(const stat_map &m, const int c)
{
    stat_map::const_iterator i = m.begin();
    for( ; i != m.end(); i++)
    {
        cout << "[" << setw(4) << setfill(' ') << i->first
             << "] == " << setprecision(4) << double(i->second) / double(c)
             << "\t";
    }
}
