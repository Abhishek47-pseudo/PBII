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

    // Function to record electricity usage for 24 hours for a single user
    void RecordElectricityUsage(string Uid)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 10.0); // Random usage between 0.0 and 10.0 kWh

        for (int hour = 0; hour < 24; ++hour)
        {
            double usage = dis(gen); // Generate random usage
            // Create an ElectricityUsage object and store it in the vector
            ElectricityUsage usageData = {hour, usage};
            userElectricityUsage.push_back(usageData);
        }

        ofstream outFile("electricityusages/" + Uid + "_usage.csv");
        if (outFile.is_open())
        {
            outFile << "Hour,Usage (kWh)\n";
            for (const auto &data : userElectricityUsage)
            {
                outFile << data.hour << "," << data.usage << "\n";
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
    double CalculateTotalCharges(double rate) const
    {
        double totalUsage = 0.0;

        // Read usage data from CSV file
        ifstream inFile("electricityusages/" + Uid + "_usage.csv");
        if (inFile.is_open())
        {
            string line;
            // Skip header line
            getline(inFile, line);
            while (getline(inFile, line))
            {
                stringstream ss(line);
                string hour_str, usage_str;
                getline(ss, hour_str, ',');
                getline(ss, usage_str, ',');
                double usage = stod(usage_str);
                totalUsage += usage;
            }
            inFile.close();
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }

        return totalUsage * rate;
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

    // Function to register electricity data
    void RegisterElectricityData()
    {
        // Your logic for registering electricity data
    }
};

int main()
{
    char choice;
    cout << "To create a NEW USER press 1:\nTo check bill status press 2:";
    cin >> choice;
    cin.ignore(); // Clear the newline character from the input buffer

    switch (choice)
    {
    case '1':
    {
        string name, address, aadhaarID, electricityProvider; // Add electricity provider field
        cout << "Name: ";
        getline(cin, name);
        cout << "Address: ";
        getline(cin, address);
        cout << "AadhaarID: ";
        getline(cin, aadhaarID);
        cout << "Electricity Provider: ";                          // Prompt for electricity provider
        getline(cin, electricityProvider);                         // Read electricity provider
        Citizen C1(name, address, aadhaarID, electricityProvider); // Pass electricity provider to constructor
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
            // You can prompt the user to enter new data or perform calculations as needed
            // For example:
            cout << "Enter new electricity usage: ";
            electricityService.RecordElectricityUsage(uid_in);

            // After updating, register the electricity data
            electricityService.RegisterElectricityData();
            double rate = 0.15;
            double totalCharges = electricityService.CalculateTotalCharges(rate);
            cout << "Total charges: $" << totalCharges << endl;
            break;
        }
        default:
            cout << "Invalid choice!" << endl;
            break;
        }
        break;
    }
    break;
    default:
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
