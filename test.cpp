#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    fstream input1, input2;
    input1.open("output1.txt", fstream::in);
    input2.open("output2.txt", fstream::in);

    int n1, n2;
    input1 >> n1;
    input2 >> n2;
    int o1, o2;
    o1 = o2 = 0;

    while(!input1.eof() && !input2.eof())
    {
        if(o1 > n1 || o2 > n2)
        {
            cout << "DIFFERENT" << endl;
            return 1;
        }
        o1 = n1;
        o2 = n2;
        input1 >> n1;
        input2 >> n2;
    }
    cout << "GOOD" << endl;
    return 0;
}
