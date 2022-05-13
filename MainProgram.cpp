#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <cstdlib>
#include <limits>

using namespace std;

#define nameL 26
#define addressL 50 
#define locationL 30
#define symptomL 50
#define MAX_NUM_PATIENTS 15
#define MAX_NUM_SYMPTOMS 13

struct Patient
{
    time_t now = time(0);
    char *dt = ctime(&now);
    int ID;
    char firstName[nameL];
    char lastName[nameL];
    int DD = -1;   // day in Date of Birth
    int MM = -1;   // month
    int YYYY = -1; // year
    char address[addressL];
    string lastOverseasTravel;
    int symptomLevel;
    vector<string> symptomsList;
    vector<string> visitedLocation;
    string covidTest = "unknown";
    string status = "unknown";
};


struct Location {
    string name;
    string id;
    // dt;
};

struct DateTime {
    int hh;
    int dd;
    int mm;
    int yyyy;
};

string RISK_LEVEL[4] = {"Unidentified", "Low", "Medium", "High"};

const string patientFName = "Database/Patient Database.txt";
const string symptomFName = "Database/Symptom Database.txt";
const string locationFName = "Database/COVID location Database.txt";

void openInput(ifstream &, string);  // for write
void openOutput(ofstream &, string); // for read
void openAppend(ofstream &, string); // for
void displayMenu();

void openInput(ifstream &fname, string filename)
{
    fname.open(filename.c_str());
    if (fname.fail()) // check for a successful open
    {
        cout << "\nFailed to open the file named " << patientFName << " for read"
             << "\n Please check that this file exists"
             << endl;
        exit(1);
    }
    return;
}
void openOutput(ofstream &fname, string filename)
{
    fname.open(filename.c_str());
    if (fname.fail()) // check for a successful open
    {
        cout << "\nFailed to open the file named " << patientFName << " for write"
             << "\n Please check that this file exists"
             << endl;
        exit(1);
    }
    return;
}
void openAppend(ofstream &fname, string filename)
{
    fname.open(filename.c_str(), ios_base::app);
    if (fname.fail()) // check for a successful open
    {
        cout << "\nFailed to open the file named " << patientFName << " for append"
             << "\n Please check that this file exists"
             << endl;
        exit(1);
    }
    return;
}

void displayMenu()
{
    cout << "----------------------------------------\n";
    cout << "\t\tMENU:\n";
    cout << "1 - Fill in the patient details\n";
    cout << "2 - Upload the patient's COVID Test result by ID\n";
    cout << "3 - Display the updated location for High-Risk location for COVID\n";
    cout << "4 - Update the new COVID Positive Patient's Detail\n";
    cout << "5 - Display the COVID Positive Patients Detail\n";
    cout << "6 - Exit the program\n";
    cout << "----------------------------------------\n\n";
}

bool areIDsDuplicated(Patient p[MAX_NUM_PATIENTS], int patientCount, int currentID)
{
    for (int i = 0; i < patientCount; i++)
    {
        if (currentID == p[i].ID)
        {
            return true;
        }
    }
    return false;
}
template <class T>
bool checkValid(const T expected[], size_t SIZE, T actual)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (actual == expected[i])
        {
            return true;
        }
    }
    return false;
}
bool isSameLocation(vector<string> location, string actual)
{
    for (string x : location)
    {
        if (x == actual)
        {
            return true;
        }
    }
    return false;
}
void recommendTreatment(int s, int n)
{ // s = symptomLevel (0 - 3); n: number of high-risk visited locations
    if (s == 0)
    {
        cout << "Unable to recommend COVID Test - required data missing";
    }
    else if (s == 1)
    {
        if (n == 0)
        {
            cout << "You have LOW-risk symptoms.\nYou visited 0 COVID-19 high-risk location.\n";
            cout << "-----YOUR COVID-19 RECOMMENDATION-----\n";
            cout << "1. Maintain a safe distance from others (at least 1 metre)\n";
            cout << "2. Stay home if you feel unwell.\n";
            cout << "3. Clean your hands often. Use soap and water, or an alcohol-based hand rub.\n";
        }
        else if (n > 0)
        {
            cout << "You have LOW-risk symptoms.\nYou visited " << n << " COVID-19 high-risk location(s).\n";
            cout << "-----YOUR COVID-19 RECOMMENDATION-----\n";
            cout << "1. Monitor carefully for COVID-19 symptoms\n";
            cout << "2. Stay home if you feel unwell.\n";
            cout << "3. Clean your hands often. Use soap and water, or an alcohol-based hand rub.\n";
            cout << "4. Get a test if you develop any symptoms and quarantine until you receive a negative result.\n";
        }
    }
    else if (s == 2)
    {
        cout << "You have MEDIUM-risk symptoms.\nYou visited " << n << " COVID-19 high-risk location(s).\n";
        cout << "-----YOUR COVID-19 RECOMMENDATION-----\n";
        cout << "1. Have a test as soon as you can and quarantine until you receive a negative result.\n";
        cout << "\t*You do not need to continue to quarantine if you receive a negative result.\n";
        cout << "2. If the first test was before day 5 (day 0 is the last date of exposure to the person with COVID-19), have another test on or after day 6.\n";
        cout << "3. Get a test if you develop any symptoms and quarantine until you receive a negative result.\n";
    }
    else if (s == 3)
    {
        cout << "You have HIGH-risk symptoms.\nYou visited " << n << " COVID-19 high-risk location(s).\n";
        cout << "-----YOUR COVID-19 RECOMMENDATION-----\n";
        cout << "1. Have a test as soon as you can and quarantine for 7 days from the last date of exposure to the person who has COVID-19 (that date is day 0).\n";
        cout << "2. Take a test on or after day 6.\n";
        cout << "3. Not visit high-risk settings (hospitals, residential aged care facilities, correctional and detention facilities and residential accommodation that support people who require frequent, close personal care and who are vulnerable to disease) from days 8 to 14 after you've left quarantine.\n";
        cout << "\t*You may still access urgent medical care or aged or disability care services.\n";
        cout << "4. Monitor for COVID-19 symptoms and get a PCR test if you develop any symptoms.\n";
    }
}
int isSameID(Patient p[MAX_NUM_PATIENTS], int &patientCount, int id)
{
    for (int i = 0; i < patientCount; i++)
    {
        if (id == p[i].ID)
        {
            return i;
        }
    }
    return -1;
}
//_____________________________________________________________________________________________________
// UPDATE
void updatePatientDataBase(ofstream &outPatientDataBase, Patient p[MAX_NUM_PATIENTS], int &patientCount)
{
    if (outPatientDataBase.is_open())
    {
        for (int i = 0; i < patientCount; i++)
        {
            outPatientDataBase << p[i].ID << ",";
            outPatientDataBase << p[i].firstName << ",";
            outPatientDataBase << p[i].lastName << ",";
            outPatientDataBase << p[i].DD << ",";
            outPatientDataBase << p[i].MM << ",";
            outPatientDataBase << p[i].YYYY << ",";
            outPatientDataBase << p[i].address << ",";
            outPatientDataBase << p[i].lastOverseasTravel << ",";
            // outPatientDataBase << p[i].dt <<",";
            outPatientDataBase << "Symptom Level,";
            outPatientDataBase << p[i].symptomLevel << ",";
            outPatientDataBase << "No of Symptoms,";
            outPatientDataBase << p[i].symptomsList.size() << ",";
            outPatientDataBase << "Symptom Lists,";
            for (int j = 0; j < p[i].symptomsList.size(); j++)
            {
                outPatientDataBase << p[i].symptomsList[j] << ",";
            }
            outPatientDataBase << "No of high-risk visited locations,";
            outPatientDataBase << p[i].visitedLocation.size() << ",";
            for (int j = 0; j < p[i].visitedLocation.size(); j++)
            {
                outPatientDataBase << p[i].visitedLocation[j] << ",";
            }
            outPatientDataBase << "covid test,";
            outPatientDataBase << p[i].covidTest << ",";
            outPatientDataBase << "status,";
            outPatientDataBase << p[i].status << ",";
            outPatientDataBase << "\n";
        }
        outPatientDataBase.close();
    }
    else
    {
        cout << "\nFailed to open the file named 'Patient Database.txt' for output"
             << "\n Pleaser check that this file exists"
             << endl;
        exit(1);
    }
}
void updateLocationDataBase(ofstream &appendLocationDatabase, vector<string> location)
{
    if (appendLocationDatabase.is_open())
    {
        for (int i = 0; i < location.size() - 1; i++)
        {
            appendLocationDatabase << location[i] << "\n";
        }
        appendLocationDatabase << location[location.size() - 1];
        appendLocationDatabase.close();
    }
    else
    {
        cout << "\nFailed to open the file named 'COVID location Database.txt' for append"
             << "\n Pleaser check that this file exists"
             << endl;
        exit(1);
    }
}
//_____________________________________________________________________________________________________
// LOAD TO ARRAY/VECTOR
// https://www.youtube.com/watch?v=_IzYGiuX8QM&t=88s
void loadPatientFile(Patient p[MAX_NUM_PATIENTS], int &patientCount, ifstream &inPatientDataBase)
{
    if (inPatientDataBase.is_open())
    {
        string line;
        string myString;
        string tempString;
        int noOfSymptoms;
        int noOfLocations;

        while (getline(inPatientDataBase, line))
        {
            stringstream ss(line);
            getline(ss, myString, ',');
            p[patientCount].ID = stoi(myString);
            ss.getline(p[patientCount].firstName, nameL, ',');
            ss.getline(p[patientCount].lastName, nameL, ',');
            getline(ss, myString, ',');
            p[patientCount].DD = stoi(myString);
            getline(ss, myString, ',');
            p[patientCount].MM = stoi(myString);
            getline(ss, myString, ',');
            p[patientCount].YYYY = stoi(myString);
            ss.getline(p[patientCount].address, addressL, ',');
            getline(ss, p[patientCount].lastOverseasTravel, ',');
            // ss.getline(p[patientCount].dt,50,',');
            getline(ss, tempString, ','); // Symptom Level
            getline(ss, myString, ',');
            p[patientCount].symptomLevel = stoi(myString);
            getline(ss, tempString, ','); // No of Symptoms
            getline(ss, myString, ',');
            noOfSymptoms = stoi(myString);
            getline(ss, tempString, ','); // Symptom Lists
            for (int i = 0; i < noOfSymptoms; i++)
            {
                getline(ss, myString, ',');
                p[patientCount].symptomsList.push_back(myString);
            }
            getline(ss, tempString, ','); // No of high-risk visited locations
            getline(ss, myString, ',');
            noOfLocations = stoi(myString);
            for (int i = 0; i < noOfLocations; i++)
            {
                getline(ss, myString, ',');
                p[patientCount].visitedLocation.push_back(myString);
            }
            getline(ss, tempString, ',');
            getline(ss, p[patientCount].covidTest, ',');
            getline(ss, tempString, ',');
            getline(ss, p[patientCount].status, ',');
            patientCount++;
        }
        inPatientDataBase.close();
    }
    else
    {
        cout << "\nFailed to open the file named 'Patient Database.txt' for input"
             << "\n Pleaser check that this file exists"
             << endl;
        exit(1);
    }
}
void loadLocationFile(vector<string> &l, int &locationCount, ifstream &inLocationDatabase)
{
    if (inLocationDatabase.is_open())
    {
        string line;
        l.clear();
        while (getline(inLocationDatabase, line))
        {
            l.push_back(line);
            locationCount++;
        }
        inLocationDatabase.close();
    }
    else
    {
        cout << "\nFailed to open the file named 'COVID location Database.txt' for input"
             << "\n Pleaser check that this file exists"
             << endl;
        exit(1);
    }
}
//____________________________________________________________________________________________________________________
// PROMPT & UPDATE THE DATABASE
void PromptPatient(Patient p[MAX_NUM_PATIENTS], int &patientCount)
{
    cout << "---------------------PATIENT " << patientCount + 1 << "---------------------\n";
    cout << "Patient ID: ";
    cin >> p[patientCount].ID;
    while (areIDsDuplicated(p, patientCount, p[patientCount].ID))
    {
        cout << "Duplicate IDs are detected!\nPlease input different ID!\n";
        cout << "Patient ID: ";
        cin >> p[patientCount].ID;
    }
    cout << "First Name: ";
    cin >> p[patientCount].firstName;
    cout << "Last Name: ";
    cin >> p[patientCount].lastName;

    cout << "Day that you were born: ";
    cin >> p[patientCount].DD;
    while (p[patientCount].DD < 1 || p[patientCount].DD > 31)
    {
        cout << "->Invalid day of birth\n";
        cout << "Day that you were born: ";
        cin >> p[patientCount].DD;
    }
    cout << "Month that you were born: ";
    cin >> p[patientCount].MM;
    while (p[patientCount].MM < 1 || p[patientCount].MM > 12)
    {
        cout << "->Invalid month of birth\n";
        cout << "Month that you were born: ";
        cin >> p[patientCount].MM;
    }
    cout << "Year that you were born: ";
    cin >> p[patientCount].YYYY;
    while (p[patientCount].YYYY < 1900 || p[patientCount].YYYY > 2022)
    {
        cout << "->Invalid year of birth\n";
        cout << "Year that you were born: ";
        cin >> p[patientCount].YYYY;
    }

    cout << "Home address: ";
    cin.ignore();
    cin.getline(p[patientCount].address, addressL);
    while (p[patientCount].lastOverseasTravel != "yes" && p[patientCount].lastOverseasTravel != "y" && p[patientCount].lastOverseasTravel != "no" && p[patientCount].lastOverseasTravel != "n")
    {
        cout << "Overseas Travel (y/n): ";
        cin >> p[patientCount].lastOverseasTravel;
    }
}
void PromptSymtomps(ifstream &inSymptomDataBase, Patient &p)
{
    int levelOfRisk = 0, noOfSymptoms;
    string ans = " ";
    string symptom;
    cout << "\nCOVID-19 DIAGNOSIS BASED ON COMMON SYMTOMPS\n";
    while (levelOfRisk < 3)
    {
        inSymptomDataBase >> noOfSymptoms;
        inSymptomDataBase.ignore();
        cout << "-----" << RISK_LEVEL[levelOfRisk + 1] << " Risk Symptoms -----\n";
        for (int i = 1; i <= noOfSymptoms; i++)
        {
            getline(inSymptomDataBase, symptom);
            cout << "Do you have " << symptom << " ? (y/n) ";
            cin >> ans;
            while (ans != "y" && ans != "n")
            {
                cout << "Invalid answer. Please try again!\n";
                cout << "Do you have " << symptom << " ? (y/n) ";
                cin >> ans;
            }
            if (ans == "y")
            {
                p.symptomsList.push_back(symptom);
                p.symptomLevel = levelOfRisk + 1;
            }
        }
        levelOfRisk++;
    }
    inSymptomDataBase.close();
}
void PromptCOVIDLocation(ifstream &inLocationDatabase, Patient &p)
{
    int id, i = 1, option = -1;
    char ans;
    string name;
    vector<string> location;
    cout << "-----Select the locations you have visited-----\n";
    while (getline(inLocationDatabase, name))
    {
        cout << i << ". " << name << endl;
        i++;
        location.push_back(name);
    }
    while (option != 0)
    {
        cout << "Select option: (0 for none/stop) ";
        cin >> option;
        if (option == 0)
        {
            break;
        }
        else if (option > 0 && option <= location.size())
        {
            if (!isSameLocation(p.visitedLocation, location[option - 1]))
            {
                p.visitedLocation.push_back(location[option - 1]);
            }
            else
            {
                cout << "-> Already added the location\n";
            }
        }
    }
    inLocationDatabase.close();
}
void PromptCOVIDTest(Patient p[MAX_NUM_PATIENTS], int &patientCount, vector<string> location, ofstream &appendLocationDatabase)
{
    cout << "Choose patient ID --> ";
    int id;
    cin >> id;
    if (isSameID(p, patientCount, id) != -1)
    {
        id = isSameID(p, patientCount, id);
        if (p[id].status == "Dead - due to COVID-19")
        {
            cout << "The patient ID " << id << " was dead due to COVID-19\n";
        }
        else
        {
            cout << "Your previous COVID 19 Test Status: " << p[id].covidTest << "\n";
            string expected[] = {"n", "p", "negative", "positive", "unknown", "dead"};
            string answer = " ";
            while (!checkValid<string>(expected, 6, answer))
            {
                cout << "Your recent COVID 19 Test Status: ('n' for negative / 'p' for positive / 'unknown' for not yet tested / 'dead' for passed-away patient)  --> ";
                cin >> answer;
            }
            if (answer == "p" || answer == "positive")
            {
                p[id].status = "Live - Not Cured";
                p[id].covidTest = "positive";
                cout << "\nAs you are positive with COVID-19, it's important to tell us where you have visited in the last 7 days\nYour visited locations: (type 0 for stop)\n";
                string ans = " ";
                vector<string> vec;
                while (true)
                {
                    getline(cin, ans);
                    if (ans == "0")
                    {
                        break;
                    }
                    else if (ans != "")
                    {
                        p[id].visitedLocation.push_back(ans);
                    }
                    if (!isSameLocation(location, ans))
                    {
                        vec.push_back(ans);
                    }
                }
                updateLocationDataBase(appendLocationDatabase, vec);
            }
            else if (answer == "n" || answer == "negative")
            {
                if (p[id].covidTest == "unknown" || p[id].covidTest == "negative")
                {
                    p[id].status = "Live - Not detected with COVID-19";
                }
                else if (p[id].covidTest == "positive")
                {
                    p[id].status = "Live - Cured";
                }
                p[id].covidTest = "negative";
            }
            else if (answer == "unknown")
            {
                p[id].covidTest = answer;
                p[id].status = "Live - Not Yet Tested";
            }
            else if (answer == "dead")
            {
                p[id].covidTest = "positive";
                p[id].status = "Dead - due to COVID-19";
            }
            time_t now = time(0);
            p[id].dt = ctime(&now);
        }
    }
    else
    {
        cout << "Invalid patient ID.\n";
    }
    appendLocationDatabase.close();
}
//____________________________________________________________________________________________________________________
// DISPLAY
void displayCOVIDLocation(vector<string> location)
{
    cout << "-----HIGH RISK COVID-19 LOCATION-----\n";
    for (int i = 0; i < location.size(); i++)
    {
        cout << i + 1 << ". " << location[i] << "\n";
    }
}
void displayCOVIDPatient(Patient p)
{
    cout << "\n---------------------PATIENT ID " << p.ID << "---------------------";
    cout << "\nFull Name: " << p.firstName << " " << p.lastName;
    cout << "\nDate of Birth: " << p.DD << "-" << p.MM << "-" << p.YYYY;
    cout << "\nAddress: " << p.address;
    cout << "\nLast Overseas Travel: " << p.lastOverseasTravel;
    cout << "\nDate Time updated: " << p.dt;
    cout << "\nSymptom Level of Risk: " << RISK_LEVEL[p.symptomLevel];
    cout << "\nSymptom List: (" << p.symptomsList.size() << ") \n";
    for (string x : p.symptomsList)
    {
        cout << "\t" << x << "\n";
    }
    cout << "Visited High-risk location: (" << p.visitedLocation.size() << ") \n";
    for (string x : p.visitedLocation)
    {
        cout << "\t" << x << "\n";
    }
    cout << "Covid Test Result: " << p.covidTest << "\n";
    cout << "Patient status: " << p.status << "\n";
}
void displayIDPatient(Patient p[MAX_NUM_PATIENTS], int &patientCount)
{
    cout << "-----PATIENT ID LIST-----\n";
    for (int i = 0; i < patientCount; i++)
    {
        cout << "ID = " << p[i].ID << " - Name = " << p[i].firstName << " " << p[i].lastName << "\n";
    }
}
//____________________________________________________________________________________________________________________
// PROGRAM
int main()
{
    ifstream inPatientDataBase, inSymptomDataBase, inLocationDatabase; // read
    ofstream outPatientDataBase;                                       // over-write (used to update)
    ofstream appendPatientDataBase, appendLocationDatabase;            // use to add more patients (option 1)
    Patient p[MAX_NUM_PATIENTS];
    int patientCount = 0;
    vector<string> location;
    int locationCount = 0;
    // Load the patient file into the p struct.
    openInput(inPatientDataBase, patientFName);
    loadPatientFile(p, patientCount, inPatientDataBase);
    // Load the COVID location Database.
    openInput(inLocationDatabase, locationFName);
    loadLocationFile(location, locationCount, inLocationDatabase);
menu:
    // display menu for choosing option:
    displayMenu();
    int option;
    cout << "--> Option: ";
    cin >> option;
    switch (option)
    {
    case 1:
    {
        PromptPatient(p, patientCount);
        openInput(inSymptomDataBase, symptomFName);
        PromptSymtomps(inSymptomDataBase, p[patientCount]);
        openInput(inLocationDatabase, locationFName);
        PromptCOVIDLocation(inLocationDatabase, p[patientCount]);
        recommendTreatment(p[patientCount].symptomLevel, p[patientCount].visitedLocation.size());
        patientCount++;
        goto menu;
        break;
    }
    case 2:
    {
        displayIDPatient(p, patientCount);
        openAppend(appendLocationDatabase, locationFName);
        PromptCOVIDTest(p, patientCount, location, appendLocationDatabase);
        goto menu;
        break;
    }
    case 3:
    {
        openInput(inLocationDatabase, locationFName);
        loadLocationFile(location, locationCount, inLocationDatabase);
        displayCOVIDLocation(location);
        goto menu;
        break;
    }
    case 4:
    {
        openOutput(outPatientDataBase, patientFName);
        updatePatientDataBase(outPatientDataBase, p, patientCount);
        cout << "Update the New COVID Positive Patient's Detail successfully.\n";
        goto menu;
        break;
    }
    case 5:
    {
        cout << "\tCOVID-19 PATIENT LISTS\n";
        if (patientCount == 0)
        {
            cout << "[] - the database is empty\n";
        }
        else
        {
            int j = 0;
            for (int i = 0; i < patientCount; i++)
            {
                if (p[i].covidTest == "positive")
                {
                    j++;
                    displayCOVIDPatient(p[i]);
                }
            }
            if (j == 0)
            {
                cout << "No COVID-19 patient from the Patient Database.txt\n";
            }
        }
        goto menu;
        break;
    }
    case 6:
    {
        openOutput(outPatientDataBase, patientFName);
        updatePatientDataBase(outPatientDataBase, p, patientCount);
        cout << "Exit the program successfully!\n";
        cout << "Goodbye. See you later!";
        exit(-1);
        break;
    }
    default:
    {
        cout << "Unknown selection, please try again\n";
        goto menu;
        break;
    }
    }
}

// task:
/*
-check the same name for the location
-convert date to string to add into the patient
-case 4.
-use time for the writing into the textfile
----
-Add the date and time to the visistedLocation for the user
-change the datatype from string to Location
-Add some lines of code that allows the user input the time that they visited
e.g:
-Name: Swinburne University
Time in hour: 11
Day: 21
Month: 4
Year: 2022

-these info is recorded in the patient's visited location list under the type of Location.
-when uploading to the database -> name,time,day,month,year,secondname,time,day,month, (at then end or press 4)
-retrieve data from the database (at the beginning)
*/