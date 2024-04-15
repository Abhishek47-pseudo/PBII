#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> // For std::max_element
using namespace std;

// Function to read CSV file and return data as a vector of vectors of strings
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

bool Edit_City_Profiles_Data(const string &file_name, const string &field_one, const string &field_two, const string &field_three, const string &field_four)
{
    ofstream file(file_name, ios::app); // Open file in append mode
    if (!file.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return false;
    }
    file << field_one << "," << field_two << "," << field_three << "," << field_four << endl;
    file.close();
    return true;
}

class Citizen
{
protected:
    string Uid;
    string Name;
    string Address;
    string AadhaarID;

public:
    Citizen(const string &b, const string &c, const string &d) : Name(b), Address(c), AadhaarID(d)
    {
        vector<vector<string>> csvData = readCSV("City_Profiles.csv");

        // Find the maximum Uid in the existing data
        vector<int> uids;
        bool skipFirstRow = true; // flag to skip the first row
        if (!csvData.empty())
        {
            for (const auto &row : csvData)
            {
                if (skipFirstRow)
                {
                    skipFirstRow = false; // Set the flag to false after skipping the first row
                    continue;             // Skipped processing the first row
                }

                // Process each row for Uid
                if (!row.empty() && row.size() >= 1) // Ensure the row is not empty and has at least one field
                {
                    try
                    {
                        uids.push_back(stoi(row[0])); // Convert the first field to integer
                    }
                    catch (const std::invalid_argument &ia)
                    {
                        // Handle invalid argument exception
                        cerr << "Invalid integer value encountered: " << ia.what() << endl;
                    }
                    catch (const std::out_of_range &oor)
                    {
                        // Handle out-of-range exception
                        cerr << "Out of range error: " << oor.what() << endl;
                    }
                }
            }

            int maxUid = 0;
            if (!uids.empty())
            {
                maxUid = *max_element(uids.begin(), uids.end());
            }

            // Assign Uid automatically
            maxUid++; // Increment the maximum Uid to generate a new one
            Uid = to_string(maxUid);

            // Check for duplicates
            bool isDuplicate = false;
            for (const auto &row : csvData)
            {
                if ((row.size() >= 3 && row[1] == Name && row[2] == Address) || row[3] == AadhaarID)
                {
                    isDuplicate = true;
                    break;
                }
            }
            if (isDuplicate)
            {
                cout << "Error: This citizen already exists in the database!" << endl;
            }
            else
            {
                cout << "A New Citizen is Successfully Registered!\nUid: " << Uid << "\nName: " << Name << "\nAddress: " << Address << "\nAadhaarID: " << AadhaarID << endl;
                if (!Edit_City_Profiles_Data("City_Profiles.csv", Uid, Name, Address, AadhaarID))
                {
                    cout << "Error: Failed to update City Profiles database!" << endl;
                }
            }
        }
    }
};

int main()
{
    char choice;
    cout << "To create a NEW USER press 1:\nTo check bill status press 2:";
    cin >> choice;
    cin.ignore(); // Clear the newline character from the input buffer
    if (choice == '1')
    {
        string b, c, d;
        cout << "Name: ";
        getline(cin, b);
        cout << "Address: ";
        getline(cin, c);
        cout << "AadhaarID: ";
        getline(cin, d);
        Citizen C1(b, c, d);
    }
    return 0;
}