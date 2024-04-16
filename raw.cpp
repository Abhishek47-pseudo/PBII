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
        // SetCharges(charges);
        // struct entry
        // {
        //     string Cid;
        //     string charges;
        // };
        // vector<entry> entry_n;
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
        // ofstream outFile("electricityusages/" + industry + "_usage.csv");
        // if (outFile.is_open())
        // {
        //     entry Data = {Cid,string(charges)};
        //     entry_n.push_back(Data);
        //     outFile << Cid << "," << string(charges) << "\n";
        //     outFile.close();
        //     cout << "Usage data saved to: " << industry << "_usage.csv\n";
        // }
        // else
        // {
        //     cerr << "Unable to open file for writing." << endl;
        // }
        if (!Edit_Charges(industry + "_Charges.csv", Cid, to_string(charges)))
        {
            cerr << "Error: Failed to update Type A charges!" << endl;
        }

        // Enter charges for the company
        // companyInstance.EnterCharges();
    }

    // Function to set charges for the company
    // friend void SetCharges(const string &charge)
    // {
    //     charges = charge;
    // }
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
                foundCompanyCid = true;
                break;
            }
        }

        if (!foundCompanyCid)
        {
            cerr << "Error: Company CID not found for company name: " << companyName << endl;
            return 0.0;
        }

        // Read Electricity_Charges.csv to get the rate associated with the Cid
        vector<vector<string>> chargesData = readCSV(companyCid + "_Charges.csv");
        double rate = 0.0;
        if (!chargesData.empty() && chargesData[0].size() >= 2)
        {
            try
            {
                rate = stod(chargesData[0][1]); // Get the rate from the first row
            }
            catch (const std::exception &ex)
            {
                cerr << "Error converting rate to double: " << ex.what() << endl;
                return 0.0;
            }
        }
        else
        {
            cerr << "Error: Electricity charges data not found for company CID: " << companyCid << endl;
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
                    double totalCharges = electricityService.CalculateTotalCharges(to_string(rate));
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
