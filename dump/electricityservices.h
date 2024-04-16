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
#include "self_header.h"

using namespace std;

bool Edit_Electricity_Data(const string &file_name, const string &field_one, const string &field_two, const string &field_three, const string &field_four, const string &field_five, const string &field_six, const string &field_seven, const string &field_eight)
{
    ofstream file(file_name, ios::app); // Open file in append mode
    if (!file.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return false;
    }
    file << field_one << "," << field_two << "," << field_three << "," << field_four << "," << field_five << "," << field_six << "," << field_seven << "," << field_eight << endl;
    file.close();
    return true;
}


class ElectricityService
{
private:
    struct ElectricityUsage
    {
        int hour;     // Hour of the day
        double usage; // Electricity usage in kWh
    };

    vector<ElectricityUsage> userElectricityUsage;
    string Uid;
    double electricityCharge;
    double dueElectricityCharges;
    double paidElectricityCharges;
    vector<pair<time_t, double>> electricityUsage; // Pair of timestamp and usage

public:
    ElectricityService(const string &uid) : Uid(uid)
    {
        // Initialize charges and usage
        electricityCharge = 0.0;
        dueElectricityCharges = 0.0;
        paidElectricityCharges = 0.0;
    }
    void RegisterElectricityData()
    {
        // Calculate total charges based on usage
        // double rate = 0.15;                                         // Rate per kWh
        double totalCharges = CalculateTotalCharges(Uid); // Function to calculate total usage from CSV
        // double totalCharges = totalUsage * rate;

        // Read paid charges and calculate pending charges
        double paidCharges = 0 /* Function to read paid charges from CSV */;
        double pendingCharges = totalCharges - paidCharges;

        // Append data to Electricity_Data.csv
        ofstream outFile("Electricity_Data.csv", ios::app);
        if (outFile.is_open())
        {
            outFile << Uid << "," << totalCharges << "," << totalCharges << "," << paidCharges << "," << pendingCharges << endl;
            outFile.close();
            cout << "Electricity data registered for UID: " << Uid << endl;
        }
        else
        {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    // Function to record electricity usage for 24 hours for a single user
    void RecordElectricityUsage(const string &Uid)
    {
        // Seed the random number generator based on the UID
        unsigned seed = 0;
        for (char c : Uid)
        {
            seed += c;
        }
        mt19937 gen(seed);

        // Generate random usage for each hour
        uniform_real_distribution<> dis(0.0, 10.0); // Random usage between 0.0 and 10.0 kWh

        ofstream outFile("electricityusages/" + Uid + "_usage.csv");
        if (outFile.is_open())
        {
            outFile << "Hour,Usage (kWh)\n";
            for (int hour = 0; hour < 24; ++hour)
            {
                double usage = dis(gen); // Generate random usage

                // Store the usage data in the vector and write to the file
                ElectricityUsage usageData = {hour, usage};
                userElectricityUsage.push_back(usageData);
                outFile << hour << "," << usage << "\n";
            }
            outFile.close();
            cout << "Usage data saved to: " << Uid << "_usage.csv\n";
        }
        else
        {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    // Function to calculate total charges based on total usage and rate
    double CalculateTotalCharges(const string &Uid) const
    {
        double totalUsage = 0.0;
        double totalCharges = 0.0;

        // Read City_Profiles.csv to get the company name associated with the Uid
        vector<vector<string>> cityData = readCSV("City_Profiles.csv");
        string companyName;
        for (const auto &row : cityData)
        {
            if (row.size() >= 1 && row[0] == Uid) // Check if Uid matches
            {
                if (row.size() >= 5)
                {
                    companyName = row[4]; // Get the company name from the row
                }
                else
                {
                    cerr << "Error: Incomplete data for UID " << Uid << endl;
                    return 0.0;
                }
                break;
            }
        }

        if (companyName.empty())
        {
            cerr << "Error: Company name not found for UID " << Uid << endl;
            return 0.0;
        }

        // Read Company_Profiles.csv to get the Cid associated with the company name
        vector<vector<string>> companyData = readCSV("Company_Profiles.csv");
        string companyCid;
        bool foundCompanyCid = false;
        for (const auto &row : companyData)
        {
            if (row.size() >= 2 && row[1] == companyName) // Check if company name matches
            {
                companyCid = row[0]; // Get the Cid from the row
                cout<<companyCid<<endl;
                foundCompanyCid = true;
                break;
            }
        }

        if (!foundCompanyCid)
        {
            cerr << "Error: Company CID not found for company name: " << companyName << endl;
            return 0.0;
        }

        // Read the charges data for the specified company CID
        vector<vector<string>> chargesData = readCSV("Electricity_Charges.csv");
        double rate = 0.0;
        for (const auto &row : chargesData)
        {
            if (row.size() >= 2 && row[0] == companyCid)
            {
                try
                {
                    rate = stod(row[1]); // Get the rate from the row
                    break;               // Exit loop after finding the rate
                }
                catch (const std::exception &ex)
                {
                    cerr << "Error converting rate to double: " << ex.what() << endl;
                    return 0.0;
                }
            }
        }

        if (rate == 0.0)
        {
            cerr << "Error: Rate not found for company CID: " << companyCid << endl;
            return 0.0;
        }

        // Read usage data from CSV file
        ifstream inFile("electricityusages/" + Uid + "_usage.csv");
        if (!inFile.is_open())
        {
            cerr << "Error: Unable to open usage file for UID: " << Uid << endl;
            return 0.0;
        }

        // Skip header line
        string line;
        getline(inFile, line);
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string hour_str, usage_str;
            getline(ss, hour_str, ',');
            getline(ss, usage_str, ',');
            try
            {
                double usage = stod(usage_str);
                totalUsage += usage;
            }
            catch (const std::exception &ex)
            {
                cerr << "Error converting usage to double: " << ex.what() << endl;
                inFile.close();
                return 0.0;
            }
        }
        inFile.close();

        totalCharges = totalUsage * rate;

        return totalCharges;
    }

    // Function to update paid electricity charges
    void UpdatePaidElectricityCharges(double charge)
    {
        paidElectricityCharges += charge;
    }

    // Function to get electricity charge
    double GetElectricityCharge() const
    {
        return electricityCharge;
    }

    // Function to get due electricity charges
    double GetDueElectricityCharges() const
    {
        return dueElectricityCharges;
    }

    // Function to get paid electricity charges
    double GetPaidElectricityCharges() const
    {
        return paidElectricityCharges;
    }

    // Function to get electricity usage
    const vector<pair<time_t, double>> &GetElectricityUsage() const
    {
        return electricityUsage;
    }
};