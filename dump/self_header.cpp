#include<iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm> // For std::max_element
#include <utility>
#include <random>

using namespace std;

bool Edit_Charges(const string &file_name, const string &cid, const string &charges)
{
    ofstream file(file_name, ios::app); // Open file in append mode
    if (!file.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return false;
    }
    file << cid << "," << charges << endl;
    file.close();
    return true;
}
bool Edit_Company_Profiles_Data(const string &file_name, const string &field_one, const string &field_two, const string &field_three)
{
    ofstream file(file_name, ios::app); // Open file in append mode
    if (!file.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return false;
    }
    file << field_one << "," << field_two << "," << field_three << endl;
    file.close();
    return true;
}
// To read CSV files
vector<vector<string>> readCSV(const string &filename)
{
    ifstream file(filename);
    vector<vector<string>> rows;
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return rows; // Return empty vector
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        vector<string> fields;
        string field;
        while (getline(ss, field, ','))
        {
            fields.push_back(field);
        }
        rows.push_back(fields);
    }
    file.close();
    return rows;
}

// To add data about citizens
bool Edit_City_Profiles_Data(const string &file_name, const string &field_one, const string &field_two, const string &field_three, const string &field_four, const string &field_five)
{
    ofstream file(file_name, ios::app); // Open file in append mode
    if (!file.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return false;
    }
    file << field_one << "," << field_two << "," << field_three << "," << field_four << "," << field_five << endl;
    file.close();
    return true;
}
