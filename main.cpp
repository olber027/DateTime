#include <iostream>
#include "DateTime.h"

using namespace std;

int main() {

    DateTime day = DateTime::Now();

    cout << day.Date() << endl;
    return 0;
}