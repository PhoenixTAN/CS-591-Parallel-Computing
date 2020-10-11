#include<utility>
#include<iostream>

int f1( int x ) {
    return x + 3;
}

int f2( int& x ) {
    return x + 4;
}

int f3( int* x ) {
    return *x + 5;
}

int f4( int&& x ) {
    return x + 6;
}   


int main( void ) {
    int x = 3;
    int a = f1(x);
    int b = f2(x);
    int c = f3(&x);
    int d = f4(std::move(x));
    return a + b + c+ d;
}


// g++ -std=c++17 -S calling_conventions.cpp