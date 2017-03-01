#include <iostream>
#include "DateTime.h"

using namespace std;

int main() {

    DateTime day = DateTime(2016, 2,27,15,44,23);

    cout << day.Date() << endl;
    day.addDays(1);
    cout << day.Date() << endl;

    return 0;
}