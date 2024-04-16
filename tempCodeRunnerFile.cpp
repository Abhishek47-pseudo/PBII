
                    cout << "Enter UID: ";
                    cin >> uid_in;

                    // Perform operations to update water data for the specified UID
                    // For example, you can create an WaterService object with the provided UID
                    WaterService waterService(uid_in);

                    // Update water usage or charges for the specified UID
                    cout << "Enter new water usage: ";
                    waterService.RecordWaterUsage(uid_in);

                    // After updating, register the water data
                    waterService.RegisterWaterData();

                    double rate = 0.15;
                    double totalCharges = waterService.CalculateTotalCharges(uid_in);
                    cout << "Total charges: " << totalCharges << " Rupees" << endl;
                    break;