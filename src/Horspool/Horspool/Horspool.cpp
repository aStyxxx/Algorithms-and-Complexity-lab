#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

string readLineFromFile(const string& filename) 
{
    ifstream file(filename);

    if (!file.is_open()) 
    {
        cerr << "Error: can't open file " << filename << "\n";
        exit(1);
    }

    string line;
    getline(file, line);

    if (line.size() >= 3 &&
        (unsigned char)line[0] == 0xEF &&
        (unsigned char)line[1] == 0xBB &&
        (unsigned char)line[2] == 0xBF)
    {
        line.erase(0, 3);
    }

    if (!line.empty() && line.back() == '\r') line.pop_back();

    return line;
}

vector<char> readCharsFromFile(const string& filename) 
{
    ifstream file(filename);

    if (!file.is_open()) 
    {
        cerr << "Error: can't open file " << filename << "\n";
        exit(1);
    }

    vector<char> chars;
    char c;
    while (file >> c) 
    {
        chars.push_back(c);
    }

    return chars;
}

vector<int> buildShiftTable(const string& pattern) {
    int m = (int)pattern.length();
    vector<int> Table(256, m);

    for (int j = 0; j <= m - 2; j++)
    {
        Table[(unsigned char)pattern[j]] = m - 1 - j;
    }

    return Table;
}

void horspoolSearch(const string& text, const string& pattern, const vector<char>& chars) 
{
    long long matches = 0;
    long long alignments = 0;

    int n = (int)text.size();
    int m = (int)pattern.size();

    if (m == 0 || n < m) {
        return;
    }

    vector<int> table = buildShiftTable(pattern);

    int i = m - 1; // last character
    while (i <= n - 1)
    {
        alignments++;
             
        int k = 0; // number of matched characters
        while (k <= m - 1 && pattern[m - 1 - k] == text[i - k])
        {
            k++;
        }
        
        if (k == m) 
        { 
            matches++;
        }
    
        i += table[(unsigned char)text[i]];
    }

    cout << matches << "\n";
    cout << alignments << "\n";
    for (size_t idx = 0; idx < chars.size(); idx++)
    {
        cout << table[(unsigned char)chars[idx]] << (idx + 1 < chars.size() ? " " : "");
    }
    cout << "\n";
}

int main()
{
    string text = readLineFromFile("../../../data/text.txt");
    string pattern = readLineFromFile("../../../data/pattern.txt");
    vector<char> chars = readCharsFromFile("../../../data/chars.txt");

    horspoolSearch(text, pattern, chars);

    return 0;
}

