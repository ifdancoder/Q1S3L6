#include "pcout.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

mutex pcout::mutexCout;
mutex mHouse;

void func1() {
    pcout{} << "First thread" << endl;
}

void func2() {
    pcout{} << "Second thread" << endl;
}

void func3(int n=0) {
    pcout{} << "Third thread, loop is " << n << endl;
}

bool isSimple(long long n) {
    for (long long i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

void nPrimeThread(long long n, short& perc, long long& result, bool& isReady) {
    long long i = 0;
    while (i < n) {
        if (isSimple(result)) {
            i++;
            perc = (double) i / n * 100;
        }
        result++;
    }
    --result;
    isReady = true;
}

long long nPrime(long long n) {
    long long result = 2;
    short perc = 0;
    bool isReady = false;
    thread t{ nPrimeThread, ref(n), ref(perc), ref(result), ref(isReady) };
    while (!isReady)
    {
        system("cls");
        cout << "Progress: " << perc << "%" << endl;
        this_thread::sleep_for(200ms);
    }
    t.join();

    system("cls");
    cout << "Progress: " << perc << "%" << endl;
    return result;
}

void master(vector<int>& things) {
    while (!things.empty()) {
        mHouse.lock();

        int newThing = rand() % 100 + 1;
        things.push_back(newThing);

        cout << "The Master came home and brought a thing with a price of: " << newThing << endl;
        cout << "Now the house has things with prices: " << endl;
        copy(things.begin(), things.end(), ostream_iterator<int>{cout, " "});
        cout << endl << endl;
        mHouse.unlock();
        this_thread::sleep_for(1500ms);
    }
}

void thief(vector<int>& things) {

    while (!things.empty()) {
        mHouse.lock();
        vector<int>::iterator highPriceThing = max_element(things.begin(), things.end());
        cout << "A thief came to the house and stole a thing with a price: " << *highPriceThing << endl;
        things.erase(highPriceThing);
        cout << "Now the house has things with prices: " << endl;
        copy(things.begin(), things.end(), ostream_iterator<int>{cout, " "});
        cout << endl << endl;
        mHouse.unlock();
        this_thread::sleep_for(1s);
    }

}

int main()
{
    srand(time(0));
    {
        // #2
        cout << nPrime(50000) << endl;
    }
    
    {
        // #1
        for (int i = 0; i < 20; i++) {
            thread t1{ func1 };
            thread t2{ func2 };
            thread t3{ func3, ref(i) };

            t1.join();
            t2.join();
            t3.join();
            pcout{} << endl;
        }
    }

    {
        // #3
        vector<int> things = { 19,25,36,99 };

        thread mt(master, ref(things));
        thread tt(thief, ref(things));
        mt.join();
        tt.join();
        cout << "The thief stole all the things from the house." << endl;
    }
    return 0;
}