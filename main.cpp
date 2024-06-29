#include <iostream>
#include <string>
#include <limits>
#include <sqlite3.h>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <regex>

using namespace std;

#include <iostream>
#include <string>
#include <limits>
#include <regex>

using namespace std;

class Tourists
{
private:
    string Name;
    int Id;
    int PassportNumber;
    string Nationality;
    long long ContactNumber;
    string TempAddress;

    bool validateName(const string& name) {
        regex nameRegex("^[A-Za-z ]+$");
        return regex_match(name, nameRegex);
    }

    bool validateId(int id) {
        return id > 0;
    }

    bool validatePassportNumber(int passportNumber) {
        return passportNumber > 0;
    }

    bool validateNationality(const string& nationality) {
        regex nationalityRegex("^[A-Za-z ]+$");
        return regex_match(nationality, nationalityRegex);
    }

    bool validateContactNumber(long long contactNumber) {
        return contactNumber > 1000000000 && contactNumber <= 9999999999;
    }

    bool validateTempAddress(const string& tempAddress) {
        return !tempAddress.empty();
    }

public:
    void ReadTouristInfo() {
        cout << "....................Enter Tourist Info................" << endl;

        while (true) {
            cout << "Name: ";
            getline(cin, Name);
            if (validateName(Name)) break;
            else cout << "Invalid name. Please enter alphabetic characters only." << endl;
        }

        while (true) {
            cout << "Id: ";
            cin >> Id;
            if (validateId(Id)) break;
            else cout << "Invalid ID. Please enter a positive integer." << endl;
        }

        while (true) {
            cout << "Passport Number: ";
            cin >> PassportNumber;
            if (validatePassportNumber(PassportNumber)) break;
            else cout << "Invalid Passport Number. Please enter a positive integer." << endl;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while (true) {
            cout << "Nationality: ";
            getline(cin, Nationality);
            if (validateNationality(Nationality)) break;
            else cout << "Invalid Nationality. Please enter alphabetic characters only." << endl;
        }

        while (true) {
            cout << "Contact Number: ";
            cin >> ContactNumber;
            if (validateContactNumber(ContactNumber)) break;
            else cout << "Invalid Contact Number. Please enter a 10-digit number." << endl;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while (true) {
            cout << "Address in Nepal: ";
            getline(cin, TempAddress);
            if (validateTempAddress(TempAddress)) break;
            else cout << "Invalid Address. Please enter a non-empty string." << endl;
        }
    }

    void ShowTouristInfo() const {
        cout << endl << "......................Tourist Info.........................." << endl;
        cout << "Name: " << Name << endl;
        cout << "Id: " << Id << endl;
        cout << "Passport Number: " << PassportNumber << endl;
        cout << "Nationality: " << Nationality << endl;
        cout << "Contact Number: " << ContactNumber << endl;
        cout << "Address in Nepal: " << TempAddress << endl;
    }

    string getName() const { return Name; }
    int getId() const { return Id; }
    int getPassportNumber() const { return PassportNumber; }
    string getNationality() const { return Nationality; }
    long long getContactNumber() const { return ContactNumber; }
    string getTempAddress() const { return TempAddress; }
};

class Date
{
private:
    int year;
    int month;
    int day;

public:
    void enterDate(string dateMode)
    {
        cout << "Enter " << dateMode << " Date:" << endl;
        while (true)
        {
            cout << "Year: ";
            cin >> year;
            cout << "Month: ";
            cin >> month;
            cout << "Day: ";
            cin >> day;
            if (validateDate(year, month, day))
                break;
            else
                cout << "Invalid date. Please enter a valid date." << endl;
        }
    }

    void displayDate(string dateMode)
    {
        cout << dateMode << " Date: " << year << "/" << month << "/" << day << endl;
    }

    string getDate() const
    {
        return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    }

    static bool validateDate(int year, int month, int day)
    {
        if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31)
            return false;
        if (month == 2)
        {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            return day <= (isLeap ? 29 : 28);
        }
        if (month == 4 || month == 6 || month == 9 || month == 11)
            return day <= 30;
        return true;
    }
};

class VisaInfo : public Tourists
{
private:
    int VisaId;
    Date EntryDate;
    Date ExpiryDate;

    bool validateVisaId(int visaId)
    {
        return visaId > 0;
    }

public:
    void ReadVisaInfo()
    {
        ReadTouristInfo();
        cout << endl
             << "................................Entry Visa Details............................" << endl;
        while (true)
        {
            cout << "Visa Id Number: ";
            cin >> VisaId;
            if (validateVisaId(VisaId))
                break;
            else
                cout << "Invalid Visa ID. Please enter a positive integer." << endl;
        }

        EntryDate.enterDate("Entry");
        ExpiryDate.enterDate("Expiry");
    }

    void ShowVisaInfo()
    {
        ShowTouristInfo();
        cout << endl
             << "........................Visa Information................................" << endl;
        cout << "Visa Id Number: " << VisaId << endl;
        EntryDate.displayDate("Entry");
        ExpiryDate.displayDate("Expiry");
    }

    // Getters for data
    int getVisaId() const { return VisaId; }
    string getEntryDate() const { return EntryDate.getDate(); }
    string getExpiryDate() const { return ExpiryDate.getDate(); }
};


// Function to create table if it does not exist
bool createTable(sqlite3* db) {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS TOURIST("
        "ID INT PRIMARY KEY NOT NULL, "
        "NAME TEXT NOT NULL, "
        "PASSPORT_NUMBER INT NOT NULL, "
        "NATIONALITY TEXT NOT NULL, "
        "CONTACT_NUMBER LONG NOT NULL, "
        "TEMP_ADDRESS TEXT NOT NULL, "
        "VISA_ID INT NOT NULL, "
        "ENTRY_DATE TEXT NOT NULL, "
        "EXPIRY_DATE TEXT NOT NULL, "
        "STATUS TEXT NOT NULL);";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Function to insert tourist data into the database
bool insertTourist(sqlite3* db, const VisaInfo& tourist) {
    string sql =
        "INSERT INTO TOURIST (ID, NAME, PASSPORT_NUMBER, NATIONALITY, CONTACT_NUMBER, TEMP_ADDRESS, VISA_ID, ENTRY_DATE, EXPIRY_DATE, STATUS) "
        "VALUES (" + to_string(tourist.getId()) + ", '" + tourist.getName() + "', " +
        to_string(tourist.getPassportNumber()) + ", '" + tourist.getNationality() + "', " +
        to_string(tourist.getContactNumber()) + ", '" + tourist.getTempAddress() + "', " +
        to_string(tourist.getVisaId()) + ", '" + tourist.getEntryDate() + "', '" + tourist.getExpiryDate() + "','VALID');";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void printHeader()
{
    cout<<left<< setw(10)<< "ID"
        << setw(20) << "Name"
        << setw(15) << "Passport No"
        << setw(15) << "Nationality"
        << setw(15) << "Contact No"
        << setw(20) << "Temp Address"
        << setw(10) << "Visa ID"
        << setw(15) << "Entry Date"
        << setw(15) << "Expiry Date"
        << setw(15) << "Status" << endl;
        cout << string(143,'-') << endl;
}
// Callback function for SQLite query
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    static const int widths[] = {10, 20, 15, 15, 15, 20, 10, 15, 15, 15}; // Define column widths here
    for (int i = 0; i < argc; i++) {
        cout << left <<  setw(widths[i]) << (argv[i] ? argv[i] : "NULL");
    }
    cout << endl;
    return 0;
}

// Function to update the status of tourists based on expiry date
bool updateStatus(sqlite3* db) {
    // Get current date
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    string currentDate = to_string(currentYear) + "-" +
                         (currentMonth < 10 ? "0" : "") + to_string(currentMonth) + "-" +
                         (currentDay < 10 ? "0" : "") + to_string(currentDay);
    cout<<endl<<"Current date is "<< currentDate<<endl;

    string sql = "UPDATE TOURIST SET STATUS = CASE "
                 "WHEN EXPIRY_DATE < '" + currentDate + "' THEN 'EXPIRED' "
                 "ELSE 'VALID' END;";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}


// Function to read and display all data from the database
bool readAllTourists(sqlite3* db) {
    const char* sql = "SELECT * FROM TOURIST;";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int add_Tourist_Data(sqlite3* db)
{
    VisaInfo tourist;
    tourist.ReadVisaInfo();
    if (!insertTourist(db, tourist)) {
        sqlite3_close(db);
        return 0;
    }
    cout << "Tourist information stored successfully!" << endl;
    cout<< "Added Tourist Data!"<<endl;
    //tourist.showVisaInfo();
    return 1;
}

int show_Tourist_Data(sqlite3* db) {
    cout << "Reading all tourists from the database:" << endl;
        // Update status before displaying
    if (!updateStatus(db)) {
        sqlite3_close(db);
        return 0;
    }
    printHeader();
    if (!readAllTourists(db)) {
        sqlite3_close(db);
        return 0;
    }
    return 1;
}


// Function to drop the TOURIST table if it exists
bool dropTable(sqlite3* db) {
    const char* sql = "DROP TABLE IF EXISTS TOURIST;";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
// Function to clear all data from the TOURIST table
bool clearAllData(sqlite3* db) {
    const char* sql = "DELETE FROM TOURIST;";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool filterTouristsByStatus(sqlite3* db, string status)
{
    cout<<"passed string is: "<< status<<endl;
    string sql = "SELECT * FROM TOURIST WHERE STATUS = '" + status + "';";
    char* errMsg = nullptr;
    printHeader();
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    sqlite3* db;
    int rc = sqlite3_open("tourist.db", &db);

    if (rc)
    {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 0;
    }
    else
    {
        cout << "........................Opened database successfully.........................." << endl<<endl;
    }
    if (!createTable(db)) {
        sqlite3_close(db);
        return 0;
    }
    int choice = 0;
    bool terminate = false;
    do
    {
        cout<<endl<<endl<<"Welcome to Visa Tracking System..........!"<<endl;
        cout<<"Enter your choice...!"<<endl<<endl;
        cout<<"1."<<"Add Tourist info"<<endl;
        cout<<"2."<<"Show Tourist info"<<endl;
        cout<<"3."<<"Clear Tourist info"<<endl;
        cout<<"4."<<"Filter Tourist info"<<endl;
        cout<<"5."<<"Update Tourist info"<<endl;
        cout<<"6."<<"Delete Tourist Table"<<endl;
        cout<<"7."<<"Exit"<<endl<<endl;
        cout<<"Choice: ";
        cin>>choice;
        switch(choice)
        {
        case 1:
            system("cls");
            add_Tourist_Data(db);
            break;
        case 2:
            system("cls");
            show_Tourist_Data(db);
            break;
        case 3:
            system("cls");
            clearAllData(db);
            break;
        case 4:
            system("cls");
            {
                int filterChoice = 0;
                cout<<endl<<"1."<<"Valid"<<endl;
                cout<<"2."<<"Expired"<<endl;
                cin>>filterChoice;
                switch(filterChoice)
                {
                case 1:
                    filterTouristsByStatus(db, "VALID");
                    break;
                case 2:
                    filterTouristsByStatus(db, "EXPIRED");
                    break;
                default:
                    cout<<"Invalid Entry..!"<<endl;
                    break;
                }
                break;
            }
        case 5:
            system("cls");
            cout<<endl<<"Updating is under development..."<<endl;
            break;
        case 6:
            system("cls");
             if (!dropTable(db))
            {
                sqlite3_close(db);
                return 0;
            }
            break;
        case 7:
            terminate = true;
            break;
        default:
            cout<<"Invalid input..!"<<endl;
            break;
        }
    }while(!terminate);

    sqlite3_close(db);
    return 0;
}
