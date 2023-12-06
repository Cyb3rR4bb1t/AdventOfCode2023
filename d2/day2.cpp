#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <exception>

using namespace std;

// Splits a string by *delim* and places its parts into a vector (ignores empty strings).
vector<string> split(const string& recordString, char delim = ';')
{
    vector<string> result;
    stringstream stream;
    stream << recordString;
    for(string line; getline(stream, line, delim); )
        if(!line.empty())
            result.push_back(line);
    return result;
}

// Represents elements from a game set.
struct CubeGameSet
{
    CubeGameSet() = default;

    CubeGameSet(int r, int g, int b)
        :red(r)
        ,green(g)
        ,blue(b)
    {}

    CubeGameSet(const string& setString)
    {
        load(setString);
    }

    void load(const string& setString)
    {
        this->red   = 0;
        this->green = 0;
        this->blue  = 0;

        vector<string> elements = split(setString, ',');
        for(const string& element: elements)
        {
            vector<string> valueColorSplit = split(element, ' ');
            if( valueColorSplit.size() != 2 )
                throw runtime_error("Exception in 'CubeGameSet::load': invalid element data.");

            int valueBuffer    = stoi(valueColorSplit[0]);
            string colorBuffer = valueColorSplit[1];

            if (colorBuffer == "red")
                red   += valueBuffer;
            else if(colorBuffer == "green")
                green += valueBuffer;
            else if(colorBuffer == "blue")
                blue  += valueBuffer;
        }
    }

    CubeGameSet& operator=(const CubeGameSet& right)
    {
        this->red   = right.red;
        this->green = right.green;
        this->blue  = right.blue;
        return *this;
    }

    CubeGameSet operator+(const CubeGameSet& right)
    {
        CubeGameSet result;
        result.red   = this->red   + right.red;
        result.green = this->green + right.green;
        result.blue  = this->blue  + right.blue;
        return result;
    }

    CubeGameSet& operator+=(const CubeGameSet& right)
    {
        this->red   += right.red;
        this->green += right.green;
        this->blue  += right.blue;
        return *this;
    }

    int red=0;
    int green=0;
    int blue=0;
};

class CubeGameInfo
{
public:

    void loadInfo(const string& recordString)
    {
        vector<string> gameSetSplit = split(recordString, ':');
        if( gameSetSplit.size() != 2 )
                throw runtime_error("Exception in 'CubeGameInfo::loadInfo': invalid game data.");

        vector<string> gameIdSplit  = split(gameSetSplit[0], ' ');
        if( gameIdSplit.size() != 2 )
                throw runtime_error("Exception in 'CubeGameInfo::loadInfo': invalid game data.");
        m_id = stoi(gameIdSplit[1]);

        vector<string> sets = split(gameSetSplit[1], ';');
        for(const string& setString : sets)
            m_sets.emplace_back(setString);
    }

    bool isEnoughElements(int red, int green, int blue)
    {
        for(const CubeGameSet& gameSet: m_sets)
            if(gameSet.red > red || gameSet.green > green || gameSet.blue > blue)
                return false;
        return true;
    }

    CubeGameSet getRequiredMinimumElements()
    {
        int maxRed=0;
        int maxGreen=0;
        int maxBlue=0;
        for(const CubeGameSet& set : m_sets)
        {
             maxRed   = max(maxRed, set.red);
             maxGreen = max(maxGreen, set.green);
             maxBlue  = max(maxBlue, set.blue);
        }
        return CubeGameSet(maxRed, maxGreen, maxBlue);
    }

    int getGameID()
    {
        return m_id;
    }

private:

    vector<CubeGameSet> m_sets;
    int m_id=0;
};

int main()
{
    fstream file("input", ios::in);

    int gameIdSum = 0;
    int gamePowerSum = 0;
    for(string line; getline(file, line);)
    {
        CubeGameInfo gameInfo;
        gameInfo.loadInfo(line);
        
        // Part 1
        if( gameInfo.isEnoughElements(12,13,14) )
            gameIdSum += gameInfo.getGameID();

        // Part 2
        CubeGameSet set = gameInfo.getRequiredMinimumElements();
        gamePowerSum += set.red * set.green * set.blue;
    }

    cout << "Sum: " << gameIdSum << endl;
    cout << "Power sum: " << gamePowerSum << endl;

    return 0;
}