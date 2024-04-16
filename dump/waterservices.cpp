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

bool Edit_Water_Data(const string &file_name, const string &field_one, const string &field_two, const string &field_three, const string &field_four, const string &field_five, const string &field_six, const string &field_seven, const string &field_eight)
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


class WaterService
{
private:
    struct WaterUsage
    {
        int hour;     // Hour of the day
        double usage; // Water usage in kWh
    };

    vector<WaterUsage> userWaterUsage;
    string Uid;
    double waterCharge;
    double dueWaterCharges;
    double paidWaterCharges;
    vector<pair<time_t, double>> waterUsage; // Pair of timestamp and usage

public:
    WaterService(const string &uid) : Uid(uid)
    {
        // Initialize charges and usage
        waterCharge = 0.0;
        dueWaterCharges = 0.0;
        paidWaterCharges = 0.0;
    }
    void RegisterWaterData()
    {
        // Calculate total charges based on usage
        // double rate = 0.15;                                         // Rate per kWh
        double totalCharges = CalculateTotalCharges(Uid); // Function to calculate total usage from CSV
        // double totalCharges = totalUsage * rate;

        // Read paid charges and calculate pending charges
        double paidCharges = 0 /* Function to read paid charges from CSV */;
        double pendingCharges = totalCharges - paidCharges;

        // Append data to Water_Data.csv
        ofstream outFile("Water_Data.csv", ios::app);
        if (outFile.is_open())
        {
            outFile << Uid << "," << totalCharges << "," << totalCharges << "," << paidCharges << "," << pendingCharges << endl;
            outFile.close();
            cout << "Water data registered for UID: " << Uid << endl;
        }
        else
        {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    // Function to record water usage for 24 hours for a single user
    void RecordWaterUsage(const string &Uid)
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

        ofstream outFile("waterusages/" + Uid + "_usage.csv");
        if (outFile.is_open())
        {
            outFile << "Hour,Usage (kWh)\n";
            for (int hour = 0; hour < 24; ++hour)
            {
                double usage = dis(gen); // Generate random usage

                // Store the usage data in the vector and write to the file
                WaterUsage usageData = {hour, usage};
                userWaterUsage.push_back(usageData);
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
        vector<vector<string>> chargesData = readCSV("Water_Charges.csv");
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
        ifstream inFile("waterusages/" + Uid + "_usage.csv");
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

    // Function to update paid water charges
    void UpdatePaidWaterCharges(double charge)
    {
        paidWaterCharges += charge;
    }

    // Function to get water charge
    double GetWaterCharge() const
    {
        return waterCharge;
    }

    // Function to get due water charges
    double GetDueWaterCharges() const
    {
        return dueWaterCharges;
    }

    // Function to get paid water charges
    double GetPaidWaterCharges() const
    {
        return paidWaterCharges;
    }

    // Function to get water usage
    const vector<pair<time_t, double>> &GetWaterUsage() const
    {
        return waterUsage;
    }
};