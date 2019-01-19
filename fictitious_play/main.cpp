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

#include "game.h"
#include "player.h"

void compute_probabilities(const stat_map &m, const int c);

int
main
(
    int argc,
    char *argv[]
)
{
    std::vector<int> v;
#if 0
    v.push_back(1);
    v.push_back(5);
    v.push_back(3);
    v.push_back(2);
#else
    v.push_back(0);
    v.push_back(4);
    v.push_back(2);
    v.push_back(1);
#endif

    game g(2,2,v);

    cout << "Game payoffs look like this:" << g << endl;

    cout << "\n\n\n";
#if 0
    cout << "1st row max: " << g.row_maximum(0).second << endl;
    cout << "2nd row max: " << g.row_maximum(1).second << endl;

    cout << "1st row min: " << g.row_minimum(0).second << endl;
    cout << "2nd row min: " << g.row_minimum(1).second << endl;

    cout << "1st col max: " << g.col_maximum(0).second << endl;
    cout << "2nd col max: " << g.col_maximum(1).second << endl;

    cout << "1st col min: " << g.col_minimum(0).second << endl;
    cout << "2nd col min: " << g.col_minimum(1).second << endl;
#endif



    player p1(2,true);
    player p2(2,false);
#undef DEBUG
#undef PAYOFF

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
