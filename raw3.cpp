#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm> // For std::max_element
#include <utility>
#include <random>
#include "electricityservices.h"
#include "waterservices.h"
#include "internetservices.h"
using namespace std;

// Citizens of the city
class Citizen
{
protected:
    string Uid;
    string Name;
    string Address;
    string AadhaarID;
    string ElectricityProvider; // Add electricity provider field

public:
    Citizen(const string &b, const string &c, const string &d, const string &e) : Name(b), Address(c), AadhaarID(d), ElectricityProvider(e) // Constructor
    {
        // Read existing citizen data from CSV file
        vector<vector<string>> csvData = readCSV("City_Profiles.csv");
        // Check for duplicates
        bool isDuplicate = IsDuplicate(csvData);
        if (isDuplicate)
        {
            cout << "Error: This citizen already exists in the database!" << endl;
        }
        else
        {
            // Generate Uid
            GenerateUid(csvData);

            // Register the new citizen
            RegisterCitizen(csvData);
        }
    }

    // Function to check duplicate citizen data
    bool IsDuplicate(const vector<vector<string>> &csvData)
    {
        for (const auto &row : csvData)
        {
            if ((row.size() >= 3 && row[1] == Name && row[2] == Address) || row[3] == AadhaarID)
            {
                return true;
            }
        }
        return false;
    }

    // Function to generate unique UID
    void GenerateUid(const vector<vector<string>> &csvData)
    {
        vector<int> uids;
        bool skipFirstRow = true; // Flag to skip the first row

        for (const auto &row : csvData)
        {
            if (skipFirstRow)
            {
                skipFirstRow = false; // Set the flag to false after skipping the first row
                continue;             // Skip processing the first row
            }

            if (row.size() >= 1) // Ensure the row is not empty
            {
                try
                {
                    uids.push_back(stoi(row[0])); // Convert the first field to integer
                }
                catch (const std::invalid_argument &ia)
                {
                    cerr << "Invalid integer value encountered: " << ia.what() << endl;
                }
                catch (const std::out_of_range &oor)
                {
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
    }

    // Print message
    void RegisterCitizen(const vector<vector<string>> &csvData)
    {
        cout << "A New Citizen is Successfully Registered!\nUid: " << Uid << "\nName: " << Name << "\nAddress: " << Address << "\nAadhaarID: " << AadhaarID << "\nElectricity Provider: " << ElectricityProvider << endl;
        if (!Edit_City_Profiles_Data("City_Profiles.csv", Uid, Name, Address, AadhaarID, ElectricityProvider))
        {
            cerr << "Error: Failed to update City Profiles database!" << endl;
        }
    }
};

class Company
{
private:
    string Cid;
    string Name;
    string Industry;
    string charges;

public:
    Company(const string &name, const string &industry) : Name(name), Industry(industry)
    {
        // Read existing company data from CSV file
        vector<vector<string>> csvData = readCSV("Company_Profiles.csv");
        // Check for duplicates
        bool isDuplicate = IsDuplicate(csvData);
        if (isDuplicate)
        {
            cout << "Error: This company already exists in the database!" << endl;
        }
        else
        {
            // Generate Uid
            GenerateCid(csvData);

            // Register the new company
            RegisterCompany(csvData);
        }
    }

    // Function to check duplicate company data
    bool IsDuplicate(const vector<vector<string>> &csvData)
    {
        for (const auto &row : csvData)
        {
            if ((row.size() >= 2 && row[1] == Name))
            {
                return true;
            }
        }
        return false;
    }

    // Function to generate unique UID
    void GenerateCid(const vector<vector<string>> &csvData)
    {
        vector<int> cids;
        bool skipFirstRow = true; // Flag to skip the first row

        for (const auto &row : csvData)
        {
            if (skipFirstRow)
            {
                skipFirstRow = false; // Set the flag to false after skipping the first row
                continue;             // Skip processing the first row
            }

            if (row.size() >= 1) // Ensure the row is not empty
            {
                try
                {
                    cids.push_back(stoi(row[0])); // Convert the first field to integer
                }
                catch (const std::invalid_argument &ia)
                {
                    cerr << "Invalid integer value encountered: " << ia.what() << endl;
                }
                catch (const std::out_of_range &oor)
                {
                    cerr << "Out of range error: " << oor.what() << endl;
                }
            }
        }

        int maxCid = 0;
        if (!cids.empty())
        {
            maxCid = *max_element(cids.begin(), cids.end());
        }

        // Assign Uid automatically
        maxCid++; // Increment the maximum Uid to generate a new one
        Cid = to_string(maxCid);
    }

    // Function to register a new company
    void RegisterCompany(const vector<vector<string>> &csvData)
    {
        cout << "A New Company is Successfully Registered!\nUid: " << Cid << "\nName: " << Name << "\nIndustry: " << Industry << endl;
        if (!Edit_Company_Profiles_Data("Company_Profiles.csv", Cid, Name, Industry))
        {
            cerr << "Error: Failed to update Company Profiles database!" << endl;
        }
    }
    // Function to enter charges of a company in different CSV files depending on the type of company
    static void EnterCharges(const string &Cid)
    {
        int charges;
        cout << "Enter Rate(Cost per quantity of service): ";
        cin >> charges;
       
        vector<vector<string>> companyData = readCSV("Company_Profiles.csv");
        string industry;

        for (const auto &row : companyData)
        {
            if (row.size() >= 1 && row[0] == Cid) // Check if CID matches
            {
                industry = row[2]; // Get the industry from the row
                break;
            }
        }

        if (!Edit_Charges(industry + "_Charges.csv", Cid, to_string(charges)))
        {
            cerr << "Error: Failed to update Type A charges!" << endl;
        }
    }
};

int main()
{
    string response_charges;
    string response_Cid;
    int password = 123456;
    while (true)
    {
        cout << "-----------LOGIN----------\n";
        char choice;
        cout << "1. Login as a Citizen\n";
        cout << "2. Login as ADMIN\n";
        cin >> choice;
        switch (choice)
        {
        case '1':
            /* code */
            break;
        case '2':
            cout << "Enter PASSWORD:\n";

            int ans;
            cin >> ans;
            if (ans != password)
            {
                cout << "Wrong Password!";
                continue;
            }
            cout << "To create a NEW USER press 1:\nTo check bill status press 2:";
            cin >> choice;
            cin.ignore(); // Clear the newline character from the input buffer

            switch (choice)
            {
            case '1':
            {
                string name, address, aadhaarID, electricityProvider;
                cout << "Name: ";
                getline(cin, name);
                cout << "Address: ";
                getline(cin, address);
                cout << "AadhaarID: ";
                getline(cin, aadhaarID);
                cout << "Electricity Provider: ";
                getline(cin, electricityProvider);
                Citizen C1(name, address, aadhaarID, electricityProvider); // Pass to constructor
                break;
            }
            case '2':
            {
                cout << "1. Update electricity data" << endl;
                cin >> choice;
                switch (choice)
                {
                case '1':
                {
                    string uid_in;
                    cout << "Enter UID: ";
                    cin >> uid_in;

                    // Perform operations to update electricity data for the specified UID
                    // For example, you can create an ElectricityService object with the provided UID
                    ElectricityService electricityService(uid_in);

                    // Update electricity usage or charges for the specified UID
                    cout << "Enter new electricity usage: ";
                    electricityService.RecordElectricityUsage(uid_in);

                    // After updating, register the electricity data
                    electricityService.RegisterElectricityData();

                    double rate = 0.15;
                    double totalCharges = electricityService.CalculateTotalCharges(uid_in);
                    cout << "Total charges: " << totalCharges << " Rupees" << endl;
                    break;
                }
                default:
                    cout << "Invalid choice!" << endl;
                    break;
                }
                break;
            default:
                cout << "Invalid choice!" << endl;
            }
            break;
            case '3':
            {
                // Register a new company
                string name, industry;
                cout << "Name: ";
                getline(cin, name);
                cout << "Industry: ";
                getline(cin, industry);
                Company newCompany(name, industry);
                break;
            }
            case '4':
            {
                cout << "Enter Cid: ";
                cin >> response_Cid;
                Company::EnterCharges(response_Cid);
            }
            }
        default:
            break;
        }
        return 0;
    }
}
