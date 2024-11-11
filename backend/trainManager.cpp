#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // for remove() and rename()

using namespace std;

// Function to add a train
void addTrain(const string &trainNo, const string &trainName, const string &availableSeats,
              const string &ticketPrice, const string &fromLocation, const string &toLocation,
              const string &arrivalTime, const string &departureTime) {
    ofstream outFile("train_data.txt", ios::app);
    if (!outFile) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    outFile << "Train," << trainNo << "," << trainName << "," << availableSeats << ","
            << ticketPrice << "," << fromLocation << "," << toLocation << ","
            << arrivalTime << "," << departureTime << endl;
    outFile.close();

    cout << "Train added successfully:\n";
    cout << "Train No: " << trainNo << ", Train Name: " << trainName << ", Available Seats: "
         << availableSeats << ", Ticket Price: " << ticketPrice << ", From: " << fromLocation
         << ", To: " << toLocation << ", Arrival Time: " << arrivalTime
         << ", Departure Time: " << departureTime << endl;
}

// Function to book a ticket
void bookTicket(const string &trainNo, const string &passengerName) {
    ofstream outFile("train_data.txt", ios::app);
    if (!outFile) {
        cerr << "Error opening file for booking." << endl;
        return;
    }

    outFile << "Booking," << trainNo << "," << passengerName << endl;
    outFile.close();

    cout << "Ticket booked for " << passengerName << " on Train No: " << trainNo << endl;
}

// Function to cancel a ticket
void cancelTicket(const string &trainNo, const string &passengerName) {
    ifstream inFile("train_data.txt");
    if (!inFile) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile) {
        cerr << "Error opening temporary file." << endl;
        inFile.close();
        return;
    }

    string line;
    bool canceled = false;

    while (getline(inFile, line)) {
        // Handle format: "Booking,<trainNo>,<passengerName>"
        size_t pos = line.find("Booking,");
        if (pos != string::npos) {
            string bookingTrainNo = line.substr(8, line.find(",", 8) - 8);
            string bookingPassengerName = line.substr(line.find(",", 8) + 1);

            if (bookingTrainNo == trainNo && bookingPassengerName == passengerName) {
                canceled = true;
                continue; // Skip this line to cancel the booking
            }
        }

        tempFile << line << endl;
    }
    inFile.close();
    tempFile.close();

    if (canceled) {
        remove("train_data.txt");
        rename("temp.txt", "train_data.txt");
        cout << "Ticket canceled for " << passengerName << " on Train No: " << trainNo << endl;
    } else {
        remove("temp.txt"); // Cleanup if no ticket was canceled
        cout << "No matching booking found." << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Invalid arguments." << endl;
        return 1;
    }

    string action = argv[1];

    if (action == "addTrain") {
        if (argc != 10) {
            cout << "Insufficient arguments for adding a train." << endl;
            return 1;
        }

        string trainNo = argv[2];
        string trainName = argv[3];
        string availableSeats = argv[4];
        string ticketPrice = argv[5];
        string fromLocation = argv[6];
        string toLocation = argv[7];
        string arrivalTime = argv[8];
        string departureTime = argv[9];

        addTrain(trainNo, trainName, availableSeats, ticketPrice, fromLocation, toLocation, arrivalTime, departureTime);

    } else if (action == "book") {
        if (argc != 4) {
            cout << "Insufficient arguments for booking a ticket." << endl;
            return 1;
        }

        string trainNo = argv[2];
        string passengerName = argv[3];

        bookTicket(trainNo, passengerName);

    } else if (action == "cancel") {
        if (argc != 4) {
            cout << "Insufficient arguments for canceling a ticket." << endl;
            return 1;
        }

        string trainNo = argv[2];
        string passengerName = argv[3];

        cancelTicket(trainNo, passengerName);

    } else {
        cout << "Unknown action. Use 'addTrain', 'book', or 'cancel'." << endl;
    }

    return 0;
}
