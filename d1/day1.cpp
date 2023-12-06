// No part 1, only part 2

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

int getFirstDigitFromString(const string& str)
{
    stringstream strNum;
    for(auto ch : str)
    {
        if(ch >= '0' && ch <= '9')
        {
            strNum << ch;
            break;
        }
    }
    return stoi(strNum.str());
}

int main()
{
    fstream file("inputDay1", ios::in);

    vector<string> vec{"zero","one","two","three","four","five","six","seven","eight","nine"};  

    string line;
    int sum=0;
    while(getline(file, line))
    {
        for(int i = vec.size()-1; i >= 0; i--)
        {
            size_t found = line.find(vec[i]);
            while(found != string::npos )
            {
                line.insert(found+1, to_string(i));
                found = line.find(vec[i]);
            }
        }
        stringstream str;
        str << getFirstDigitFromString(line);
        reverse(line.begin(), line.end());
        str << getFirstDigitFromString(line);
        sum += stoi(str.str());
    }

    cout << sum << endl;
    file.close();
    return 0;
}