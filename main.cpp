/**
    Simone Renzo
    MAT. 781616
*/
#include <iostream>
#include <cstdlib>  // exit
#include "cbuffer.h"

// Just for convenience
using namespace std;

// Simple function to test out check feature
bool fun1(int x);

int main() {
	cbuffer<int> cb(10);    // initializing cbuffer with size = 10
    cout << cb << endl;     // testing the empty buffer print

	for(int i = 0; i < 12; i++) {
		cb.add(i);  // filling the buffer with more than size items
	}

	cbuffer<int> cb2 = cb;  // new cbuffer copied from previous
	cout << cb << endl;     // testing print of new cbuffer

	// printing the max and cur size of cb2
	cout << "maxsize: " << cb2.get_max_size()
        << " cursize: " << cb2.get_current_size() << endl;

    // adding two new items in cb2
    cb2.add(69);
    cb2.add(98);

    // printing cb2 to test the new added items
	cout << cb2 << endl;

	// reassignment of items in cb
	cb[2] = 99;
	cb[4] = 101;

	// printing cb to test out the modified items
	cout << cb << endl;
	cout << cb[2] << " " << cb[4] << endl;

	// function pointer
    bool (*funptr)(int) = fun1;
	check(cb, funptr);  // calling check function on cb, with fun1 as pointer

	// printing the max and cur size of cb
	cout << "maxsize: " << cb.get_max_size()
        << " cursize: " << cb.get_current_size() << endl;

    // removing all items in cb
    int cur_size = cb.get_current_size();
	for(int i = 0; i < cur_size; i++) {
        cb.remove();
	}

	// printing the cleaned up cb
	cout << "Cleaned up: " << cb << endl;

	//exit(EXIT_SUCCESS);

	return 0;
}

// Finds if the number passed is greater than 8
bool fun1(int x) {
    if(x > 8)
        return true;
    else
        return false;
}
