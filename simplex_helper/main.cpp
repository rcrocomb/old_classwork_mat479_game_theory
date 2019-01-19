#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;
using std::setprecision;

#include <fstream>
using std::ofstream;

#include <vector>

#include "simplex_array.h"

int
main
(
    int argc,
    char *argv[]
)
{
    simplex_vector v;
    v.push_back(0);
    v.push_back(-3);
    v.push_back(-3);
    v.push_back(1);
    v.push_back(-4);
    v.push_back(-1);
    v.push_back(-2);
    v.push_back(1);
    v.push_back(2);
    v.push_back(-4);
    v.push_back(-1);
    v.push_back(1);
    v.push_back(-1);
    v.push_back(-1);
    v.push_back(-1);
    v.push_back(0);

    simplex_array a(4,4,&v[0]);
    cout << "Starting table looks like this:\n" << a << "\n\n" << endl;

    xy_pair pivot;
    int count = 0;
    const int BREAK = 150;
    do {

        cout << "\n\nPerforming pivot " << count << "\n\n" << endl;

        pivot = a.pick_pivot();
        if( (pivot.first == -1) && (pivot.second == -1))
        {
            break;
        }

        a.perform_transform();

        ++count;
    } while( count < BREAK);
/*
    print_array(&v[0], 4, 4);

    simplex_array a(4,4,&v[0]);

    cout << "Starting table looks like this:\n" << a << endl;

    xy_pair pivot_1 = a.pick_pivot();
    cout << "Pivot row == " << pivot_1.first << " col == " << pivot_1.second << endl;

    a.perform_transform();

    xy_pair pivot_2 = a.pick_pivot();
    cout << "\n\n2nd Pivot row == " << pivot_2.first << " col == " << pivot_2.second << endl;

    a.perform_transform();

    xy_pair pivot_3 = a.pick_pivot();
    cout << "\n\n3rd Pivot row == " << pivot_3.first << " col == " << pivot_3.second << endl;

    a.perform_transform();
*/
#if 0

    const int ITERATIONS = (argc < 2) ? 200 : atoi(argv[1]);
    ofstream out("data.dat");
    for(int i = 0; i < ITERATIONS; i++)
    {
        //cout << "\n\nRound == " << i << "\n" << endl;
#ifdef DEBUG
        cout << "\nPlayer1 picking strategy:\n";
#endif

        int s1 = p1.pick_strategy(g);

#ifdef DEBUG
        cout << "\nPlayer2 picking strategy:\n";
#endif

        int s2 = p2.pick_strategy(g);

        p1.increment_strategy(s2);
        p2.increment_strategy(s1);

#ifdef DEBUG
        cout << "\nCalculating payoff\n";
#endif

        int payout = g.get_value(s1, s2);

#ifdef PAYOFF
        cout << "P1 == " << s1 << " and P2 == " << s2 << " -> "
             << payout << endl;
#endif

        p1.increment_payoff(payout);
        p2.increment_payoff(payout);
        ++g;

#if 1
    double value = double(p1.get_payoff()) / double(g.get_rounds());
/*
    stat_map m1 = p2.get_strats();
    compute_probabilities(m1,ITERATIONS);
    cout << " | ";
    stat_map m2 = p1.get_strats();
    compute_probabilities(m2,ITERATIONS);
    cout << " value == " << setw(8) << setprecision(6) << value << " ";
    const double TRUE_VALUE = 1.6;
    double oscillation = TRUE_VALUE - value;
    char data[50];
    sprintf(data, "%+12.8f", oscillation);
    cout << data << "\n";
*/
    if( i == 0) { continue; }
    out << setw(10)<< i << " " << setw(12) << setprecision(10) << value << "\n";
#endif

    }

    cout << "\n\n\nCUMULATIVE RESULTS for " << g.get_rounds() << "\n\n\n" << endl;

    double value = double(p1.get_payoff()) / double(g.get_rounds());
    cout << "Value of the game == " << value << endl;

    cout << "\n\nStrategy results are:\n\n";

    cout << "Player 1:\n";
    cout << "raw strategy counts: ";
    stat_map m1 = p2.get_strats();
    cout << "\n" << m1 << endl;
    compute_probabilities(m1,ITERATIONS);
    cout << endl;

    cout << "\n\nPlayer 2:\n";
    cout << "raw strategy counts: ";
    stat_map m2 = p1.get_strats();
    cout << "\n" << m2 << endl;
    compute_probabilities(m2,ITERATIONS);
    cout << endl;
#endif
    return 0;
}
