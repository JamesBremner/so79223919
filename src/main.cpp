#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <cmath>
#include "cRunWatch.h"


struct numb
{
    int value;
    bool op1, op2;
    numb(int v)
        : value(v),
          op1(false), op2(false)
    {
    }
};

struct op1
{
    static int reduction(const numb &n);
    static void apply(numb &n);
};
struct op2
{
    static int reduction(const numb &n);
    static void apply(numb &n);
};

std::vector<numb> theNumbers;
int kop1, kop2;
int theK;

int op1::reduction(const numb &n)
{
    if (n.op1)
        return 0;
    if (kop1 <= 0)
        return 0;
    return n.value - ceil(n.value / 2);
}
void op1::apply(numb &n)
{
    //std::cout << "op1 " << n.value << " -> ";
    if (n.op1)
        return;
    if (kop1 <= 0)
        return;
    n.value = ceil(n.value / 2);
    n.op1 = true;
    kop1--;
    //std::cout << n.value << "\n";
}
int op2::reduction(const numb &n)
{
    if (n.op2)
        return 0;
    if (kop2 <= 0)
        return 0;
    if (n.value < theK)
        return 0;
    return theK;
}
void op2::apply(numb &n)
{
    //std::cout << "op2 " << n.value << " -> ";
    if (n.op2)
        return;
    if (kop2 <= 0)
        return;
    if (n.value < theK)
        return;
    n.value -= theK;
    n.op2 = true;
    kop2--;
     //std::cout << n.value << "\n";
}

void gen1(int count)
{
    srand(time(NULL));
    for (int k = 0; k < count; k++)
    {
        theNumbers.emplace_back(rand() % 100 + 1);
    }
    theK = 5;
    kop1 = count / 2;
    kop2 = count / 2;
}

void gen2()
{
    theNumbers = { 2,8,3,19,3 };
    theK = 3;
    kop1 = 1;
    kop2 = 1;
}

void solve()
{
    // keep looping until no more ops possible
    for (;;)
    {
        // find largest reduction possible with one operation
        int bestReduction = 0;
        numb &bestNumber = theNumbers[0];
        int bestOp = 0;
        for (auto &n : theNumbers)
        {
            int r = op1::reduction(n);
            if (r > bestReduction)
            {
                bestReduction = r;
                bestNumber = n;
                bestOp = 1;
            }
            r = op2::reduction(n);
            if (r > bestReduction)
            {
                bestReduction = r;
                bestNumber = n;
                bestOp = 2;
            }
        }
        // no reduction possible - break out of loop
        if (!bestReduction)
            break;

        // spply op that gives biggest reduction
        switch (bestOp)
        {
        case 1:
            op1::apply(bestNumber);
            break;
        case 2:
            op2::apply(bestNumber);
            break;
        }
    }

    // calculate sum of numbers
    int sum = 0;
    for (auto &n : theNumbers)
        sum += n.value;

    std::cout << "Minimum sum " << sum << "\n";
}

// performance test
void performance()
{
    // start timing profiler
    raven::set::cRunWatch::Start();
    gen1(1000);
    {
        raven::set::cRunWatch aWatcher("N=1000");
        solve();
    }

    gen1(10000);
    {
        raven::set::cRunWatch aWatcher("N=10000");
        solve();
    }

    gen1(100000);
    {
        raven::set::cRunWatch aWatcher("N=100000");
        solve();
    }

    // display timing profiler report
    raven::set::cRunWatch::Report();
}


main()
{
    gen2();
    solve();

    performane();

    return 0;
}
