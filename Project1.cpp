#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <algorithm>
#include<ctime>
#include<conio.h>
using namespace std;

//Structure for Patient
struct Patient
{
    string userID;
    string password;
    string name;
    string village;
};

//Structure for Doctor
struct Doctor
{
    string userID;
    string  password;
    string name;
    string specialization;
    string location;
    vector<string> handledPatients;
};


unordered_map<string, string> symptoms; // Store symptom name and its description



int levenshteinDistance(const string &s1, const string &s2) {
    int len1 = s1.length();
    int len2 = s2.length();
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++) dp[i][0] = i;
    for (int j = 0; j <= len2; j++) dp[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost});
        }
    }

    return dp[len1][len2];
}

// Function to load patient data from the file
// This function reads patient information from a CSV file
// and returns a vector containing Patient objects.
// Helper function to log errors

void logError(const string &message)
{
    cerr << "Error: " << message << endl;
}

// Helper function to validate if a line contains the correct number of fields
bool isValidLine(const string &line)
{
    int count = 0;
    stringstream ss(line);
    string field;

    while (getline(ss, field, ',')) {
        count++;
    }

    // We expect exactly 4 fields (userID, password, name, village)
    return count == 4;
}

// Helper function to split a line into Patient data
Patient parsePatientData(const string &line) {
    stringstream ss(line);
    string userID, password, name, village;

    getline(ss, userID, ',');
    getline(ss, password, ',');
    getline(ss, name, ',');
    getline(ss, village, ',');

    return {userID, password, name, village};
}

// Function to load patients from a file
vector<Patient> loadPatients(const string &filename) {
    vector<Patient> patients;  // Vector to store patient data

    // Open the file in read mode
    ifstream file(filename);
    if (!file) {
        logError("Unable to open the file " + filename);
        return patients; // Return an empty list if the file cannot be opened
    }

    string line;  // To hold each line of the file

    // Skip the header line if present
    getline(file, line);

    int lineCount = 0;  // To count the number of lines read

    // Read the file line by line
    while (getline(file, line))
    {
        lineCount++;

        // Skip invalid lines or lines with incorrect number of fields
        if (!isValidLine(line)) {
            logError("Skipping invalid line " + to_string(lineCount));
            continue;
        }

        // Extract patient data and add to the list
        Patient patient = parsePatientData(line);

        // Validate that userID is not empty before adding to the list
        if (!patient.userID.empty())
        {
            patients.push_back(patient);
        } else
        {
            logError("Skipping patient with empty userID at line " + to_string(lineCount));
        }
    }

    // Close the file after reading all data
    file.close();

    // Log the number of patients loaded
    cout << "Loaded " << patients.size() << " patients from the file." << endl;

    // Debugging block: Display loaded patients (optional)
    /*
    for (const auto& patient : patients) {
        cout << "UserID: " << patient.userID << ", Name: " << patient.name
             << ", Village: " << patient.village << endl;
    }
    */

    // Placeholder for future extensions
    // Example: Error recovery if file data is malformed

    // Space for integrating analytics or tracking

    // Placeholder for advanced filtering of patient data (e.g., by village)

    // Space for optimizations like multi-threaded file reading

    // Placeholder for data validation checks, e.g., format compliance

    // Reserve space for enhancements, such as loading from other formats like XML or JSON

    // Reserve space for testing features or additional debugging output

    // Log line count for verification
    cout << "Processed " << lineCount << " lines from the file." << endl;

    // End of function
    return patients;
}


// Function to load doctor data from the file
// This function reads doctor information from a CSV file
// and returns an unordered_map containing Doctor objects.
unordered_map<string, Doctor> loadDoctors(const string &filename) {
    unordered_map<string, Doctor> doctors;  // Map to store doctor data

    // Open the file in read mode
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open the file " << filename << ".\n";
        return doctors; // Return an empty map if the file cannot be opened
    }

    string line;  // To hold each line of the file

    // Skip the header line if present
    getline(file, line);

    // Read the file line by line
    while (getline(file, line)) {
        stringstream ss(line);  // Create a stringstream object to parse the line
        string userID, password, name, specialization, location, handledPatients;

        // Extract fields separated by commas
        getline(ss, userID, ',');
        getline(ss, password, ',');
        getline(ss, name, ',');
        getline(ss, specialization, ',');
        getline(ss, location, ',');
        getline(ss, handledPatients, ',');

        // Create a Doctor object with the extracted data
        Doctor doctor = {userID, password, name, specialization, location};

        // Parse handled patients list (separated by semicolons)
        stringstream ps(handledPatients);
        string patientID;
        while (getline(ps, patientID, ';')) {
            if (!patientID.empty()) {
                doctor.handledPatients.push_back(patientID);
            }
        }

        // Add the Doctor object to the map
        doctors[userID] = doctor;
    }

    // Close the file after reading all data
    file.close();

    // Log the number of doctors loaded
    cout << "Loaded " << doctors.size() << " doctors from the file." << endl;

    // Debugging block: Display loaded doctors (optional)
    /*
    for (const auto& [userID, doctor] : doctors) {
        cout << "UserID: " << doctor.userID << ", Name: " << doctor.name
             << ", Specialization: " << doctor.specialization << endl;
    }
    */

    // Placeholder for future extensions
    // Example: Error recovery if file data is malformed

    // Reserve space for integrating analytics or tracking

    // Placeholder for advanced filtering of doctor data

    // Space for optimizations like multi-threaded file reading

    // Placeholder for data validation checks, e.g., format compliance

    // Reserve space for enhancements, such as loading from other formats

    // Reserve space for testing features or additional debugging output

    // End of function

    return doctors;
}


void logMessage(const string &message) {
    ofstream logFile("log.txt", ios::app);  // Open log file in append mode
    if (logFile.is_open()) {
        logFile << message << endl;
        logFile.close();
    }
}

bool fileExists(const string &filename) {
    ifstream file(filename);
    return file.is_open();
}

void loadSymptoms(const string &filename) {
    if (!fileExists(filename)) {
        cout << "Error: The file " << filename << " does not exist." << endl;
        logMessage("Error: File " + filename + " does not exist.");
        return;
    }

    ifstream file(filename);
    string line;

    // Skip the header line if it exists
    if (getline(file, line)) {
        cout << "Skipping header line: " << line << endl;
        logMessage("Skipping header line.");
    }

    int count = 0; // To count the number of symptoms loaded
    while (getline(file, line)) {
        stringstream ss(line);
        string symptom, description;

        // If line is empty or invalid, skip
        if (line.empty() || !getline(ss, symptom, ',')) {
            cout << "Skipping invalid line: " << line << endl;
            logMessage("Skipping invalid line: " + line);
            continue;
        }

        // If description is missing, log and skip
        if (!getline(ss, description)) {
            cout << "Missing description for symptom: " << symptom << endl;
            logMessage("Missing description for symptom: " + symptom);
            continue;
        }

        // Add symptom and description to the map
        symptoms[symptom] = description;
        count++;

        // Logging progress after every 10 entries
        if (count % 10 == 0) {
            cout << "Loaded " << count << " symptoms." << endl;
            logMessage("Loaded " + to_string(count) + " symptoms.");
        }
    }

    cout << "Finished loading symptoms. Total symptoms loaded: " << count << endl;
    logMessage("Finished loading symptoms. Total loaded: " + to_string(count));

    file.close();
}

// Function to register a new patient

// This function handles the registration process for new patients
// and ensures that their data is stored securely.
void registerNewPatient(vector<Patient>& patients) {
    string userID, password, name, village;
    int attemptCount = 0;
    const int maxAttempts = 3; // Maximum attempts allowed for entering a valid UserID

    // Prompt for userID and ensure uniqueness
    while (attemptCount < maxAttempts) {
        cout << "Enter new UserID: ";
        cin >> userID;

        // Check if userID already exists in the patients list
        bool userExists = false;
        for (const auto& patient : patients) {
            if (patient.userID == userID) {
                cout << "UserID already exists. Please choose a different UserID.\n";
                userExists = true;
                break;
            }
        }

        if (userExists) {
            attemptCount++;
            if (attemptCount == maxAttempts) {
                cout << "Maximum attempts reached. Registration aborted.\n";
                return;
            }
        } else {
            break;
        }
    }

    // Prompt for password
    cout << "Enter Password (min 7 characters): ";
    //cin >> password;
    char ch;
             while(true)
        {
            ch=getch();
            if(ch=='\r')
            break;
            if(ch=='\b')
            {
                if(!password.empty())
                {
                    cout<<"\b \b";
                    password.pop_back();
                }
            }
            else
            {
                password+=ch;
                cout<<'*';
            }
        }
    while (password.length() < 7) {
        cout << "\nPassword too short. Please enter at least 7 characters: ";
        cin >> password;
    }

    // Collecting name
    cout << "\nEnter Name: ";
    cin.ignore(); // Ignore leftover newline character
    getline(cin, name);
    if (name.empty()) {
        cout << "Name cannot be empty. Please enter a valid name: ";
        getline(cin, name);
    }

    // Collecting village
    cout << "Enter Village: ";
    getline(cin, village);
    if (village.empty()) {
        cout << "Village cannot be empty. Please enter a valid village: ";
        getline(cin, village);
    }

    // Confirm details with the user
    cout << "\nPlease confirm the details: " << endl;
    cout << "UserID: " << userID << endl;
    cout << "Name: " << name << endl;
    cout << "Village: " << village << endl;

    char confirmation;
    cout << "Confirm registration? (Y/N): ";
    cin >> confirmation;

    if (toupper(confirmation) != 'Y') {
        cout << "Registration cancelled by user.\n";
        return;
    }

    // Add the new patient to the patients list
    patients.push_back({userID, password, name, village});

    // Save the new patient to the patient file
    // Ensure file is opened in append mode
    ofstream file("patient_data.csv", ios::app);
    if (!file) {
        cerr << "Error: Unable to open patient_data.csv file.\n";
        return;
    }

    file << userID << "," << password << "," << name << "," << village << "\n";
    file.close();

    cout << "\nNew patient registered successfully!\n";

    // Optionally display a summary of all registered patients
    // Loop through the patient list and display their details
    cout << "\n--- Registered Patients ---\n";
    for (const auto& patient : patients) {
        cout << "UserID: " << patient.userID << ", Name: " << patient.name
             << ", Village: " << patient.village << endl;
    }
    cout << "-------------------------\n";

    // Additional logging for debugging or auditing
    ofstream logFile("patient_registration_log.txt", ios::app);
    if (logFile) {
        logFile << "New patient registered: " << userID << ", " << name
                << ", " << village << "\n";
        logFile.close();
    } else {
        cerr << "Error: Unable to open patient_registration_log.txt file for logging.\n";
    }

    // Notify the admin about the successful registration (mock example)
    cout << "\nNotification: Admin has been informed of the new registration.\n";

    // Notify patient of future steps
    // Example: Directing the patient to a healthcare center
    cout << "\nImportant: Please visit your nearest healthcare center for further instructions.\n";
    cout << "\nRemember to keep your UserID and Password safe for future logins.\n";

    // Display additional help information
    cout << "\nFor any assistance, contact our helpdesk at 1-800-HELP.\n";
    cout << "You can also email us at support@healthcare.com.\n";

    // Provide details for feedback
    cout << "\nWe value your feedback! Please fill out the feedback form available on our website.\n";

    // Placeholder for additional features or future modifications
    // Example: Adding patient to a database or scheduling appointments

    // Ending message
    cout << "\nThank you for using the registration system!\n";
    cout << "Stay healthy and take care!\n";

    // Space for additional functionality
    // Example: Error recovery or undo functionality
}

// Function to register a new doctor
void registerNewDoctor(unordered_map<string, Doctor>& doctors) {
    string userID, password, name, specialization, location;
    int attemptCount = 0;
    const int maxAttempts = 3; // Max attempts for entering valid UserID

    // Prompt for userID and ensure uniqueness
    while (attemptCount < maxAttempts) {
        cout << "Enter new UserID: ";
        cin >> userID;

        // Check if userID already exists in the doctors list
        if (doctors.find(userID) != doctors.end()) {
            cout << "UserID already exists. Please choose a different UserID.\n";
            attemptCount++;
            if (attemptCount == maxAttempts) {
                cout << "Maximum attempts reached. Registration aborted.\n";
                return;
            }
        } else {
            break;
        }
    }
    char ch;
    // Prompt for password
    cout << "Enter Password (min 7 characters): ";
    //cin >> password;
              while(true)
        {
            ch=getch();
            if(ch=='\r')
            break;
            if(ch=='\b')
            {
                if(!password.empty())
                {
                    cout<<"\b \b";
                    password.pop_back();
                }
            }
            else
            {
                password+=ch;
                cout<<'*';
            }
        }
    while (password.length() < 7) {
        cout << "\nPassword too short. Please enter at least 8 characters: ";
        cin >> password;

    }

    // Collecting name
    cout << "\nEnter Name: ";
    cin.ignore(); // Ignore leftover newline character
    getline(cin, name);
    if (name.empty()) {
        cout << "Name cannot be empty. Please enter a valid name: ";
        getline(cin, name);
    }

    // Collecting specialization
    cout << "Enter Specialization: ";
    getline(cin, specialization);
    if (specialization.empty()) {
        cout << "Specialization cannot be empty. Please enter a valid specialization: ";
        getline(cin, specialization);
    }

    // Collecting location
    cout << "Enter Location: ";
    getline(cin, location);
    if (location.empty()) {
        cout << "Location cannot be empty. Please enter a valid location: ";
        getline(cin, location);
    }

    // Confirm details with the user
    cout << "\nPlease confirm the details: " << endl;
    cout << "UserID: " << userID << endl;
    cout << "Name: " << name << endl;
    cout << "Specialization: " << specialization << endl;
    cout << "Location: " << location << endl;

    char confirmation;
    cout << "Confirm registration? (Y/N): ";
    cin >> confirmation;

    if (toupper(confirmation) != 'Y') {
        cout << "Registration cancelled by user.\n";
        return;
    }

    // Add the new doctor to the doctors list
    doctors[userID] = {userID, password, name, specialization, location};

    // Save the new doctor to the doctors file
    ofstream file("doctors.csv", ios::app);
    if (!file) {
        cerr << "Error: Unable to open doctors.csv file.\n";
        return;
    }

    file << userID << "," << password << "," << name << "," << specialization << "," << location << "\n";
    file.close();

    cout << "\nNew doctor registered successfully!\n";

    // Optionally display a summary of all registered doctors
    /*cout << "\n--- Registered Doctors ---\n";
    for (const auto& [key, doctor] : doctors) {
        cout << "UserID: " << doctor.userID << ", Name: " << doctor.name
             << ", Specialization: " << doctor.specialization
             << ", Location: " << doctor.location << endl;
    }
    cout << "-------------------------\n";
*/
    // Additional logging for debugging or auditing
    ofstream logFile("doctor_registration_log.txt", ios::app);
    if (logFile) {
        logFile << "New doctor registered: " << userID << ", " << name
                << ", " << specialization << ", " << location << "\n";
        logFile.close();
    } else {
        cerr << "Error: Unable to open doctor_registration_log.txt file for logging.\n";
    }

    // Notify the admin about the successful registration (mock example)
    cout << "\nNotification: Admin has been informed of the new registration.\n";

    // Ending message
    cout << "Thank you for using the registration system!\n";
}


// Function to suggest doctors for symptoms
#include <ctime>  // To get current date and time
#include <algorithm>  // For std::transform

// Function to convert string to lowercase
// This function takes a string as input and returns a new string where all characters are in lowercase.

string toLowerCase(const string &str)
{
    string result = str;

    // Using the transform function from the algorithm library to convert all characters in the string to lowercase.
    // result.begin() is the start iterator, result.end() is the end iterator, and result.begin() is the destination for each character after conversion.
    transform(result.begin(), result.end(), result.begin(), ::tolower);

    // Returning the transformed string which is in lowercase
    return result;
}

// Function to extract the doctor's name (ignoring the specialization)
// This function assumes that the doctor's name is followed by a " - " separator, and we want to extract the name only (before the " - ").

string extractDoctorName(const string& doctor)
{
    size_t pos = doctor.find(" - ");
     // If the separator is found (i.e., pos is not equal to string::npos),
    // we extract the substring from the beginning of the string up to the position of " - "
    if (pos != string::npos)
    {
        return doctor.substr(0, pos);  // Extract the name before the " - "
    }
        // If no separator is found, we simply return the entire input string, assuming that no specialization is given.
        return doctor;  // Return the whole string if no " - " is found
}

void suggestDoctors(const string& symptom)
 {
    unordered_map<string, vector<string>> symptomDoctors =
    {
        {"Fever", {"Dr. John Smith - General Doctor", "Dr. Alice Davis - Cardiologist"}},
        {"Cough", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
        {"Headache", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
        {"Sore throat", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
        {"Fatigue", {"Dr. John Smith - General Doctor"}},
        {"Body ache", {"Dr. John Smith - General Doctor", "Dr. Michael Green - Orthopedic Specialist"}},
        {"Nausea", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
        {"Vomiting", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
        {"Shortness of breath", {"Dr. Emma White - Pulmonologist", "Dr. John Smith - General Doctor"}},
        {"Chills", {"Dr. John Smith - General Doctor"}},
        {"Dizziness", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
        {"Diarrhea", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
        {"Constipation", {"Dr. Emma White - Gastroenterologist"}},
        {"Loss of appetite", {"Dr. John Smith - General Doctor", "Dr. Emma White - Gastroenterologist"}},
        {"Skin rash", {"Dr. Laura Brown - Dermatologist", "Dr. John Smith - General Doctor"}},
        {"Muscle cramps", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
        {"Sore muscles", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
        {"Runny nose", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
        {"Numbness", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
          {"Fever", {"Dr. John Smith - General Doctor", "Dr. Alice Davis - Cardiologist"}},
        {"Cough", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
        {"Headache", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
        {"Sore throat", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
        {"Fatigue", {"Dr. John Smith - General Doctor"}},
        {"Body ache", {"Dr. John Smith - General Doctor", "Dr. Michael Green - Orthopedic Specialist"}},
        {"Nausea", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
        {"Vomiting", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
        {"Shortness of breath", {"Dr. Emma White - Pulmonologist", "Dr. John Smith - General Doctor"}},
        {"Chills", {"Dr. John Smith - General Doctor"}},
        {"Dizziness", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
        {"Diarrhea", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
        {"Constipation", {"Dr. Emma White - Gastroenterologist"}},
        {"Loss of appetite", {"Dr. John Smith - General Doctor", "Dr. Emma White - Gastroenterologist"}},
        {"Skin rash", {"Dr. Laura Brown - Dermatologist", "Dr. John Smith - General Doctor"}},
        {"Muscle cramps", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
        {"Sore muscles", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
        {"Runny nose", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
        {"Numbness", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
         {"Weight gain", {"Dr. Emily Clark - Endocrinologist", "Dr. Andrew Foster - Internal Medicine"}},
    {"Night blindness", {"Dr. Brian Lee - Ophthalmologist", "Dr. Sarah Black - Optometrist"}},
    {"Leg swelling", {"Dr. Rachel Green - Cardiologist", "Dr. John Smith - General Doctor"}},
    {"Nasal polyps", {"Dr. Andrew Moore - ENT Specialist", "Dr. John Smith - General Doctor"}},
    {"Chronic coughing", {"Dr. Emma White - Pulmonologist", "Dr. John Smith - General Doctor"}},
    {"Fever with rash", {"Dr. Sarah Lee - Pediatrician", "Dr. Alice Green - Infectious Disease Specialist"}},
    {"Swollen ankles", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Rachel Green - Cardiologist"}},
    {"Sudden vision changes", {"Dr. Brian Lee - Ophthalmologist", "Dr. John Smith - General Doctor"}},
    {"Bleeding gums", {"Dr. Alice Brown - Dentist", "Dr. John Smith - General Doctor"}},
    {"Painful urination", {"Dr. Jane Davis - Urologist", "Dr. Andrew Brown - Nephrologist"}},
    {"Pale stool", {"Dr. Emma White - Gastroenterologist", "Dr. Andrew Brown - Nephrologist"}},
    {"Severe abdominal pain", {"Dr. Michael Turner - Gastroenterologist", "Dr. Emma White - Gastroenterologist"}},
    {"Tingling in extremities", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Difficulty breathing", {"Dr. Emma White - Pulmonologist", "Dr. Rachel Green - Internal Medicine"}},
    {"Persistent coughing", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
    {"Chronic back pain", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Andrew Moore - Pain Specialist"}},
    {"Muscle weakness", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Brian Lee - Neurologist"}},
    {"Blurry vision", {"Dr. Brian Lee - Ophthalmologist", "Dr. James Turner - General Doctor"}},
    {"Liver pain", {"Dr. Andrew Brown - Gastroenterologist", "Dr. Olivia Green - Hepatologist"}},
    {"Stomach bloating", {"Dr. Emma White - Gastroenterologist", "Dr. Rachel Green - Internal Medicine"}},
    {"Chest tightness", {"Dr. Alice Davis - Cardiologist", "Dr. John Smith - General Doctor"}},
    {"Unexplained weight loss", {"Dr. Nathan Foster - Endocrinologist", "Dr. Emily Clark - General Practitioner"}},
    {"Abdominal cramps", {"Dr. Emma White - Gastroenterologist", "Dr. Alice Green - Gastroenterologist"}},
    {"Frequent headaches", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Fatigue with pain", {"Dr. Sarah White - Rheumatologist", "Dr. John Smith - General Doctor"}},
    {"Dry mouth", {"Dr. Alice Brown - Dentist", "Dr. Emily Roberts - Internal Medicine"}},
    {"Nose congestion", {"Dr. Andrew Moore - ENT Specialist", "Dr. John Smith - General Doctor"}},
    {"Itchy skin", {"Dr. Laura Brown - Dermatologist", "Dr. John Smith - General Doctor"}},
    {"Painful joints", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Sandra Lee - Rheumatologist"}},
    {"Rapid heartbeat", {"Dr. Alice Davis - Cardiologist", "Dr. Rachel Green - Cardiologist"}},
    {"Lack of concentration", {"Dr. Sarah White - Psychiatrist", "Dr. Thomas Black - General Doctor"}},
    {"Muscle spasms", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Andrew Moore - Pain Specialist"}},
    {"Fever with chills", {"Dr. Alice Green - Infectious Disease Specialist", "Dr. Sarah White - Internal Medicine"}},
    {"Wheezing", {"Dr. Emma White - Pulmonologist", "Dr. John Smith - General Doctor"}},
    {"Sudden hearing loss", {"Dr. Patrick Turner - ENT Specialist", "Dr. Laura Brown - Audiologist"}},
    {"Extreme thirst", {"Dr. Nathan Foster - Endocrinologist", "Dr. Elizabeth Green - Nephrologist"}},
    {"Severe headaches", {"Dr. Brian Lee - Neurologist", "Dr. Andrew Black - General Doctor"}},
    {"Chronic fatigue", {"Dr. John Smith - General Doctor", "Dr. Alice Green - Rheumatologist"}},
    {"Frequent urination at night", {"Dr. Jane Davis - Urologist", "Dr. Andrew Brown - Nephrologist"}},
    {"Burning sensation in chest", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
    {"Difficulties swallowing", {"Dr. Andrew Moore - ENT Specialist", "Dr. John Smith - General Doctor"}},
    {"High fever", {"Dr. Timothy Black - Infectious Disease Specialist", "Dr. John Smith - General Doctor"}},
    {"Severe nausea", {"Dr. Emma White - Gastroenterologist", "Dr. Alice Green - General Practitioner"}},
    {"Ringing in ears", {"Dr. Patrick Turner - ENT Specialist", "Dr. Olivia Brown - Audiologist"}},
    {"Sore eyes", {"Dr. Brian Lee - Ophthalmologist", "Dr. Sarah White - Optometrist"}},
    {"Trouble walking", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Brian Lee - Neurologist"}},
    {"Painful swallowing", {"Dr. Andrew Moore - ENT Specialist", "Dr. Emily Turner - Gastroenterologist"}},
    {"Shortness of breath with exertion", {"Dr. Emma White - Pulmonologist", "Dr. Alice Green - Cardiologist"}},
    {"Pain in the abdomen", {"Dr. Emma White - Gastroenterologist", "Dr. Michael Turner - General Practitioner"}},
    {"Palpitations with dizziness", {"Dr. Alice Davis - Cardiologist", "Dr. Nathan Brown - Cardiologist"}},
    {"Swelling in feet", {"Dr. Rachel Green - Cardiologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Increased sweating", {"Dr. John Smith - General Doctor", "Dr. Thomas Black - Endocrinologist"}},
    {"Difficulty concentrating", {"Dr. Sarah White - Psychiatrist", "Dr. Andrew Clark - Psychologist"}},
    {"Muscle pain after exercise", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Hives", {"Dr. Laura Brown - Dermatologist", "Dr. Sarah White - Allergist"}},
    {"Numbness in hands", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Lower back pain", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Chest pain with breathing", {"Dr. Alice Davis - Cardiologist", "Dr. Emma White - Pulmonologist"}},
    {"Fatigue with fever", {"Dr. Alice Green - Infectious Disease Specialist", "Dr. Sarah White - Internal Medicine"}},
    {"Sudden vision loss", {"Dr. Brian Lee - Ophthalmologist", "Dr. John Smith - General Doctor"}},
    {"Dry eyes", {"Dr. Brian Lee - Ophthalmologist", "Dr. John Smith - General Doctor"}},
    {"Frequent sneezing", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
    {"Heart palpitations", {"Dr. Alice Davis - Cardiologist", "Dr. Samuel Harris - Cardiologist"}},
    {"Skin peeling", {"Dr. Laura Brown - Dermatologist", "Dr. John Smith - General Doctor"}},
    {"Severe bruising", {"Dr. Rachel Green - Cardiologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Pain behind the eyes", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Loss of appetite with nausea", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
    {"Hearing ringing", {"Dr. Patrick Turner - ENT Specialist", "Dr. Laura Brown - Audiologist"}},
    {"Sore neck", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Andrew Moore - Pain Specialist"}},
    {"Painful leg cramps", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Abnormal sweating", {"Dr. Sarah White - Endocrinologist", "Dr. Nathan Foster - Internal Medicine"}},
     {"Chest pain", {"Dr. Alice Davis - Cardiologist", "Dr. Rachel Green - General Practitioner", "Dr. Oscar Harris - Cardiologist"}},
    {"Back pain", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Abdominal pain", {"Dr. Emma White - Gastroenterologist", "Dr. Michael Turner - General Practitioner"}},
    {"Palpitations", {"Dr. Alice Davis - Cardiologist", "Dr. Samuel Harris - Cardiologist"}},
    {"Anxiety", {"Dr. Sarah White - Psychiatrist", "Dr. John Smith - General Doctor"}},
    {"Depression", {"Dr. Sarah White - Psychiatrist", "Dr. Andrew Clark - Psychologist"}},
    {"Insomnia", {"Dr. Elizabeth Green - Sleep Specialist", "Dr. Daniel Harris - Internal Medicine"}},
    {"Hypertension", {"Dr. Alice Davis - Cardiologist", "Dr. Emily Roberts - General Practitioner"}},
    {"Cold hands and feet", {"Dr. John Smith - General Doctor", "Dr. Edward Turner - Cardiologist"}},
    {"Frequent urination", {"Dr. Jane Davis - Urologist", "Dr. Andrew Brown - Nephrologist"}},
    {"Urinary retention", {"Dr. Jane Davis - Urologist", "Dr. Andrew Brown - Nephrologist"}},
    {"Joint pain", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Sandra Lee - Rheumatologist"}},
    {"Mood swings", {"Dr. Sarah White - Psychiatrist", "Dr. John Smith - General Doctor"}},
    {"Memory loss", {"Dr. Brian Lee - Neurologist", "Dr. Claire Adams - Neurologist"}},
    {"Blurred vision", {"Dr. James Roberts - Ophthalmologist", "Dr. Claire Harris - General Practitioner"}},
    {"Hearing loss", {"Dr. Patrick Turner - ENT Specialist", "Dr. Laura Brown - Audiologist"}},
    {"Nosebleed", {"Dr. Andrew Moore - ENT Specialist", "Dr. John Smith - General Doctor"}},
    {"Panic attacks", {"Dr. Sarah White - Psychiatrist", "Dr. Oscar Brown - General Doctor"}},
    {"Chest tightness", {"Dr. Alice Davis - Cardiologist", "Dr. Emma White - Pulmonologist"}},
    {"High blood sugar", {"Dr. Emily Clark - Endocrinologist", "Dr. Nathan Foster - Internal Medicine"}},
    {"Low blood pressure", {"Dr. Emily Clark - Endocrinologist", "Dr. Rachel Green - General Doctor"}},
    {"Weakness", {"Dr. John Smith - General Doctor", "Dr. David Moore - Neurologist"}},
    {"Excessive thirst", {"Dr. Nathan Foster - Endocrinologist", "Dr. Elizabeth White - Nephrologist"}},
    {"Excessive sweating", {"Dr. Sarah Green - Endocrinologist", "Dr. Thomas Black - General Doctor"}},
    {"Hiccups", {"Dr. Andrew Clark - Gastroenterologist", "Dr. Emily Turner - General Practitioner"}},
    {"Swelling", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Rachel Green - General Doctor"}},
    {"Fainting", {"Dr. Emily Clark - Cardiologist", "Dr. John Smith - General Doctor"}},
    {"Frequent headaches", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Excessive hunger", {"Dr. Nathan Foster - Endocrinologist", "Dr. Emily Clark - Internal Medicine"}},
    {"Heartburn", {"Dr. Emma White - Gastroenterologist", "Dr. Daniel Harris - Internal Medicine"}},
    {"Gastric bloating", {"Dr. Emma White - Gastroenterologist", "Dr. Alice Brown - Internal Medicine"}},
    {"Tingling sensation", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Dry skin", {"Dr. Laura Brown - Dermatologist", "Dr. John Smith - General Doctor"}},
    {"Rash", {"Dr. Laura Brown - Dermatologist", "Dr. Sarah White - Dermatologist"}},
    {"Cramps", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Andrew Moore - General Practitioner"}},
    {"Shivering", {"Dr. John Smith - General Doctor", "Dr. Edward Harris - Pulmonologist"}},
    {"Hair loss", {"Dr. Laura Brown - Dermatologist", "Dr. Emily Clark - General Practitioner"}},
    {"Pale skin", {"Dr. John Smith - General Doctor", "Dr. Alice Davis - Cardiologist"}},
    {"Skin irritation", {"Dr. Laura Brown - Dermatologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Breathing difficulty", {"Dr. Emma White - Pulmonologist", "Dr. Andrew Moore - Pulmonologist"}},
    {"Abnormal stool", {"Dr. Emma White - Gastroenterologist", "Dr. Andrew Brown - General Practitioner"}},
    {"Dark urine", {"Dr. Emily Clark - Gastroenterologist", "Dr. Andrew Brown - Nephrologist"}},
    {"Loss of taste", {"Dr. John Smith - General Doctor", "Dr. Olivia White - ENT Specialist"}},
    {"Difficulty swallowing", {"Dr. Andrew Moore - ENT Specialist", "Dr. Emily Clark - Gastroenterologist"}},
    {"Loss of smell", {"Dr. Olivia White - ENT Specialist", "Dr. John Smith - General Doctor"}},
    {"Coughing blood", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
    {"Feverish chills", {"Dr. John Smith - General Doctor", "Dr. Timothy Black - Infectious Disease Specialist"}},
    {"Yellowing skin", {"Dr. John Smith - General Doctor", "Dr. Andrew Moore - Gastroenterologist"}},
    {"Nausea with headache", {"Dr. John Smith - General Doctor", "Dr. Brian Lee - Neurologist"}},
    {"Sudden weight loss", {"Dr. Emily Roberts - Endocrinologist", "Dr. Nathan Foster - Internal Medicine"}},
    {"Night sweats", {"Dr. Sarah White - Internal Medicine", "Dr. Emily Roberts - Infectious Disease Specialist"}},
    {"Swollen lymph nodes", {"Dr. Alice Green - Oncologist", "Dr. Rachel Brown - Internal Medicine"}},
    {"Severe back pain", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Abdominal bloating", {"Dr. Emma White - Gastroenterologist", "Dr. Michael Turner - General Practitioner"}},
    {"Chronic fatigue", {"Dr. John Smith - General Doctor", "Dr. Alice Green - Rheumatologist"}},
    {"Nasal congestion", {"Dr. Andrew Moore - ENT Specialist", "Dr. Emily Turner - Pulmonologist"}},
    {"Sore tongue", {"Dr. John Smith - General Doctor", "Dr. Alice Brown - Oral Surgeon"}},
    {"Neck pain", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Trouble concentrating", {"Dr. Sarah White - Psychiatrist", "Dr. Thomas Black - General Doctor"}},
    {"Tired eyes", {"Dr. Brian Lee - Ophthalmologist", "Dr. Sarah White - Neurologist"}},
    {"Cough with phlegm", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
    {"Joint stiffness", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Sandra Lee - Rheumatologist"}},
     {"Pain in the chest", {"Dr. Alice Davis - Cardiologist", "Dr. Thomas Harris - Cardiologist"}},
    {"Painful joints", {"Dr. Sandra Lee - Rheumatologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Rash with fever", {"Dr. Sarah Lee - Infectious Disease Specialist", "Dr. Emily Turner - Pediatrician"}},
    {"Sudden weight loss", {"Dr. Nathan Foster - Endocrinologist", "Dr. Elizabeth Green - General Practitioner"}},
    {"Bloating and gas", {"Dr. Emma White - Gastroenterologist", "Dr. Alice Green - Gastroenterologist"}},
    {"Acid reflux", {"Dr. Emma White - Gastroenterologist", "Dr. Sarah Lee - Internal Medicine"}},
    {"Elevated blood pressure", {"Dr. Alice Davis - Cardiologist", "Dr. Rachel Green - Cardiologist"}},
    {"Cold intolerance", {"Dr. Emily Clark - Endocrinologist", "Dr. Nathan Brown - Internal Medicine"}},
    {"Chronic cough", {"Dr. Emma White - Pulmonologist", "Dr. John Smith - General Doctor"}},
    {"Dry skin", {"Dr. Laura Brown - Dermatologist", "Dr. John Smith - General Doctor"}},
    {"Swollen glands", {"Dr. Andrew Moore - ENT Specialist", "Dr. Rachel Green - Internal Medicine"}},
    {"Joint stiffness", {"Dr. Sandra Lee - Rheumatologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Nausea with dizziness", {"Dr. Emma White - Gastroenterologist", "Dr. Brian Lee - Neurologist"}},
    {"Trouble sleeping", {"Dr. Sarah White - Psychiatrist", "Dr. John Smith - General Doctor"}},
    {"Unexplained bruising", {"Dr. Rachel Green - Hematologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Fatigue and weakness", {"Dr. Sarah White - Internal Medicine", "Dr. Nathan Foster - Endocrinologist"}},
    {"Stomach pain after eating", {"Dr. Michael Turner - Gastroenterologist", "Dr. Emma White - Gastroenterologist"}},
    {"Ringing in ears", {"Dr. Patrick Turner - ENT Specialist", "Dr. Olivia Brown - Audiologist"}},
    {"Painful urination", {"Dr. Jane Davis - Urologist", "Dr. John Smith - General Doctor"}},
    {"Sudden hearing loss", {"Dr. Patrick Turner - ENT Specialist", "Dr. Laura Brown - Audiologist"}},
    {"Pale skin", {"Dr. Nathan Brown - Dermatologist", "Dr. Emily Turner - Internal Medicine"}},
    {"Severe abdominal bloating", {"Dr. Emma White - Gastroenterologist", "Dr. Michael Turner - Gastroenterologist"}},
    {"Painful swelling", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Frequent headaches", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Excessive thirst", {"Dr. Nathan Foster - Endocrinologist", "Dr. Elizabeth Green - Nephrologist"}},
    {"Burning sensation in chest", {"Dr. Emma White - Gastroenterologist", "Dr. Alice Green - Internal Medicine"}},
    {"Cramping in legs", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Severe chest pain", {"Dr. Alice Davis - Cardiologist", "Dr. Rachel Green - Cardiologist"}},
    {"Tingling in hands and feet", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Unexplained weight gain", {"Dr. Emily Clark - Endocrinologist", "Dr. Alice Green - Internal Medicine"}},
    {"Bleeding gums", {"Dr. Alice Brown - Dentist", "Dr. John Smith - General Doctor"}},
    {"Decreased appetite", {"Dr. Emma White - Gastroenterologist", "Dr. Sarah Lee - Internal Medicine"}},
    {"Swollen feet", {"Dr. Rachel Green - Cardiologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Frequent urination", {"Dr. Jane Davis - Urologist", "Dr. Andrew Brown - Nephrologist"}},
    {"Frequent nosebleeds", {"Dr. Andrew Moore - ENT Specialist", "Dr. Rachel Green - Internal Medicine"}},
    {"Painful eyes", {"Dr. Brian Lee - Ophthalmologist", "Dr. Sarah White - Optometrist"}},
    {"Feeling faint", {"Dr. Nathan Foster - Endocrinologist", "Dr. John Smith - General Doctor"}},
    {"Extreme sweating", {"Dr. Sarah White - Endocrinologist", "Dr. Nathan Foster - Internal Medicine"}},
    {"Difficulty swallowing", {"Dr. Andrew Moore - ENT Specialist", "Dr. Emily Turner - Gastroenterologist"}},
    {"Dry cough", {"Dr. Emma White - Pulmonologist", "Dr. John Smith - General Doctor"}},
    {"Sharp abdominal pain", {"Dr. Emma White - Gastroenterologist", "Dr. Michael Turner - Gastroenterologist"}},
    {"Swollen legs", {"Dr. Rachel Green - Cardiologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Frequent dizziness", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Shortness of breath on exertion", {"Dr. Emma White - Pulmonologist", "Dr. Alice Green - Cardiologist"}},
    {"Vision problems", {"Dr. Brian Lee - Ophthalmologist", "Dr. John Smith - General Doctor"}},
    {"Numbness in arms", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Fainting spells", {"Dr. Nathan Brown - Cardiologist", "Dr. Sarah White - Neurologist"}},
    {"Pain in joints", {"Dr. Sandra Lee - Rheumatologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Cold sores", {"Dr. Laura Brown - Dermatologist", "Dr. John Smith - General Doctor"}},
    {"Trouble concentrating", {"Dr. Sarah White - Psychiatrist", "Dr. Thomas Black - Neurologist"}},
    {"Rapid weight loss", {"Dr. Nathan Foster - Endocrinologist", "Dr. Elizabeth Green - Internal Medicine"}},
    {"Sore gums", {"Dr. Alice Brown - Dentist", "Dr. John Smith - General Doctor"}},
    {"Painful periods", {"Dr. Sarah Lee - Gynecologist", "Dr. Emily Turner - OB/GYN"}},
    {"Itchy throat", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
    {"Thickened skin", {"Dr. Laura Brown - Dermatologist", "Dr. Sarah White - Rheumatologist"}},
    {"Dry mouth", {"Dr. Alice Brown - Dentist", "Dr. Emily Roberts - Internal Medicine"}},
    {"Severe pain in back", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Rachel Green - Internal Medicine"}},
     {"Fatigue and fever", {"Dr. Rachel Green - Infectious Disease Specialist", "Dr. Sarah White - General Doctor"}},
    {"Tingling in limbs", {"Dr. Brian Lee - Neurologist", "Dr. Michael Turner - Orthopedic Specialist"}},
    {"Severe abdominal bloating", {"Dr. Emma White - Gastroenterologist", "Dr. Michael Turner - Gastroenterologist"}},
    {"Panic attacks", {"Dr. Sarah White - Psychiatrist", "Dr. Thomas Black - Psychologist"}},
    {"Fever and chills", {"Dr. Emma White - Pulmonologist", "Dr. Nathan Brown - Infectious Disease Specialist"}},
    {"Blurred vision", {"Dr. Brian Lee - Ophthalmologist", "Dr. Michael Green - Neurologist"}},
    {"Painful swallowing", {"Dr. Andrew Moore - ENT Specialist", "Dr. Emma White - Gastroenterologist"}},
    {"Nausea and dizziness", {"Dr. Emma White - Gastroenterologist", "Dr. Brian Lee - Neurologist"}},
    {"Joint pain and stiffness", {"Dr. Sandra Lee - Rheumatologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Fainting or dizziness on standing", {"Dr. Nathan Foster - Cardiologist", "Dr. John Smith - General Doctor"}},
    {"Memory loss", {"Dr. Thomas Black - Neurologist", "Dr. Sarah White - Psychiatrist"}},
    {"Bloating after meals", {"Dr. Emma White - Gastroenterologist", "Dr. Nathan Brown - Gastroenterologist"}},
    {"Shortness of breath and wheezing", {"Dr. Emma White - Pulmonologist", "Dr. Rachel Green - Cardiologist"}},
    {"Persistent cough", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
    {"Panic attacks and shortness of breath", {"Dr. Sarah White - Psychiatrist", "Dr. Nathan Foster - Cardiologist"}},
    {"Cold hands and feet", {"Dr. Emily Clark - Endocrinologist", "Dr. Alice Davis - Cardiologist"}},
    {"Chest tightness", {"Dr. Alice Davis - Cardiologist", "Dr. Rachel Green - Pulmonologist"}},
    {"Pain when breathing", {"Dr. Emma White - Pulmonologist", "Dr. Nathan Brown - Cardiologist"}},
    {"Blood in urine", {"Dr. Jane Davis - Urologist", "Dr. Rachel Green - Nephrologist"}},
    {"Back pain radiating to the legs", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Sandra Lee - Rheumatologist"}},
    {"Sudden weight gain", {"Dr. Emily Clark - Endocrinologist", "Dr. Sarah White - General Practitioner"}},
    {"Rashes with fever", {"Dr. Sarah Lee - Infectious Disease Specialist", "Dr. Rachel Green - Dermatologist"}},
    {"Cough with mucus", {"Dr. John Smith - General Doctor", "Dr. Emma White - Pulmonologist"}},
    {"Heart palpitations", {"Dr. Alice Davis - Cardiologist", "Dr. Rachel Green - Cardiologist"}},
    {"Difficulty breathing while lying down", {"Dr. Rachel Green - Pulmonologist", "Dr. Alice Davis - Cardiologist"}},
    {"Numbness in the face", {"Dr. Brian Lee - Neurologist", "Dr. Sarah White - Neurologist"}},
    {"Cramps in feet", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Andrew Moore - Podiatrist"}},
    {"Joint tenderness", {"Dr. Sandra Lee - Rheumatologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Increased urination", {"Dr. Jane Davis - Urologist", "Dr. Nathan Brown - Nephrologist"}},
    {"High fever and vomiting", {"Dr. Emma White - Gastroenterologist", "Dr. Rachel Green - Infectious Disease Specialist"}},
    {"Abdominal cramping", {"Dr. Emma White - Gastroenterologist", "Dr. Michael Turner - Gastroenterologist"}},
    {"Swollen ankles", {"Dr. Rachel Green - Cardiologist", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Pain in the heel", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Andrew Moore - Podiatrist"}},
    {"Nausea with headache", {"Dr. Brian Lee - Neurologist", "Dr. Emma White - Gastroenterologist"}},
    {"Fatigue with skin rash", {"Dr. Laura Brown - Dermatologist", "Dr. Sarah White - Internal Medicine"}},
    {"Chronic sore throat", {"Dr. John Smith - General Doctor", "Dr. Andrew Moore - ENT Specialist"}},
    {"Excessive thirst and urination", {"Dr. Nathan Foster - Endocrinologist", "Dr. Elizabeth Green - Internal Medicine"}},
    {"Painful periods and bloating", {"Dr. Sarah Lee - Gynecologist", "Dr. Emma White - Gastroenterologist"}},
    {"Red, itchy eyes", {"Dr. Brian Lee - Ophthalmologist", "Dr. John Smith - General Doctor"}},
    {"Heavy breathing and coughing", {"Dr. Emma White - Pulmonologist", "Dr. Alice Green - Cardiologist"}},
    {"Unexplained weight loss", {"Dr. Nathan Foster - Endocrinologist", "Dr. Elizabeth Green - Internal Medicine"}},
    {"Pain in upper abdomen", {"Dr. Emma White - Gastroenterologist", "Dr. Sarah Lee - Internal Medicine"}},
    {"Painful swallowing with fever", {"Dr. Andrew Moore - ENT Specialist", "Dr. John Smith - General Doctor"}},
    {"Persistent low-grade fever", {"Dr. Rachel Green - Infectious Disease Specialist", "Dr. John Smith - General Doctor"}},
    {"Tingling sensation in the face", {"Dr. Brian Lee - Neurologist", "Dr. John Smith - General Doctor"}},
    {"Difficulty sleeping", {"Dr. Sarah White - Psychiatrist", "Dr. Thomas Black - Neurologist"}},
    {"Mucus in stool", {"Dr. Emma White - Gastroenterologist", "Dr. John Smith - General Doctor"}},
    {"Wheezing and chest tightness", {"Dr. Rachel Green - Pulmonologist", "Dr. Alice Davis - Cardiologist"}},
    {"Pain and swelling in knees", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Sandra Lee - Rheumatologist"}},
    {"Itchy skin after a meal", {"Dr. Laura Brown - Dermatologist", "Dr. Nathan Foster - Allergist"}},
    {"Fatigue and difficulty concentrating", {"Dr. Sarah White - Psychiatrist", "Dr. Nathan Foster - Internal Medicine"}},
    {"Shortness of breath after exertion", {"Dr. Emma White - Pulmonologist", "Dr. Alice Davis - Cardiologist"}},
    {"Pain in the neck and shoulders", {"Dr. Michael Green - Orthopedic Specialist", "Dr. John Smith - General Doctor"}},
    {"Swelling around the eyes", {"Dr. Andrew Moore - ENT Specialist", "Dr. Rachel Green - Dermatologist"}},
    {"Tightness in the chest with pain", {"Dr. Rachel Green - Cardiologist", "Dr. Alice Davis - Cardiologist"}},
    {"Fainting during physical activity", {"Dr. Nathan Brown - Cardiologist", "Dr. Sarah White - Internal Medicine"}},
    {"Headache with vision problems", {"Dr. Brian Lee - Neurologist", "Dr. Michael Green - Ophthalmologist"}},
    {"Persistent itchy skin", {"Dr. Laura Brown - Dermatologist", "Dr. Sarah Lee - Dermatologist"}},
    {"Pale nails and skin", {"Dr. Nathan Brown - Dermatologist", "Dr. Rachel Green - Cardiologist"}},
    {"Liver pain with nausea", {"Dr. Sarah Lee - Hepatologist", "Dr. Michael Turner - Gastroenterologist"}},
    {"Sore muscles with fever", {"Dr. John Smith - General Doctor", "Dr. Michael Green - Orthopedic Specialist"}},
    {"Difficulty breathing while exercising", {"Dr. Emma White - Pulmonologist", "Dr. Rachel Green - Cardiologist"}},
    {"Fever with chills and night sweats", {"Dr. Rachel Green - Infectious Disease Specialist", "Dr. Emma White - Pulmonologist"}},
    {"Pain when swallowing food", {"Dr. Andrew Moore - ENT Specialist", "Dr. Michael Turner - Gastroenterologist"}},
    {"Blurry vision and dizziness", {"Dr. Brian Lee - Neurologist", "Dr. Michael Green - Ophthalmologist"}},
    {"Pain in legs after walking", {"Dr. Michael Green - Orthopedic Specialist", "Dr. Rachel Green - Cardiologist"}},
    {"Heavy menstrual bleeding", {"Dr. Sarah Lee - Gynecologist", "Dr. Emily Turner - OB/GYN"}},
    {"Feeling cold with fever", {"Dr. John Smith - General Doctor", "Dr. Sarah White - Internal Medicine"}}
    };


   auto it = symptomDoctors.find(symptom);
    if (it != symptomDoctors.end())
    {
        cout << "Suggested doctors for " << symptom << ":\n";
        for (const string& doctor : it->second)
        {
            cout << doctor << endl;
        }
        cout << "Do you want to book an appointment with one of these doctors? (yes/no): ";
        string response;
        cin >> response;

        if (response == "yes")
        {
            string doctorChoice;
            cout << "Enter doctor name to book an appointment: ";
            cin.ignore();  // Clear leftover newline
            getline(cin, doctorChoice);

            // Convert entered doctor's name to lowercase
            string lowerDoctorChoice = toLowerCase(doctorChoice);

            // Check if entered doctor name exists in the suggested list (case-insensitive)
            bool validDoctor = false;
            for (const string& doctor : it->second)
            {
                // Extract just the doctor's name part and convert it to lowercase
                string doctorName = extractDoctorName(doctor);
                if (toLowerCase(doctorName) == lowerDoctorChoice) {
                    validDoctor = true;
                    break;
                }
            }

            if (validDoctor)
            {
                // Get current date and time
                time_t now = time(0);
                tm *ltm = localtime(&now);
                cout << "Appointment booked for " << doctorChoice << " on "
                     << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
                     << " at " << 1 + ltm->tm_hour << ":" << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec << ".\n";
            }
             else
            {
                cout << "Invalid doctor name. Please enter a valid name from the suggested list.\n";
            }
        }
        else
            {
            cout << "You can continue searching for symptoms or doctors.\n";
        }
    }
     else
     {
        cout << "No doctors found for this symptom.\n";
    }
}
/*
void searchSymptoms() {
    int searchChoice;
    cout << "1. Search by symptom name\n2. Search by symptom description\nChoose an option: ";
    cin >> searchChoice;

    cin.ignore();  // Ignore any leftover newline character after choosing the option

    vector<string> matches;
    if (searchChoice == 1) {
        string symptomName;
        cout << "Enter the symptom name (partial matches allowed): ";
        getline(cin, symptomName);

        // Convert entered symptom name to lowercase for case-insensitive comparison
        string lowerSymptomName = toLowerCase(symptomName);

        // Search by symptom name using Levenshtein distance for close matching
        int threshold = 3; // Set threshold for "close enough" match
        bool foundMatch = false;
        for (const auto &symptom : symptoms) {
            // Convert stored symptom name to lowercase for comparison
            string lowerSymptom = toLowerCase(symptom.first);
            int distance = levenshteinDistance(lowerSymptom, lowerSymptomName);
            if (distance <= threshold) { // If the distance is within the threshold, it's a match
                matches.push_back(symptom.first + " - " + symptom.second);
                foundMatch = true;
            }
        }

        if (foundMatch) {
            cout << "Matching symptoms found:\n";
            for (const auto &match : matches) {
                cout << match << endl;
            }
            cout << "Would you like to book an appointment based on any of these symptoms? (yes/no): ";
            string response;
            cin >> response;
            if (response == "yes") {
                cout << "Enter symptom name to book an appointment: ";
                string symptom;
                cin.ignore();  // Ignore leftover newline
                getline(cin, symptom);

                // Convert entered symptom name to lowercase for case-insensitive comparison
                string lowerSymptom = toLowerCase(symptom);

                // Suggest doctors for the symptom (case-insensitive)
                suggestDoctors(lowerSymptom);
            }
        } else {
            cout << "No matching symptoms found.\n";
        }
    } else if (searchChoice == 2) {
        string description;
        cout << "Enter the symptom description (partial matches allowed): ";
        getline(cin, description);

        // Search by symptom description (partial match)
        bool foundMatch = false;
        for (const auto &symptom : symptoms) {
            if (symptom.second.find(description) != string::npos) { // Partial match
                matches.push_back(symptom.first + " - " + symptom.second);
                foundMatch = true;
            }
        }

        if (foundMatch) {
            cout << "Matching symptoms found:\n";
            for (const auto &match : matches) {
                cout << match << endl;
            }
            cout << "Would you like to book an appointment based on any of these symptoms? (yes/no): ";
            string response;
            cin >> response;
            if (response == "yes") {
                cout << "Enter symptom name to book an appointment: ";
                string symptom;
                cin >> symptom;
                suggestDoctors(symptom); // Suggest doctors for the symptom
            }
        } else {
            cout << "No matching symptoms found.\n";
        }
    } else {
        cout << "Invalid choice.\n";
    }
}
*/
// Function to allow doctor to add a new symptom
// This function enables a doctor to add a symptom with its name and description
// to the system's symptom database.
void addSymptom(unordered_map<string, Doctor>& doctors)
 {
    string symptomName, description;

    // Prompt the doctor to enter the name of the symptom
    cout << "Enter the name of the symptom: ";
    cin.ignore();  // To clear the newline left by the previous input
    getline(cin, symptomName);

    // Prompt the doctor to provide a description of the symptom
    cout << "Enter the description of the symptom: ";
    getline(cin, description);

    // Check if the symptom already exists in the database
    // This prevents duplication of symptoms in the system
    if (symptoms.find(symptomName) != symptoms.end())
    {
        cout << "Symptom already exists.\n";
        return;
    }

    // Add the new symptom to the symptoms map
    // Storing the symptom name and its description
    symptoms[symptomName] = description;

    // Save the new symptom to the symptoms file
    // Opening the file in append mode to retain existing data
    ofstream file("symptoms.csv", ios::app);
    if (!file) {
        cerr << "Error: Unable to open symptoms.csv file.\n";
        return;
    }
    file << symptomName << "," << description << "\n";
    file.close();

    cout << "Symptom added successfully!\n";

    // Provide feedback to the doctor about the added symptom
    cout << "\nNewly added symptom details:\n";
    cout << "Symptom Name: " << symptomName << endl;
    cout << "Description: " << description << endl;

    // Log the addition for audit purposes
    // The log helps track changes and additions for debugging or review
    ofstream logFile("symptom_log.txt", ios::app);
    if (logFile) {
        logFile << "Symptom added: " << symptomName << " - " << description << "\n";
        logFile.close();
    } else {
        cerr << "Error: Unable to open symptom_log.txt file for logging.\n";
    }

    // Placeholder for future features
    // Example: Sending notifications to administrators or other users

    // Notify the doctor about next steps or additional options
    cout << "\nThank you for keeping the system up to date.\n";
    cout << "For any issues, contact support at support@healthcare.com.\n";

    // Space for additional lines or comments without changing functionality
    // These lines ensure the function can be expanded easily in the future

    // Reserve lines for integration with other systems
    // Example: API calls or database updates

    // Reserve lines for implementing more robust error handling mechanisms

    // Space for analytics or tracking feature additions

    // Future scope: Adding a confirmation dialog before saving

    // Space for customization options (e.g., user-defined symptom categories)

    // Placeholder for additional user feedback mechanisms

    // End of the addSymptom function
}

// Function for doctor login and providing additional functionalities
// Modify the doctor login section to add an option to add symptoms
void doctorLogin(unordered_map<string, Doctor>& doctors)
{
    string userID, password;  // Variables to store login credentials

    // Prompt for doctor credentials
    cout << "Enter UserID: ";
    cin >> userID;
    cout << "Enter Password: ";
    //cin >> password;
    char ch;
     while(true)
        {
            ch=getch();
            if(ch=='\r')
            break;
            if(ch=='\b')
            {
                if(!password.empty())
                {
                    cout<<"\b \b";
                    password.pop_back();
                }
            }
            else
            {
                password+=ch;
                cout<<'*';
            }
        }
    // Check if the entered credentials match any doctor record
    if (doctors.find(userID) != doctors.end() && doctors[userID].password == password)
    {
        cout << "Login successful!\nWelcome, Dr. " << doctors[userID].name
             << " (" << doctors[userID].specialization << ").\n";

        // Provide options for the doctor
        int doctorChoice;
        do
        {
            cout << "\n--- Doctor Menu ---\n";
            cout << "1. Add new symptom\n";
            cout << "2. View handled patients\n";
            cout << "3. Exit\n";
            cout << "Choose an option: ";
            cin >> doctorChoice;

            // Handle the doctor choice
            switch (doctorChoice)
        {
                case 1:
                    // Call the function to add a symptom
                    addSymptom(doctors);
                    break;

                case 2:
                    // Display handled patients
                    cout << "\nPatients handled by Dr. " << doctors[userID].name << ":\n";
                    for (const string& patientID : doctors[userID].handledPatients) {
                        cout << "- " << patientID << "\n";
                    }
                    break;

                case 3:
                    // Exit the menu
                    cout << "Exiting Doctor Menu. Have a great day!\n";
                    break;

                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        }
        while (doctorChoice != 3);

    }
     else
    {
        // Invalid credentials message
        cout << "Invalid credentials.\n";
    }

    // Placeholder for future extensions
    // Example: Implementing password reset functionality

    // Reserve space for integrating doctor-specific analytics

    // Placeholder for advanced features like multi-factor authentication

    // Space for optimizations, such as caching recent login attempts

    // End of function
}

#include <algorithm>  // For std::transform

// Function to convert string to lowercase
string toLowerCase1(const string &str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
// Function to search for symptoms
void searchSymptoms()
{
    int searchChoice;
    cout << "1. Search by symptom name\n2. Search by symptom description\nChoose an option: ";
    cin >> searchChoice;

    cin.ignore();  // Ignore any leftover newline character after choosing the option

    vector<string> matches;

    if (searchChoice == 1)
    {
        string symptomName;
        cout << "Enter the symptom name (partial matches allowed): ";
        getline(cin, symptomName);

        // Convert entered symptom name to lowercase for case-insensitive comparison
        string lowerSymptomName = toLowerCase(symptomName);

        // List of symptoms and their descriptions
        unordered_map<string, string> symptoms = {
             {"Fever", "A temporary increase in body temperature, often due to an infection."},
        {"Cough", "A reflex action to clear the throat and airways."},
        {"Headache", "Pain in the head or upper neck region."},
        {"Sore throat", "Pain or irritation in the throat, especially when swallowing."},
        {"Fatigue", "Extreme tiredness or lack of energy."},
        {"Body ache", "Muscular pain or discomfort throughout the body."},
        {"Nausea", "The sensation of wanting to vomit."},
        {"Vomiting", "The act of forcefully expelling stomach contents through the mouth."},
        {"Shortness of breath", "A feeling of not being able to breathe deeply or quickly enough."},
        {"Chills", "Shivering or feeling cold often accompanied by fever."},
        {"Dizziness", "A sensation of lightheadedness or vertigo, where the person feels they might faint."},
        {"Diarrhea", "Frequent, watery bowel movements that can be a sign of infection or digestive issues."},
        {"Constipation", "Difficulty passing stool or infrequent bowel movements."},
        {"Loss of appetite", "A decrease in the desire to eat, often due to illness or stress."},
        {"Skin rash", "A noticeable change in the skin's appearance, typically caused by an allergy or infection."},
        {"Muscle cramps", "Involuntary muscle contractions causing pain, usually in legs or arms."},
        {"Sore muscles", "Pain and discomfort in muscles, often due to overuse or injury."},
        {"Runny nose", "Excess mucus production in the nasal passages, commonly associated with colds."},
        {"Numbness", "A loss of sensation or feeling in a part of the body."},
        {"Fever", "A temporary increase in body temperature, often due to an infection."},
        {"Cough", "A reflex action to clear the throat and airways."},
        {"Headache", "Pain in the head or upper neck region."},
        {"Sore throat", "Pain or irritation in the throat, especially when swallowing."},
        {"Fatigue", "Extreme tiredness or lack of energy."},
        {"Body ache", "Muscular pain or discomfort throughout the body."},
        {"Nausea", "The sensation of wanting to vomit."},
        {"Vomiting", "The act of forcefully expelling stomach contents through the mouth."},
        {"Shortness of breath", "A feeling of not being able to breathe deeply or quickly enough."},
        {"Chills", "Shivering or feeling cold often accompanied by fever."},
        {"Dizziness", "A sensation of lightheadedness or vertigo, where the person feels they might faint."},
        {"Diarrhea", "Frequent, watery bowel movements that can be a sign of infection or digestive issues."},
        {"Constipation", "Difficulty passing stool or infrequent bowel movements."},
        {"Loss of appetite", "A decrease in the desire to eat, often due to illness or stress."},
        {"Skin rash", "A noticeable change in the skin's appearance, typically caused by an allergy or infection."},
        {"Muscle cramps", "Involuntary muscle contractions causing pain, usually in legs or arms."},
        {"Sore muscles", "Pain and discomfort in muscles, often due to overuse or injury."},
        {"Runny nose", "Excess mucus production in the nasal passages, commonly associated with colds."},
        {"Numbness", "A loss of sensation or feeling in a part of the body."}, {"Fever", "A temporary increase in body temperature, often due to an infection."},


    {"Cough", "A reflex action to clear the throat and airways."},
    {"Headache", "Pain in the head or upper neck region."},
    {"Sore throat", "Pain or irritation in the throat, especially when swallowing."},
    {"Fatigue", "Extreme tiredness or lack of energy."},
    {"Body ache", "Muscular pain or discomfort throughout the body."},
    {"Nausea", "The sensation of wanting to vomit."},
    {"Vomiting", "The act of forcefully expelling stomach contents through the mouth."},
    {"Shortness of breath", "A feeling of not being able to breathe deeply or quickly enough."},
    {"Chills", "Shivering or feeling cold often accompanied by fever."},
    {"Dizziness", "A sensation of lightheadedness or vertigo, where the person feels they might faint."},
    {"Diarrhea", "Frequent, watery bowel movements that can be a sign of infection or digestive issues."},
    {"Constipation", "Difficulty passing stool or infrequent bowel movements."},
    {"Loss of appetite", "A decrease in the desire to eat, often due to illness or stress."},
    {"Skin rash", "A noticeable change in the skin's appearance, typically caused by an allergy or infection."},
    {"Muscle cramps", "Involuntary muscle contractions causing pain, usually in legs or arms."},
    {"Sore muscles", "Pain and discomfort in muscles, often due to overuse or injury."},
    {"Runny nose", "Excess mucus production in the nasal passages, commonly associated with colds."},
    {"Numbness", "A loss of sensation or feeling in a part of the body."},
    {"Abdominal pain", "Pain in the stomach or abdominal region, which can be caused by various factors."},
    {"Back pain", "Pain in the back, ranging from mild discomfort to severe pain."},
    {"Chest pain", "Pain or discomfort in the chest area, potentially related to heart conditions."},
    {"Earache", "Pain in the ear, commonly caused by infections or pressure changes."},
    {"Fatigue and weakness", "Excessive tiredness and lack of strength, often indicating underlying health issues."},
    {"Frequent urination", "The need to urinate more often than usual, potentially due to infection or other issues."},
    {"Heartburn", "A burning sensation in the chest, typically caused by acid reflux."},
    {"Heavy breathing", "Difficulty breathing or labored breathing, often associated with respiratory conditions."},
    {"Insomnia", "Difficulty falling or staying asleep, affecting overall well-being."},
    {"Jaundice", "A yellowing of the skin or eyes, often indicating liver issues."},
    {"Knee pain", "Discomfort or pain in the knee, which may arise from overuse or injury."},
    {"Lightheadedness", "A feeling of faintness or dizziness, often a sign of low blood pressure or dehydration."},
    {"Mouth ulcers", "Painful sores in the mouth, often caused by viral infections or injury."},
    {"Muscle weakness", "A reduction in the strength of the muscles, often linked to various health conditions."},
    {"Neck pain", "Pain in the neck, commonly due to poor posture or muscle strain."},
    {"Night sweats", "Excessive sweating during sleep, potentially caused by infections or other medical conditions."},
    {"Panic attacks", "Sudden feelings of intense fear or anxiety, often accompanied by physical symptoms."},
    {"Palpitations", "Irregular or unusually strong heartbeats, which can be caused by stress or heart conditions."},
    {"Pneumonia", "An infection of the lungs that causes cough, fever, and difficulty breathing."},
    {"Post-nasal drip", "The sensation of mucus dripping down the back of the throat, often caused by sinus issues."},
    {"Rash", "A visible change in skin color or texture, often due to an allergic reaction or infection."},
    {"Ringing in the ears", "A sensation of hearing ringing or buzzing, typically related to ear issues."},
    {"Severe pain", "Intense pain that may arise from injury, inflammation, or illness."},
    {"Shortness of breath while lying down", "Difficulty breathing when lying flat, often related to heart or lung issues."},
    {"Sinus pressure", "Pain or fullness in the sinus cavities, often caused by sinus infections or allergies."},
    {"Sore eyes", "Pain or irritation in the eyes, which may be caused by dryness, allergies, or infection."},
    {"Swelling", "An abnormal enlargement of a part of the body, often due to fluid retention or inflammation."},
    {"Throat swelling", "Swelling of the throat, which can affect swallowing and may be caused by infections."},
    {"Toothache", "Pain in or around a tooth, often due to dental issues such as cavities or infections."},
    {"Vision problems", "Changes in vision, including blurred or double vision, often signaling eye or neurological issues."},
    {"Vomiting blood", "The act of throwing up blood, which can be a serious symptom of digestive tract bleeding."},
    {"Wheezing", "A high-pitched sound when breathing, often indicating respiratory issues such as asthma."},
    {"Yellowing of the skin", "A symptom often related to liver or gallbladder issues, causing jaundice."},
    {"Hoarseness", "A raspy or strained voice, often caused by irritation or inflammation in the vocal cords."},
    {"Anxiety", "A feeling of unease, worry, or nervousness, sometimes accompanied by physical symptoms."},
    {"Cold sores", "Painful blisters around the mouth or lips, typically caused by a viral infection."},
    {"Constipation with bloating", "Difficulty passing stool combined with a feeling of fullness or tightness in the abdomen."},
    {"Excessive thirst", "A strong, persistent desire to drink fluids, often due to dehydration or diabetes."},
    {"Fainting", "A temporary loss of consciousness, typically caused by a drop in blood pressure."},
    {"Hair loss", "The shedding or thinning of hair, which may result from various medical conditions or stress."},
    {"Heart palpitations with dizziness", "A sensation of rapid heartbeats accompanied by lightheadedness or fainting."},
    {"High blood pressure", "Elevated blood pressure levels that may indicate an increased risk for heart disease."},
    {"Hot flashes", "A sudden feeling of intense heat, often associated with menopause or certain medications."},
    {"Hunger pangs", "A sensation of discomfort caused by a need for food, typically associated with low blood sugar."},
    {"Joint swelling", "Inflammation or fluid accumulation in the joints, often caused by arthritis or injury."},
    {"Leg cramps", "Involuntary muscle contractions, commonly occurring in the calves, often at night."},
    {"Mucus in the stool", "The presence of mucus in bowel movements, which can indicate gastrointestinal issues."},
    {"Painful urination", "A burning or stinging sensation during urination, often caused by urinary tract infections."},
    {"Persistent cough", "A cough that lasts for several weeks, often due to chronic bronchitis or other conditions."},
    {"Rashes with blisters", "Inflammation of the skin accompanied by blister formation, which may result from infections or allergies."},
    {"Severe dehydration", "A condition in which the body loses too much water, leading to dizziness, fatigue, and dry skin."},
    {"Sore gums", "Pain or swelling in the gums, which may be caused by gingivitis or other oral health issues."},
    {"Tingling in the hands", "A sensation of numbness or pins-and-needles feeling in the hands, often due to nerve issues."},
    {"Tired eyes", "Discomfort or fatigue in the eyes, commonly caused by prolonged screen time or eye strain."},
    {"Urinary retention", "The inability to fully empty the bladder, often due to obstructions or nerve issues."},
    {"Vision loss", "A partial or total loss of sight, which can be caused by eye injuries, diseases, or neurological conditions."},
    {"Wrist pain", "Pain in the wrist joint, often resulting from strain, arthritis, or injury."},
    {"Weight loss with no reason", "Unexplained weight loss, which can be a symptom of various medical conditions such as thyroid disorders."},
    {"Dry mouth", "A condition where the mouth feels dry, often due to medication side effects or dehydration."},
    {"Abnormal bruising", "Unexplained bruises appearing on the body, which may be caused by blood disorders or injury."},
    {"Bloating after meals", "A feeling of fullness and swelling in the abdomen after eating, often caused by digestive issues."},
    {"Breathlessness during exercise", "A feeling of difficulty breathing when performing physical activities, often caused by asthma or cardiovascular issues."},
    {"Chronic fatigue", "Persistent tiredness not relieved by rest, often associated with conditions like chronic fatigue syndrome."},
    {"Cramps in the stomach", "Painful muscle contractions in the stomach, often caused by indigestion or menstrual cramps."},
    {"Dark urine", "Urine that appears darker than usual, which may indicate liver or kidney issues."},
    {"Dry skin", "Skin that appears rough, flaky, or itchy, often due to dehydration or skin conditions."},
    {"Ear ringing", "A ringing or buzzing sound in the ears, typically caused by damage to the ear or hearing loss."},
    {"Excessive sweating", "Abnormal perspiration, often due to hyperhidrosis or an underlying health condition."},
    {"Extreme thirst", "A persistent desire to drink fluids, often seen in diabetes or dehydration."},
    {"Hearing loss", "Partial or complete inability to hear, which may occur gradually or suddenly."},
    {"Hip pain", "Pain in the hip joint, which can be caused by arthritis, injury, or overuse."},
    {"Indigestion", "A discomfort or pain in the upper abdomen after eating, often related to food intolerance or gastrointestinal issues."},
    {"Irregular heartbeat", "A heart rhythm that is abnormal, potentially caused by arrhythmia or heart disease."},
    {"Irritable bowel syndrome", "A digestive disorder characterized by abdominal cramps, diarrhea, and constipation."},
    {"Itchy scalp", "A sensation of itching on the scalp, often caused by dandruff, dryness, or fungal infections."},



    {"Jaundice", "A yellowing of the skin or eyes, often caused by liver disease or bile duct obstruction."},
    {"Knee pain", "Pain or discomfort in the knee joint, potentially caused by arthritis, injury, or overuse."},
    {"Lung pain", "Pain in the chest area, typically associated with respiratory infections or conditions like pneumonia."},
    {"Malaise", "A general feeling of discomfort or illness, often preceding other symptoms of disease."},
    {"Memory loss", "A decrease in the ability to remember things, which can result from aging, stress, or medical conditions."},
    {"Mild fever", "A slight increase in body temperature, often indicating a mild infection or inflammation."},
    {"Mouth ulcers", "Painful sores inside the mouth, often caused by viral infections or stress."},
    {"Neck stiffness", "Pain or tightness in the neck, often caused by poor posture, tension, or injury."},
    {"Nosebleeds", "Bleeding from the nose, often caused by dryness, injury, or underlying medical conditions."},
    {"Numbness in the limbs", "Loss of sensation or tingling in the arms or legs, often due to nerve compression or circulatory issues."},
    {"Numbness in the face", "A loss of sensation in the facial area, often due to nerve damage or other neurological conditions."},
    {"Oily skin", "Excessive secretion of oil from the sebaceous glands, which may cause acne or skin irritation."},
    {"Pale skin", "A lighter skin tone, often indicating anemia, low blood pressure, or shock."},
    {"Paralysis", "The loss of muscle function in part of the body, often due to nerve damage or injury."},
    {"Peeling skin", "The shedding of dead skin cells, often caused by sunburn or certain skin conditions."},
    {"Persistent hunger", "An ongoing feeling of hunger despite eating, which could be related to metabolic disorders or diabetes."},
    {"Poor circulation", "Reduced blood flow to certain areas of the body, which can cause numbness or cold extremities."},
    {"Postural dizziness", "A sensation of dizziness when changing positions, such as standing up from a sitting position."},
    {"Puffy eyes", "Swelling or puffiness around the eyes, often caused by lack of sleep, allergies, or fluid retention."},
    {"Rashes with swelling", "Skin inflammation that includes redness and swelling, which may result from allergic reactions or infections."},
    {"Restlessness", "A feeling of unease or inability to relax, often associated with anxiety or stress."},
    {"Rib pain", "Pain in the rib area, which can result from injury, inflammation, or respiratory issues."},
    {"Sallow complexion", "A pale and unhealthy appearance of the skin, often linked to anemia or liver disease."},
    {"Scaly skin", "Rough, dry, and flaky skin, often due to eczema, psoriasis, or dehydration."},
    {"Severe abdominal cramps", "Intense and painful stomach cramps, often related to digestive disorders or menstruation."},
    {"Severe headache", "A painful and often debilitating headache that can be caused by various factors, including tension or migraines."},
    {"Shaking hands", "Involuntary hand tremors, which may be associated with neurological disorders such as Parkinson's disease."},
    {"Short-term memory loss", "Difficulty remembering recent events or information, often related to stress, sleep deprivation, or aging."},
    {"Sore breasts", "Tenderness or discomfort in the breast area, which may be linked to hormonal changes or infection."},
    {"Sore feet", "Pain or discomfort in the feet, often due to standing for long periods, injury, or conditions like plantar fasciitis."},
    {"Sore gums", "Inflammation and tenderness in the gums, which may be caused by gingivitis or other oral health problems."},
    {"Sore lips", "Pain or cracking on the lips, often caused by dryness, chapped lips, or a viral infection."},
    {"Sore shoulders", "Pain or stiffness in the shoulder area, commonly due to muscle strain or injury."},
    {"Sore throat with fever", "A painful throat combined with an elevated body temperature, typically due to viral or bacterial infections."},
    {"Sputum production", "The production of mucus from the respiratory tract, often seen in respiratory infections like bronchitis."},
    {"Stomach bloating", "A sensation of fullness or tightness in the stomach, often due to digestive issues or excessive gas."},
    {"Stomach cramps", "Painful muscle contractions in the stomach, which may result from indigestion, menstruation, or infection."},
    {"Stomach gas", "Excessive accumulation of gas in the stomach, often leading to discomfort or belching."},
    {"Stress", "A mental and emotional state caused by external pressures, which can affect physical health."},
    {"Swollen ankles", "Enlarged or puffed-up ankles, often caused by fluid retention or circulatory problems."},
    {"Swollen glands", "Enlarged lymph nodes, often a response to infection or illness."},
    {"Tightness in the chest", "A feeling of pressure or constriction in the chest, which may signal heart or lung issues."},
    {"Tingling sensation", "A prickling or 'pins and needles' feeling, often caused by nerve compression or poor circulation."},
    {"Tiredness", "An overall feeling of fatigue or lack of energy, often resulting from lack of sleep or illness."},
    {"Unexplained weight loss", "The loss of body weight without a clear cause, which may indicate underlying medical conditions."},
    {"Upper back pain", "Pain in the upper back area, which can be caused by muscle strain or spinal issues."},
    {"Urinary incontinence", "The involuntary loss of urine, often due to age, pregnancy, or medical conditions."},
    {"Urinary tract infection", "An infection in any part of the urinary system, commonly causing painful urination and frequent urges."},
    {"Vomiting with blood", "Expelling blood along with stomach contents, often due to gastrointestinal bleeding or ulcers."},
    {"Weakness in the arms", "Reduced strength or control in the arms, often resulting from nerve or muscle disorders."},
    {"Weakness in the legs", "Reduced strength or difficulty in using the legs, potentially caused by neurological or muscular conditions."},
    {"Weight gain", "The accumulation of excess body weight, which may result from overeating, inactivity, or hormonal changes."},
    {"Wheezing with a cough", "A whistling sound while breathing combined with a persistent cough, often due to asthma or bronchitis."},
    {"Yellow stool", "Stool that appears yellow in color, often related to digestive issues, such as bile duct problems."},
    {"Yellow teeth", "A yellowish appearance of teeth, commonly due to staining, poor hygiene, or aging."},
    {"Zits", "Small, inflamed pimples that typically appear on the face due to clogged pores."},
    {"Abdominal bloating", "A feeling of fullness or distention in the abdomen, often caused by digestive issues or overeating."},
    {"Abnormal heartbeat", "An irregular or erratic heartbeat, which can be caused by arrhythmia or heart disease."},
    {"Acid reflux", "A condition where stomach acid flows back into the esophagus, causing heartburn and discomfort."},
    {"Ankle sprain", "A stretching or tearing of ligaments in the ankle, often caused by twisting or turning the foot."},
    {"Apathy", "A lack of interest or emotion, often seen in conditions like depression or neurological disorders."},
    {"Arthritis", "Inflammation of the joints that can lead to pain, swelling, and limited movement."},
    {"Asthma", "A chronic respiratory condition marked by difficulty breathing, wheezing, and shortness of breath."},
    {"Blurry vision", "A lack of sharpness in vision, often caused by refractive errors, cataracts, or other eye conditions."},
    {"Bronchitis", "Inflammation of the bronchial tubes, often causing coughing, mucus production, and difficulty breathing."},
    {"Carpal tunnel syndrome", "A condition where pressure on the median nerve in the wrist causes numbness, tingling, and weakness."},
    {"Chest tightness", "A sensation of pressure or constriction in the chest, often related to heart or lung conditions."},
    {"Chronic cough", "A persistent cough lasting for more than three weeks, which can indicate underlying health conditions."},
    {"Chronic pain", "Long-lasting pain that persists for weeks, months, or even years, often related to injury or disease."},
    {"Chronic stress", "Long-term stress that can lead to a variety of physical and emotional health problems."},
    {"Clammy skin", "Skin that feels moist and cool, often associated with fever, shock, or anxiety."},
    {"Cold hands", "A feeling of coldness in the hands, often due to poor circulation or cold temperatures."},
    {"Cold intolerance", "A sensitivity to cold temperatures, often seen in conditions like hypothyroidism."},
    {"Cramps during menstruation", "Painful contractions of the uterus during menstruation, commonly known as period cramps."},
    {"Cracked heels", "Dry, cracked skin on the heels, often caused by standing for long periods or lack of moisture."},
    {"Cystic acne", "A severe form of acne that results in large, painful cysts under the skin."},
    {"Dandruff", "Flaking of dead skin from the scalp, which may be caused by dry skin or seborrheic dermatitis."},
    {"Depression", "A mental health condition characterized by persistent feelings of sadness, hopelessness, and loss of interest."},
    {"Difficulty swallowing", "The sensation of food getting stuck or difficulty moving food down the throat, often due to esophageal issues."},
    {"Discolored skin", "Skin that appears darker or lighter than normal, often due to sun exposure, injury, or medical conditions."},
    {"Ear infection", "An infection of the ear canal or middle ear, often causing pain, swelling, and hearing issues."},
    {"Eczema", "A chronic skin condition characterized by itchy, red, and inflamed patches of skin."},
    {"Elevated heart rate", "A faster-than-normal heart rate, which can occur due to stress, exercise, or heart condotion"}

        };

        bool foundMatch = false;
        for (const auto& symptom : symptoms) {
            // Convert stored symptom name to lowercase for comparison
            string lowerStoredSymptom = toLowerCase(symptom.first);
            if (lowerStoredSymptom.find(lowerSymptomName) != string::npos) {
                matches.push_back(symptom.first + " - " + symptom.second);
                foundMatch = true;
            }
        }

        if (foundMatch) {
            cout << "Matching symptoms found:\n";
            for (const auto& match : matches) {
                cout << match << endl;
            }
            cout << "Would you like to book an appointment based on any of these symptoms? (yes/no): ";
            string response;
            cin >> response;
            if (response == "yes") {
                cout << "Enter symptom name to book an appointment: ";
                string symptom;
                cin.ignore();  // Ignore leftover newline
                getline(cin, symptom);

                // Suggest doctors for the symptom
                suggestDoctors(symptom);
            }
        } else {
            cout << "No matching symptoms found.\n";
        }
    } else if (searchChoice == 2) {
        string description;
        cout << "Enter the symptom description (partial matches allowed): ";
        getline(cin, description);

        // Search by symptom description (partial match)
        bool foundMatch = false;
        unordered_map<string, string> symptoms = {
             {"Fever", "A temporary increase in body temperature, often due to an infection."},
        {"Cough", "A reflex action to clear the throat and airways."},
        {"Headache", "Pain in the head or upper neck region."},
        {"Sore throat", "Pain or irritation in the throat, especially when swallowing."},
        {"Fatigue", "Extreme tiredness or lack of energy."},
        {"Body ache", "Muscular pain or discomfort throughout the body."},
        {"Nausea", "The sensation of wanting to vomit."},
        {"Vomiting", "The act of forcefully expelling stomach contents through the mouth."},
        {"Shortness of breath", "A feeling of not being able to breathe deeply or quickly enough."},
        {"Chills", "Shivering or feeling cold often accompanied by fever."},
        {"Dizziness", "A sensation of lightheadedness or vertigo, where the person feels they might faint."},
        {"Diarrhea", "Frequent, watery bowel movements that can be a sign of infection or digestive issues."},
        {"Constipation", "Difficulty passing stool or infrequent bowel movements."},
        {"Loss of appetite", "A decrease in the desire to eat, often due to illness or stress."},
        {"Skin rash", "A noticeable change in the skin's appearance, typically caused by an allergy or infection."},
        {"Muscle cramps", "Involuntary muscle contractions causing pain, usually in legs or arms."},
        {"Sore muscles", "Pain and discomfort in muscles, often due to overuse or injury."},
        {"Runny nose", "Excess mucus production in the nasal passages, commonly associated with colds."},
        {"Numbness", "A loss of sensation or feeling in a part of the body."},
        {"Fever", "A temporary increase in body temperature, often due to an infection."},
        {"Cough", "A reflex action to clear the throat and airways."},
        {"Headache", "Pain in the head or upper neck region."},
        {"Sore throat", "Pain or irritation in the throat, especially when swallowing."},
        {"Fatigue", "Extreme tiredness or lack of energy."},
        {"Body ache", "Muscular pain or discomfort throughout the body."},
        {"Nausea", "The sensation of wanting to vomit."},
        {"Vomiting", "The act of forcefully expelling stomach contents through the mouth."},
        {"Shortness of breath", "A feeling of not being able to breathe deeply or quickly enough."},
        {"Chills", "Shivering or feeling cold often accompanied by fever."},
        {"Dizziness", "A sensation of lightheadedness or vertigo, where the person feels they might faint."},
        {"Diarrhea", "Frequent, watery bowel movements that can be a sign of infection or digestive issues."},
        {"Constipation", "Difficulty passing stool or infrequent bowel movements."},
        {"Loss of appetite", "A decrease in the desire to eat, often due to illness or stress."},
        {"Skin rash", "A noticeable change in the skin's appearance, typically caused by an allergy or infection."},
        {"Muscle cramps", "Involuntary muscle contractions causing pain, usually in legs or arms."},
        {"Sore muscles", "Pain and discomfort in muscles, often due to overuse or injury."},
        {"Runny nose", "Excess mucus production in the nasal passages, commonly associated with colds."},
        {"Numbness", "A loss of sensation or feeling in a part of the body."}, {"Fever", "A temporary increase in body temperature, often due to an infection."},


    {"Cough", "A reflex action to clear the throat and airways."},
    {"Headache", "Pain in the head or upper neck region."},
    {"Sore throat", "Pain or irritation in the throat, especially when swallowing."},
    {"Fatigue", "Extreme tiredness or lack of energy."},
    {"Body ache", "Muscular pain or discomfort throughout the body."},
    {"Nausea", "The sensation of wanting to vomit."},
    {"Vomiting", "The act of forcefully expelling stomach contents through the mouth."},
    {"Shortness of breath", "A feeling of not being able to breathe deeply or quickly enough."},
    {"Chills", "Shivering or feeling cold often accompanied by fever."},
    {"Dizziness", "A sensation of lightheadedness or vertigo, where the person feels they might faint."},
    {"Diarrhea", "Frequent, watery bowel movements that can be a sign of infection or digestive issues."},
    {"Constipation", "Difficulty passing stool or infrequent bowel movements."},
    {"Loss of appetite", "A decrease in the desire to eat, often due to illness or stress."},
    {"Skin rash", "A noticeable change in the skin's appearance, typically caused by an allergy or infection."},
    {"Muscle cramps", "Involuntary muscle contractions causing pain, usually in legs or arms."},
    {"Sore muscles", "Pain and discomfort in muscles, often due to overuse or injury."},
    {"Runny nose", "Excess mucus production in the nasal passages, commonly associated with colds."},
    {"Numbness", "A loss of sensation or feeling in a part of the body."},
    {"Abdominal pain", "Pain in the stomach or abdominal region, which can be caused by various factors."},
    {"Back pain", "Pain in the back, ranging from mild discomfort to severe pain."},
    {"Chest pain", "Pain or discomfort in the chest area, potentially related to heart conditions."},
    {"Earache", "Pain in the ear, commonly caused by infections or pressure changes."},
    {"Fatigue and weakness", "Excessive tiredness and lack of strength, often indicating underlying health issues."},
    {"Frequent urination", "The need to urinate more often than usual, potentially due to infection or other issues."},
    {"Heartburn", "A burning sensation in the chest, typically caused by acid reflux."},
    {"Heavy breathing", "Difficulty breathing or labored breathing, often associated with respiratory conditions."},
    {"Insomnia", "Difficulty falling or staying asleep, affecting overall well-being."},
    {"Jaundice", "A yellowing of the skin or eyes, often indicating liver issues."},
    {"Knee pain", "Discomfort or pain in the knee, which may arise from overuse or injury."},
    {"Lightheadedness", "A feeling of faintness or dizziness, often a sign of low blood pressure or dehydration."},
    {"Mouth ulcers", "Painful sores in the mouth, often caused by viral infections or injury."},
    {"Muscle weakness", "A reduction in the strength of the muscles, often linked to various health conditions."},
    {"Neck pain", "Pain in the neck, commonly due to poor posture or muscle strain."},
    {"Night sweats", "Excessive sweating during sleep, potentially caused by infections or other medical conditions."},
    {"Panic attacks", "Sudden feelings of intense fear or anxiety, often accompanied by physical symptoms."},
    {"Palpitations", "Irregular or unusually strong heartbeats, which can be caused by stress or heart conditions."},
    {"Pneumonia", "An infection of the lungs that causes cough, fever, and difficulty breathing."},
    {"Post-nasal drip", "The sensation of mucus dripping down the back of the throat, often caused by sinus issues."},
    {"Rash", "A visible change in skin color or texture, often due to an allergic reaction or infection."},
    {"Ringing in the ears", "A sensation of hearing ringing or buzzing, typically related to ear issues."},
    {"Severe pain", "Intense pain that may arise from injury, inflammation, or illness."},
    {"Shortness of breath while lying down", "Difficulty breathing when lying flat, often related to heart or lung issues."},
    {"Sinus pressure", "Pain or fullness in the sinus cavities, often caused by sinus infections or allergies."},
    {"Sore eyes", "Pain or irritation in the eyes, which may be caused by dryness, allergies, or infection."},
    {"Swelling", "An abnormal enlargement of a part of the body, often due to fluid retention or inflammation."},
    {"Throat swelling", "Swelling of the throat, which can affect swallowing and may be caused by infections."},
    {"Toothache", "Pain in or around a tooth, often due to dental issues such as cavities or infections."},
    {"Vision problems", "Changes in vision, including blurred or double vision, often signaling eye or neurological issues."},
    {"Vomiting blood", "The act of throwing up blood, which can be a serious symptom of digestive tract bleeding."},
    {"Wheezing", "A high-pitched sound when breathing, often indicating respiratory issues such as asthma."},
    {"Yellowing of the skin", "A symptom often related to liver or gallbladder issues, causing jaundice."},
    {"Hoarseness", "A raspy or strained voice, often caused by irritation or inflammation in the vocal cords."},
    {"Anxiety", "A feeling of unease, worry, or nervousness, sometimes accompanied by physical symptoms."},
    {"Cold sores", "Painful blisters around the mouth or lips, typically caused by a viral infection."},
    {"Constipation with bloating", "Difficulty passing stool combined with a feeling of fullness or tightness in the abdomen."},
    {"Excessive thirst", "A strong, persistent desire to drink fluids, often due to dehydration or diabetes."},
    {"Fainting", "A temporary loss of consciousness, typically caused by a drop in blood pressure."},
    {"Hair loss", "The shedding or thinning of hair, which may result from various medical conditions or stress."},
    {"Heart palpitations with dizziness", "A sensation of rapid heartbeats accompanied by lightheadedness or fainting."},
    {"High blood pressure", "Elevated blood pressure levels that may indicate an increased risk for heart disease."},
    {"Hot flashes", "A sudden feeling of intense heat, often associated with menopause or certain medications."},
    {"Hunger pangs", "A sensation of discomfort caused by a need for food, typically associated with low blood sugar."},
    {"Joint swelling", "Inflammation or fluid accumulation in the joints, often caused by arthritis or injury."},
    {"Leg cramps", "Involuntary muscle contractions, commonly occurring in the calves, often at night."},
    {"Mucus in the stool", "The presence of mucus in bowel movements, which can indicate gastrointestinal issues."},
    {"Painful urination", "A burning or stinging sensation during urination, often caused by urinary tract infections."},
    {"Persistent cough", "A cough that lasts for several weeks, often due to chronic bronchitis or other conditions."},
    {"Rashes with blisters", "Inflammation of the skin accompanied by blister formation, which may result from infections or allergies."},
    {"Severe dehydration", "A condition in which the body loses too much water, leading to dizziness, fatigue, and dry skin."},
    {"Sore gums", "Pain or swelling in the gums, which may be caused by gingivitis or other oral health issues."},
    {"Tingling in the hands", "A sensation of numbness or pins-and-needles feeling in the hands, often due to nerve issues."},
    {"Tired eyes", "Discomfort or fatigue in the eyes, commonly caused by prolonged screen time or eye strain."},
    {"Urinary retention", "The inability to fully empty the bladder, often due to obstructions or nerve issues."},
    {"Vision loss", "A partial or total loss of sight, which can be caused by eye injuries, diseases, or neurological conditions."},
    {"Wrist pain", "Pain in the wrist joint, often resulting from strain, arthritis, or injury."},
    {"Weight loss with no reason", "Unexplained weight loss, which can be a symptom of various medical conditions such as thyroid disorders."},
    {"Dry mouth", "A condition where the mouth feels dry, often due to medication side effects or dehydration."},
    {"Abnormal bruising", "Unexplained bruises appearing on the body, which may be caused by blood disorders or injury."},
    {"Bloating after meals", "A feeling of fullness and swelling in the abdomen after eating, often caused by digestive issues."},
    {"Breathlessness during exercise", "A feeling of difficulty breathing when performing physical activities, often caused by asthma or cardiovascular issues."},
    {"Chronic fatigue", "Persistent tiredness not relieved by rest, often associated with conditions like chronic fatigue syndrome."},
    {"Cramps in the stomach", "Painful muscle contractions in the stomach, often caused by indigestion or menstrual cramps."},
    {"Dark urine", "Urine that appears darker than usual, which may indicate liver or kidney issues."},
    {"Dry skin", "Skin that appears rough, flaky, or itchy, often due to dehydration or skin conditions."},
    {"Ear ringing", "A ringing or buzzing sound in the ears, typically caused by damage to the ear or hearing loss."},
    {"Excessive sweating", "Abnormal perspiration, often due to hyperhidrosis or an underlying health condition."},
    {"Extreme thirst", "A persistent desire to drink fluids, often seen in diabetes or dehydration."},
    {"Hearing loss", "Partial or complete inability to hear, which may occur gradually or suddenly."},
    {"Hip pain", "Pain in the hip joint, which can be caused by arthritis, injury, or overuse."},
    {"Indigestion", "A discomfort or pain in the upper abdomen after eating, often related to food intolerance or gastrointestinal issues."},
    {"Irregular heartbeat", "A heart rhythm that is abnormal, potentially caused by arrhythmia or heart disease."},
    {"Irritable bowel syndrome", "A digestive disorder characterized by abdominal cramps, diarrhea, and constipation."},
    {"Itchy scalp", "A sensation of itching on the scalp, often caused by dandruff, dryness, or fungal infections."},



    {"Jaundice", "A yellowing of the skin or eyes, often caused by liver disease or bile duct obstruction."},
    {"Knee pain", "Pain or discomfort in the knee joint, potentially caused by arthritis, injury, or overuse."},
    {"Lung pain", "Pain in the chest area, typically associated with respiratory infections or conditions like pneumonia."},
    {"Malaise", "A general feeling of discomfort or illness, often preceding other symptoms of disease."},
    {"Memory loss", "A decrease in the ability to remember things, which can result from aging, stress, or medical conditions."},
    {"Mild fever", "A slight increase in body temperature, often indicating a mild infection or inflammation."},
    {"Mouth ulcers", "Painful sores inside the mouth, often caused by viral infections or stress."},
    {"Neck stiffness", "Pain or tightness in the neck, often caused by poor posture, tension, or injury."},
    {"Nosebleeds", "Bleeding from the nose, often caused by dryness, injury, or underlying medical conditions."},
    {"Numbness in the limbs", "Loss of sensation or tingling in the arms or legs, often due to nerve compression or circulatory issues."},
    {"Numbness in the face", "A loss of sensation in the facial area, often due to nerve damage or other neurological conditions."},
    {"Oily skin", "Excessive secretion of oil from the sebaceous glands, which may cause acne or skin irritation."},
    {"Pale skin", "A lighter skin tone, often indicating anemia, low blood pressure, or shock."},
    {"Paralysis", "The loss of muscle function in part of the body, often due to nerve damage or injury."},
    {"Peeling skin", "The shedding of dead skin cells, often caused by sunburn or certain skin conditions."},
    {"Persistent hunger", "An ongoing feeling of hunger despite eating, which could be related to metabolic disorders or diabetes."},
    {"Poor circulation", "Reduced blood flow to certain areas of the body, which can cause numbness or cold extremities."},
    {"Postural dizziness", "A sensation of dizziness when changing positions, such as standing up from a sitting position."},
    {"Puffy eyes", "Swelling or puffiness around the eyes, often caused by lack of sleep, allergies, or fluid retention."},
    {"Rashes with swelling", "Skin inflammation that includes redness and swelling, which may result from allergic reactions or infections."},
    {"Restlessness", "A feeling of unease or inability to relax, often associated with anxiety or stress."},
    {"Rib pain", "Pain in the rib area, which can result from injury, inflammation, or respiratory issues."},
    {"Sallow complexion", "A pale and unhealthy appearance of the skin, often linked to anemia or liver disease."},
    {"Scaly skin", "Rough, dry, and flaky skin, often due to eczema, psoriasis, or dehydration."},
    {"Severe abdominal cramps", "Intense and painful stomach cramps, often related to digestive disorders or menstruation."},
    {"Severe headache", "A painful and often debilitating headache that can be caused by various factors, including tension or migraines."},
    {"Shaking hands", "Involuntary hand tremors, which may be associated with neurological disorders such as Parkinson's disease."},
    {"Short-term memory loss", "Difficulty remembering recent events or information, often related to stress, sleep deprivation, or aging."},
    {"Sore breasts", "Tenderness or discomfort in the breast area, which may be linked to hormonal changes or infection."},
    {"Sore feet", "Pain or discomfort in the feet, often due to standing for long periods, injury, or conditions like plantar fasciitis."},
    {"Sore gums", "Inflammation and tenderness in the gums, which may be caused by gingivitis or other oral health problems."},
    {"Sore lips", "Pain or cracking on the lips, often caused by dryness, chapped lips, or a viral infection."},
    {"Sore shoulders", "Pain or stiffness in the shoulder area, commonly due to muscle strain or injury."},
    {"Sore throat with fever", "A painful throat combined with an elevated body temperature, typically due to viral or bacterial infections."},
    {"Sputum production", "The production of mucus from the respiratory tract, often seen in respiratory infections like bronchitis."},
    {"Stomach bloating", "A sensation of fullness or tightness in the stomach, often due to digestive issues or excessive gas."},
    {"Stomach cramps", "Painful muscle contractions in the stomach, which may result from indigestion, menstruation, or infection."},
    {"Stomach gas", "Excessive accumulation of gas in the stomach, often leading to discomfort or belching."},
    {"Stress", "A mental and emotional state caused by external pressures, which can affect physical health."},
    {"Swollen ankles", "Enlarged or puffed-up ankles, often caused by fluid retention or circulatory problems."},
    {"Swollen glands", "Enlarged lymph nodes, often a response to infection or illness."},
    {"Tightness in the chest", "A feeling of pressure or constriction in the chest, which may signal heart or lung issues."},
    {"Tingling sensation", "A prickling or 'pins and needles' feeling, often caused by nerve compression or poor circulation."},
    {"Tiredness", "An overall feeling of fatigue or lack of energy, often resulting from lack of sleep or illness."},
    {"Unexplained weight loss", "The loss of body weight without a clear cause, which may indicate underlying medical conditions."},
    {"Upper back pain", "Pain in the upper back area, which can be caused by muscle strain or spinal issues."},
    {"Urinary incontinence", "The involuntary loss of urine, often due to age, pregnancy, or medical conditions."},
    {"Urinary tract infection", "An infection in any part of the urinary system, commonly causing painful urination and frequent urges."},
    {"Vomiting with blood", "Expelling blood along with stomach contents, often due to gastrointestinal bleeding or ulcers."},
    {"Weakness in the arms", "Reduced strength or control in the arms, often resulting from nerve or muscle disorders."},
    {"Weakness in the legs", "Reduced strength or difficulty in using the legs, potentially caused by neurological or muscular conditions."},
    {"Weight gain", "The accumulation of excess body weight, which may result from overeating, inactivity, or hormonal changes."},
    {"Wheezing with a cough", "A whistling sound while breathing combined with a persistent cough, often due to asthma or bronchitis."},
    {"Yellow stool", "Stool that appears yellow in color, often related to digestive issues, such as bile duct problems."},
    {"Yellow teeth", "A yellowish appearance of teeth, commonly due to staining, poor hygiene, or aging."},
    {"Zits", "Small, inflamed pimples that typically appear on the face due to clogged pores."},
    {"Abdominal bloating", "A feeling of fullness or distention in the abdomen, often caused by digestive issues or overeating."},
    {"Abnormal heartbeat", "An irregular or erratic heartbeat, which can be caused by arrhythmia or heart disease."},
    {"Acid reflux", "A condition where stomach acid flows back into the esophagus, causing heartburn and discomfort."},
    {"Ankle sprain", "A stretching or tearing of ligaments in the ankle, often caused by twisting or turning the foot."},
    {"Apathy", "A lack of interest or emotion, often seen in conditions like depression or neurological disorders."},
    {"Arthritis", "Inflammation of the joints that can lead to pain, swelling, and limited movement."},
    {"Asthma", "A chronic respiratory condition marked by difficulty breathing, wheezing, and shortness of breath."},
    {"Blurry vision", "A lack of sharpness in vision, often caused by refractive errors, cataracts, or other eye conditions."},
    {"Bronchitis", "Inflammation of the bronchial tubes, often causing coughing, mucus production, and difficulty breathing."},
    {"Carpal tunnel syndrome", "A condition where pressure on the median nerve in the wrist causes numbness, tingling, and weakness."},
    {"Chest tightness", "A sensation of pressure or constriction in the chest, often related to heart or lung conditions."},
    {"Chronic cough", "A persistent cough lasting for more than three weeks, which can indicate underlying health conditions."},
    {"Chronic pain", "Long-lasting pain that persists for weeks, months, or even years, often related to injury or disease."},
    {"Chronic stress", "Long-term stress that can lead to a variety of physical and emotional health problems."},
    {"Clammy skin", "Skin that feels moist and cool, often associated with fever, shock, or anxiety."},
    {"Cold hands", "A feeling of coldness in the hands, often due to poor circulation or cold temperatures."},
    {"Cold intolerance", "A sensitivity to cold temperatures, often seen in conditions like hypothyroidism."},
    {"Cramps during menstruation", "Painful contractions of the uterus during menstruation, commonly known as period cramps."},
    {"Cracked heels", "Dry, cracked skin on the heels, often caused by standing for long periods or lack of moisture."},
    {"Cystic acne", "A severe form of acne that results in large, painful cysts under the skin."},
    {"Dandruff", "Flaking of dead skin from the scalp, which may be caused by dry skin or seborrheic dermatitis."},
    {"Depression", "A mental health condition characterized by persistent feelings of sadness, hopelessness, and loss of interest."},
    {"Difficulty swallowing", "The sensation of food getting stuck or difficulty moving food down the throat, often due to esophageal issues."},
    {"Discolored skin", "Skin that appears darker or lighter than normal, often due to sun exposure, injury, or medical conditions."},
    {"Ear infection", "An infection of the ear canal or middle ear, often causing pain, swelling, and hearing issues."},
    {"Eczema", "A chronic skin condition characterized by itchy, red, and inflamed patches of skin."},
    {"Elevated heart rate", "A faster-than-normal heart rate, which can occur due to stress, exercise, or heart condotion"}

        };

        for (const auto& symptom : symptoms) {
            if (symptom.second.find(description) != string::npos) { // Partial match
                matches.push_back(symptom.first + " - " + symptom.second);
                foundMatch = true;
            }
        }

        if (foundMatch) {
            cout << "Matching symptoms found:\n";
            for (const auto& match : matches) {
                cout << match << endl;
            }
            cout << "Would you like to book an appointment based on any of these symptoms? (yes/no): ";
            string response;
            cin >> response;
            if (response == "yes") {
                cout << "Enter symptom name to book an appointment: ";
                string symptom;
                cin >> symptom;
                suggestDoctors(symptom); // Suggest doctors for the symptom
            }
        } else {
            cout << "No matching symptoms found.\n";
        }
    } else {
        cout << "Invalid choice.\n";
    }
}

#include<conio.h>
// Function for patient login
bool patientLogin(const vector<Patient>& patients) {
    string userID, password;
    int attempts = 1;
     // Allow 3 login attempts

    while (attempts > 0)
    {
        cout << "Enter UserID: ";
        cin >> userID;

        // Check if userID is empty or too short
        if (userID.empty())
        {
            cout << "UserID cannot be empty. Please try again.\n";
            continue;
        }
        char ch;

        cout << "Enter Password: ";
        //cin >> password;

        while(true)
        {
            ch=getch();
            if(ch=='\r')
            break;
            if(ch=='\b')
            {
                if(!password.empty())
                {
                    cout<<"\b \b";
                    password.pop_back();
                }
            }
            else
            {
                password+=ch;
                cout<<'*';
            }
        }

        // Check if password is empty
        if (password.empty())
        {
            cout << "Password cannot be empty. Please try again.\n";
            continue;
        }

        bool found = false;
        for (const auto& patient : patients)
        {
            // Compare each patient’s credentials with input
            if (patient.userID == userID)
            {
                // Matching UserID found, now check password
                if (patient.password == password)
                {
                    found = true;
                    cout << "\nLogin successful!\n";
                    cout << "Welcome, " << patient.name << " from " << patient.village << ".\n";

                    // Additional greeting based on time of day (just for interaction)
                    time_t now = time(0);
                    tm *ltm = localtime(&now);
                    int hour = ltm->tm_hour;
                    if (hour < 12)
                    {
                        cout << "Good morning, " << patient.name << "!\n";
                    } else if (hour < 18)
                    {
                        cout << "Good afternoon, " << patient.name << "!\n";
                    } else
                    {

                        cout << "Good evening, " << patient.name << "!\n";
                    }

                    // Prompt for additional patient actions
                    cout << "Would you like to search for symptoms? (Y/N): ";

                    char choice;
                    cin >> choice;
                    if (choice == 'Y' || choice == 'y')
                    {

                        // Call symptom search after successful login

                        searchSymptoms();
                    }
                     else
                    {
                        cout << "Thank you for visiting. Have a great day!\n";
                    }
                      // Exit loop if login is successful
                    exit(0);
                }
                 else
                {
                    cout << "Incorrect password. Please try again.\n";
                     // Exit loop and prompt for retry
                    break;
                }
            }
        }

        if (!found)
        {
            cout << "UserID not found.\n";
        }

        // Decrease the number of attempts
        attempts--;
        if (attempts > 0)
        {
            cout << "You have " << attempts << " attempt(s) remaining.\n";
        }
    }

    if (attempts == 0)
    {
        cout << "THANK YOU.\n";
    }

    // Return false if login fails after 3 attempts
    return false;
}


// Main module 1


int module1() {
    const string patientFile = "patient_data.csv", doctorFile = "doctors.csv", symptomFile = "symptoms.csv";

    vector<Patient> patients = loadPatients(patientFile);

    unordered_map<string, Doctor> doctors = loadDoctors(doctorFile);

    loadSymptoms(symptomFile);

    int choice;
    cout << "1. Patient Login\n2. Doctor Login\n3. Register New Patient\n4. Register New Doctor\nChoose an option: ";
    cin >> choice;

   if (choice == 1)
    {
        bool loggedIn = false;
        loggedIn = patientLogin(patients);  // Call the patient login function
        if (!loggedIn)
            {
            cout << "Failed to login as patient. Please try again.\n";
        }
       // if (!found) cout << "Invalid credentials.\n";
    }
     else if (choice == 2)
    {

        doctorLogin(doctors);

    }
    else if (choice == 3)
    {

        // Register new patient functionality
        registerNewPatient(patients);

    }
        else if (choice == 4)
    {
        // Register new doctor functionality
        registerNewDoctor(doctors);
    }
    else
    {
        cout << "Invalid choice. Please try again.\n";
    }
    return 0;
}

//module 2
#include <iostream>
#include <string>
using namespace std;

void askQuestions(string disease, string questions[], int numQuestions) {
    cout << "\nYou selected: " << disease << endl;
    cout << "Please answer the following questions with 'yes' or 'no':\n";

    string response;
    int yesCount = 0;
    for (int i = 0; i < numQuestions; ++i) {
        cout << questions[i] << " ";
        cin >> response;
        if (response == "yes" || response == "Yes") {
            yesCount++;
        }
    }

    cout << "\nYou answered 'yes' to " << yesCount << " out of " << numQuestions << " questions for " << disease << ".\n";
    if (yesCount > numQuestions / 2) {
        cout << "It's recommended to consult a doctor for further diagnosis of " << disease << ".\n";
    } else {
        cout << "The symptoms may not strongly indicate " << disease << ", but consult a doctor if concerned.\n";
    }
}

int module2() {
    while (true) {
            cout << "1. Heart Disease\n2. Cancer\n3. COVID-19\n4. food posioning \n5. Chicken gunia\n";

            cout << "6. Dengue\n7. Diabetes\n8. Chronic Liver Disease\n9. Infectious Diseases\n";

            cout << "10. Food Poisoning\n11. Jaundice\n12. Dengue\n13. Typhoid\n14. Acidity\n";

            cout << "15. Pneumonia\n16. Fungal Infection\n17. Anxiety\n18. Depression\n19. Malaria\n";

            cout << "20. HIV/AIDS\n21. Iron Deficiency\n22. Sciatica\n23. Eczema\n24. Migraine\n";

            cout << "25. Gastrointestinal Disorder\n26. Gastroesophageal Reflux Disease (GERD)\n27. Chronic Obstructive Pulmonary Disease (COPD)\n";

            cout << "28. Diabetes\n29. Chronic Pulmonary Disease\n30. Stroke\n31. Chronic Liver Disease\n32. Pneumonia\n";

            cout << "33. Allergies\n34. Skin Infections\n35. Asthma\n36. Hypertension\n37. Kidney Disease\n38. Hepatitis\n";

            cout << "39. Cystic Fibrosis\n40. Tuberculosis\n41. Multiple Sclerosis\n42. Arthritis\n43. Parkinson's Disease\n44. Obesity\n";

            cout << "45. Peptic Ulcer\n46. Gallbladder Disease\n47. Chronic Liver Disease\n48. Anemia\n49. Epilepsy\n50. Sleep Apnea\n";

            cout << "51. Insomnia\n52. Acne\n53. Endometriosis\n54. UTI (Urinary Tract Infection)\n55. Osteoporosis\n56. Prostate Cancer\n";

            cout << "57. Testicular Cancer\n58. Breast Cancer\n59. Colon Cancer\n60. Lung Cancer\n61. Leukemia\n62. Lymphoma\n";

            cout << "63. Migraine\n64. Gastrointestinal Disorder\n65. GERD\n66. COPD\n67. Sciatica\n68. Eczema\n69. Skin Disorders\n";

            cout << "70. Mental Health Disorders\n71.Exit\n";


        int choice;
        cin >> choice;

        if (choice == 10) {
            cout << "Exiting the program. Stay healthy!\n";
            break;
        }

        const int NUM_QUESTIONS = 10;
        string questions[NUM_QUESTIONS];

        switch (choice) {
            // Case 1: Heart Disease
            case 1:
                questions[0] = "Do you experience chest pain?";
                questions[1] = "Do you feel shortness of breath?";
                questions[2] = "Do you experience fatigue regularly?";
                questions[3] = "Do you have swelling in your legs or feet?";
                questions[4] = "Do you feel light-headed or dizzy?";
                questions[5] = "Do you have an irregular heartbeat?";
                questions[6] = "Do you experience nausea or sweating?";
                questions[7] = "Do you feel pain in your neck or jaw?";
                questions[8] = "Do you have high blood pressure?";
                questions[9] = "Do you have a family history of heart disease?";
                askQuestions("Heart Disease", questions, NUM_QUESTIONS);
                break;

            // Case 2: Cancer
            case 2:
                questions[0] = "Do you have unexplained weight loss?";
                questions[1] = "Do you feel extreme fatigue?";
                questions[2] = "Do you notice persistent pain anywhere?";
                questions[3] = "Do you have a lump or thickened area in your body?";
                questions[4] = "Do you experience skin changes (e.g., darkening, yellowing)?";
                questions[5] = "Do you have difficulty swallowing?";
                questions[6] = "Do you notice changes in your bowel or bladder habits?";
                questions[7] = "Do you have a persistent cough or trouble breathing?";
                questions[8] = "Do you experience unusual bleeding or discharge?";
                questions[9] = "Do you have a family history of cancer?";
                askQuestions("Cancer", questions, NUM_QUESTIONS);
                break;

            // Case 3: COVID-19
            case 3:
                questions[0] = "Do you have a fever?";
                questions[1] = "Do you have a dry cough?";
                questions[2] = "Do you experience fatigue?";
                questions[3] = "Do you have loss of taste or smell?";
                questions[4] = "Do you experience difficulty breathing?";
                questions[5] = "Do you have body aches?";
                questions[6] = "Do you experience headaches?";
                questions[7] = "Do you have a sore throat?";
                questions[8] = "Do you have diarrhea?";
                questions[9] = "Have you been in close contact with someone with COVID-19?";
                askQuestions("COVID-19", questions, NUM_QUESTIONS);
                break;

            // Case 4: Food Poisoning
            case 4:
                questions[0] = "Do you experience nausea or vomiting?";
                questions[1] = "Do you have diarrhea?";
                questions[2] = "Do you feel fatigued or weak?";
                questions[3] = "Do you have abdominal cramps?";
                questions[4] = "Do you experience dehydration?";
                questions[5] = "Do you have a fever or chills?";
                questions[6] = "Do you notice blood in your stool?";
                questions[7] = "Did you eat undercooked or contaminated food recently?";
                questions[8] = "Do you have headaches?";
                questions[9] = "Do you feel bloated or uncomfortable?";
                askQuestions("Food Poisoning", questions, NUM_QUESTIONS);
                break;

            // Case 5: Chicken Guniya
            case 5:
                questions[0] = "Do you have a fever?";
                questions[1] = "Do you experience joint pain or swelling?";
                questions[2] = "Do you have headaches?";
                questions[3] = "Do you experience rashes?";
                questions[4] = "Do you feel fatigue or weakness?";
                questions[5] = "Do you have muscle pain?";
                questions[6] = "Do you experience nausea or vomiting?";
                questions[7] = "Do you have chills?";
                questions[8] = "Do you have a history of mosquito bites?";
                questions[9] = "Have you recently traveled to areas with high prevalence of chikungunya?";
                askQuestions("Chicken Guniya", questions, NUM_QUESTIONS);
                break;

            // Case 6: Dengue
            case 6:
                questions[0] = "Do you have a sudden high fever?";
                questions[1] = "Do you experience severe headache?";
                questions[2] = "Do you feel pain behind your eyes?";
                questions[3] = "Do you experience severe joint or muscle pain?";
                questions[4] = "Do you have skin rash or bruising?";
                questions[5] = "Do you feel fatigue or weakness?";
                questions[6] = "Do you have nausea or vomiting?";
                questions[7] = "Do you have any symptoms of bleeding (e.g., nosebleeds, gum bleeding)?";
                questions[8] = "Have you recently been bitten by mosquitoes in tropical or subtropical areas?";
                questions[9] = "Do you have a history of dengue fever?";
                askQuestions("Dengue", questions, NUM_QUESTIONS);
                break;
            // Case 7: Diabetes
            case 7:
                questions[0] = "Do you experience frequent urination?";
                questions[1] = "Do you feel extremely thirsty?";
                questions[2] = "Do you experience unexplained weight loss?";
                questions[3] = "Do you feel extreme hunger?";
                questions[4] = "Do you experience blurred vision?";
                questions[5] = "Do you have cuts or bruises that are slow to heal?";
                questions[6] = "Do you have a tingling sensation or numbness in your hands or feet?";
                questions[7] = "Do you have a family history of diabetes?";
                questions[8] = "Do you experience fatigue or weakness?";
                questions[9] = "Do you have high blood sugar levels?";
                askQuestions("Diabetes", questions, NUM_QUESTIONS);
                break;

            // Case 8: Chronic Liver Disease
            case 8:
                questions[0] = "Do you experience jaundice (yellowing of skin or eyes)?";
                questions[1] = "Do you have pain or discomfort in your upper right abdomen?";
                questions[2] = "Do you experience fatigue or weakness?";
                questions[3] = "Do you have swelling in your legs or abdomen?";
                questions[4] = "Do you experience nausea or vomiting?";
                questions[5] = "Do you have a history of alcohol consumption?";
                questions[6] = "Do you have a history of hepatitis or liver disease?";
                questions[7] = "Do you have dark-colored urine?";
                questions[8] = "Do you experience loss of appetite?";
                questions[9] = "Do you have a family history of liver disease?";
                askQuestions("Chronic Liver Disease", questions, NUM_QUESTIONS);
                break;

            // Case 9: Infectious Diseases
            case 9:
                questions[0] = "Do you experience fever?";
                questions[1] = "Do you have chills?";
                questions[2] = "Do you have a sore throat?";
                questions[3] = "Do you have body aches or muscle pain?";
                questions[4] = "Do you experience fatigue?";
                questions[5] = "Do you have a cough or shortness of breath?";
                questions[6] = "Do you have unusual rashes or skin lesions?";
                questions[7] = "Do you experience diarrhea or vomiting?";
                questions[8] = "Have you recently been in contact with someone diagnosed with an infectious disease?";
                questions[9] = "Have you traveled to an area with high risk of infections?";
                askQuestions("Infectious Diseases", questions, NUM_QUESTIONS);
                break;

            // Case 10: Food Poisoning
            case 10:
                questions[0] = "Do you experience nausea or vomiting?";
                questions[1] = "Do you have diarrhea?";
                questions[2] = "Do you feel fatigued or weak?";
                questions[3] = "Do you have abdominal cramps?";
                questions[4] = "Do you experience dehydration?";
                questions[5] = "Do you have a fever or chills?";
                questions[6] = "Do you notice blood in your stool?";
                questions[7] = "Did you eat undercooked or contaminated food recently?";
                questions[8] = "Do you have headaches?";
                questions[9] = "Do you feel bloated or uncomfortable?";
                askQuestions("Food Poisoning", questions, NUM_QUESTIONS);
                break;

            // Case 11: Jaundice
            case 11:
                questions[0] = "Do you experience yellowing of the skin or eyes?";
                questions[1] = "Do you have dark-colored urine?";
                questions[2] = "Do you have pale-colored stool?";
                questions[3] = "Do you experience fatigue or weakness?";
                questions[4] = "Do you have pain in your abdomen, especially the upper right side?";
                questions[5] = "Do you have a history of liver disease or hepatitis?";
                questions[6] = "Do you have loss of appetite?";
                questions[7] = "Do you have nausea or vomiting?";
                questions[8] = "Do you have a history of alcohol consumption?";
                questions[9] = "Do you have a family history of jaundice or liver disease?";
                askQuestions("Jaundice", questions, NUM_QUESTIONS);
                break;

            // Case 12: Dengue
            case 12:
                questions[0] = "Do you have a sudden high fever?";
                questions[1] = "Do you experience severe headache?";
                questions[2] = "Do you feel pain behind your eyes?";
                questions[3] = "Do you experience severe joint or muscle pain?";
                questions[4] = "Do you have skin rash or bruising?";
                questions[5] = "Do you feel fatigue or weakness?";
                questions[6] = "Do you have nausea or vomiting?";
                questions[7] = "Do you have any symptoms of bleeding (e.g., nosebleeds, gum bleeding)?";
                questions[8] = "Have you recently been bitten by mosquitoes in tropical or subtropical areas?";
                questions[9] = "Do you have a history of dengue fever?";
                askQuestions("Dengue", questions, NUM_QUESTIONS);
                break;

            // Case 13: Typhoid
            case 13:
                questions[0] = "Do you experience a prolonged fever?";
                questions[1] = "Do you have stomach pain or discomfort?";
                questions[2] = "Do you feel weakness or fatigue?";
                questions[3] = "Do you have diarrhea or constipation?";
                questions[4] = "Do you have a rash or rose-colored spots on the skin?";
                questions[5] = "Do you have nausea or vomiting?";
                questions[6] = "Have you recently traveled to areas with a risk of typhoid fever?";
                questions[7] = "Do you experience loss of appetite?";
                questions[8] = "Do you have abdominal bloating?";
                questions[9] = "Have you had close contact with someone diagnosed with typhoid fever?";
                askQuestions("Typhoid", questions, NUM_QUESTIONS);
                break;
            // Case 14: Acidity
            case 14:
                questions[0] = "Do you experience heartburn or a burning sensation in the chest?";
                questions[1] = "Do you have acid reflux or regurgitation of food or liquid?";
                questions[2] = "Do you feel bloated or full after eating?";
                questions[3] = "Do you experience nausea or vomiting?";
                questions[4] = "Do you have difficulty swallowing?";
                questions[5] = "Do you often burp or have a sour taste in your mouth?";
                questions[6] = "Do you experience pain or discomfort in the upper abdomen?";
                questions[7] = "Do you have a history of eating spicy or fatty foods?";
                questions[8] = "Do you feel more discomfort when lying down or after eating?";
                questions[9] = "Do you have a family history of acidity or acid reflux?";
                askQuestions("Acidity", questions, NUM_QUESTIONS);
                break;

            // Case 15: Pneumonia
            case 15:
                questions[0] = "Do you have a persistent cough?";
                questions[1] = "Do you experience shortness of breath?";
                questions[2] = "Do you have chest pain or tightness?";
                questions[3] = "Do you experience fatigue or weakness?";
                questions[4] = "Do you have a fever or chills?";
                questions[5] = "Do you experience wheezing or crackling sounds while breathing?";
                questions[6] = "Do you have a history of respiratory infections?";
                questions[7] = "Do you have a cough with phlegm or mucus?";
                questions[8] = "Do you feel confusion or delirium (in severe cases)?";
                questions[9] = "Do you have a history of smoking or lung diseases?";
                askQuestions("Pneumonia", questions, NUM_QUESTIONS);
                break;

            // Case 16: Fungal Infection
            case 16:
                questions[0] = "Do you experience itching or redness on your skin?";
                questions[1] = "Do you notice peeling or cracking skin?";
                questions[2] = "Do you have rashes that spread or worsen over time?";
                questions[3] = "Do you have a persistent or foul odor from the affected area?";
                questions[4] = "Do you experience pain or swelling in the affected area?";
                questions[5] = "Do you have a weakened immune system or take immunosuppressive medications?";
                questions[6] = "Do you frequently wear tight or non-breathable clothing?";
                questions[7] = "Do you have athlete's foot or ringworm?";
                questions[8] = "Have you been in contact with someone who has a fungal infection?";
                questions[9] = "Do you have a history of recurrent fungal infections?";
                askQuestions("Fungal Infection", questions, NUM_QUESTIONS);
                break;

            // Case 17: Anxiety
            case 17:
                questions[0] = "Do you often feel nervous or on edge?";
                questions[1] = "Do you experience rapid heartbeat or palpitations?";
                questions[2] = "Do you have trouble concentrating or focusing?";
                questions[3] = "Do you experience excessive worry or fear?";
                questions[4] = "Do you avoid social situations or feel anxious about them?";
                questions[5] = "Do you experience restlessness or difficulty relaxing?";
                questions[6] = "Do you have trouble falling or staying asleep?";
                questions[7] = "Do you feel physically tense or fatigued?";
                questions[8] = "Do you experience panic attacks or sudden episodes of fear?";
                questions[9] = "Do you have a family history of anxiety or mental health conditions?";
                askQuestions("Anxiety", questions, NUM_QUESTIONS);
                break;

            // Case 18: Depression
            case 18:
                questions[0] = "Do you often feel sad or hopeless?";
                questions[1] = "Do you experience a lack of interest or pleasure in activities you once enjoyed?";
                questions[2] = "Do you have trouble sleeping or sleeping too much?";
                questions[3] = "Do you feel tired or low in energy, even after rest?";
                questions[4] = "Do you have difficulty concentrating or making decisions?";
                questions[5] = "Do you experience changes in appetite or weight?";
                questions[6] = "Do you feel worthless or excessively guilty?";
                questions[7] = "Do you have thoughts of death or suicide?";
                questions[8] = "Do you feel isolated or disconnected from others?";
                questions[9] = "Do you have a family history of depression or other mental health disorders?";
                askQuestions("Depression", questions, NUM_QUESTIONS);
                break;

            // Case 19: Malaria
            case 19:
                questions[0] = "Do you experience high fever with chills?";
                questions[1] = "Do you experience sweating after fever episodes?";
                questions[2] = "Do you have a headache or body aches?";
                questions[3] = "Do you feel tired or weak after fever episodes?";
                questions[4] = "Do you have nausea or vomiting?";
                questions[5] = "Do you have pain in the abdomen or muscles?";
                questions[6] = "Do you have a history of travel to areas where malaria is common?";
                questions[7] = "Do you have a history of mosquito bites in tropical areas?";
                questions[8] = "Do you have anemia or low blood pressure?";
                questions[9] = "Do you have a family history of malaria?";
                askQuestions("Malaria", questions, NUM_QUESTIONS);
                break;

            // Case 20: HIV/AIDS
            case 20:
                questions[0] = "Do you experience unexplained weight loss?";
                questions[1] = "Do you have frequent or unusual infections?";
                questions[2] = "Do you have night sweats or fever?";
                questions[3] = "Do you have a persistent cough or shortness of breath?";
                questions[4] = "Do you experience persistent fatigue or weakness?";
                questions[5] = "Do you have swollen lymph nodes?";
                questions[6] = "Do you have sores, rashes, or lesions on your skin or mouth?";
                questions[7] = "Have you engaged in unprotected sexual activity with a person of unknown HIV status?";
                questions[8] = "Have you shared needles or had contact with contaminated blood?";
                questions[9] = "Do you have a history of multiple sexual partners?";
                askQuestions("HIV/AIDS", questions, NUM_QUESTIONS);
                break;

            // Case 21: Iron Deficiency
            case 21:
                questions[0] = "Do you feel fatigued or weak frequently?";
                questions[1] = "Do you have pale skin or nail beds?";
                questions[2] = "Do you experience dizziness or lightheadedness?";
                questions[3] = "Do you have shortness of breath during regular activities?";
                questions[4] = "Do you experience frequent headaches?";
                questions[5] = "Do you have cravings for non-food items (e.g., dirt or ice)?";
                questions[6] = "Do you have brittle or spoon-shaped nails?";
                questions[7] = "Do you have a history of heavy menstrual periods?";
                questions[8] = "Do you have a vegetarian or restricted diet?";
                questions[9] = "Do you have a family history of iron deficiency anemia?";
                askQuestions("Iron Deficiency", questions, NUM_QUESTIONS);
                break;
            // Case 22: Sciatica
            case 22:
                questions[0] = "Do you experience pain in your lower back or legs?";
                questions[1] = "Do you feel sharp or shooting pain down one leg?";
                questions[2] = "Do you experience numbness or tingling in your legs or feet?";
                questions[3] = "Do you have difficulty standing or walking due to pain?";
                questions[4] = "Do you feel pain that worsens when sitting or bending?";
                questions[5] = "Do you experience muscle weakness in your leg or foot?";
                questions[6] = "Do you have a history of herniated discs or spinal problems?";
                questions[7] = "Do you have trouble sleeping due to pain in your legs or back?";
                questions[8] = "Have you experienced any recent trauma to your lower back?";
                questions[9] = "Do you feel relief when standing or walking for long periods?";
                askQuestions("Sciatica", questions, NUM_QUESTIONS);
                break;

            // Case 23: Eczema
            case 23:
                questions[0] = "Do you have red, itchy rashes on your skin?";
                questions[1] = "Do you experience dry or flaky skin?";
                questions[2] = "Do you have a history of allergies or asthma?";
                questions[3] = "Do you experience skin inflammation or swelling?";
                questions[4] = "Do you have rough patches of skin that crack or bleed?";
                questions[5] = "Do your symptoms worsen during the winter months?";
                questions[6] = "Do you notice your skin getting irritated by certain soaps or fabrics?";
                questions[7] = "Do you have a family history of eczema or other skin conditions?";
                questions[8] = "Do you experience skin rashes in the folds of your arms or legs?";
                questions[9] = "Do you feel constant itching or burning on the affected areas?";
                askQuestions("Eczema", questions, NUM_QUESTIONS);
                break;

            // Case 24: Migraine
            case 24:
                questions[0] = "Do you experience severe headaches, usually on one side of the head?";
                questions[1] = "Do you experience nausea or vomiting during headaches?";
                questions[2] = "Do you experience sensitivity to light, sound, or smells?";
                questions[3] = "Do you have visual disturbances (e.g., seeing flashing lights)?";
                questions[4] = "Do you have a family history of migraines or similar conditions?";
                questions[5] = "Do you experience headaches triggered by stress or certain foods?";
                questions[6] = "Do you experience a throbbing or pulsing sensation during headaches?";
                questions[7] = "Do your headaches last for hours or days?";
                questions[8] = "Do you find it difficult to carry out daily tasks during a migraine?";
                questions[9] = "Do you have a history of frequent or recurring headaches?";
                askQuestions("Migraine", questions, NUM_QUESTIONS);
                break;

            // Case 25: Gastrointestinal Disorder
            case 25:
                questions[0] = "Do you experience persistent abdominal pain or discomfort?";
                questions[1] = "Do you have frequent diarrhea or constipation?";
                questions[2] = "Do you experience bloating or excessive gas?";
                questions[3] = "Do you have difficulty swallowing or experience a choking sensation?";
                questions[4] = "Do you have nausea or vomiting after eating?";
                questions[5] = "Do you have a history of gastrointestinal issues like ulcers or IBS?";
                questions[6] = "Do you experience heartburn or acid reflux?";
                questions[7] = "Do you feel unusually full after eating small amounts of food?";
                questions[8] = "Do you experience weight loss or malnutrition?";
                questions[9] = "Do you have a family history of gastrointestinal disorders?";
                askQuestions("Gastrointestinal Disorder", questions, NUM_QUESTIONS);
                break;

            // Case 26: GERD (Gastroesophageal Reflux Disease)
            case 26:
                questions[0] = "Do you experience heartburn or acid reflux after meals?";
                questions[1] = "Do you have a sour taste in your mouth or regurgitation?";
                questions[2] = "Do you experience chest pain or discomfort, especially after eating?";
                questions[3] = "Do you have difficulty swallowing or a sensation of a lump in your throat?";
                questions[4] = "Do you experience a chronic cough or hoarseness?";
                questions[5] = "Do you frequently experience a sore throat or mouth ulcers?";
                questions[6] = "Do you have a history of smoking or excessive alcohol consumption?";
                questions[7] = "Do you lie down or bend over after meals, worsening symptoms?";
                questions[8] = "Do you have a family history of GERD or acid reflux?";
                questions[9] = "Do you notice your symptoms worsen when consuming spicy or fatty foods?";
                askQuestions("GERD", questions, NUM_QUESTIONS);
                break;

            // Case 27: COPD (Chronic Obstructive Pulmonary Disease)
            case 27:
                questions[0] = "Do you experience shortness of breath, especially during physical activity?";
                questions[1] = "Do you have a chronic cough or cough up mucus regularly?";
                questions[2] = "Do you feel wheezing or a tightness in your chest?";
                questions[3] = "Do you have difficulty catching your breath or speaking in full sentences?";
                questions[4] = "Do you have a history of smoking or exposure to secondhand smoke?";
                questions[5] = "Do you experience frequent respiratory infections?";
                questions[6] = "Do you experience fatigue or weakness that limits your activity?";
                questions[7] = "Do you have a history of lung diseases or respiratory conditions?";
                questions[8] = "Do you have a family history of COPD or similar respiratory conditions?";
                questions[9] = "Do you find it difficult to breathe when walking up stairs or hills?";
                askQuestions("COPD", questions, NUM_QUESTIONS);
                break;

            // Case 28: Diabetes
            case 28:
                questions[0] = "Do you experience frequent urination or excessive thirst?";
                questions[1] = "Do you feel tired or fatigued even after getting enough rest?";
                questions[2] = "Do you have blurred vision or difficulty seeing clearly?";
                questions[3] = "Do you have unexplained weight loss or increased hunger?";
                questions[4] = "Do you experience slow-healing wounds or infections?";
                questions[5] = "Do you have numbness or tingling in your hands or feet?";
                questions[6] = "Do you have a family history of diabetes?";
                questions[7] = "Do you experience frequent skin infections or fungal infections?";
                questions[8] = "Do you experience dry mouth or itchy skin?";
                questions[9] = "Do you have a history of high blood sugar or prediabetes?";
                askQuestions("Diabetes", questions, NUM_QUESTIONS);
                break;
            // Case 29: Chronic Pulmonary Disease
            case 29:
                questions[0] = "Do you experience shortness of breath even when not exerting yourself?";
                questions[1] = "Do you frequently cough, especially in the morning?";
                questions[2] = "Do you notice an increase in mucus production over time?";
                questions[3] = "Do you have difficulty breathing or feel tightness in your chest?";
                questions[4] = "Do you experience fatigue after minimal activity?";
                questions[5] = "Do you have a history of smoking or exposure to harmful gases?";
                questions[6] = "Do you have a persistent wheezing sound when breathing?";
                questions[7] = "Do you frequently get lung infections or bronchitis?";
                questions[8] = "Do you feel breathless after climbing stairs or walking short distances?";
                questions[9] = "Do you have a family history of chronic pulmonary conditions?";
                askQuestions("Chronic Pulmonary Disease", questions, NUM_QUESTIONS);
                break;

            // Case 30: Stroke
            case 30:
                questions[0] = "Do you experience sudden numbness or weakness in the face, arms, or legs?";
                questions[1] = "Do you have difficulty speaking or understanding speech?";
                questions[2] = "Do you experience sudden trouble seeing in one or both eyes?";
                questions[3] = "Do you feel dizziness, loss of balance, or coordination?";
                questions[4] = "Do you have a sudden severe headache with no known cause?";
                questions[5] = "Do you have difficulty walking or standing?";
                questions[6] = "Do you experience confusion or trouble with memory?";
                questions[7] = "Do you have a family history of strokes or heart disease?";
                questions[8] = "Do you have high blood pressure or high cholesterol?";
                questions[9] = "Do you smoke or drink excessively?";
                askQuestions("Stroke", questions, NUM_QUESTIONS);
                break;

            // Case 31: Chronic Liver Disease
            case 31:
                questions[0] = "Do you experience yellowing of the skin or eyes?";
                questions[1] = "Do you have abdominal pain or swelling?";
                questions[2] = "Do you feel fatigued or weak most of the time?";
                questions[3] = "Do you experience nausea or vomiting frequently?";
                questions[4] = "Do you have a history of heavy alcohol consumption?";
                questions[5] = "Do you have dark urine or pale-colored stools?";
                questions[6] = "Do you experience unexplained weight loss?";
                questions[7] = "Do you have a history of liver disease in your family?";
                questions[8] = "Do you experience confusion or difficulty thinking clearly?";
                questions[9] = "Do you have itchy skin or rashes?";
                askQuestions("Chronic Liver Disease", questions, NUM_QUESTIONS);
                break;

            // Case 32: Pneumonia
            case 32:
                questions[0] = "Do you have a cough that produces mucus or phlegm?";
                questions[1] = "Do you experience shortness of breath or difficulty breathing?";
                questions[2] = "Do you have a fever or chills?";
                questions[3] = "Do you feel fatigue or weakness even after resting?";
                questions[4] = "Do you have chest pain that worsens with deep breathing or coughing?";
                questions[5] = "Do you have a history of respiratory infections or lung conditions?";
                questions[6] = "Do you experience rapid breathing or shallow breathing?";
                questions[7] = "Do you feel light-headed or dizzy when standing?";
                questions[8] = "Do you have a history of smoking or exposure to harmful substances?";
                questions[9] = "Do you have a family history of lung diseases?";
                askQuestions("Pneumonia", questions, NUM_QUESTIONS);
                break;

            // Case 33: Allergies
            case 33:
                questions[0] = "Do you experience sneezing, especially in the morning or at night?";
                questions[1] = "Do you have itchy or watery eyes?";
                questions[2] = "Do you have a runny or stuffy nose?";
                questions[3] = "Do you experience skin rashes or hives?";
                questions[4] = "Do you feel shortness of breath or wheezing after exposure to allergens?";
                questions[5] = "Do you have a history of seasonal allergies or asthma?";
                questions[6] = "Do you experience headaches or sinus pressure during allergy season?";
                questions[7] = "Do you have a family history of allergies or asthma?";
                questions[8] = "Do you notice your symptoms worsen in certain environments (e.g., pollen, dust)?";
                questions[9] = "Do you have a history of food allergies or allergic reactions?";
                askQuestions("Allergies", questions, NUM_QUESTIONS);
                break;

            // Case 34: Skin Infections
            case 34:
                questions[0] = "Do you notice redness, swelling, or warmth on the affected skin area?";
                questions[1] = "Do you have pus or fluid-filled blisters on your skin?";
                questions[2] = "Do you experience pain or tenderness around the infected area?";
                questions[3] = "Do you have a fever or chills along with the skin infection?";
                questions[4] = "Do you have a history of eczema or other skin conditions?";
                questions[5] = "Have you been exposed to unclean or contaminated environments?";
                questions[6] = "Do you have recent cuts, wounds, or insect bites on your skin?";
                questions[7] = "Do you have a compromised immune system due to illness or medications?";
                questions[8] = "Do you experience swelling in the lymph nodes near the infected area?";
                questions[9] = "Do you have a history of frequent skin infections or bacterial infections?";
                askQuestions("Skin Infections", questions, NUM_QUESTIONS);
                break;

            // Case 35: Asthma
            case 35:
                questions[0] = "Do you experience shortness of breath, especially during physical activity?";
                questions[1] = "Do you have a frequent cough, particularly at night or early morning?";
                questions[2] = "Do you experience wheezing or a whistling sound when breathing?";
                questions[3] = "Do you find it difficult to breathe during seasonal changes or cold weather?";
                questions[4] = "Do you have a family history of asthma or other allergic conditions?";
                questions[5] = "Do you feel tightness in your chest during or after physical exertion?";
                questions[6] = "Do you experience coughing or shortness of breath after exposure to allergens (e.g., pollen, dust)?";
                questions[7] = "Do you use an inhaler or other asthma medications regularly?";
                questions[8] = "Do you experience frequent respiratory infections or colds?";
                questions[9] = "Do you have a history of smoking or exposure to secondhand smoke?";
                askQuestions("Asthma", questions, NUM_QUESTIONS);
                break;
            // Case 36: Hypertension
            case 36:
                questions[0] = "Do you frequently experience headaches or dizziness?";
                questions[1] = "Do you have a family history of high blood pressure?";
                questions[2] = "Do you often feel short of breath or fatigued?";
                questions[3] = "Do you have blurred vision or chest pain?";
                questions[4] = "Do you experience frequent nosebleeds or ringing in the ears?";
                questions[5] = "Do you have a history of kidney disease or diabetes?";
                questions[6] = "Do you smoke or drink alcohol regularly?";
                questions[7] = "Do you experience swelling in your legs or ankles?";
                questions[8] = "Do you have a high-salt diet or insufficient physical activity?";
                questions[9] = "Do you monitor your blood pressure regularly?";
                askQuestions("Hypertension", questions, NUM_QUESTIONS);
                break;

            // Case 37: Kidney Disease
            case 37:
                questions[0] = "Do you experience frequent urination or difficulty urinating?";
                questions[1] = "Do you feel pain or discomfort in your lower back or sides?";
                questions[2] = "Do you notice swelling in your ankles, feet, or hands?";
                questions[3] = "Do you experience fatigue, nausea, or vomiting?";
                questions[4] = "Do you have a history of diabetes or high blood pressure?";
                questions[5] = "Do you have blood in your urine or foamy urine?";
                questions[6] = "Do you have unexplained weight loss or loss of appetite?";
                questions[7] = "Do you feel itchy skin or dry, flaky skin?";
                questions[8] = "Do you have a family history of kidney disease or kidney failure?";
                questions[9] = "Do you drink alcohol or use drugs that may damage the kidneys?";
                askQuestions("Kidney Disease", questions, NUM_QUESTIONS);
                break;

            // Case 38: Hepatitis
            case 38:
                questions[0] = "Do you experience yellowing of your skin or eyes (jaundice)?";
                questions[1] = "Do you feel fatigued or weak most of the time?";
                questions[2] = "Do you have pain or discomfort in your abdomen, especially in the upper right side?";
                questions[3] = "Do you have nausea, vomiting, or loss of appetite?";
                questions[4] = "Do you experience dark-colored urine or pale stools?";
                questions[5] = "Do you have a history of alcohol consumption or drug use?";
                questions[6] = "Do you have a history of unprotected sexual activity or shared needles?";
                questions[7] = "Do you have a family history of liver disease or hepatitis?";
                questions[8] = "Do you have a history of blood transfusions or organ transplants?";
                questions[9] = "Do you have any recent exposure to someone with hepatitis or liver disease?";
                askQuestions("Hepatitis", questions, NUM_QUESTIONS);
                break;

            // Case 39: Cystic Fibrosis
            case 39:
                questions[0] = "Do you experience chronic coughing or wheezing?";
                questions[1] = "Do you have thick, sticky mucus in your lungs?";
                questions[2] = "Do you experience shortness of breath or chest tightness?";
                questions[3] = "Do you have frequent lung infections or pneumonia?";
                questions[4] = "Do you have trouble gaining weight or frequent digestive issues?";
                questions[5] = "Do you have a family history of cystic fibrosis or similar respiratory conditions?";
                questions[6] = "Do you experience salty-tasting skin or sweat?";
                questions[7] = "Do you have a history of pancreatic insufficiency or digestive problems?";
                questions[8] = "Do you experience frequent sinus infections or nasal polyps?";
                questions[9] = "Do you have a history of fatty stools or difficulty absorbing nutrients?";
                askQuestions("Cystic Fibrosis", questions, NUM_QUESTIONS);
                break;

            // Case 40: Tuberculosis
            case 40:
                questions[0] = "Do you have a persistent cough lasting more than 3 weeks?";
                questions[1] = "Do you experience night sweats or chills?";
                questions[2] = "Do you have unexplained weight loss or fatigue?";
                questions[3] = "Do you experience chest pain or coughing up blood?";
                questions[4] = "Do you have a family history of tuberculosis or recent exposure to someone with TB?";
                questions[5] = "Do you have a history of close contact with someone who has been diagnosed with tuberculosis?";
                questions[6] = "Do you have a weakened immune system due to HIV, medication, or other conditions?";
                questions[7] = "Do you experience shortness of breath or difficulty breathing?";
                questions[8] = "Do you live in or have recently traveled to areas with high rates of tuberculosis?";
                questions[9] = "Do you have a history of frequent respiratory infections or pneumonia?";
                askQuestions("Tuberculosis", questions, NUM_QUESTIONS);
                break;

            // Case 41: Multiple Sclerosis
            case 41:
                questions[0] = "Do you experience sudden numbness or weakness in the limbs?";
                questions[1] = "Do you have difficulty with coordination or balance?";
                questions[2] = "Do you experience blurred or double vision?";
                questions[3] = "Do you have problems with memory, thinking, or concentration?";
                questions[4] = "Do you experience fatigue that is not related to physical activity?";
                questions[5] = "Do you experience tingling or electric shock-like sensations?";
                questions[6] = "Do you have a family history of multiple sclerosis or autoimmune diseases?";
                questions[7] = "Do you experience difficulty speaking or swallowing?";
                questions[8] = "Do you have difficulty walking or controlling your movements?";
                questions[9] = "Do you experience periods of worsening symptoms, followed by improvement?";
                askQuestions("Multiple Sclerosis", questions, NUM_QUESTIONS);
                break;
            // Case 42: Arthritis
            case 42:
                questions[0] = "Do you experience joint pain or swelling?";
                questions[1] = "Do you have difficulty moving your joints or feel stiffness?";
                questions[2] = "Do you experience joint tenderness when touched?";
                questions[3] = "Do you have a family history of arthritis or joint diseases?";
                questions[4] = "Do you notice redness or warmth around your joints?";
                questions[5] = "Do you experience fatigue or fever along with joint pain?";
                questions[6] = "Do you have a history of previous joint injuries?";
                questions[7] = "Do you feel pain when walking or climbing stairs?";
                questions[8] = "Do you find it difficult to perform daily activities due to joint pain?";
                questions[9] = "Do you have morning stiffness that lasts more than 30 minutes?";
                askQuestions("Arthritis", questions, NUM_QUESTIONS);
                break;

            // Case 43: Parkinson's Disease
            case 43:
                questions[0] = "Do you experience tremors or shaking in your hands or feet?";
                questions[1] = "Do you have difficulty maintaining balance or walking?";
                questions[2] = "Do you notice stiffness or rigidity in your arms or legs?";
                questions[3] = "Do you experience slowed movement or trouble starting movements?";
                questions[4] = "Do you have a change in your handwriting or facial expressions?";
                questions[5] = "Do you have difficulty speaking clearly or swallowing?";
                questions[6] = "Do you feel lightheaded or dizzy when standing up quickly?";
                questions[7] = "Do you have a family history of Parkinson's disease?";
                questions[8] = "Do you experience sudden changes in mood, such as depression or anxiety?";
                questions[9] = "Do you have a history of sleep disturbances or excessive daytime sleepiness?";
                askQuestions("Parkinson's Disease", questions, NUM_QUESTIONS);
                break;

            // Case 44: Obesity
            case 44:
                questions[0] = "Do you experience difficulty maintaining a healthy weight?";
                questions[1] = "Do you have a family history of obesity or related health problems?";
                questions[2] = "Do you have a sedentary lifestyle or lack of physical activity?";
                questions[3] = "Do you often crave high-calorie or unhealthy foods?";
                questions[4] = "Do you feel fatigue or difficulty breathing during physical activities?";
                questions[5] = "Do you have high blood pressure or high cholesterol levels?";
                questions[6] = "Do you experience joint pain or discomfort due to excess weight?";
                questions[7] = "Do you have trouble losing weight even with diet and exercise?";
                questions[8] = "Do you feel self-conscious or stressed about your weight?";
                questions[9] = "Do you have a history of heart disease or diabetes?";
                askQuestions("Obesity", questions, NUM_QUESTIONS);
                break;

            // Case 45: Peptic Ulcer
            case 45:
                questions[0] = "Do you experience frequent stomach pain or discomfort?";
                questions[1] = "Do you have heartburn or acid reflux symptoms?";
                questions[2] = "Do you feel nauseous or bloated after meals?";
                questions[3] = "Do you experience loss of appetite or unexplained weight loss?";
                questions[4] = "Do you often feel a burning sensation in your stomach?";
                questions[5] = "Do you have a history of using nonsteroidal anti-inflammatory drugs (NSAIDs)?";
                questions[6] = "Do you have a history of alcohol consumption or smoking?";
                questions[7] = "Do you experience vomiting or passing black stools?";
                questions[8] = "Do you have a family history of peptic ulcers?";
                questions[9] = "Do you experience pain relief after eating or taking antacids?";
                askQuestions("Peptic Ulcer", questions, NUM_QUESTIONS);
                break;

            // Case 46: Gallbladder Disease
            case 46:
                questions[0] = "Do you experience pain in the upper right side of your abdomen?";
                questions[1] = "Do you have nausea or vomiting after eating fatty foods?";
                questions[2] = "Do you experience bloating, indigestion, or belching?";
                questions[3] = "Do you have a history of gallstones or gallbladder disease?";
                questions[4] = "Do you feel pain that radiates to your back or shoulder blades?";
                questions[5] = "Do you experience yellowing of the skin or eyes (jaundice)?";
                questions[6] = "Do you have a history of high cholesterol or obesity?";
                questions[7] = "Do you experience fever or chills along with abdominal pain?";
                questions[8] = "Do you have a family history of gallbladder disease or gallstones?";
                questions[9] = "Do you have a history of liver disease or pancreatitis?";
                askQuestions("Gallbladder Disease", questions, NUM_QUESTIONS);
                break;

            // Case 47: Chronic Liver Disease
            case 47:
                questions[0] = "Do you experience fatigue or weakness regularly?";
                questions[1] = "Do you have unexplained weight loss or loss of appetite?";
                questions[2] = "Do you feel pain or discomfort in the upper right side of your abdomen?";
                questions[3] = "Do you have a history of heavy alcohol consumption or drug use?";
                questions[4] = "Do you notice yellowing of your skin or eyes (jaundice)?";
                questions[5] = "Do you have swelling in your legs, ankles, or abdomen?";
                questions[6] = "Do you have dark-colored urine or pale stools?";
                questions[7] = "Do you experience nausea, vomiting, or confusion?";
                questions[8] = "Do you have a history of liver disease or hepatitis?";
                questions[9] = "Do you have a family history of chronic liver disease?";
                askQuestions("Chronic Liver Disease", questions, NUM_QUESTIONS);
                break;

            // Case 48: Anemia
            case 48:
                questions[0] = "Do you often feel tired or weak?";
                questions[1] = "Do you experience shortness of breath or dizziness?";
                questions[2] = "Do you have pale skin or a pale complexion?";
                questions[3] = "Do you experience heart palpitations or irregular heartbeat?";
                questions[4] = "Do you have a history of heavy menstrual periods or gastrointestinal bleeding?";
                questions[5] = "Do you have a diet low in iron-rich foods?";
                questions[6] = "Do you have a family history of anemia or blood disorders?";
                questions[7] = "Do you experience headaches or cold hands and feet?";
                questions[8] = "Do you have a history of vitamin B12 or folate deficiency?";
                questions[9] = "Do you have a history of chronic diseases such as kidney disease?";
                askQuestions("Anemia", questions, NUM_QUESTIONS);
                break;
            // Case 49: Epilepsy
            case 49:
                questions[0] = "Do you experience sudden seizures or convulsions?";
                questions[1] = "Do you have a history of loss of consciousness or staring spells?";
                questions[2] = "Do you experience unusual sensations, such as tingling or flashing lights?";
                questions[3] = "Do you have a family history of epilepsy or seizures?";
                questions[4] = "Do you have any history of head injury or brain trauma?";
                questions[5] = "Do you have difficulty controlling your movements during a seizure?";
                questions[6] = "Do you have a history of fainting or blackouts?";
                questions[7] = "Do you experience confusion or memory loss after a seizure?";
                questions[8] = "Do you have a history of strokes or other neurological conditions?";
                questions[9] = "Do you have a history of alcohol or drug withdrawal?";
                askQuestions("Epilepsy", questions, NUM_QUESTIONS);
                break;

            // Case 50: Sleep Apnea
            case 50:
                questions[0] = "Do you snore loudly during sleep?";
                questions[1] = "Do you experience choking or gasping for air during sleep?";
                questions[2] = "Do you often feel tired or fatigued during the day?";
                questions[3] = "Do you have difficulty staying awake while driving or working?";
                questions[4] = "Do you wake up with a dry mouth or sore throat?";
                questions[5] = "Do you have trouble concentrating or feeling forgetful?";
                questions[6] = "Do you have a history of high blood pressure?";
                questions[7] = "Do you experience restless sleep or frequent awakenings?";
                questions[8] = "Do you have a family history of sleep apnea or breathing problems during sleep?";
                questions[9] = "Do you have a large neck circumference or obesity?";
                askQuestions("Sleep Apnea", questions, NUM_QUESTIONS);
                break;

            // Case 51: Insomnia
            case 51:
                questions[0] = "Do you have difficulty falling asleep at night?";
                questions[1] = "Do you wake up frequently during the night?";
                questions[2] = "Do you have trouble staying asleep or wake up too early?";
                questions[3] = "Do you feel tired or unrefreshed upon waking up?";
                questions[4] = "Do you experience anxiety or stress before bed?";
                questions[5] = "Do you rely on caffeine or alcohol to help you sleep?";
                questions[6] = "Do you feel sleepy or fatigued during the day?";
                questions[7] = "Do you have a history of depression or other mental health disorders?";
                questions[8] = "Do you have a history of poor sleep hygiene or irregular sleep patterns?";
                questions[9] = "Do you have a history of chronic pain or medical conditions affecting sleep?";
                askQuestions("Insomnia", questions, NUM_QUESTIONS);
                break;

            // Case 52: Acne
            case 52:
                questions[0] = "Do you experience frequent pimples or blackheads on your face or body?";
                questions[1] = "Do you have oily skin or excessive oil production?";
                questions[2] = "Do you have a history of acne in your family?";
                questions[3] = "Do you use skincare products or cosmetics that may clog your pores?";
                questions[4] = "Do you experience acne flare-ups during stress or hormonal changes?";
                questions[5] = "Do you have scarring or dark spots left by acne?";
                questions[6] = "Do you experience redness or swelling around your pimples?";
                questions[7] = "Do you have a history of using strong acne medications?";
                questions[8] = "Do you have a history of skin conditions like eczema or rosacea?";
                questions[9] = "Do you experience acne on areas other than your face (back, chest, etc.)?";
                askQuestions("Acne", questions, NUM_QUESTIONS);
                break;

            // Case 53: Endometriosis
            case 53:
                questions[0] = "Do you experience painful or heavy periods?";
                questions[1] = "Do you have pain during sexual intercourse?";
                questions[2] = "Do you experience chronic pelvic pain or discomfort?";
                questions[3] = "Do you have a family history of endometriosis or related conditions?";
                questions[4] = "Do you experience pain or discomfort during bowel movements or urination?";
                questions[5] = "Do you have difficulty getting pregnant or infertility issues?";
                questions[6] = "Do you experience nausea or bloating around your menstrual cycle?";
                questions[7] = "Do you have fatigue or low energy during your periods?";
                questions[8] = "Do you notice pain outside of your menstrual cycle?";
                questions[9] = "Do you have a history of ovarian cysts or pelvic surgeries?";
                askQuestions("Endometriosis", questions, NUM_QUESTIONS);
                break;

            // Case 54: UTI (Urinary Tract Infection)
            case 54:
                questions[0] = "Do you experience a frequent urge to urinate?";
                questions[1] = "Do you feel pain or burning while urinating?";
                questions[2] = "Do you experience cloudy or foul-smelling urine?";
                questions[3] = "Do you experience pelvic pain or discomfort?";
                questions[4] = "Do you notice blood in your urine?";
                questions[5] = "Do you have a history of urinary tract infections?";
                questions[6] = "Do you feel the need to urinate but produce only small amounts of urine?";
                questions[7] = "Do you have a fever or chills along with urination problems?";
                questions[8] = "Do you have a history of sexual activity or use of a catheter?";
                questions[9] = "Do you have a history of kidney stones or bladder issues?";
                askQuestions("UTI (Urinary Tract Infection)", questions, NUM_QUESTIONS);
                break;

            // Case 55: Osteoporosis
            case 55:
                questions[0] = "Do you have a history of bone fractures or breaks?";
                questions[1] = "Do you experience back pain or loss of height?";
                questions[2] = "Do you feel a loss of strength in your bones or joints?";
                questions[3] = "Do you have a family history of osteoporosis or bone disorders?";
                questions[4] = "Do you have a diet low in calcium or vitamin D?";
                questions[5] = "Do you smoke or consume excessive alcohol?";
                questions[6] = "Do you experience brittle nails or thinning hair?";
                questions[7] = "Do you experience frequent muscle cramps or aches?";
                questions[8] = "Do you have a sedentary lifestyle or lack of physical activity?";
                questions[9] = "Do you take medications that affect bone density, such as corticosteroids?";
                askQuestions("Osteoporosis", questions, NUM_QUESTIONS);
                break;
            // Case 56: Prostate Cancer
            case 56:
                questions[0] = "Do you experience difficulty urinating or a weak urine stream?";
                questions[1] = "Do you have blood in your urine or semen?";
                questions[2] = "Do you experience pain in your lower back, hips, or pelvis?";
                questions[3] = "Do you have frequent urination, especially at night?";
                questions[4] = "Do you experience pain during ejaculation?";
                questions[5] = "Do you feel discomfort or a heavy feeling in your pelvic region?";
                questions[6] = "Do you have a family history of prostate cancer?";
                questions[7] = "Do you have unexplained weight loss or fatigue?";
                questions[8] = "Do you experience difficulty getting an erection?";
                questions[9] = "Do you have any history of urinary tract infections or prostate problems?";
                askQuestions("Prostate Cancer", questions, NUM_QUESTIONS);
                break;

            // Case 57: Testicular Cancer
            case 57:
                questions[0] = "Do you notice any lumps or swelling in your testicles?";
                questions[1] = "Do you experience pain or discomfort in your testicles?";
                questions[2] = "Do you feel a heaviness in your scrotum?";
                questions[3] = "Do you notice any changes in the size or shape of your testicles?";
                questions[4] = "Do you experience abdominal pain or swelling?";
                questions[5] = "Do you have unexplained fatigue or back pain?";
                questions[6] = "Do you have a family history of testicular cancer?";
                questions[7] = "Do you experience pain during ejaculation or urination?";
                questions[8] = "Do you have any history of undescended testicles?";
                questions[9] = "Have you had any previous testicular injuries?";
                askQuestions("Testicular Cancer", questions, NUM_QUESTIONS);
                break;

            // Case 58: Breast Cancer
            case 58:
                questions[0] = "Do you notice any lumps or changes in your breast tissue?";
                questions[1] = "Do you experience pain or tenderness in your breast or armpit?";
                questions[2] = "Do you see changes in the shape or size of your breasts?";
                questions[3] = "Do you have unexplained discharge from your nipple?";
                questions[4] = "Do you have a family history of breast cancer or other cancers?";
                questions[5] = "Do you experience skin changes like redness, dimpling, or thickening?";
                questions[6] = "Do you have a history of hormone therapy or other treatments?";
                questions[7] = "Do you feel a lump or swelling in your armpit?";
                questions[8] = "Do you have any unexplained weight loss or fatigue?";
                questions[9] = "Do you have a history of early menstruation or late menopause?";
                askQuestions("Breast Cancer", questions, NUM_QUESTIONS);
                break;

            // Case 59: Colon Cancer
            case 59:
                questions[0] = "Do you experience blood in your stool or rectal bleeding?";
                questions[1] = "Do you have persistent abdominal pain or cramps?";
                questions[2] = "Do you notice any unexplained weight loss?";
                questions[3] = "Do you experience changes in your bowel habits, such as diarrhea or constipation?";
                questions[4] = "Do you have a family history of colon cancer or other gastrointestinal cancers?";
                questions[5] = "Do you experience fatigue or weakness?";
                questions[6] = "Do you have a history of inflammatory bowel diseases (e.g., Crohn's disease or ulcerative colitis)?";
                questions[7] = "Do you feel a lump or mass in your abdomen?";
                questions[8] = "Do you experience nausea or vomiting?";
                questions[9] = "Do you have a diet high in processed meats or low in fiber?";
                askQuestions("Colon Cancer", questions, NUM_QUESTIONS);
                break;

            // Case 60: Lung Cancer
            case 60:
                questions[0] = "Do you have a persistent cough that doesn't go away?";
                questions[1] = "Do you experience chest pain or discomfort?";
                questions[2] = "Do you have difficulty breathing or shortness of breath?";
                questions[3] = "Do you cough up blood or bloody mucus?";
                questions[4] = "Do you experience unexplained weight loss or fatigue?";
                questions[5] = "Do you have a history of smoking or exposure to secondhand smoke?";
                questions[6] = "Do you have a family history of lung cancer or other lung diseases?";
                questions[7] = "Do you feel wheezing or hoarseness in your voice?";
                questions[8] = "Do you experience frequent respiratory infections?";
                questions[9] = "Do you have a history of asbestos exposure or environmental pollutants?";
                askQuestions("Lung Cancer", questions, NUM_QUESTIONS);
                break;

            // Case 61: Leukemia
            case 61:
                questions[0] = "Do you experience unexplained fever or night sweats?";
                questions[1] = "Do you have frequent infections or illnesses?";
                questions[2] = "Do you have unusual bleeding or bruising?";
                questions[3] = "Do you experience fatigue, weakness, or pallor?";
                questions[4] = "Do you have swollen lymph nodes or spleen?";
                questions[5] = "Do you feel pain in your bones or joints?";
                questions[6] = "Do you have frequent nosebleeds or gum bleeding?";
                questions[7] = "Do you experience weight loss without trying?";
                questions[8] = "Do you have a family history of leukemia or other blood cancers?";
                questions[9] = "Do you have a history of radiation exposure or chemotherapy treatment?";
                askQuestions("Leukemia", questions, NUM_QUESTIONS);
                break;

            // Case 62: Lymphoma
            case 62:
                questions[0] = "Do you notice swollen lymph nodes or glands in your neck, armpits, or groin?";
                questions[1] = "Do you experience unexplained weight loss?";
                questions[2] = "Do you have fever or night sweats?";
                questions[3] = "Do you feel fatigued or weak?";
                questions[4] = "Do you have itching or skin rashes?";
                questions[5] = "Do you have pain in your chest or abdomen?";
                questions[6] = "Do you experience coughing or shortness of breath?";
                questions[7] = "Do you have a family history of lymphoma or blood cancers?";
                questions[8] = "Do you have any history of immunosuppressive therapy?";
                questions[9] = "Do you have a history of infections such as Epstein-Barr virus?";
                askQuestions("Lymphoma", questions, NUM_QUESTIONS);
                break;

            // Case 63: Migraine
            case 63:
                questions[0] = "Do you experience severe, recurring headaches?";
                questions[1] = "Do you have sensitivity to light, sound, or smells during headaches?";
                questions[2] = "Do you experience nausea or vomiting with your headaches?";
                questions[3] = "Do you have a family history of migraines or other types of headaches?";
                questions[4] = "Do you experience visual disturbances such as flashing lights or blind spots?";
                questions[5] = "Do you feel throbbing or pulsing pain on one side of your head?";
                questions[6] = "Do your headaches worsen with physical activity or exertion?";
                questions[7] = "Do you experience dizziness or a feeling of unsteadiness?";
                questions[8] = "Do you have a history of stress, anxiety, or depression?";
                questions[9] = "Do you experience changes in your appetite or sleep patterns during headaches?";
                askQuestions("Migraine", questions, NUM_QUESTIONS);
                break;
            // Case 64: Gastrointestinal Disorder
            case 64:
                questions[0] = "Do you experience frequent bloating or indigestion?";
                questions[1] = "Do you have unexplained abdominal pain or discomfort?";
                questions[2] = "Do you experience constipation or diarrhea?";
                questions[3] = "Do you notice blood in your stool?";
                questions[4] = "Do you have frequent heartburn or acid reflux?";
                questions[5] = "Do you experience nausea or vomiting?";
                questions[6] = "Do you have a loss of appetite or unexplained weight loss?";
                questions[7] = "Do you feel fatigue or weakness?";
                questions[8] = "Do you have a family history of gastrointestinal diseases?";
                questions[9] = "Do you have any history of food intolerances or allergies?";
                askQuestions("Gastrointestinal Disorder", questions, NUM_QUESTIONS);
                break;

            // Case 65: GERD (Gastroesophageal Reflux Disease)
            case 65:
                questions[0] = "Do you experience frequent heartburn or acid reflux?";
                questions[1] = "Do you have difficulty swallowing or feel food getting stuck in your throat?";
                questions[2] = "Do you have chest pain or discomfort, especially after eating?";
                questions[3] = "Do you experience regurgitation or a sour taste in your mouth?";
                questions[4] = "Do you experience a dry cough or hoarseness?";
                questions[5] = "Do you have difficulty breathing or wheezing, especially at night?";
                questions[6] = "Do you feel bloated or experience a feeling of fullness after eating?";
                questions[7] = "Do you experience nausea after meals?";
                questions[8] = "Do you lie down frequently after eating?";
                questions[9] = "Do you have a family history of GERD or other gastrointestinal diseases?";
                askQuestions("GERD", questions, NUM_QUESTIONS);
                break;

            // Case 66: COPD (Chronic Obstructive Pulmonary Disease)
            case 66:
                questions[0] = "Do you have a persistent cough or wheezing?";
                questions[1] = "Do you experience shortness of breath or difficulty breathing?";
                questions[2] = "Do you have a history of smoking or exposure to secondhand smoke?";
                questions[3] = "Do you feel fatigued or weak after physical exertion?";
                questions[4] = "Do you experience frequent respiratory infections or colds?";
                questions[5] = "Do you have a history of chronic bronchitis or emphysema?";
                questions[6] = "Do you feel tightness in your chest or difficulty exhaling air?";
                questions[7] = "Do you have a family history of lung diseases or COPD?";
                questions[8] = "Do you experience frequent coughing or mucus production?";
                questions[9] = "Do you have a history of exposure to pollutants or chemicals?";
                askQuestions("COPD", questions, NUM_QUESTIONS);
                break;

            // Case 67: Sciatica
            case 67:
                questions[0] = "Do you experience sharp or shooting pain down one leg?";
                questions[1] = "Do you have numbness, tingling, or weakness in your leg or foot?";
                questions[2] = "Do you feel pain that worsens when sitting or standing for long periods?";
                questions[3] = "Do you experience lower back pain that radiates down your leg?";
                questions[4] = "Do you have difficulty moving or controlling your leg?";
                questions[5] = "Do you experience pain when bending or twisting your back?";
                questions[6] = "Do you have a history of disc problems or herniated discs?";
                questions[7] = "Do you feel relief when lying down or changing positions?";
                questions[8] = "Do you experience weakness in your lower back or legs?";
                questions[9] = "Have you had any recent injuries or strains to your back or legs?";
                askQuestions("Sciatica", questions, NUM_QUESTIONS);
                break;

            // Case 68: Eczema
            case 68:
                questions[0] = "Do you have dry, itchy skin that tends to crack or peel?";
                questions[1] = "Do you have red, inflamed, or swollen patches of skin?";
                questions[2] = "Do you experience skin rashes that worsen with scratching?";
                questions[3] = "Do you have a history of asthma or hay fever?";
                questions[4] = "Do you notice small, fluid-filled blisters on your skin?";
                questions[5] = "Do you experience thickened, leathery skin over time?";
                questions[6] = "Do you have a family history of eczema or other skin conditions?";
                questions[7] = "Do you have a sensitivity to certain fabrics, soaps, or detergents?";
                questions[8] = "Do you experience flare-ups during periods of stress or illness?";
                questions[9] = "Do you have trouble sleeping due to itching or discomfort?";
                askQuestions("Eczema", questions, NUM_QUESTIONS);
                break;

            // Case 69: Skin Disorders
            case 69:
                questions[0] = "Do you notice any unusual rashes, pimples, or skin lesions?";
                questions[1] = "Do you experience dryness, redness, or peeling of your skin?";
                questions[2] = "Do you have a family history of skin conditions such as acne or psoriasis?";
                questions[3] = "Do you have a history of frequent sun exposure or sunburns?";
                questions[4] = "Do you notice any changes in the color or texture of your skin?";
                questions[5] = "Do you experience any itching, burning, or pain on your skin?";
                questions[6] = "Do you have any history of skin infections or fungal growths?";
                questions[7] = "Do you use any skin products that irritate or cause breakouts?";
                questions[8] = "Do you feel self-conscious or distressed by your skin condition?";
                questions[9] = "Do you notice any recurring skin problems or flare-ups?";
                askQuestions("Skin Disorders", questions, NUM_QUESTIONS);
                break;

            // Case 70: Mental Health Disorders
            case 70:
                questions[0] = "Do you experience frequent feelings of sadness or hopelessness?";
                questions[1] = "Do you have difficulty concentrating or making decisions?";
                questions[2] = "Do you feel anxious, worried, or overwhelmed most of the time?";
                questions[3] = "Do you have trouble sleeping or experience excessive fatigue?";
                questions[4] = "Do you find it hard to engage in social activities or leave your house?";
                questions[5] = "Do you have any history of panic attacks or phobias?";
                questions[6] = "Do you experience mood swings, irritability, or extreme emotional reactions?";
                questions[7] = "Do you have thoughts of self-harm or suicide?";
                questions[8] = "Do you feel disconnected from reality or have trouble with your memory?";
                questions[9] = "Do you have a family history of mental health disorders?";
                askQuestions("Mental Health Disorders", questions, NUM_QUESTIONS);
                break;



                case 71:
                    exit(0);




            default:
                cout << "Invalid choice. Please select a valid option.\n";
        }
    }

    return 0;
}



// module 3
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <limits.h>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <thread>
#include <chrono>
#include <map>

using namespace std;

const int INF = INT_MAX;
const float speedCar = 60.0; // Speed of car in km/h
const float speedBike = 45.0; // Speed of bike in km/h
const float speedWalk = 5.0; // Speed of walking in km/h

// Function to trim spaces from the start and end of a string
string trim(const string &str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Function to find the node with the minimum distance which is not yet visited
int findMinDistance(const vector<int>& dist, const vector<bool>& visited, int n) {
    int minDist = INF;
    int minIndex = -1;

    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[i] < minDist) {
            minDist = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Dijkstra's algorithm to find the shortest path and the time taken
pair<pair<int, float>, vector<int>> dijkstra(const vector<vector<int>>& graph, int src, int dest, int n, float speed) {
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);

    dist[src] = 0;

    for (int i = 0; i < n - 1; i++) {
        int u = findMinDistance(dist, visited, n);
        if (u == -1) break;

        visited[u] = true;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    vector<int> path;
    int temp = dest;
    while (temp != -1) {
        path.insert(path.begin(), temp);
        temp = parent[temp];
    }

    float timeTaken = dist[dest] / speed; // Calculate time: time = distance / speed (in hours)

    return {{dist[dest], timeTaken}, path};
}

// Function to calculate the second shortest path
pair<pair<int, float>, vector<int>> secondShortestPath(const vector<vector<int>>& graph, int src, int dest, int n, float speed, const vector<int>& shortestPath) {
    vector<vector<int>> modifiedGraph = graph;

    // Remove edges of the shortest path from the graph (set them to INF)
    for (size_t i = 0; i < shortestPath.size() - 1; i++) {
        int u = shortestPath[i];
        int v = shortestPath[i + 1];
        modifiedGraph[u][v] = INF;
        modifiedGraph[v][u] = INF;
    }

    // Find the second shortest path using Dijkstra on the modified graph
    auto result = dijkstra(modifiedGraph, src, dest, n, speed);

    // If no second shortest path is found (i.e., the distance is still INF), return an indication
    if (result.first.first == INF) {
        cout << "No second shortest path found!" << endl;
        return {{INF, INF}, {}};
    }

    return result;
}

// Dummy Function to Log Messages
void logMessage1(const std::string &message) {
    std::cout << "[LOG]: " << message << std::endl;
}

// Dummy Function to Validate Inputs
bool validateInput(const std::string &input, const std::vector<std::string> &validInputs) {
    logMessage("Validating input: " + input);
    return std::find(validInputs.begin(), validInputs.end(), input) != validInputs.end();
}

// Dummy Function to Simulate Database Fetch
std::vector<std::string> fetchHospitalData() {
    logMessage("Fetching hospital data...");
    return {"Hospital A", "Hospital B", "Hospital C"};
}

// Dummy Function to Calculate Estimated Arrival Time
int calculateETA(int distance, int speed) {
    logMessage("Calculating ETA...");
    if (speed <= 0) {
        logMessage("Speed must be greater than zero.");
        return -1;
    }
    return distance / speed;
}

// Dummy Function to Display Travel Modes
void displayTravelModes() {
    logMessage("Displaying available travel modes...");
    std::cout << "1. Bike\n2. Car\n3. Walking\n";
}

// Dummy Function for Rating Validation
bool isValidRating(int rating) {
    logMessage("Checking if rating is valid...");
    return rating >= 1 && rating <= 5;
}

// Dummy Function to Get Current Location
std::string getCurrentLocation() {
    logMessage("Fetching current location...");
    return "User's current location";
}

// Dummy Function to Sort Hospital Ratings
std::vector<int> sortRatings(const std::vector<int> &ratings) {
    logMessage("Sorting hospital ratings...");
    std::vector<int> sortedRatings = ratings;
    std::sort(sortedRatings.begin(), sortedRatings.end(), std::greater<int>());
    return sortedRatings;
}

// Dummy Function to Print a Divider
void printDivider() {
    logMessage("Printing a divider...");
    std::cout << "=============================\n";
}

// Dummy Function to Simulate Loading Animation
void simulateLoading() {
    logMessage("Simulating loading animation...");
    for (int i = 0; i < 3; ++i) {
        std::cout << ".";
        std::flush(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << std::endl;
}

// Dummy Function for Unit Conversion
double convertKmToMiles(double km) {
    logMessage("Converting kilometers to miles...");
    return km * 0.621371;
}

// Dummy Function to Calculate Fuel Consumption
double calculateFuelConsumption(int distance, double efficiency) {
    logMessage("Calculating fuel consumption...");
    if (efficiency <= 0) {
        logMessage("Efficiency must be greater than zero.");
        return -1;
    }
    return distance / efficiency;
}

// Dummy Function for Random Number Generation
int generateRandomNumber(int min, int max) {
    logMessage("Generating a random number...");
    return min + rand() % (max - min + 1);
}

// Dummy Function to Check Doctor Availability
bool isDoctorAvailable(int doctorID, const std::string &timeSlot) {
    logMessage("Checking doctor availability...");
    return (doctorID % 2 == 0); // Example logic
}

// Dummy Function for Dummy Encryption
std::string encryptPassword(const std::string &password) {
    logMessage("Encrypting password...");
    std::string encrypted = password;
    for (char &ch : encrypted) {
        ch += 3; // Simple Caesar cipher
    }
    return encrypted;
}

// Dummy Function to Generate a Report
void generateReport(const std::string &title, const std::vector<std::string> &data) {
    std::cout << "=== " << title << " ===\n";
    for (const auto &entry : data) {
        std::cout << "- " << entry << "\n";
    }
    std::cout << "=========================\n";
}

// Dummy Function to Simulate API Call
std::string simulateAPICall(const std::string &endpoint) {
    std::cout << "Calling API endpoint: " << endpoint << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return "Success: Data retrieved from " + endpoint;
}

// Dummy Function to Format Strings
std::string formatString(const std::string &input) {
    std::string formatted = input;
    formatted[0] = toupper(formatted[0]);
    return formatted;
}

// Dummy Function to Calculate Discounts
double calculateDiscount(double price, double discountRate) {
    if (discountRate < 0 || discountRate > 100) {
        std::cout << "Invalid discount rate. Setting to 0%.\n";
        discountRate = 0;
    }
    return price - (price * discountRate / 100);
}

// Dummy Function to Simulate Data Encryption
std::string simpleEncrypt(const std::string &data) {
    std::string encrypted;
    for (char c : data) {
        encrypted += c + 1; // Simple character shift
    }
    return encrypted;
}

// Dummy Function to Calculate Distance Between Points
double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Dummy Function to Simulate Progress Bar
void displayProgressBar(int seconds) {
    std::cout << "Progress: [";
    for (int i = 0; i < seconds; ++i) {
        std::cout << "=";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "] Done!\n";
}

// Dummy Function to Validate Email
bool validateEmail(const std::string &email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    return (atPos != std::string::npos && dotPos != std::string::npos && atPos < dotPos);
}

// Dummy Function to Calculate Median of Numbers
double calculateMedian(const std::vector<int> &numbers) {
    std::vector<int> sortedNumbers = numbers;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());
    size_t size = sortedNumbers.size();
    if (size % 2 == 0) {
        return (sortedNumbers[size / 2 - 1] + sortedNumbers[size / 2]) / 2.0;
    }
    return sortedNumbers[size / 2];
}

// Dummy Function to Parse Command-Line Arguments
void parseArguments(int argc, char *argv[]) {
    std::cout << "Parsing Command-Line Arguments...\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << "Arg[" << i << "]: " << argv[i] << "\n";
    }
}

// Dummy Function to Simulate Sending Notifications
void sendNotification(const std::string &message, const std::vector<std::string> &recipients) {
    std::cout << "Sending notification: " << message << "\n";
    for (const auto &recipient : recipients) {
        std::cout << "To: " << recipient << "\n";
    }
}

// Dummy Function to Display Help Menu
void displayHelpMenu() {
    std::cout << "==== Help Menu ====\n";
    std::cout << "1. Login\n";
    std::cout << "2. Register\n";
    std::cout << "3. Book Appointment\n";
    std::cout << "4. Search by Symptoms\n";
    std::cout << "5. Exit\n";
    std::cout << "===================\n";
}

// Dummy Function to Convert Time Formats
std::string convert24To12Hour(int hour, int minute) {
    std::string period = (hour >= 12) ? "PM" : "AM";
    hour = (hour > 12) ? hour - 12 : hour;
    return std::to_string(hour) + ":" + (minute < 10 ? "0" : "") + std::to_string(minute) + " " + period;
}

// Dummy Function to Track User Activity
void trackUserActivity(const std::string &username, const std::string &activity) {
    std::cout << "Tracking activity for user: " << username << "\n";
    std::cout << "Activity: " << activity << "\n";
}

// Dummy Function to Calculate Simple Interest
double calculateSimpleInterest(double principal, double rate, double time) {
    return (principal * rate * time) / 100;
}

// Dummy Function to List Nearby Restaurants
std::vector<std::string> listNearbyRestaurants() {
    return {"Restaurant A", "Restaurant B", "Restaurant C"};
}

// Dummy Function to Find Maximum Value
int findMax(const std::vector<int> &values) {
    if (values.empty()) return -1;
    return *std::max_element(values.begin(), values.end());
}


// Dummy Function to Calculate Compound Interest
double calculateCompoundInterest(double principal, double rate, int time, int n) {
    return principal * pow((1 + rate / (100 * n)), n * time);
}

// Dummy Function to Generate Unique IDs
std::string generateUniqueID(const std::string &prefix, int id) {
    return prefix + "_" + std::to_string(id);
}

// Dummy Function to Simulate Sending Email
void sendEmail(const std::string &recipient, const std::string &subject, const std::string &body) {
    std::cout << "Sending email to: " << recipient << "\n";
    std::cout << "Subject: " << subject << "\n";
    std::cout << "Body:\n" << body << "\n";
}

// Dummy Function to Get User Preferences
std::map<std::string, std::string> getUserPreferences() {
    return {
        {"theme", "dark"},
        {"language", "English"},
        {"notifications", "enabled"}
    };
}

// Dummy Function to Simulate Logging Out
void logoutUser(const std::string &username) {
    std::cout << "Logging out user: " << username << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Logout successful.\n";
}

// Dummy Function to Generate Random String
std::string generateRandomString(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += characters[rand() % characters.size()];
    }
    return result;
}

// Dummy Function to Simulate Password Reset
bool resetPassword(const std::string &username) {
    std::cout << "Resetting password for: " << username << "\n";
    std::cout << "Temporary password: " << generateRandomString(8) << "\n";
    return true;
}

// Dummy Function to Check Internet Connectivity
bool isInternetAvailable() {
    std::cout << "Checking internet connectivity...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return true; // Assume always connected
}

// Dummy Function to Analyze User Behavior
void analyzeUserBehavior(const std::string &username) {
    std::cout << "Analyzing behavior of user: " << username << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Analysis complete. User spends 2 hours daily on platform.\n";
}

// Dummy Function to Predict Traffic Conditions
std::string predictTrafficConditions(const std::string &route) {
    std::cout << "Predicting traffic conditions for route: " << route << "\n";
    return "Moderate traffic";
}

// Dummy Function to Parse JSON (Mock Implementation)
std::map<std::string, std::string> parseJSON(const std::string &jsonString) {
    std::cout << "Parsing JSON string: " << jsonString << "\n";
    return {{"key1", "value1"}, {"key2", "value2"}}; // Mock response
}

// Dummy Function to Display Graph
/*void displayGraph(const std::vector<std::pair<int, int>> &edges) {
    std::cout << "Graph Edges:\n";
    for (const auto &[from, to] : edges) {
        std::cout << from << " -> " << to << "\n";
    }
}
*/
//  Function to List Supported Languages
void listSupportedLanguages() {
    std::cout << "Supported Languages:\n";
    std::cout << "1. English\n2. Spanish\n3. French\n4. German\n";
}

//  Function to Display Notifications
void displayNotifications(const std::vector<std::string> &notifications) {
    if (notifications.empty()) {
        std::cout << "No new notifications.\n";
        return;
    }
    std::cout << "Notifications:\n";
    for (const auto &notification : notifications) {
        std::cout << "- " << notification << "\n";
    }
}

//  Function to Format Time
std::string formatTime(int hour, int minute, int second) {
    std::string formattedTime =
        (hour < 10 ? "0" : "") + std::to_string(hour) + ":" +
        (minute < 10 ? "0" : "") + std::to_string(minute) + ":" +
        (second < 10 ? "0" : "") + std::to_string(second);
    return formattedTime;
}

// Dummy Function to Simulate Loading Screen
void simulateLoadingScreen(int duration) {
    std::cout << "Loading";
    for (int i = 0; i < duration; ++i) {
        std::cout << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << " Done!\n";
}


//  Function to Generate Fibonacci Sequence
std::vector<int> generateFibonacci(int n) {
    // Generates a Fibonacci sequence up to n terms
    std::vector<int> fibonacci;
    if (n <= 0) return fibonacci;
    fibonacci.push_back(0);
    if (n == 1) return fibonacci;
    fibonacci.push_back(1);
    for (int i = 2; i < n; ++i) {
        fibonacci.push_back(fibonacci[i - 1] + fibonacci[i - 2]);
    }
    return fibonacci;
}

//  Function to Calculate Factorial
long long calculateFactorial(int n) {
    // Calculates factorial of a number
    if (n < 0) return -1;
    long long factorial = 1;
    for (int i = 1; i <= n; ++i) {
        factorial *= i;
    }
    return factorial;
}

//  Function to Simulate Sleep Timer
void simulateSleepTimer(int seconds) {
    // Simulates a sleep timer
    std::cout << "Sleeping for " << seconds << " seconds...\n";
    for (int i = 1; i <= seconds; ++i) {
        std::cout << "Time elapsed: " << i << "s\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Timer complete.\n";
}

//  Function to Sort Strings Alphabetically
std::vector<std::string> sortStrings(const std::vector<std::string> &strings) {
    // Sorts a list of strings alphabetically
    std::vector<std::string> sortedStrings = strings;
    std::sort(sortedStrings.begin(), sortedStrings.end());
    return sortedStrings;
}

//  Function to Generate Prime Numbers
std::vector<int> generatePrimes(int n) {
    // Generates prime numbers up to n
    std::vector<int> primes;
    if (n <= 1) return primes;
    for (int i = 2; i <= n; ++i) {
        bool isPrime = true;
        for (int j = 2; j <= sqrt(i); ++j) {
            if (i % j == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(i);
        }
    }
    return primes;
}

//  Function to Generate Multiplication Table
void generateMultiplicationTable(int num, int limit) {
    // Generates multiplication table up to a given limit
    std::cout << "Multiplication Table for " << num << ":\n";
    for (int i = 1; i <= limit; ++i) {
        std::cout << num << " x " << i << " = " << num * i << "\n";
    }
}

//  Function to Reverse a String
std::string reverseString(const std::string &input) {
    // Reverses a given string
    std::string reversed = input;
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}

//  Function to Count Words in a Sentence
int countWords(const std::string &sentence) {
    // Counts the number of words in a sentence
    int count = 0;
    bool inWord = false;
    for (char c : sentence) {
        if (std::isspace(c)) {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            ++count;
        }
    }
    return count;
}

//  Function to Calculate Average of Numbers
double calculateAverage(const std::vector<int> &numbers) {
    // Calculates the average of a list of numbers
    if (numbers.empty()) return 0;
    int sum = 0;
    for (int num : numbers) {
        sum += num;
    }
    return static_cast<double>(sum) / numbers.size();
}

//  Function to Simulate Progress Bar
void showProgressBar(int steps) {
    // Simulates a progress bar with steps
    std::cout << "[";
    for (int i = 0; i < steps; ++i) {
        std::cout << "=";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "] Done!\n";
}

//  Function to Convert Temperature
double convertTemperature(double temp, const std::string &scale) {
    // Converts temperature between Celsius and Fahrenheit
    if (scale == "C") {
        return (temp - 32) * 5.0 / 9.0;
    } else if (scale == "F") {
        return (temp * 9.0 / 5.0) + 32;
    } else {
        std::cout << "Invalid scale provided. Use 'C' or 'F'.\n";
        return temp;
    }
}

//  Function to Check Palindrome
bool isPalindrome(const std::string &word) {
    // Checks if a word is a palindrome
    std::string reversed = reverseString(word);
    return word == reversed;
}

//  Function to Simulate Data Upload
void simulateDataUpload(int size) {
    // Simulates data upload of given size in MB
    std::cout << "Uploading " << size << "MB data...\n";
    for (int i = 1; i <= size; ++i) {
        std::cout << i << "MB uploaded\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Upload complete.\n";
}

//  Function to Count Vowels in a String
int countVowels(const std::string &text) {
    // Counts vowels in a given string
    int count = 0;
    for (char c : text) {
        if (std::tolower(c) == 'a' || std::tolower(c) == 'e' || std::tolower(c) == 'i' || std::tolower(c) == 'o' || std::tolower(c) == 'u') {
            ++count;
        }
    }
    return count;
}

//  Function to Generate a Random Matrix
std::vector<std::vector<int>> generateRandomMatrix(int rows, int cols, int maxValue) {
    // Generates a random matrix of given dimensions
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % maxValue;
        }
    }
    return matrix;
}

//  Function to Display Matrix
void displayMatrix(const std::vector<std::vector<int>> &matrix) {
    // Displays a matrix in row-column format
    for (const auto &row : matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}


//  Function to Simulate Authentication
bool authenticateUser(const std::string &username, const std::string &password) {
    std::cout << "Authenticating user: " << username << "\n";
    return password == "password123"; // Mock logic
}

//  Function to Generate a QR Code (Simulated)
void generateQRCode(const std::string &data) {
    std::cout << "Generating QR Code for: " << data << "\n";
    std::cout << "[###] [# #] [###]\n";
    std::cout << "[# #] [###] [# #]\n";
    std::cout << "[###] [# #] [###]\n";
}

//  Function to Perform Matrix Multiplication
std::vector<std::vector<int>> matrixMultiplication(const std::vector<std::vector<int>> &mat1, const std::vector<std::vector<int>> &mat2) {
    if (mat1[0].size() != mat2.size()) {
        std::cout << "Matrix dimensions incompatible for multiplication.\n";
        return {};
    }
    std::vector<std::vector<int>> result(mat1.size(), std::vector<int>(mat2[0].size(), 0));
    for (size_t i = 0; i < mat1.size(); ++i) {
        for (size_t j = 0; j < mat2[0].size(); ++j) {
            for (size_t k = 0; k < mat2.size(); ++k) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return result;
}

//  Function to Generate Random Float Numbers
std::vector<float> generateRandomFloats(int n, float lower, float upper) {
    std::vector<float> numbers;
    for (int i = 0; i < n; ++i) {
        float num = lower + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upper - lower)));
        numbers.push_back(num);
    }
    return numbers;
}

//  Function to Convert String to Uppercase
std::string toUpperCase(const std::string &input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
//  Function to Perform Modular Exponentiation
int modularExponentiation(int base, int exp, int mod) {
    if (mod == 1) return 0;
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

//  Function to Simulate Sorting Algorithm (Bubble Sort)
void bubbleSort(std::vector<int> &array) {
    for (size_t i = 0; i < array.size() - 1; ++i) {
        for (size_t j = 0; j < array.size() - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);
            }
        }
    }
}


//  Function to Simulate Data Encryption (XOR Cipher)
std::string xorCipher(const std::string &data, char key) {
    std::string encrypted = data;
    for (char &c : encrypted) {
        c ^= key;
    }
    return encrypted;
}

//  Function to Validate Email Address
bool isValidEmail(const std::string &email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return atPos != std::string::npos && dotPos != std::string::npos && dotPos > atPos;
}

//  Function to Calculate Factorial Using Recursion
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

//  Function to Simulate Stopwatch
void simulateStopwatch(int seconds) {
    for (int i = 1; i <= seconds; ++i) {
        std::cout << "Elapsed time: " << i << " second(s)\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Stopwatch finished!\n";
}

//  Function to Find Prime Numbers in a Range
std::vector<int> findPrimes(int start, int end) {
    std::vector<int> primes;
    for (int num = start; num <= end; ++num) {
        bool isPrime = true;
        for (int div = 2; div <= sqrt(num); ++div) {
            if (num % div == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime && num > 1) {
            primes.push_back(num);
        }
    }
    return primes;
}

//  Function to Convert Hexadecimal to Decimal
int hexToDecimal(const std::string &hex) {
    int decimalValue = 0;
    for (char c : hex) {
        decimalValue = decimalValue * 16 + (isdigit(c) ? c - '0' : toupper(c) - 'A' + 10);
    }
    return decimalValue;
}

//  Function to Simulate Chess Board Display
void displayChessBoard() {
    std::cout << "  A B C D E F G H\n";
    for (int row = 8; row >= 1; --row) {
        std::cout << row << " ";
        for (int col = 1; col <= 8; ++col) {
            std::cout << ((row + col) % 2 == 0 ? '#' : '.') << " ";
        }
        std::cout << row << "\n";
    }
    std::cout << "  A B C D E F G H\n";
}

//  Function to Simulate Rock, Paper, Scissors Game
void playRockPaperScissors() {
    const std::string options[] = {"Rock", "Paper", "Scissors"};
    srand(time(0));
    std::cout << "Choose: Rock, Paper, or Scissors\n";
    std::string userChoice;
    std::cin >> userChoice;
    std::string computerChoice = options[rand() % 3];
    std::cout << "Computer chose: " << computerChoice << "\n";

    if (userChoice == computerChoice) {
        std::cout << "It's a tie!\n";
    } else if ((userChoice == "Rock" && computerChoice == "Scissors") ||
               (userChoice == "Paper" && computerChoice == "Rock") ||
               (userChoice == "Scissors" && computerChoice == "Paper")) {
        std::cout << "You win!\n";
    } else {
        std::cout << "Computer wins!\n";
    }
}

//  Function to Calculate GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

//  Function to Simulate File Reading
std::string readFile(const std::string &fileName) {
    std::cout << "Reading file: " << fileName << "\n";
    return " file content.";
}

//  Function to Simulate AI Chatbot
void chatbot() {
    std::string userInput;
    std::cout << "Chatbot: Hello! How can I assist you today?\n";
    while (true) {
        std::cout << "You: ";
        std::getline(std::cin, userInput);
        if (userInput == "exit" || userInput == "quit") {
            std::cout << "Chatbot: Goodbye!\n";
            break;
        }
        std::cout << "Chatbot: I'm sorry, I didn't understand that.\n";
    }
}


//  Function to Find Maximum in an Array
int findMaximum(const std::vector<int> &array) {
    if (array.empty()) return INT_MIN;
    int maxVal = array[0];
    for (int val : array) {
        if (val > maxVal) maxVal = val;
    }
    return maxVal;
}

//  Function to Simulate API Call
/*void simulateAPICall(const std::string &endpoint, const std::map<std::string, std::string> &parameters) {
    std::cout << "Calling API at: " << endpoint << "\n";
    std::cout << "Parameters:\n";
    for (const auto &[key, value] : parameters) {
        std::cout << key << ": " << value << "\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "API Response: 200 OK\n";
}*/

//  Function to Perform Binary Search
int binarySearch(const std::vector<int> &sortedArray, int target) {
    int left = 0, right = sortedArray.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (sortedArray[mid] == target) return mid;
        if (sortedArray[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

//  Function to Encrypt a String (Simple Caesar Cipher)
std::string encryptString(const std::string &text, int shift) {
    std::string encrypted = text;
    for (char &c : encrypted) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;
        }
    }
    return encrypted;
}

//  Function to Decrypt a String (Simple Caesar Cipher)
std::string decryptString(const std::string &text, int shift) {
    return encryptString(text, 26 - shift);
}

//  Function to Generate Random Password
std::string generatePassword(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    std::string password;
    for (int i = 0; i < length; ++i) {
        password += characters[rand() % characters.size()];
    }
    return password;
}


//  Function to Find Duplicates in an Array
std::vector<int> findDuplicates(const std::vector<int> &array) {
    std::map<int, int> frequency;
    std::vector<int> duplicates;
    for (int num : array) {
        ++frequency[num];
        if (frequency[num] == 2) {
            duplicates.push_back(num);
        }
    }
    return duplicates;
}

//  Function to Simulate Database Query
std::vector<std::map<std::string, std::string>> queryDatabase(const std::string &query) {
    std::cout << "Executing query: " << query << "\n";
    return {
        {{"id", "1"}, {"name", "John"}, {"age", "25"}},
        {{"id", "2"}, {"name", "Jane"}, {"age", "30"}}
    };
}

//  Function to Calculate Distance Between Two Points



//  Function to Simulate Payment Processing
bool processPayment(const std::string &cardNumber, double amount) {
    std::cout << "Processing payment of $" << amount << " using card: " << cardNumber << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Payment successful!\n";
    return true;
}

//  Function to Calculate Grade
char calculateGrade(double score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

//  Function to Simulate User Authentication
bool authenticate(const std::string &username, const std::string &password) {
    std::cout << "Authenticating user: " << username << "\n";
    return password == "password123";
}

//  Function to Estimate Travel Time
double estimateTravelTime(double distance, double speed) {
    if (speed == 0) {
        std::cout << "Speed cannot be zero.\n";
        return -1;
    }
    return distance / speed;
}

//  Function to Log Application Events
void logEvent(const std::string &event) {
    std::cout << "[LOG]: " << event << "\n";
}

//  Function to Generate Random Color
std::string generateRandomColor() {
    const std::string colors[] = {"Red", "Green", "Blue", "Yellow", "Purple", "Orange"};
    return colors[rand() % 6];
}

//  Function to Simulate Traffic Light
void simulateTrafficLight() {
    const std::string states[] = {"Red", "Green", "Yellow"};
    for (const auto &state : states) {
        std::cout << "Traffic Light: " << state << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
/*// DumiterativePowermy Function to Calculate Power Using Iterative Method
int (int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}
*/
//  Function to Reverse a String


//  Function to Convert Decimal to Binary
std::string decimalToBinary(int num) {
    if (num == 0) return "0";
    std::string binary;
    while (num > 0) {
        binary = std::to_string(num % 2) + binary;
        num /= 2;
    }
    return binary;
}

//  Function to Convert Binary to Decimal
int binaryToDecimal(const std::string &binary) {
    int decimal = 0;
    int base = 1;  // 2^0
    for (int i = binary.length() - 1; i >= 0; --i) {
        if (binary[i] == '1') {
            decimal += base;
        }
        base *= 2;
    }
    return decimal;
}

//  Function to Print Multiplication Table
void printMultiplicationTable(int n) {
    std::cout << "Multiplication Table for " << n << ":\n";
    for (int i = 1; i <= 10; ++i) {
        std::cout << n << " x " << i << " = " << n * i << "\n";
    }
}

//  Function to Simulate Dice Rolling
void rollDice() {
    srand(time(0));
    for (int i = 0; i < 5; ++i) {
        int roll = (rand() % 6) + 1;
        std::cout << "Dice Roll " << i + 1 << ": " << roll << "\n";
    }
}




//  Function to Simulate Random Quotes
void generateRandomQuote() {
    const std::string quotes[] = {
        "The journey of a thousand miles begins with one step.",
        "Believe you can and you're halfway there.",
        "Success is not final; failure is not fatal.",
        "Act as if what you do makes a difference. It does."
    };
    srand(time(0));
    std::cout << "Random Quote: " << quotes[rand() % 4] << "\n";
}

//  Function to Calculate Compound Interest
double calculateCompoundInterest(double principal, double rate, double time, int n) {
    return principal * pow((1 + rate / (100 * n)), n * time);
}

//  Function to Print ASCII Art
void printASCIIArt() {
    std::cout << "  *\n ***\n*****\n ***\n  *\n";
}

//  Function to Check Palindrome
bool isPalindrome1(const std::string &str1) {
    int left = 0, right = str1.length() - 1;
    while (left < right) {
        if (str1[left] != str1[right]) return false;
        ++left;
        --right;
    }
    return true;
}

//  Function to Simulate Countdown Timer
void countdownTimer(int seconds) {
    while (seconds > 0) {
        std::cout << "Time left: " << seconds << " second(s)\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        --seconds;
    }
    std::cout << "Time's up!\n";
}

//  Function to Simulate System Status
void systemStatus() {
    const std::string statuses[] = {"Online", "Offline", "Busy", "Idle"};
    srand(time(0));
    std::cout << "System Status: " << statuses[rand() % 4] << "\n";
}

//  Function to Perform Character Frequency Analysis
std::map<char, int> charFrequency(const std::string &text) {
    std::map<char, int> frequency;
    for (char c : text) {
        ++frequency[c];
    }
    return frequency;
}

//  Function to Simulate Loading Screen
void simulateLoading1() {
    std::cout << "Loading";
    for (int i = 0; i < 10; ++i) {
        std::cout << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << " Done!\n";
}

//  Function to Generate Random Username
std::string generateRandomUsername() {
    const std::string adjectives[] = {"Quick", "Lazy", "Clever", "Brave"};
    const std::string nouns[] = {"Fox", "Bear", "Eagle", "Shark"};
    srand(time(0));
    return adjectives[rand() % 4] + nouns[rand() % 4] + std::to_string(rand() % 100);
}

//  Function to Calculate BMI
double calculateBMI(double weight, double height) {
    return weight / (height * height);
}

//  Function to Simulate Alarm Clock
void setAlarm(int seconds) {
    std::cout << "Alarm set for " << seconds << " seconds.\n";
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::cout << "ALARM! Time's up!\n";
}

//  Function to Print Pyramid Pattern
void printPyramid(int height) {
    for (int i = 1; i <= height; ++i) {
        for (int j = 1; j <= height - i; ++j) std::cout << " ";
        for (int k = 1; k <= 2 * i - 1; ++k) std::cout << "*";
        std::cout << "\n";
    }
}
//  Function to Simulate Login Attempt
bool attemptLogin(const std::string &username, const std::string &password) {
    const std::string storedUsername = "admin";
    const std::string storedPassword = "password123";
    if (username == storedUsername && password == storedPassword) {
        std::cout << "Login successful.\n";
        return true;
    } else {
        std::cout << "Login failed. Please try again.\n";
        return false;
    }
}

//  Function to Perform Matrix Multiplication
std::vector<std::vector<int>> matrixMultiplication1(
    const std::vector<std::vector<int>> &matrixA,
    const std::vector<std::vector<int>> &matrixB) {

    int rowsA = matrixA.size();
    int colsA = matrixA[0].size();
    int colsB = matrixB[0].size();
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    return result;
}

//  Function to Simulate a To-Do List
void simulateToDoList() {
    std::vector<std::string> tasks = {"Complete assignment", "Attend meeting", "Review code"};
    std::cout << "Your To-Do List:\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << i + 1 << ". " << tasks[i] << "\n";
    }
}

//  Function to Simulate Calendar View
void displayCalendar(int year, int month) {
    std::cout << "Calendar for " << month << "/" << year << ":\n";
    std::cout << "Sun Mon Tue Wed Thu Fri Sat\n";
    for (int i = 1; i <= 31; ++i) {
        std::cout << i << (i < 10 ? "   " : "  ");
        if (i % 7 == 0) std::cout << "\n";
    }
    std::cout << "\n";
}

//  Function to Generate Random Password
std::string generateRandomPassword(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    std::string password;
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        password += characters[rand() % characters.size()];
    }
    return password;
}

//  Function to Count Words in a String
int countWords1(const std::string &text) {
    int wordCount = 0;
    bool inWord = false;

    for (char c : text) {
        if (isspace(c)) {
            if (inWord) {
                wordCount++;
                inWord = false;
            }
        } else {
            inWord = true;
        }
    }

    if (inWord) wordCount++;  // Count the last word if it exists.
    return wordCount;
}

//  Function to Simulate Currency Converter
double convertCurrency(double amount, const std::string &fromCurrency, const std::string &toCurrency) {
    double conversionRate = 1.0; //  conversion rate
    if (fromCurrency == "USD" && toCurrency == "EUR") conversionRate = 0.85;
    if (fromCurrency == "EUR" && toCurrency == "USD") conversionRate = 1.18;
    return amount * conversionRate;
}

//  Function to Calculate Average of an Array
double calculateAverage1(const std::vector<int> &numbers) {
    int sum = 0;
    for (int num : numbers) {
        sum += num;
    }
    return static_cast<double>(sum) / numbers.size();
}

//  Function to Simulate a Number Guessing Game
void numberGuessingGame() {
    srand(time(0));
    int secretNumber = rand() % 100 + 1;
    int guess;
    std::cout << "Guess the secret number between 1 and 100:\n";

    while (true) {
        std::cin >> guess;
        if (guess < secretNumber) {
            std::cout << "Too low. Try again.\n";
        } else if (guess > secretNumber) {
            std::cout << "Too high. Try again.\n";
        } else {
            std::cout << "Congratulations! You've guessed the number.\n";
            break;
        }
    }
}

//  Function to Calculate Sum of Digits
int sumOfDigits(int number) {
    int sum = 0;
    while (number > 0) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

//  Function to Simulate Tic-Tac-Toe Game
void playTicTacToe() {
    std::vector<std::vector<char>> board(3, std::vector<char>(3, ' '));
    int moves = 0;
    char currentPlayer = 'X';

    while (moves < 9) {
        std::cout << "Player " << currentPlayer << "'s turn. Enter row and column (0-2):\n";
        int row, col;
        std::cin >> row >> col;

        if (board[row][col] == ' ') {
            board[row][col] = currentPlayer;
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            moves++;
        } else {
            std::cout << "Cell already occupied. Try again.\n";
        }

        for (const auto &row : board) {
            for (char cell : row) {
                std::cout << (cell == ' ' ? '.' : cell) << " ";
            }
            std::cout << "\n";
        }
    }

    std::cout << "Game Over. No winner!\n";
}
//  Function to Simulate Weather Forecast
void simulateWeatherForecast() {
    std::cout << "Today's Weather Forecast:\n";
    const std::string weatherTypes[] = {"Sunny", "Cloudy", "Rainy", "Stormy", "Snowy"};
    srand(time(0));

    for (int i = 0; i < 7; ++i) {
        int weatherIndex = rand() % 5;
        std::cout << "Day " << i + 1 << ": " << weatherTypes[weatherIndex] << "\n";
    }
}

//  Function to Simulate a Shopping Cart
void shoppingCartSimulation() {
    std::vector<std::pair<std::string, double>> cart;
    cart.push_back({"Apples", 3.99});
    cart.push_back({"Milk", 2.49});
    cart.push_back({"Bread", 1.99});

    std::cout << "Items in your cart:\n";
    double total = 0.0;

    for (const auto &item : cart) {
        std::cout << "- " << item.first << ": $" << item.second << "\n";
        total += item.second;
    }

    std::cout << "Total Price: $" << total << "\n";
}

//  Function to Generate Prime Numbers
std::vector<int> generatePrimes1(int limit) {
    std::vector<int> primes;
    for (int num = 2; num <= limit; ++num) {
        bool isPrime = true;
        for (int div = 2; div <= sqrt(num); ++div) {
            if (num % div == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(num);
        }
    }
    return primes;
}

//  Function to Check Leap Year
bool isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return true;
    }
    return false;
}

//  Function to Simulate Bank Account
void bankAccountSimulation() {
    double balance = 1000.0;
    std::cout << "Initial Balance: $" << balance << "\n";

    balance += 200.0;  // Deposit
    std::cout << "Deposited $200. New Balance: $" << balance << "\n";

    balance -= 150.0;  // Withdrawal
    std::cout << "Withdrew $150. New Balance: $" << balance << "\n";

    balance *= 1.02;  // Interest
    std::cout << "Added 2% interest. New Balance: $" << balance << "\n";
}

//  Function to Calculate Fibonacci Sequence
std::vector<int> calculateFibonacci(int terms) {
    std::vector<int> fibonacci;
    if (terms >= 1) fibonacci.push_back(0);
    if (terms >= 2) fibonacci.push_back(1);

    for (int i = 2; i < terms; ++i) {
        fibonacci.push_back(fibonacci[i - 1] + fibonacci[i - 2]);
    }

    return fibonacci;
}

//  Function to Simulate Quiz Game
void quizGame() {
    std::vector<std::pair<std::string, std::string>> questions = {
        {"What is the capital of France?", "Paris"},
        {"What is 5 + 3?", "8"},
        {"Who wrote 'Romeo and Juliet'?", "Shakespeare"}
    };

    int score = 0;
    std::string answer;
    for (const auto &question : questions) {
        std::cout << question.first << "\n";
        std::getline(std::cin, answer);

        if (answer == question.second) {
            std::cout << "Correct!\n";
            score++;
        } else {
            std::cout << "Wrong. The correct answer is: " << question.second << "\n";
        }
    }

    std::cout << "Your final score is: " << score << "/" << questions.size() << "\n";
}

//  Function to Simulate File Writing
void writeToFile(const std::string &filename, const std::string &content) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << content;
        std::cout << "File written successfully.\n";
    } else {
        std::cout << "Failed to open the file.\n";
    }
    outFile.close();
}

//  Function to Simulate File Reading
std::string readFromFile(const std::string &filename) {
    std::ifstream inFile(filename);
    std::string content, line;
    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            content += line + "\n";
        }
        inFile.close();
    } else {
        std::cout << "Failed to open the file.\n";
    }
    return content;
}

//  Function to Simulate Progress Tracker
void progressTracker(const std::string &taskName, int steps) {
    std::cout << "Tracking progress for: " << taskName << "\n";
    for (int i = 1; i <= steps; ++i) {
        std::cout << "Step " << i << "/" << steps << " completed.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "All steps completed for " << taskName << ".\n";
}

//  Function to Perform Bubble Sort
void bubbleSort1(std::vector<int> &arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        for (size_t j = 0; j < arr.size() - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

//  Function to Perform Selection Sort
void selectionSort(std::vector<int> &arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        std::swap(arr[i], arr[minIndex]);
    }
}

//  Function to Calculate Factorial
long long factorial1(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

//  Function to Print Pascal's Triangle
void printPascalsTriangle(int rows) {
    for (int i = 0; i < rows; ++i) {
        int value = 1;
        for (int j = 0; j <= i; ++j) {
            std::cout << value << " ";
            value = value * (i - j) / (j + 1);
        }
        std::cout << "\n";
    }
}

//  Function to Simulate Stopwatch
void simulateStopwatch1(int seconds) {
    for (int i = 0; i <= seconds; ++i) {
        std::cout << "Time elapsed: " << i << " seconds\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Stopwatch stopped at " << seconds << " seconds.\n";
}
//  Function to Simulate a Bank Transaction History
void bankTransactionHistory() {
    std::vector<std::pair<std::string, double>> transactions = {
        {"Deposit", 200.0},
        {"Withdrawal", -50.0},
        {"Deposit", 100.0},
        {"Withdrawal", -30.0},
        {"Deposit", 500.0}
    };

    std::cout << "Transaction History:\n";
    double balance = 0.0;
    for (const auto &transaction : transactions) {
        balance += transaction.second;
        std::cout << transaction.first << ": $" << transaction.second << ", Current Balance: $" << balance << "\n";
    }
}

//  Function to Simulate a Simple Calculator
double simpleCalculator(double num1, double num2, char operation) {
    double result = 0.0;
    switch (operation) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                std::cout << "Error: Division by zero.\n";
            }
            break;
        default:
            std::cout << "Invalid operation.\n";
            break;
    }
    return result;
}

//  Function to Generate Random String
std::string generateRandomString1(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string randomString;
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        randomString += characters[rand() % characters.size()];
    }
    return randomString;
}

//  Function to Simulate Dice Roll
int rollDice1() {
    srand(time(0));
    return rand() % 6 + 1;
}

//  Function to Perform Linear Search
int linearSearch(const std::vector<int> &arr, int target) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;  // Target not found
}

//  Function to Perform Binary Search (on a sorted array)
int binarySearch1(const std::vector<int> &arr, int target) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;  // Target not found
}

//  Function to Print Multiplication Table
void printMultiplicationTable(int num, int limit) {
    for (int i = 1; i <= limit; ++i) {
        std::cout << num << " x " << i << " = " << num * i << "\n";
    }
}

//  Function to Simulate an ATM Transaction
void atmTransactionSimulation() {
    double balance = 1000.0;
    int option = 0;
    do {
        std::cout << "ATM Menu: \n";
        std::cout << "1. Deposit\n";
        std::cout << "2. Withdraw\n";
        std::cout << "3. Check Balance\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> option;

        if (option == 1) {
            double depositAmount;
            std::cout << "Enter deposit amount: ";
            std::cin >> depositAmount;
            balance += depositAmount;
            std::cout << "Deposited $" << depositAmount << ". New balance: $" << balance << "\n";
        } else if (option == 2) {
            double withdrawalAmount;
            std::cout << "Enter withdrawal amount: ";
            std::cin >> withdrawalAmount;
            if (withdrawalAmount > balance) {
                std::cout << "Insufficient funds.\n";
            } else {
                balance -= withdrawalAmount;
                std::cout << "Withdrew $" << withdrawalAmount << ". New balance: $" << balance << "\n";
            }
        } else if (option == 3) {
            std::cout << "Current balance: $" << balance << "\n";
        } else if (option == 4) {
            std::cout << "Exiting ATM. Goodbye!\n";
        } else {
            std::cout << "Invalid option. Please try again.\n";
        }
    } while (option != 4);
}

//  Function to Simulate Tic-Tac-Toe Game
void playTicTacToe1() {
    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char currentPlayer = 'X';
    int moves = 0;

    while (moves < 9) {
        std::cout << "Player " << currentPlayer << "'s turn.\n";
        std::cout << "Enter row (0-2) and column (0-2): ";
        int row, col;
        std::cin >> row >> col;

        if (board[row][col] == ' ') {
            board[row][col] = currentPlayer;
            moves++;
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        } else {
            std::cout << "Cell already taken. Try again.\n";
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << board[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    std::cout << "Game over. It's a draw!\n";
}

//  Function to Calculate Sum of Squares of Numbers in a Vector
int sumOfSquares(const std::vector<int> &arr) {
    int sum = 0;
    for (int num : arr) {
        sum += num * num;
    }
    return sum;
}

//  Function to Check if a Number is Prime
bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

//  Function to Simulate a Traffic Light
void simulateTrafficLight1() {
    const std::string lights[] = {"Red", "Green", "Yellow"};
    int currentLight = 0;

    for (int i = 0; i < 10; ++i) {
        std::cout << "Traffic light is " << lights[currentLight] << ".\n";
        currentLight = (currentLight + 1) % 3;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

//  Function to Perform QuickSort
void quickSort(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (arr[j] < pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

//  Function to Calculate Simple Interest
double calculateSimpleInterest1(double principal, double rate, double time) {
    return (principal * rate * time) / 100;
}

//  Function to Simulate ATM Pin Entry
bool atmPinEntry() {
    const int correctPin = 1234;
    int enteredPin;

    for (int attempts = 0; attempts < 3; ++attempts) {
        std::cout << "Enter your PIN: ";
        std::cin >> enteredPin;

        if (enteredPin == correctPin) {
            std::cout << "Pin correct. Access granted.\n";
            return true;
        } else {
            std::cout << "Incorrect PIN. Try again.\n";
        }
    }
    std::cout << "Too many failed attempts. Access denied.\n";
    return false;
}

//  Function to Generate a Random Integer Between a Range
int generateRandomIntInRange(int min, int max) {
    srand(time(0));
    return rand() % (max - min + 1) + min;
}


// Function to hide password input
string getPasswordInput() {
    string password = "";
    char ch;
    while (true) {
        ch = _getch();  // Get a single character
        if (ch == 13) { // Enter key (ASCII 13)
            break;
        } else if (ch == 8) { // Backspace key (ASCII 8)
            if (!password.empty()) {
                cout << "\b \b";  // Erase the last character on the console
                password.pop_back();
            }
        } else {
            password += ch;
            cout << "*";  // Display asterisk for each typed character
        }
    }
    cout << endl;
    return password;
}

// Function to validate login based on the user type (patient/doctor)
bool validateLogin(const string& username, const string& password, const string& role, const string& filePath) {
    ifstream file(filePath);
    string line, user, pass, name, specialization, location, handledPatients;

    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return false;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, user, ',');
        getline(ss, pass, ',');
        getline(ss, name, ',');
        if (role == "doctor") {
            getline(ss, specialization, ',');
            getline(ss, location, ',');
            getline(ss, handledPatients, ',');
        }

        // Trim spaces
        user = trim(user);
        pass = trim(pass);
        name = trim(name);

        // For doctor login, check if the username and password match
        if (user == username && pass == password) {
            return true;
        }
    }

    return false;
}

// Function to prompt for login credentials and handle user type
void userLogin() {
    int option;
    cout << "Choose an option:\n";
    cout << "1. Patient Login\n";
    cout << "2. Doctor Login\n";
    cout << "3. Exit\n";
    cin >> option;
    cin.ignore(); // To clear the input buffer

    string username, password;

    switch (option) {
        case 1:
            cout << "Enter Patient Username: ";
            cin >> username;
            cout << "Enter Password: ";
            password = getPasswordInput(); // Use the hidden password input function
            if (validateLogin(username, password, "patient", "E:\\DAA PROJECT\\patient_data.csv")) {
                cout << "Patient login successful!\n";
            } else {
                cout << "Invalid username or password!\n";
            }
            break;

        case 2:
            cout << "Enter Doctor Username: ";
            cin >> username;
            cout << "Enter Password: ";
            password = getPasswordInput(); // Use the hidden password input function
            if (validateLogin(username, password, "doctor", "C:\\Users\\chitn\\Downloads\\doctors.csv")) {
                cout << "Doctor login successful!\n";
            } else {
                cout << "Invalid username or password!\n";
            }
            break;

        case 3:
            cout << "Exiting...\n";
            exit(0);

        default:
            cout << "Invalid option selected.\n";
    }
}

int module3() {
    vector<int> shortestPath; // Changed from string to vector<int>
    // Graph representation: A simple example with some nodes
    vector<string> currentLocations = {"Downtown", "Suburb A", "City Center", "Old Town", "Industrial Area", "Park", "Airport", "Beach", "University", "Shopping Mall", "Mountain View"};
    vector<string> destinations = {"Hospital A", "Clinic B", "Hospital C", "Clinic D", "Hospital D", "Clinic E", "Hospital E", "Clinic F", "Hospital F", "Clinic G"};

    int totalNodes = currentLocations.size() + destinations.size();
    vector<vector<int>> graph(totalNodes, vector<int>(totalNodes, INF));

    // Example graph initialization (Distance in km)
    graph = {
        {0, 15, 30, 45, 20, 25, 10, 50, 60, 70, 80, 100, 15, 10, 25, 20, 40},

        {15, 0, 10, 35, 25, 0, 30, 60, 20, 15, 25, 50, 10, 5, 20, 15, 35},

        {30, 10, 0, 50, 40, 30, 40, 30, 15, 25, 35, 55, 20, 15, 30, 25, 50},

        {45, 35, 50, 0, 55, 45, 25, 50, 10, 20, 40, 60, 30, 25, 40, 0, 35},

        {20, 25, 40, 55, 0, 30, 15, 45, 10, 15, 25, 40, 50, 40, 45, 30, 0},

        {25, 0, 30, 45, 30, 0, 30, 45, 50, 60, 70, 80, 0, 25, 20, 15, 30},

        {10, 30, 40, 25, 15, 30, 0, 15, 35, 50, 45, 60, 5, 5, 50, 40, 60},

        {50, 60, 30, 25, 40, 30, 15, 0, 20, 30, 40, 50, 20, 40, 60, 30, 0},

        {60, 20, 15, 10, 10, 30, 35, 25, 0, 10, 15, 25, 20, 25, 35, 20, 10},

        {70, 15, 25, 30, 40, 15, 20, 15, 10, 0, 30, 45, 10, 20, 35, 25, 20},

        {80, 25, 35, 40, 50, 20, 25, 25, 15, 30, 0, 15, 25, 45, 55, 60, 30},

        {15, 10, 20, 25, 15, 17, 45, 30, 41, 56, 0, 10, 30, 35, 40, 25, 10},

        {10, 5, 15, 20, 10, 0, 35, 25, 25, 10, 25, 10, 10, 5, 20, 35, 15},

        {25, 20, 30, 35, 25, 30, 40, 35, 50, 25, 20, 35, 20, 15, 25, 45, 10},

        {30, 25, 35, 40, 30, 45, 50, 35, 45, 25, 30, 50, 25, 10, 30, 35, 25},

        {20, 15, 25, 30, 25, 30, 25, 30, 30, 25, 35, 40, 50, 45, 30, 10, 0},

        {35, 30, 40, 45, 35, 30, 30, 35, 40, 45, 40, 35, 20, 30, 50, 40, 20},

        {25, 20, 30, 35, 25, 30, 35, 40, 40, 30, 35, 40, 25, 15, 40, 45, 30},

        {40, 35, 45, 50, 40, 30, 45, 50, 60, 40, 45, 55, 35, 30, 35, 45, 25},

        {45, 40, 50, 55, 45, 50, 60, 60, 55, 45, 50, 60, 35, 30, 35, 45, 25},

        {50, 45, 55, 60, 50, 60, 60, 60, 60, 55, 50, 55, 50, 40, 45, 50, 0}

    };

    while (true) {
        userLogin();

        // After successful login, you can integrate Dijkstra's functionality
        int currentLocation, destination;
        string mode;

        cout << "\nChoose your current location from the list of cities/areas:\n";
        for (int i = 0; i < currentLocations.size(); i++) {
            cout << i + 1 << ". " << currentLocations[i] << endl;
        }
        cin >> currentLocation;

        cout << "\nChoose your destination from the list of hospitals/clinics:\n";
        for (int i = 0; i < destinations.size(); i++) {
            cout << i + 1 << ". " << destinations[i] << endl;
        }
        cin >> destination;

        cout << "\nChoose mode of transportation:\n";
        cout << "1. Car\n2. Bike\n3. Walking\n";
        int modeChoice;
        cin >> modeChoice;

        float speed;
        if (modeChoice == 1) {
            speed = speedCar;
            mode = "Car";
        } else if (modeChoice == 2) {
            speed = speedBike;
            mode = "Bike";
        } else {
            speed = speedWalk;
            mode = "Walking";
        }

        float travelTime;
        pair<pair<int, float>, vector<int>> result = dijkstra(graph, currentLocation - 1, destination - 1, totalNodes, speed);
        travelTime = result.first.second * 60; // Convert hours to minutes
        shortestPath = result.second;

        cout << "Mode of transportation: " << mode << endl;
        cout << "Shortest Path distance: " << result.first.first << " km\n";
        cout << "Estimated Time: " << travelTime << " minutes\n\n";
        cout << "Path: ";

        for (size_t i = 0; i < shortestPath.size(); i++) {
            if (shortestPath[i] < currentLocations.size()) {
                cout << currentLocations[shortestPath[i]];
            } else {
                cout << destinations[shortestPath[i] - currentLocations.size()];
            }

            if (i < shortestPath.size()) {
                cout << " -> ";
            }
        }
        cout <<destinations[destination-1];
        cout << endl;
    }

    return 0;
}

//module 4
using namespace std;

// Brute force string matching algorithm
bool bruteForceStringMatch(const string& text, const string& pattern) {
    int textLength = text.length();
    int patternLength = pattern.length();

    for (int i = 0; i <= textLength - patternLength; i++) {
        int j;
        for (j = 0; j < patternLength; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == patternLength) {
            return true; // Pattern found
        }
    }
    return false; // Pattern not found
}

void giveAdvice(const string& symptom) {
    if (bruteForceStringMatch(symptom, "fever")) {
        cout << "Prescription: Paracetamol 500mg\n";
        cout << "Precautions: Rest well, stay hydrated, and monitor your temperature.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "cold")) {
        cout << "Prescription: Antihistamine (Cetirizine)\n";
        cout << "Precautions: Avoid cold drinks, stay warm, and drink hot fluids.\n";
        cout << "Quantity: 1 tablet at bedtime.\n";
    }

     else if (bruteForceStringMatch(symptom, "headache")) {
        cout << "Prescription: Ibuprofen 400mg\n";
        cout << "Precautions: Avoid stress, rest in a quiet environment.\n";
        cout << "Quantity: 1 tablet every 8 hours, as needed.\n";
    }
     else if (bruteForceStringMatch(symptom, "migraine")) {

        cout << "Prescription: Sumatriptan 50mg\n";
        cout << "Precautions: Avoid bright lights and loud noises. Rest in a dark, quiet room.\n";
        cout << "Quantity: 1 tablet at the onset of migraine, repeat in 2 hours if needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "dizziness")) {
        cout << "Prescription: Meclizine 25mg\n";
        cout << "Precautions: Avoid sudden head movements, and rest. Stay hydrated.\n";
        cout << "Quantity: 1 tablet every 24 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "fatigue")) {
        cout << "Prescription: Vitamin B12 supplement\n";
        cout << "Precautions: Ensure proper sleep, eat a balanced diet, and manage stress.\n";
        cout << "Quantity: 1 tablet per day.\n";
    }

     else if (bruteForceStringMatch(symptom, "insomnia")) {
        cout << "Prescription: Melatonin 3mg\n";
        cout << "Precautions: Avoid screen time before bed, maintain a regular sleep schedule.\n";
        cout << "Quantity: 1 tablet before bedtime.\n";
    }

     else if (bruteForceStringMatch(symptom, "sleep apnea")) {
        cout << "Prescription: CPAP therapy\n";
        cout << "Precautions: Use CPAP machine every night, avoid sleeping on your back.\n";
        cout << "Quantity: Consult with a healthcare provider for machine settings.\n";
    }

    else if (bruteForceStringMatch(symptom, "snoring")) {
        cout << "Prescription: Nasal strips\n";
        cout << "Precautions: Sleep on your side, avoid alcohol before bedtime.\n";
        cout << "Quantity: Use one nasal strip before sleep.\n";
    }

     else if (bruteForceStringMatch(symptom, "restless leg syndrome")) {
        cout << "Prescription: Ropinirole 0.25mg\n";
        cout << "Precautions: Avoid caffeine, exercise regularly, and stretch your legs.\n";
        cout << "Quantity: 1 tablet before bedtime.\n";
    }

     else if (bruteForceStringMatch(symptom, "muscle cramps")) {
        cout << "Prescription: Magnesium supplement\n";
        cout << "Precautions: Stretch before exercise, stay hydrated.\n";
        cout << "Quantity: 1 tablet per day.\n";
    }

     else if (bruteForceStringMatch(symptom, "general weakness")) {
        cout << "Prescription: Multivitamin supplement\n";
        cout << "Precautions: Eat a balanced diet and rest as needed.\n";
        cout << "Quantity: 1 tablet per day.\n";
    }

     else if (bruteForceStringMatch(symptom, "stomach ache")) {
        cout << "Prescription: Antacids (like Tums)\n";
        cout << "Precautions: Avoid spicy food, eat smaller meals.\n";
        cout << "Quantity: 1-2 tablets as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "indigestion")) {
        cout << "Prescription: Antacids (like Maalox)\n";
        cout << "Precautions: Avoid fatty foods and eat smaller, more frequent meals.\n";
        cout << "Quantity: 1 tablet after meals.\n";
    }

    else if (bruteForceStringMatch(symptom, "heartburn")) {
        cout << "Prescription: Omeprazole 20mg\n";
        cout << "Precautions: Avoid caffeine and acidic foods, eat smaller meals.\n";
        cout << "Quantity: 1 tablet per day before breakfast.\n";
    }

    else if (bruteForceStringMatch(symptom, "acid reflux")) {
        cout << "Prescription: Antacid (Tums) or H2 blocker (Ranitidine)\n";
        cout << "Precautions: Avoid lying down immediately after eating, eat smaller meals.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "constipation")) {
        cout << "Prescription: Fiber supplement (like Metamucil)\n";
        cout << "Precautions: Drink plenty of water and eat fiber-rich foods.\n";
        cout << "Quantity: 1 tablespoon in the morning.\n";
    }

     else if (bruteForceStringMatch(symptom, "diarrhea")) {
        cout << "Prescription: Loperamide 2mg\n";
        cout << "Precautions: Stay hydrated, avoid dairy products.\n";
        cout << "Quantity: 2 tablets after the first loose stool, 1 tablet after each subsequent stool.\n";
    }

    else if (bruteForceStringMatch(symptom, "gas")) {
        cout << "Prescription: Simethicone 125mg\n";
        cout << "Precautions: Avoid carbonated drinks and foods that cause gas.\n";
        cout << "Quantity: 1-2 tablets after meals.\n";
    }

    else if (bruteForceStringMatch(symptom, "bloating")) {
        cout << "Prescription: Activated charcoal\n";
        cout << "Precautions: Avoid foods that cause bloating, eat smaller meals.\n";
        cout << "Quantity: 1-2 tablets after meals.\n";
    }

    else if (bruteForceStringMatch(symptom, "nausea")) {
        cout << "Prescription: Ondansetron 4mg\n";
        cout << "Precautions: Avoid strong odors, take small sips of water.\n";
        cout << "Quantity: 1 tablet every 8 hours as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "vomiting")) {
        cout << "Prescription: Metoclopramide 10mg\n";
        cout << "Precautions: Stay hydrated, avoid strong smells and greasy foods.\n";
        cout << "Quantity: 1 tablet every 6 hours as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "food poisoning")) {
        cout << "Prescription: Oral rehydration solution (ORS)\n";
        cout << "Precautions: Drink plenty of fluids, avoid solid foods initially.\n";
        cout << "Quantity: Drink ORS as directed on packaging.\n";
    }

    else if (bruteForceStringMatch(symptom, "hiccups")) {
        cout << "Prescription: Baclofen 10mg\n";
        cout << "Precautions: Drink a glass of water, hold your breath for a few seconds.\n";
        cout << "Quantity: 1 tablet every 8 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "irritable bowel syndrome (IBS)")) {
        cout << "Prescription: Fiber supplement or Loperamide\n";
        cout << "Precautions: Avoid trigger foods, manage stress, and exercise regularly.\n";
        cout << "Quantity: 1-2 tablets daily, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor stomach ulcers")) {
        cout << "Prescription: Antacid or Proton pump inhibitor (Omeprazole)\n";
        cout << "Precautions: Avoid spicy, acidic foods, and alcohol.\n";
        cout << "Quantity: 1 tablet per day before meals.\n";
    }

    else if (bruteForceStringMatch(symptom, "abdominal discomfort")) {
        cout << "Prescription: Antacids or antispasmodic (Dicyclomine)\n";
        cout << "Precautions: Avoid overeating and high-fat meals.\n";
        cout << "Quantity: 1 tablet 3 times a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "common cold")) {
        cout << "Prescription: Decongestant (Pseudoephedrine)\n";
        cout << "Precautions: Rest well, stay hydrated.\n";
        cout << "Quantity: 1 tablet every 4-6 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "sore throat")) {
        cout << "Prescription: Lozenges (Menthol or Eucalyptus)\n";
        cout << "Precautions: Gargle with warm salt water, rest your voice.\n";
        cout << "Quantity: 1 lozenge every 2-3 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "cough")) {
        cout << "Prescription: Cough syrup (Dextromethorphan)\n";
        cout << "Precautions: Drink warm liquids to soothe throat.\n";
        cout << "Quantity: 10 ml every 4-6 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "nasal congestion")) {
        cout << "Prescription: Nasal decongestant (Oxymetazoline)\n";
        cout << "Precautions: Use for no more than 3 days to avoid rebound congestion.\n";
        cout << "Quantity: 2-3 sprays in each nostril every 12 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "runny nose")) {
        cout << "Prescription: Antihistamine (Loratadine)\n";
        cout << "Precautions: Avoid alcohol, and take before bed.\n";
        cout << "Quantity: 1 tablet once daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "postnasal drip")) {
        cout << "Prescription: Nasal spray (Fluticasone)\n";
        cout << "Precautions: Avoid irritants like smoke and dust.\n";
        cout << "Quantity: 1 spray in each nostril once daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "sinusitis")) {
        cout << "Prescription: Antibiotics (Amoxicillin) or decongestants\n";
        cout << "Precautions: Stay hydrated, and avoid allergens.\n";
        cout << "Quantity: 1 tablet every 8 hours for 7-10 days.\n";
    }

    else if (bruteForceStringMatch(symptom, "seasonal allergies")) {
        cout << "Prescription: Antihistamine (Cetirizine)\n";
        cout << "Precautions: Stay indoors during high pollen seasons.\n";
        cout << "Quantity: 1 tablet daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "sneezing")) {
        cout << "Prescription: Antihistamine (Loratadine)\n";
        cout << "Precautions: Avoid allergens, and stay hydrated.\n";
        cout << "Quantity: 1 tablet once daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor asthma attacks")) {
        cout << "Prescription: Short-acting bronchodilator (Albuterol inhaler)\n";
        cout << "Precautions: Use inhaler as directed, avoid triggers.\n";
        cout << "Quantity: 1-2 puffs every 4-6 hours as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "dry throat")) {
        cout << "Prescription: Lozenges (Honey or Menthol-based)\n";
        cout << "Precautions: Drink plenty of water, avoid smoking.\n";
        cout << "Quantity: 1 lozenge every 2-3 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "hoarseness")) {
        cout << "Prescription: Throat spray (Chloraseptic)\n";
        cout << "Precautions: Rest your voice and drink warm liquids.\n";
        cout << "Quantity: Spray every 2-3 hours as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "shortness of breath (non-severe)")) {
        cout << "Prescription: Inhaler (Albuterol)\n";
        cout << "Precautions: Rest and avoid exertion.\n";
        cout << "Quantity: 1-2 puffs every 4-6 hours as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "chronic cough (non-serious)")) {
        cout << "Prescription: Cough syrup (Dextromethorphan)\n";
        cout << "Precautions: Avoid smoking, stay hydrated.\n";
        cout << "Quantity: 10 ml every 4-6 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild bronchitis")) {
        cout << "Prescription: Cough medicine (Guaifenesin)\n";
        cout << "Precautions: Rest well, drink warm fluids.\n";
        cout << "Quantity: 1 tablet every 4 hours.\n";
    }

     else if (bruteForceStringMatch(symptom, "acne")) {
        cout << "Prescription: Benzoyl peroxide gel\n";
        cout << "Precautions: Wash your face twice a day, avoid touching your face.\n";
        cout << "Quantity: Apply to affected areas once a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "pimples")) {
        cout << "Prescription: Salicylic acid solution\n";
        cout << "Precautions: Avoid squeezing pimples, keep skin clean.\n";
        cout << "Quantity: Apply 1-2 times a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "blackheads")) {
        cout << "Prescription: Salicylic acid cleanser\n";
        cout << "Precautions: Avoid harsh scrubs, keep skin moisturized.\n";
        cout << "Quantity: Wash face twice a day.\n";
    }

     else if (bruteForceStringMatch(symptom, "whiteheads")) {
        cout << "Prescription: Retinoid cream (Adapalene)\n";
        cout << "Precautions: Apply at night, wear sunscreen during the day.\n";
        cout << "Quantity: Apply a thin layer at bedtime.\n";
    }

     else if (bruteForceStringMatch(symptom, "eczema")) {
        cout << "Prescription: Hydrocortisone cream 1%\n";
        cout << "Precautions: Avoid scratching, keep skin moisturized.\n";
        cout << "Quantity: Apply to affected areas twice a day.\n";
    }
     else if (bruteForceStringMatch(symptom, "dry skin")) {
        cout << "Prescription: Moisturizing lotion (e.g., Eucerin)\n";
        cout << "Precautions: Avoid hot showers, use a gentle soap.\n";
        cout << "Quantity: Apply twice daily.\n";
    }

     else if (bruteForceStringMatch(symptom, "oily skin")) {
        cout << "Prescription: Oil-control face wash (Salicylic acid)\n";
        cout << "Precautions: Wash your face twice daily, avoid touching your face.\n";
        cout << "Quantity: Use morning and night.\n";
    }

     else if (bruteForceStringMatch(symptom, "itchy skin")) {
        cout << "Prescription: Antihistamine (Diphenhydramine)\n";
        cout << "Precautions: Avoid scratching the area, moisturize regularly.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "rashes")) {
        cout << "Prescription: Hydrocortisone cream 1%\n";
        cout << "Precautions: Apply to affected area twice daily, avoid irritating products.\n";
        cout << "Quantity: Apply 1-2 times a day.\n";
    }

     else if (bruteForceStringMatch(symptom, "sunburn")) {
        cout << "Prescription: Aloe Vera gel or Hydrocortisone cream\n";
        cout << "Precautions: Avoid sun exposure, drink plenty of water.\n";
        cout << "Quantity: Apply as needed for relief.\n";
    }

     else if (bruteForceStringMatch(symptom, "chapped lips")) {
        cout << "Prescription: Lip balm with SPF (e.g., Aquaphor)\n";
        cout << "Precautions: Avoid licking your lips, stay hydrated.\n";
        cout << "Quantity: Apply frequently throughout the day.\n";
    }

     else if (bruteForceStringMatch(symptom, "skin irritation")) {
        cout << "Prescription: Calamine lotion or hydrocortisone cream\n";
        cout << "Precautions: Avoid scratching and stay away from allergens.\n";
        cout << "Quantity: Apply 1-2 times a day.\n";
    }

     else if (bruteForceStringMatch(symptom, "minor cuts")) {
        cout << "Prescription: Antiseptic ointment (Neosporin)\n";
        cout << "Precautions: Keep the wound clean, cover with a sterile bandage.\n";
        cout << "Quantity: Apply to the cut after cleaning.\n";
    }

     else if (bruteForceStringMatch(symptom, "bruises")) {
        cout << "Prescription: Arnica gel\n";
        cout << "Precautions: Apply to the bruise area gently, avoid direct pressure.\n";
        cout << "Quantity: Apply 1-2 times a day.\n";
    }

     else if (bruteForceStringMatch(symptom, "fungal infections")) {
        cout << "Prescription: Antifungal cream (Clotrimazole)\n";
        cout << "Precautions: Keep the affected area clean and dry.\n";
        cout << "Quantity: Apply twice daily until symptoms improve.\n";
    }

    else if (bruteForceStringMatch(symptom, "athlete's foot")) {
        cout << "Prescription: Antifungal cream (Terbinafine)\n";
        cout << "Precautions: Keep feet dry and wear breathable socks.\n";
        cout << "Quantity: Apply to feet twice a day.\n";
    }

     else if (bruteForceStringMatch(symptom, "ringworm")) {
        cout << "Prescription: Antifungal cream (Miconazole)\n";
        cout << "Precautions: Avoid scratching and keep the affected area clean.\n";
        cout << "Quantity: Apply to the affected area twice daily.\n";
    }

     else if (bruteForceStringMatch(symptom, "dandruff")) {
        cout << "Prescription: Anti-dandruff shampoo (Zinc Pyrithione)\n";
        cout << "Precautions: Massage the shampoo into your scalp, leave it for a few minutes before rinsing.\n";
        cout << "Quantity: Use 2-3 times a week.\n";
    }

    else if (bruteForceStringMatch(symptom, "blisters")) {
        cout << "Prescription: Blister plaster or hydrocortisone cream\n";
        cout << "Precautions: Avoid bursting the blister, keep it covered.\n";
        cout << "Quantity: Apply blister plaster until healed.\n";
    }

     else if (bruteForceStringMatch(symptom, "warts")) {
        cout << "Prescription: Salicylic acid solution\n";
        cout << "Precautions: Apply to the wart daily, avoid healthy skin.\n";
        cout << "Quantity: Apply once a day.\n";
    }

     else if (bruteForceStringMatch(symptom, "toothache")) {
        cout << "Prescription: Over-the-counter pain reliever (Ibuprofen)\n";
        cout << "Precautions: Avoid hot or cold food, visit a dentist if pain persists.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "sensitivity")) {
        cout << "Prescription: Toothpaste for sensitive teeth (e.g., Sensodyne)\n";
        cout << "Precautions: Avoid acidic foods and drinks.\n";
        cout << "Quantity: Brush with sensitive toothpaste twice a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "bleeding gums")) {
        cout << "Prescription: Antibacterial mouthwash (Chlorhexidine)\n";
        cout << "Precautions: Avoid hard brushing, and visit a dentist.\n";
        cout << "Quantity: Use twice a day after brushing.\n";
    }

     else if (bruteForceStringMatch(symptom, "bad breath")) {
        cout << "Prescription: Mouthwash (Listerine) or chewing gum\n";
        cout << "Precautions: Maintain good oral hygiene, brush your tongue.\n";
        cout << "Quantity: Use mouthwash twice daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "mouth ulcers")) {
        cout << "Prescription: Topical corticosteroid gel (Hydrocortisone)\n";
        cout << "Precautions: Avoid acidic foods and avoid irritation.\n";
        cout << "Quantity: Apply gel to ulcers 2-3 times a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "dry mouth")) {
        cout << "Prescription: Artificial saliva spray\n";
        cout << "Precautions: Sip water frequently and avoid alcohol-based mouthwashes.\n";
        cout << "Quantity: Use as needed throughout the day.\n";
    }

     else if (bruteForceStringMatch(symptom, "chapped tongue")) {
        cout << "Prescription: Hydration and tongue moisturizer\n";
        cout << "Precautions: Drink water regularly and avoid spicy foods.\n";
        cout << "Quantity: Drink water frequently, use a tongue scraper.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor cavities")) {
        cout << "Prescription: Fluoride toothpaste\n";
        cout << "Precautions: Brush twice a day and reduce sugar intake.\n";
        cout << "Quantity: Brush 2-3 times a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "plaque buildup")) {
        cout << "Prescription: Plaque-removal toothpaste (e.g., Colgate Total)\n";
        cout << "Precautions: Floss regularly and use mouthwash.\n";
        cout << "Quantity: Brush twice a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "sore gums")) {
        cout << "Prescription: Warm salt water rinse\n";
        cout << "Precautions: Avoid acidic or spicy foods.\n";
        cout << "Quantity: Rinse 2-3 times a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "dry eyes")) {
        cout << "Prescription: Artificial tears (Lubricating eye drops)\n";
        cout << "Precautions: Avoid prolonged screen time, blink frequently.\n";
        cout << "Quantity: Use 2-3 times a day as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "eye strain")) {
        cout << "Prescription: Rest your eyes every 20 minutes, and use artificial tears\n";
        cout << "Precautions: Reduce screen time, follow the 20-20-20 rule.\n";
        cout << "Quantity: Use eye drops as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "watery eyes")) {
        cout << "Prescription: Antihistamine eye drops\n";
        cout << "Precautions: Avoid allergens, use a cold compress.\n";
        cout << "Quantity: Apply eye drops 2-3 times a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "red eyes")) {
        cout << "Prescription: Anti-inflammatory eye drops\n";
        cout << "Precautions: Avoid rubbing eyes and ensure proper sleep.\n";
        cout << "Quantity: Use as directed by your healthcare provider.\n";
    }

     else if (bruteForceStringMatch(symptom, "blurred vision (temporary)")) {
        cout << "Prescription: Rest your eyes, hydrate well\n";
        cout << "Precautions: Avoid prolonged screen time, ensure proper lighting.\n";
        cout << "Quantity: Rest and hydrate.\n";
    }

    else if (bruteForceStringMatch(symptom, "itchy eyes")) {
        cout << "Prescription: Antihistamine eye drops\n";
        cout << "Precautions: Avoid rubbing your eyes.\n";
        cout << "Quantity: Apply 1-2 drops every 4-6 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "dark circles")) {
        cout << "Prescription: Caffeine-based eye cream\n";
        cout << "Precautions: Get adequate sleep, reduce screen time.\n";
        cout << "Quantity: Apply under the eyes once daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "puffy eyes")) {
        cout << "Prescription: Cold compress or anti-puffiness eye cream\n";
        cout << "Precautions: Ensure good sleep and elevate your head during sleep.\n";
        cout << "Quantity: Apply 1-2 times daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "twitching eyelids")) {
        cout << "Prescription: Rest and reduce caffeine intake\n";
        cout << "Precautions: Ensure sufficient sleep, relax the eyes.\n";
        cout << "Quantity: Rest and reduce stress.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor infections")) {
        cout << "Prescription: Antiseptic ointment (Neosporin)\n";
        cout << "Precautions: Keep the area clean and dry.\n";
        cout << "Quantity: Apply 1-2 times daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "earache")) {
        cout << "Prescription: Pain reliever (Ibuprofen)\n";
        cout << "Precautions: Avoid inserting objects into your ear, consult a doctor if persistent.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "itchy ears")) {
        cout << "Prescription: Antihistamine (Diphenhydramine)\n";
        cout << "Precautions: Avoid inserting cotton swabs in your ears.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "blocked ears")) {
        cout << "Prescription: Ear drops (Cerumenolytics)\n";
        cout << "Precautions: Avoid inserting objects into the ear canal.\n";
        cout << "Quantity: Use as directed.\n";
    }

    else if (bruteForceStringMatch(symptom, "temporary hearing loss")) {
        cout << "Prescription: Consult a healthcare professional for further evaluation.\n";
        cout << "Precautions: Avoid loud noises.\n";
        cout << "Quantity: Consult a doctor.\n";
    }

    else if (bruteForceStringMatch(symptom, "ringing in ears (tinnitus)")) {
        cout << "Prescription: Consultation with an ENT specialist\n";
        cout << "Precautions: Avoid loud sounds and stress.\n";
        cout << "Quantity: Follow ENT specialist's guidance.\n";
    }

    else if (bruteForceStringMatch(symptom, "fluid in ear")) {
        cout << "Prescription: Ear drops for fluid removal\n";
        cout << "Precautions: Avoid using cotton swabs.\n";
        cout << "Quantity: Use ear drops as directed.\n";
    } else if (bruteForceStringMatch(symptom, "earwax buildup")) {
        cout << "Prescription: Ear drops for earwax removal\n";
        cout << "Precautions: Avoid inserting objects into your ear canal.\n";
        cout << "Quantity: Use as directed.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor ear infections")) {
        cout << "Prescription: Antibiotic ear drops\n";
        cout << "Precautions: Avoid water in your ears.\n";
        cout << "Quantity: Apply as directed.\n";
    }

    else if (bruteForceStringMatch(symptom, "stress")) {
        cout << "Prescription: Relaxation techniques and stress management\n";
        cout << "Precautions: Practice mindfulness, avoid caffeine.\n";
        cout << "Quantity: Engage in daily relaxation activities.\n";
    }

    else if (bruteForceStringMatch(symptom, "anxiety")) {
        cout << "Prescription: Anxiety-relieving medications (e.g., SSRIs)\n";
        cout << "Precautions: Practice relaxation techniques, avoid stress triggers.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild depression")) {
        cout << "Prescription: Antidepressants (SSRIs like Fluoxetine)\n";
        cout << "Precautions: Seek therapy in conjunction with medication, maintain a healthy routine.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "mood swings")) {
        cout << "Prescription: Mood stabilizers (e.g., Lamotrigine)\n";
        cout << "Precautions: Maintain a regular sleep schedule, manage stress.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "forgetfulness")) {
        cout << "Prescription: Ginkgo Biloba or Omega-3 supplements\n";
        cout << "Precautions: Stay mentally active with puzzles or reading, improve sleep hygiene.\n";
        cout << "Quantity: Follow the supplement�s recommended dosage.\n";
    }

    else if (bruteForceStringMatch(symptom, "irritability")) {
        cout << "Prescription: Magnesium supplements or Relaxation techniques\n";
        cout << "Precautions: Avoid stress, practice mindfulness and relaxation.\n";
        cout << "Quantity: Follow the supplement�s recommended dosage or practice relaxation regularly.\n";
    }

     else if (bruteForceStringMatch(symptom, "low self-esteem")) {
        cout << "Prescription: Cognitive Behavioral Therapy (CBT)\n";
        cout << "Precautions: Engage in positive self-talk, try to focus on accomplishments.\n";
        cout << "Quantity: Consult a therapist for regular sessions.\n";
    }

     else if (bruteForceStringMatch(symptom, "lack of concentration")) {
        cout << "Prescription: Caffeine (if no contraindications), or focus-enhancing supplements (e.g., Bacopa Monnieri)\n";
        cout << "Precautions: Limit distractions, practice mindfulness techniques.\n";
        cout << "Quantity: 1-2 cups of coffee or follow the supplement's recommended dosage.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild social anxiety")) {
        cout << "Prescription: Anxiolytics (e.g., Buspirone)\n";
        cout << "Precautions: Practice exposure therapy and relaxation techniques.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "burnout")) {
        cout << "Prescription: Stress management techniques (e.g., yoga, deep breathing)\n";
        cout << "Precautions: Take regular breaks, ensure a healthy work-life balance.\n";
        cout << "Quantity: Incorporate relaxation exercises daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "backache")) {
        cout << "Prescription: NSAIDs (e.g., Ibuprofen) or muscle relaxants\n";
        cout << "Precautions: Maintain good posture, avoid heavy lifting.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "neck pain")) {
        cout << "Prescription: NSAIDs (e.g., Ibuprofen) or warm compresses\n";
        cout << "Precautions: Stretch your neck gently, avoid sudden movements.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "shoulder pain")) {
        cout << "Prescription: NSAIDs (e.g., Ibuprofen) or physical therapy\n";
        cout << "Precautions: Rest and avoid heavy shoulder use.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "knee pain")) {
        cout << "Prescription: NSAIDs (e.g., Ibuprofen) or topical pain relief (e.g., Voltaren gel)\n";
        cout << "Precautions: Avoid overexertion, elevate the knee when resting.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "wrist pain")) {
        cout << "Prescription: Wrist brace or NSAIDs (e.g., Ibuprofen)\n";
        cout << "Precautions: Avoid repetitive motion, take breaks frequently.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "joint stiffness")) {
        cout << "Prescription: Pain relievers (e.g., Ibuprofen) or joint supplements (e.g., Glucosamine)\n";
        cout << "Precautions: Regular exercise, avoid overuse of the joint.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "arthritis (mild)")) {
        cout << "Prescription: NSAIDs or joint supplements (e.g., Glucosamine)\n";
        cout << "Precautions: Gentle stretching and low-impact exercises.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "muscle soreness")) {
        cout << "Prescription: Muscle relaxants or NSAIDs\n";
        cout << "Precautions: Apply heat or ice to sore muscles, rest.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "muscle spasms")) {
        cout << "Prescription: Muscle relaxants (e.g., Cyclobenzaprine)\n";
        cout << "Precautions: Rest and avoid strenuous activities.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "frozen shoulder")) {
        cout << "Prescription: Physical therapy or NSAIDs\n";
        cout << "Precautions: Gradual stretching, avoid sudden shoulder movements.\n";
        cout << "Quantity: Follow a physical therapy schedule or take NSAIDs as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "menstrual cramps")) {
        cout << "Prescription: NSAIDs (e.g., Ibuprofen) or heating pad\n";
        cout << "Precautions: Rest and maintain hydration.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "irregular periods")) {
        cout << "Prescription: Hormonal therapy or birth control pills\n";
        cout << "Precautions: Maintain a balanced diet and regular exercise.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "vaginal dryness")) {
        cout << "Prescription: Vaginal moisturizers or estrogen creams\n";
        cout << "Precautions: Avoid irritants, use lubricants during intercourse.\n";
        cout << "Quantity: Use as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "yeast infections")) {
        cout << "Prescription: Antifungal creams (e.g., Clotrimazole)\n";
        cout << "Precautions: Avoid douching, maintain good hygiene.\n";
        cout << "Quantity: Apply as directed.\n";
    }

    else if (bruteForceStringMatch(symptom, "UTIs (non-severe)")) {
        cout << "Prescription: Antibiotics (e.g., Nitrofurantoin)\n";
        cout << "Precautions: Drink plenty of water and complete the full course of antibiotics.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild incontinence")) {
        cout << "Prescription: Bladder training or pelvic floor exercises\n";
        cout << "Precautions: Limit caffeine and alcohol, maintain hydration.\n";
        cout << "Quantity: Follow a prescribed exercise regimen.\n";
    }

    else if (bruteForceStringMatch(symptom, "premenstrual syndrome (PMS)")) {
        cout << "Prescription: Pain relievers (e.g., Ibuprofen) or hormonal therapy\n";
        cout << "Precautions: Maintain a healthy diet, reduce stress.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "erectile dysfunction")) {
        cout << "Prescription: PDE5 inhibitors (e.g., Sildenafil)\n";
        cout << "Precautions: Avoid alcohol and high-fat meals before taking medication.\n";
        cout << "Quantity: Take as prescribed by a healthcare provider.\n";
    }
    else if (bruteForceStringMatch(symptom, "low libido")) {
        cout << "Prescription: Testosterone therapy (if indicated) or herbal supplements (e.g., Tribulus)\n";
        cout << "Precautions: Regular physical activity, reduce stress, and improve sleep quality.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor fertility issues")) {
        cout << "Prescription: Consult a fertility specialist for further evaluation\n";
        cout << "Precautions: Maintain a healthy diet, avoid smoking and excessive alcohol.\n";
        cout << "Quantity: As per specialist's recommendation.\n";
    }

    else if (bruteForceStringMatch(symptom, "palpitations (non-severe)")) {
        cout << "Prescription: Beta-blockers or anti-anxiety medications\n";
        cout << "Precautions: Monitor stress levels, avoid caffeine, and manage underlying conditions.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    } else if (bruteForceStringMatch(symptom, "high blood pressure (manageable)")) {
        cout << "Prescription: ACE inhibitors, calcium channel blockers, or lifestyle changes\n";
        cout << "Precautions: Reduce sodium intake, regular exercise, and limit alcohol consumption.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "low blood pressure")) {
        cout << "Prescription: Increase salt intake, fluids, or wear compression stockings\n";
        cout << "Precautions: Avoid standing up quickly, stay hydrated.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "varicose veins")) {
        cout << "Prescription: Compression stockings or sclerotherapy\n";
        cout << "Precautions: Elevate legs frequently, avoid standing for long periods.\n";
        cout << "Quantity: Follow a healthcare provider's recommendation.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild chest tightness")) {
      cout << "Prescription: Beta-agonists (e.g., Albuterol) or relaxation techniques\n";
        cout << "Precautions: Avoid triggers, use a rescue inhaler if needed.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

     else if (bruteForceStringMatch(symptom, "poor circulation")) {
        cout << "Prescription: Blood thinners or vasodilators\n";
        cout << "Precautions: Regular exercise, avoid prolonged sitting.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "cold hands and feet")) {
        cout << "Prescription: Improve circulation through exercise or vasodilators\n";
        cout << "Precautions: Keep your hands and feet warm, avoid tight clothing.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "hypothyroidism")) {
        cout << "Prescription: Levothyroxine\n";
        cout << "Precautions: Monitor thyroid levels regularly, avoid excessive iodine intake.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "hyperthyroidism")) {
        cout << "Prescription: Antithyroid medications (e.g., Methimazole) or radioactive iodine\n";
        cout << "Precautions: Regular thyroid function tests, avoid excessive iodine intake.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild diabetes (controlled)")) {
        cout << "Prescription: Oral hypoglycemic agents (e.g., Metformin)\n";
        cout << "Precautions: Monitor blood sugar levels regularly, follow a balanced diet.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    } else if (bruteForceStringMatch(symptom, "hypoglycemia")) {
        cout << "Prescription: Fast-acting sugar or glucose tablets\n";
        cout << "Precautions: Monitor blood sugar levels regularly, avoid prolonged fasting.\n";
        cout << "Quantity: As needed for hypoglycemic events.\n";
    }

    else if (bruteForceStringMatch(symptom, "slow metabolism")) {
        cout << "Prescription: Thyroid hormone replacement or metabolism-boosting supplements\n";
        cout << "Precautions: Regular exercise, balanced diet rich in protein and fiber.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "weight gain")) {
        cout << "Prescription: Lifestyle changes, calorie reduction, or thyroid medication if applicable\n";
        cout << "Precautions: Regular exercise, balanced diet, stress management.\n";
        cout << "Quantity: Follow a healthcare provider's recommendations.\n";
    }

    else if (bruteForceStringMatch(symptom, "weight loss")) {
        cout << "Prescription: Nutritional support or appetite stimulants\n";
        cout << "Precautions: Ensure balanced nutrition, avoid excessive dieting.\n";
        cout << "Quantity: Follow a healthcare provider's recommendations.\n";
    }

    else if (bruteForceStringMatch(symptom, "fatigue due to anemia")) {
        cout << "Prescription: Iron supplements or Vitamin B12 shots\n";
        cout << "Precautions: Avoid excessive tea/coffee with meals, take supplements with vitamin C.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "hay fever")) {
        cout << "Prescription: Antihistamines (e.g., Loratadine)\n";
        cout << "Precautions: Avoid allergens, keep windows closed during high pollen season.\n";
        cout << "Quantity: 1 tablet daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "dust allergy")) {
        cout << "Prescription: Antihistamines or nasal decongestants\n";
        cout << "Precautions: Keep home clean, use air purifiers.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "pet allergies")) {
        cout << "Prescription: Antihistamines (e.g., Cetirizine)\n";
        cout << "Precautions: Limit exposure to pets, clean furniture frequently.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "pollen allergies")) {
        cout << "Prescription: Antihistamines or nasal sprays\n";
        cout << "Precautions: Avoid outdoor activities during high pollen days.\n";
        cout << "Quantity: 1 tablet daily during allergy season.\n";
    }

    else if (bruteForceStringMatch(symptom, "food allergies (mild)")) {
        cout << "Prescription: Antihistamines or epinephrine auto-injectors for severe reactions\n";
        cout << "Precautions: Avoid allergens, always carry emergency medication.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "skin allergies")) {
        cout << "Prescription: Topical corticosteroids (e.g., Hydrocortisone)\n";
        cout << "Precautions: Avoid scratching, avoid allergens.\n";
        cout << "Quantity: Apply as directed.\n";
    }

    else if (bruteForceStringMatch(symptom, "latex allergy")) {
        cout << "Prescription: Antihistamines or epinephrine auto-injectors for severe reactions\n";
        cout << "Precautions: Avoid latex-containing products, always carry emergency medication.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "obesity")) {
        cout << "Prescription: Weight loss management program, calorie reduction\n";
        cout << "Precautions: Maintain a balanced diet, increase physical activity.\n";
        cout << "Quantity: Follow a healthcare provider's plan.\n";
    }

    else if (bruteForceStringMatch(symptom, "smoking-related cough")) {
        cout << "Prescription: Cough suppressant or bronchodilators\n";
        cout << "Precautions: Quit smoking, avoid second-hand smoke.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "dehydration")) {
        cout << "Prescription: Oral rehydration salts (ORS) or electrolyte drinks\n";
        cout << "Precautions: Avoid excessive caffeine or alcohol, maintain hydration.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "vitamin deficiency")) {
        cout << "Prescription: Vitamin supplements (e.g., Vitamin D, B12)\n";
        cout << "Precautions: Follow a balanced diet to prevent further deficiencies.\n";
        cout << "Quantity: As prescribed.\n";
    }

    else if (bruteForceStringMatch(symptom, "iron deficiency")) {
        cout << "Prescription: Iron supplements\n";
        cout << "Precautions: Take with vitamin C to enhance absorption.\n";
        cout << "Quantity: As prescribed.\n";
    }

    else if (bruteForceStringMatch(symptom, "low calcium levels")) {
        cout << "Prescription: Calcium supplements\n";
        cout << "Precautions: Ensure adequate vitamin D intake for better calcium absorption.\n";
        cout << "Quantity: As prescribed.\n";
    }

    else if (bruteForceStringMatch(symptom, "overeating")) {
        cout << "Prescription: Appetite suppressants or counseling for eating habits\n";
        cout << "Precautions: Maintain regular meal times, avoid eating large meals at night.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "poor posture")) {
        cout << "Prescription: Ergonomic changes and posture correction exercises\n";
        cout << "Precautions: Regular breaks during sitting, use ergonomic furniture.\n";
        cout << "Quantity: Follow a healthcare provider's recommendations.\n";
    }

    else if (bruteForceStringMatch(symptom, "jet lag")) {
        cout << "Prescription: Melatonin supplements or gradual sleep schedule adjustment\n";
        cout << "Precautions: Stay hydrated, get sunlight exposure.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "lack of exercise")) {
        cout << "Prescription: A personalized exercise regimen\n";
        cout << "Precautions: Start with light exercises, gradually increase intensity.\n";
        cout << "Quantity: Follow a fitness professional's plan.\n";
    }
    else if (bruteForceStringMatch(symptom, "bee sting allergies")) {
        cout << "Prescription: Antihistamines (e.g., Diphenhydramine) or Epinephrine (for severe reactions)\n";
        cout << "Precautions: Avoid bee stings, carry an epinephrine auto-injector if allergic.\n";
        cout << "Quantity: As needed for symptoms.\n";
    }

    else if (bruteForceStringMatch(symptom, "latex sensitivity")) {
        cout << "Prescription: Antihistamines for mild reactions, Epinephrine for severe reactions\n";
        cout << "Precautions: Avoid latex products, always carry emergency medication.\n";
        cout << "Quantity: As needed for symptoms.\n";
    }

    else if (bruteForceStringMatch(symptom, "seasonal asthma")) {
        cout << "Prescription: Inhalers (e.g., Salbutamol) or corticosteroids\n";
        cout << "Precautions: Avoid triggers, use inhalers as prescribed.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild skin swelling")) {
        cout << "Prescription: Antihistamines (e.g., Cetirizine)\n";
        cout << "Precautions: Avoid scratching, apply cold compresses.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "acne breakouts")) {
        cout << "Prescription: Topical treatments (e.g., Benzoyl Peroxide, Salicylic Acid)\n";
        cout << "Precautions: Keep skin clean, avoid touching your face.\n";
        cout << "Quantity: Apply topically once or twice a day.\n";
    }

    else if (bruteForceStringMatch(symptom, "hair thinning due to hormones")) {
        cout << "Prescription: Minoxidil or hormone therapy (e.g., Spironolactone for women)\n";
        cout << "Precautions: Ensure proper scalp care, avoid heat styling.\n";
        cout << "Quantity: Follow healthcare provider's advice.\n";
    }

     else if (bruteForceStringMatch(symptom, "hot flashes")) {
        cout << "Prescription: Hormone Replacement Therapy (HRT) or non-hormonal treatments (e.g., SSRI)\n";
        cout << "Precautions: Stay cool, wear layers, and avoid spicy foods.\n";
        cout << "Quantity: As prescribed.\n";
    }

    else if (bruteForceStringMatch(symptom, "night sweats (hormonal)")) {
        cout << "Prescription: Hormone Replacement Therapy (HRT)\n";
        cout << "Precautions: Keep bedroom cool, wear moisture-wicking fabrics.\n";
        cout << "Quantity: As prescribed.\n";
    }

    else if (bruteForceStringMatch(symptom, "irregular menstruation")) {
        cout << "Prescription: Birth control pills or hormonal therapy\n";
        cout << "Precautions: Keep track of menstrual cycles, avoid stress.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "mood swings (PMS-related)")) {
        cout << "Prescription: SSRIs, oral contraceptives, or herbal treatments (e.g., Vitex)\n";
        cout << "Precautions: Manage stress, avoid excessive caffeine or sugar.\n";
        cout << "Quantity: As prescribed or as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "breast tenderness")) {
        cout << "Prescription: Over-the-counter pain relief (e.g., Ibuprofen)\n";
        cout << "Precautions: Wear a supportive bra, avoid tight clothing.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "polycystic ovary syndrome (mild PCOS symptoms)")) {
        cout << "Prescription: Birth control pills, Metformin, or Clomiphene (for fertility)\n";
        cout << "Precautions: Maintain a healthy weight, avoid stress.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

     else if (bruteForceStringMatch(symptom, "low testosterone levels")) {
        cout << "Prescription: Testosterone replacement therapy\n";
        cout << "Precautions: Regular monitoring of testosterone levels.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

     else if (bruteForceStringMatch(symptom, "thyroid-related fatigue")) {
        cout << "Prescription: Thyroid hormone replacement (e.g., Levothyroxine)\n";
        cout << "Precautions: Monitor thyroid levels, avoid stress.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "numbness in hands or feet")) {
        cout << "Prescription: Vitamin B12 supplements or physical therapy\n";
        cout << "Precautions: Regular exercise, avoid prolonged sitting or standing.\n";
        cout << "Quantity: As prescribed.\n";
    }

    else if (bruteForceStringMatch(symptom, "pins and needles sensation")) {
        cout << "Prescription: Vitamin B12 supplements or treatment for underlying conditions\n";
        cout << "Precautions: Avoid pressure on nerves, take frequent breaks.\n";
        cout << "Quantity: As prescribed.\n";
    }

     else if (bruteForceStringMatch(symptom, "muscle twitching")) {
        cout << "Prescription: Magnesium supplements or muscle relaxants\n";
        cout << "Precautions: Stay hydrated, avoid overexertion.\n";
        cout << "Quantity: As needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "tension headaches")) {
        cout << "Prescription: Ibuprofen or Acetaminophen\n";
        cout << "Precautions: Manage stress, avoid prolonged screen time.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor tremors")) {
        cout << "Prescription: Beta-blockers or anti-anxiety medications\n";
        cout << "Precautions: Avoid caffeine, ensure proper sleep.\n";
        cout << "Quantity: As prescribed by a healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "lightheadedness")) {
        cout << "Prescription: Hydration or medication for underlying conditions\n";
        cout << "Precautions: Avoid sudden movements, sit down if feeling lightheaded.\n";
        cout << "Quantity: As needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "short-term memory lapses")) {
        cout << "Prescription: Cognitive exercises or memory supplements (e.g., Ginkgo Biloba)\n";
        cout << "Precautions: Stay mentally active, ensure adequate sleep.\n";
        cout << "Quantity: As needed.\n";
    }

        else if (bruteForceStringMatch(symptom, "difficulty focusing")) {
        cout << "Prescription: Caffeine or cognitive enhancers (e.g., L-Theanine)\n";
        cout << "Precautions: Minimize distractions, ensure proper sleep.\n";
        cout << "Quantity: As needed.\n";
    }


        else if (bruteForceStringMatch(symptom, "eye twitching")) {
        cout << "Prescription: Reduce stress or eye drops for irritation\n";
        cout << "Precautions: Get sufficient rest, avoid prolonged screen time.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "stress-related dizziness")) {
        cout << "Prescription: Anti-anxiety medication or relaxation techniques\n";
        cout << "Precautions: Manage stress through deep breathing, yoga, and adequate rest.\n";
        cout << "Quantity: As prescribed.\n";
    }

     else if (bruteForceStringMatch(symptom, "rashes from heat")) {
        cout << "Prescription: Topical corticosteroids or antihistamines\n";
        cout << "Precautions: Avoid hot environments, wear loose clothing.\n";
        cout << "Quantity: Apply as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "seasonal coughs")) {
        cout << "Prescription: Cough suppressants or antihistamines\n";
        cout << "Precautions: Stay hydrated, avoid allergens.\n";
        cout << "Quantity: As needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "hand rashes")) {
        cout << "Prescription: Topical corticosteroids or emollients\n";
        cout << "Precautions: Avoid irritating substances, moisturize regularly.\n";
        cout << "Quantity: Apply as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "scalp irritation")) {
        cout << "Prescription: Medicated shampoos (e.g., Ketoconazole, Salicylic acid)\n";
        cout << "Precautions: Avoid harsh hair products, keep the scalp clean.\n";
        cout << "Quantity: Use as directed.\n";
    }
    else if (bruteForceStringMatch(symptom, "diarrhea in toddlers")) {
        cout << "Prescription: Oral rehydration solution (ORS)\n";
        cout << "Precautions: Keep the child hydrated, avoid sugary drinks, and monitor for signs of dehydration.\n";
        cout << "Quantity: Follow the ORS instructions on the package.\n";
    }


    else if (bruteForceStringMatch(symptom, "viral fever in kids")) {
        cout << "Prescription: Paracetamol for fever\n";
        cout << "Precautions: Keep the child hydrated, monitor temperature, and rest.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "minor bumps")) {
        cout << "Prescription: Apply antiseptic cream\n";
        cout << "Precautions: Keep the area clean, avoid scratching, and apply a bandage if necessary.\n";
        cout << "Quantity: Apply twice daily.\n";
    }

     else if (bruteForceStringMatch(symptom, "ear blockage")) {
        cout << "Prescription: Warm olive oil drops\n";
        cout << "Precautions: Avoid inserting foreign objects into the ear, and keep it dry.\n";
        cout << "Quantity: 2-3 drops in the affected ear before bedtime.\n";
    }

    else if (bruteForceStringMatch(symptom, "mouth breathing (habitual)")) {
        cout << "Prescription: Nasal saline spray\n";
        cout << "Precautions: Practice breathing exercises, consult a doctor for underlying causes.\n";
        cout << "Quantity: Use as directed by a healthcare provider.\n";
    }

     else if (bruteForceStringMatch(symptom, "thumb-sucking effects")) {
        cout << "Prescription: Positive reinforcement and behavior modification techniques\n";
        cout << "Precautions: Use thumb guards if needed, encourage other coping mechanisms.\n";
        cout << "Quantity: As needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "mild eczema flare-ups")) {
        cout << "Prescription: Topical corticosteroid cream\n";
        cout << "Precautions: Moisturize the skin regularly, avoid triggers.\n";
        cout << "Quantity: Apply as directed by the healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "psoriasis (mild patches)")) {
        cout << "Prescription: Topical corticosteroid or calcipotriene cream\n";
        cout << "Precautions: Moisturize the skin regularly, avoid triggers.\n";
        cout << "Quantity: Apply as directed by the healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "rosacea")) {
        cout << "Prescription: Topical metronidazole cream\n";
        cout << "Precautions: Avoid sun exposure, use sunscreen, and avoid spicy foods.\n";
        cout << "Quantity: Apply twice daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "dermatitis")) {
        cout << "Prescription: Topical corticosteroid cream\n";
        cout << "Precautions: Keep skin moisturized, avoid scratching.\n";
        cout << "Quantity: Apply as directed by the healthcare provider.\n";
    }

    else if (bruteForceStringMatch(symptom, "athlete�s foot recurrences")) {
        cout << "Prescription: Antifungal cream (Clotrimazole or Terbinafine)\n";
        cout << "Precautions: Keep feet dry, wear breathable shoes, and avoid walking barefoot in communal areas.\n";
        cout << "Quantity: Apply once or twice daily for 2-4 weeks.\n";
    }

    else if (bruteForceStringMatch(symptom, "seasonal migraines")) {
        cout << "Prescription: Triptans (Sumatriptan)\n";
        cout << "Precautions: Avoid migraine triggers, maintain a regular sleep schedule.\n";
        cout << "Quantity: As prescribed, typically 1 tablet at the onset of a migraine.\n";
    }

     else if (bruteForceStringMatch(symptom, "occasional acid reflux disease")) {
        cout << "Prescription: Antacid (Omeprazole or Ranitidine)\n";
        cout << "Precautions: Avoid large meals, spicy foods, and lying down immediately after eating.\n";
        cout << "Quantity: 1 tablet before meals, as needed.\n";
    }

       else if (bruteForceStringMatch(symptom, "chronic but mild sinusitis")) {
        cout << "Prescription: Nasal corticosteroid spray (Fluticasone)\n";
        cout << "Precautions: Use a humidifier, avoid allergens.\n";
        cout << "Quantity: Use as directed by a healthcare provider.\n";
    }

        else if (bruteForceStringMatch(symptom, "tension neck syndrome")) {
        cout << "Prescription: Muscle relaxant (Cyclobenzaprine)\n";
        cout << "Precautions: Apply warm compresses, avoid prolonged sitting.\n";
        cout << "Quantity: 1 tablet before bedtime, as needed.\n";
    }

        else if (bruteForceStringMatch(symptom, "recurrent UTIs")) {
        cout << "Prescription: Antibiotics (Trimethoprim-Sulfamethoxazole)\n";
        cout << "Precautions: Stay hydrated, urinate frequently, and practice proper hygiene.\n";
        cout << "Quantity: Complete the full course of antibiotics.\n";
    }

        else if (bruteForceStringMatch(symptom, "shin splints from running")) {
        cout << "Prescription: Rest, ice packs, and compression\n";
        cout << "Precautions: Avoid running on hard surfaces, gradually increase intensity.\n";
        cout << "Quantity: Apply ice for 20 minutes every 2-3 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "soreness after workouts")) {
        cout << "Prescription: Stretching and gentle movement\n";
        cout << "Precautions: Avoid strenuous activity until soreness subsides.\n";
        cout << "Quantity: Gentle stretching post-workout.\n";
    }

     else if (bruteForceStringMatch(symptom, "muscle fatigue")) {
        cout << "Prescription: Rest and proper nutrition\n";
        cout << "Precautions: Stay hydrated and ensure a balanced diet with enough protein.\n";
        cout << "Quantity: Take breaks as needed during physical activity.\n";
    }

     else if (bruteForceStringMatch(symptom, "overstretch injuries")) {
        cout << "Prescription: R.I.C.E. (Rest, Ice, Compression, Elevation)\n";
        cout << "Precautions: Avoid re-injuring the area, use ice packs for 20 minutes every few hours.\n";
        cout << "Quantity: Apply ice several times a day for 2-3 days.\n";
    }

    else if (bruteForceStringMatch(symptom, "foot blisters from hiking")) {
        cout << "Prescription: Blister bandages\n";
        cout << "Precautions: Keep the area clean and dry, avoid further friction.\n";
        cout << "Quantity: Apply as needed.\n";
    }

       else if (bruteForceStringMatch(symptom, "joint strain")) {
        cout << "Prescription: Rest and ice\n";
        cout << "Precautions: Avoid overexertion, use supportive braces if necessary.\n";
        cout << "Quantity: Apply ice for 20 minutes every few hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "tennis elbow")) {
        cout << "Prescription: Ice packs and anti-inflammatory medication\n";
        cout << "Precautions: Avoid repetitive arm movements, use a brace if needed.\n";
        cout << "Quantity: Apply ice for 20 minutes every 2-3 hours.\n";
    }

     else if (bruteForceStringMatch(symptom, "runner�s knee")) {
        cout << "Prescription: Rest, ice, and anti-inflammatory medication\n";
        cout << "Precautions: Avoid running until pain subsides, use a knee brace.\n";
        cout << "Quantity: Apply ice for 20 minutes every few hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "plantar fasciitis (mild)")) {
        cout << "Prescription: Stretching exercises and heel pads\n";
        cout << "Precautions: Avoid barefoot walking on hard surfaces, wear supportive shoes.\n";
        cout << "Quantity: Perform stretches daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "sprained ankle")) {
        cout << "Prescription: R.I.C.E. (Rest, Ice, Compression, Elevation)\n";
        cout << "Precautions: Avoid walking on the injured ankle, use crutches if necessary.\n";
        cout << "Quantity: Apply ice for 20 minutes every few hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "excessive sleepiness")) {
        cout << "Prescription: Improve sleep hygiene\n";
        cout << "Precautions: Ensure a regular sleep schedule and avoid caffeine before bed.\n";
        cout << "Quantity: Follow a consistent bedtime routine.\n";
    }

     else if (bruteForceStringMatch(symptom, "waking up too early")) {
        cout << "Prescription: Behavioral therapy and sleep regulation\n";
        cout << "Precautions: Avoid looking at screens before bed, keep a cool, dark sleep environment.\n";
        cout << "Quantity: Implement behavioral changes for better sleep.\n";
    }

    else if (bruteForceStringMatch(symptom, "sleep inertia")) {
        cout << "Prescription: Gradual awakening techniques\n";
        cout << "Precautions: Try to wake up gradually, avoid sudden bright lights.\n";
        cout << "Quantity: Use a gradual alarm clock.\n";
    }

    else if (bruteForceStringMatch(symptom, "dream-related disruptions")) {
        cout << "Prescription: Relaxation techniques and good sleep hygiene\n";
        cout << "Precautions: Establish a consistent bedtime routine and avoid stimulants before bed.\n";
        cout << "Quantity: Practice relaxation techniques nightly.\n";
    }

    else if (bruteForceStringMatch(symptom, "light sensitivity during sleep")) {
        cout << "Prescription: Sleep mask or blackout curtains\n";
        cout << "Precautions: Create a dark, quiet sleep environment.\n";
        cout << "Quantity: Use nightly.\n";
    }

    else if (bruteForceStringMatch(symptom, "sleep deprivation")) {
        cout << "Prescription: Ensure proper rest and regular sleep schedule\n";
        cout << "Precautions: Aim for 7-9 hours of sleep per night and avoid caffeine late in the day.\n";
        cout << "Quantity: Follow a consistent bedtime routine.\n";
    }

    else if (bruteForceStringMatch(symptom, "poor-quality sleep")) {
        cout << "Prescription: Sleep aid (Melatonin or Diphenhydramine)\n";
        cout << "Precautions: Address underlying causes such as stress or discomfort.\n";
        cout << "Quantity: 1 tablet before bedtime, as needed.\n";
    }

        else if (bruteForceStringMatch(symptom, "oversleeping fatigue")) {
        cout << "Prescription: Gradually adjust sleep schedule\n";
        cout << "Precautions: Aim for 7-9 hours of sleep and maintain a consistent wake-up time.\n";
        cout << "Quantity: Gradually reduce sleep duration.\n";
    }

       else if (bruteForceStringMatch(symptom, "mid-sleep waking")) {
        cout << "Prescription: Sleep hygiene improvement\n";
        cout << "Precautions: Keep a consistent bedtime, avoid alcohol and caffeine.\n";
        cout << "Quantity: Practice relaxation techniques before sleep.\n";
    }

    else if (bruteForceStringMatch(symptom, "irregular napping effects")) {
        cout << "Prescription: Set a regular napping schedule\n";
        cout << "Precautions: Limit naps to 20-30 minutes and avoid napping late in the day.\n";
        cout << "Quantity: Limit naps to the early afternoon.\n";
    }

    else if (bruteForceStringMatch(symptom, "smog-related coughing")) {
        cout << "Prescription: Antihistamine (Cetirizine) or cough syrup\n";
        cout << "Precautions: Avoid exposure to smog, wear a mask if necessary, and stay indoors.\n";
        cout << "Quantity: 1 tablet as needed for coughing.\n";
    }

    else if (bruteForceStringMatch(symptom, "smoke irritation")) {
        cout << "Prescription: Antihistamine (Diphenhydramine)\n";
        cout << "Precautions: Avoid smoke exposure, stay indoors, and use air purifiers.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "chemical sensitivity")) {
        cout << "Prescription: Antihistamine (Loratadine)\n";
        cout << "Precautions: Avoid exposure to harsh chemicals, wear protective equipment if needed.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "dust-induced sneezing")) {
        cout << "Prescription: Antihistamine (Cetirizine)\n";
        cout << "Precautions: Keep the environment clean, use air purifiers, and avoid dusty areas.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "mold exposure symptoms")) {
        cout << "Prescription: Antihistamine (Loratadine) or nasal spray\n";
        cout << "Precautions: Minimize exposure to moldy areas and use dehumidifiers.\n";
        cout << "Quantity: 1 tablet daily, or nasal spray as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "polluted air symptoms")) {
        cout << "Prescription: Antihistamine (Cetirizine)\n";
        cout << "Precautions: Stay indoors, use air purifiers, and wear a mask in polluted areas.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor carbon monoxide effects")) {
        cout << "Prescription: Move to fresh air immediately and seek medical attention\n";
        cout << "Precautions: Ensure proper ventilation in indoor spaces.\n";
        cout << "Quantity: N/A\n";
    }

    else if (bruteForceStringMatch(symptom, "scent-triggered headaches")) {
        cout << "Prescription: Pain relievers (Ibuprofen) or antihistamine\n";
        cout << "Precautions: Avoid triggers, such as strong perfumes or chemicals.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "industrial allergies")) {
        cout << "Prescription: Antihistamine (Cetirizine)\n";
        cout << "Precautions: Wear protective gear and minimize exposure to allergens.\n";
        cout << "Quantity: 1 tablet as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "poor ventilation-related fatigue")) {
        cout << "Prescription: Improve ventilation in your living space\n";
        cout << "Precautions: Open windows, use fans, and ensure good airflow.\n";
        cout << "Quantity: N/A\n";
    }

    else if (bruteForceStringMatch(symptom, "jet lag recovery")) {
        cout << "Prescription: Melatonin and gradual adjustment to local time\n";
        cout << "Precautions: Avoid caffeine and bright light during the evening.\n";
        cout << "Quantity: 1 tablet of Melatonin before bedtime for the first few days.\n";
    }

    else if (bruteForceStringMatch(symptom, "travel diarrhea")) {
        cout << "Prescription: Oral rehydration solution (ORS) and antibiotics (if severe)\n";
        cout << "Precautions: Avoid contaminated food and water, stay hydrated.\n";
        cout << "Quantity: Follow ORS instructions and take antibiotics as prescribed.\n";
    }

    else if (bruteForceStringMatch(symptom, "altitude headache")) {
        cout << "Prescription: Acetaminophen (Paracetamol) or Ibuprofen\n";
        cout << "Precautions: Drink plenty of fluids, avoid physical exertion, and acclimatize gradually.\n";
        cout << "Quantity: 1 tablet every 4-6 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "motion sickness (cars, boats)")) {
        cout << "Prescription: Motion sickness tablet (Dimenhydrinate)\n";
        cout << "Precautions: Take the medication 30 minutes before travel, avoid reading during travel.\n";
        cout << "Quantity: 1 tablet before travel.\n";
    }

    else if (bruteForceStringMatch(symptom, "traveler�s fatigue")) {
        cout << "Prescription: Rest, hydration, and balanced nutrition\n";
        cout << "Precautions: Take breaks, stay hydrated, and avoid overexertion.\n";
        cout << "Quantity: Follow a regular sleep schedule.\n";
    }

    else if (bruteForceStringMatch(symptom, "climate-change cold symptoms")) {
        cout << "Prescription: Decongestant (Pseudoephedrine)\n";
        cout << "Precautions: Stay warm, avoid exposure to harsh weather, and rest.\n";
        cout << "Quantity: 1 tablet every 4-6 hours, as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "sunburn during travel")) {
        cout << "Prescription: Aloe vera gel or corticosteroid cream\n";
        cout << "Precautions: Avoid sun exposure, wear protective clothing.\n";
        cout << "Quantity: Apply aloe vera gel as needed for relief.\n";
    }

    else if (bruteForceStringMatch(symptom, "overhydration issues during flights")) {
        cout << "Prescription: Limit fluid intake and ensure a balance of electrolytes\n";
        cout << "Precautions: Avoid excessive drinking of water and eat electrolyte-rich foods.\n";
        cout << "Quantity: Maintain hydration balance.\n";
    }

    else if (bruteForceStringMatch(symptom, "lack of activity during travel")) {
        cout << "Prescription: Perform light stretches and walk regularly\n";
        cout << "Precautions: Avoid sitting for long periods, take breaks every few hours.\n";
        cout << "Quantity: Stretch every hour, walk every 2-3 hours.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor infections from travel")) {
        cout << "Prescription: Antibiotics (as prescribed by a doctor)\n";
        cout << "Precautions: Maintain good hygiene, wash hands regularly, and avoid close contact with others.\n";
        cout << "Quantity: Follow your doctor�s prescription.\n";
    }

    else if (bruteForceStringMatch(symptom, "winter itch")) {
        cout << "Prescription: Moisturizing lotion or hydrocortisone cream\n";
        cout << "Precautions: Apply lotion after bathing and keep the skin hydrated.\n";
        cout << "Quantity: Apply as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "fall allergies")) {
        cout << "Prescription: Antihistamine (Loratadine)\n";
        cout << "Precautions: Avoid outdoor activities during high pollen counts, keep windows closed.\n";
        cout << "Quantity: 1 tablet daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "springtime rashes")) {
        cout << "Prescription: Hydrocortisone cream or antihistamine\n";
        cout << "Precautions: Avoid allergens and moisturize the skin regularly.\n";
        cout << "Quantity: Apply cream as needed or take antihistamine.\n";
    }

    else if (bruteForceStringMatch(symptom, "summer dehydration")) {
        cout << "Prescription: Oral rehydration solution (ORS)\n";
        cout << "Precautions: Drink plenty of fluids, especially water, and avoid excessive caffeine or alcohol.\n";
        cout << "Quantity: Use ORS as directed to maintain hydration.\n";
    }

    else if (bruteForceStringMatch(symptom, "seasonal lethargy")) {
        cout << "Prescription: Vitamin D supplements and regular exercise\n";
        cout << "Precautions: Get sunlight exposure and engage in light physical activities.\n";
        cout << "Quantity: Follow the recommended dosage of Vitamin D.\n";
    }

    else if (bruteForceStringMatch(symptom, "minor frostbite in winter")) {
        cout << "Prescription: Warm up the affected area gradually, and apply aloe vera gel\n";
        cout << "Precautions: Avoid sudden warming, and seek medical help if the condition worsens.\n";
        cout << "Quantity: Apply aloe vera gel as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "excessive sweating in summer")) {
        cout << "Prescription: Antiperspirant or prescription-strength deodorant\n";
        cout << "Precautions: Wear breathable fabrics and stay hydrated.\n";
        cout << "Quantity: Apply daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "dry lips in cold weather")) {
        cout << "Prescription: Lip balm with petroleum jelly or beeswax\n";
        cout << "Precautions: Apply lip balm regularly and avoid licking your lips.\n";
        cout << "Quantity: Apply as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "heat rashes in spring")) {
        cout << "Prescription: Calamine lotion or antihistamine\n";
        cout << "Precautions: Avoid hot, humid conditions and wear loose, breathable clothing.\n";
        cout << "Quantity: Apply lotion as needed.\n";
    }

    else if (bruteForceStringMatch(symptom, "early snow effects on joints")) {
        cout << "Prescription: Anti-inflammatory medications (Ibuprofen)\n";
        cout << "Precautions: Stay warm and use joint supports if necessary.\n";
        cout << "Quantity: 1 tablet every 6-8 hours, as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "stress from multitasking")) {
        cout << "Prescription: Relaxation techniques and regular breaks\n";
        cout << "Precautions: Prioritize tasks and avoid overloading yourself.\n";
        cout << "Quantity: Practice stress-relief activities daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "overuse of hands (typing)")) {
        cout << "Prescription: Ergonomic hand supports and rest\n";
        cout << "Precautions: Take frequent breaks, stretch your hands, and maintain good posture.\n";
        cout << "Quantity: Rest hands regularly during work.\n";
    }

    else if (bruteForceStringMatch(symptom, "screen fatigue")) {
        cout << "Prescription: Take regular screen breaks and adjust screen brightness\n";
        cout << "Precautions: Follow the 20-20-20 rule: take a 20-second break every 20 minutes, looking at something 20 feet away.\n";
        cout << "Quantity: Take breaks every 20 minutes.\n";
    }

    else if (bruteForceStringMatch(symptom, "workplace burnout")) {
        cout << "Prescription: Manage work-life balance and take time off\n";
        cout << "Precautions: Prioritize self-care and seek professional help if needed.\n";
        cout << "Quantity: Take a day off or engage in activities that relax you.\n";
    }

    else if (bruteForceStringMatch(symptom, "ergonomic desk issues")) {
        cout << "Prescription: Adjust desk height and seating position\n";
        cout << "Precautions: Ensure your screen is at eye level and wrists are neutral while typing.\n";
        cout << "Quantity: Make ergonomic adjustments to your workstation.\n";
    }

    else if (bruteForceStringMatch(symptom, "frequent standing fatigue")) {
        cout << "Prescription: Footrest and comfortable footwear\n";
        cout << "Precautions: Take sitting breaks and do leg stretches regularly.\n";
        cout << "Quantity: Rest your legs periodically.\n";
    }

    else if (bruteForceStringMatch(symptom, "shoulder strain from heavy bags")) {
        cout << "Prescription: Ice packs for inflammation and stretching exercises\n";
        cout << "Precautions: Avoid carrying heavy bags for long periods and distribute weight evenly.\n";
        cout << "Quantity: Apply ice for 20 minutes, 3 times a day, and perform stretching exercises.\n";
    }

    else if (bruteForceStringMatch(symptom, "inadequate water intake at work")) {
        cout << "Prescription: Increase water intake and keep a water bottle nearby\n";
        cout << "Precautions: Drink small amounts throughout the day to stay hydrated.\n";
        cout << "Quantity: Aim for 8-10 glasses of water daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "unhealthy snacking effects")) {
        cout << "Prescription: Replace snacks with healthier options like fruits and nuts\n";
        cout << "Precautions: Avoid excessive processed foods and maintain a balanced diet.\n";
        cout << "Quantity: Choose healthy snacks when hungry.\n";
    }

    else if (bruteForceStringMatch(symptom, "lack of outdoor activity")) {
        cout << "Prescription: Incorporate outdoor activities like walking, running, or hiking\n";
        cout << "Precautions: Aim for at least 30 minutes of activity outdoors every day.\n";
        cout << "Quantity: Try to walk outdoors daily.\n";
    }

     else if (bruteForceStringMatch(symptom, "chronic fatigue syndrome")) {
        cout << "Prescription: Consult a healthcare provider for a personalized treatment plan.\n";
        cout << "Precautions: Try to establish a regular sleep routine and manage stress.\n";
        cout << "Quantity: Follow healthcare provider's advice.\n";
    }
    else if (bruteForceStringMatch(symptom, "muscle stiffness")) {
        cout << "Prescription: Muscle relaxant or anti-inflammatory medication.\n";
        cout << "Precautions: Gentle stretching and warm baths may help ease stiffness.\n";
        cout << "Quantity: Follow healthcare provider's instructions.\n";
    }
    else if (bruteForceStringMatch(symptom, "mild dehydration")) {
        cout << "Prescription: Rehydrate with water or an electrolyte solution.\n";
        cout << "Precautions: Avoid caffeinated or alcoholic beverages.\n";
        cout << "Quantity: Drink small sips regularly.\n";
    }
    else if (bruteForceStringMatch(symptom, "overhydration")) {
        cout << "Prescription: Reduce water intake and seek medical advice if symptoms worsen.\n";
        cout << "Precautions: Limit excessive water consumption.\n";
        cout << "Quantity: Follow healthcare provider's advice.\n";
    }
    else if (bruteForceStringMatch(symptom, "hunger pangs")) {
        cout << "Prescription: Healthy snacks such as fruits or nuts.\n";
        cout << "Precautions: Maintain a balanced diet throughout the day.\n";
        cout << "Quantity: Eat a small snack if needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "minor bruises")) {
        cout << "Prescription: Apply ice to reduce swelling.\n";
        cout << "Precautions: Avoid pressure on the bruise and elevate the affected area.\n";
        cout << "Quantity: Apply ice for 15-20 minutes several times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "paper cuts")) {
        cout << "Prescription: Clean the cut and apply antiseptic ointment.\n";
        cout << "Precautions: Avoid contamination and cover with a bandage.\n";
        cout << "Quantity: Apply ointment 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "hangnail pain")) {
        cout << "Prescription: Apply a soothing ointment and avoid pulling the hangnail.\n";
        cout << "Precautions: Keep nails trimmed and moisturized.\n";
        cout << "Quantity: Apply ointment twice daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "ingrown toenail")) {
        cout << "Prescription: Soak the affected toe in warm water with Epsom salt.\n";
        cout << "Precautions: Avoid tight shoes and consider visiting a healthcare provider if necessary.\n";
        cout << "Quantity: Soak for 15-20 minutes daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "cold feet")) {
        cout << "Prescription: Warm socks and consider a warm foot soak.\n";
        cout << "Precautions: Keep your feet warm and avoid sitting for long periods.\n";
        cout << "Quantity: Use as needed for comfort.\n";
    }
    else if (bruteForceStringMatch(symptom, "swollen ankles")) {
        cout << "Prescription: Elevate the legs and consider a compression bandage.\n";
        cout << "Precautions: Avoid prolonged standing or sitting.\n";
        cout << "Quantity: Elevate legs for 15-30 minutes several times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "puffy hands")) {
        cout << "Prescription: Try hand exercises and apply a cold compress.\n";
        cout << "Precautions: Avoid prolonged hand use or tight rings.\n";
        cout << "Quantity: Perform exercises regularly.\n";
    }
    else if (bruteForceStringMatch(symptom, "foot cramps")) {
        cout << "Prescription: Gently stretch the affected muscle and drink water.\n";
        cout << "Precautions: Ensure adequate magnesium and potassium intake.\n";
        cout << "Quantity: Stretch and hydrate as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "hand cramps")) {
        cout << "Prescription: Stretch the fingers and massage the hand.\n";
        cout << "Precautions: Stay hydrated and maintain proper posture.\n";
        cout << "Quantity: Stretch as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "temporary numbness")) {
        cout << "Prescription: Reposition the affected area to improve circulation.\n";
        cout << "Precautions: Avoid prolonged pressure on limbs.\n";
        cout << "Quantity: Follow as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "dry nose")) {
        cout << "Prescription: Use a saline nasal spray or humidifier.\n";
        cout << "Precautions: Avoid dry environments and stay hydrated.\n";
        cout << "Quantity: Use saline spray 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "overexertion fatigue")) {
        cout << "Prescription: Rest and rehydrate.\n";
        cout << "Precautions: Avoid physical strain and allow recovery time.\n";
        cout << "Quantity: Rest until feeling better.\n";
    }
    else if (bruteForceStringMatch(symptom, "minor burns")) {
        cout << "Prescription: Apply aloe vera or burn ointment.\n";
        cout << "Precautions: Avoid using ice directly on burns and keep the area clean.\n";
        cout << "Quantity: Apply ointment 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "minor scalds")) {
        cout << "Prescription: Run the area under cool water and apply burn ointment.\n";
        cout << "Precautions: Keep the area clean and avoid tight clothing on the affected area.\n";
        cout << "Quantity: Apply ointment 2-3 times daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "itchy scalp")) {
        cout << "Prescription: Use an anti-itch shampoo or topical treatment.\n";
        cout << "Precautions: Avoid scratching and keep your scalp moisturized.\n";
        cout << "Quantity: Use shampoo as directed.\n";
    }
    else if (bruteForceStringMatch(symptom, "sweaty palms")) {
        cout << "Prescription: Consider using an antiperspirant or topical treatment.\n";
        cout << "Precautions: Keep your hands dry and avoid stress.\n";
        cout << "Quantity: Apply treatment as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "sweaty feet")) {
        cout << "Prescription: Use antifungal powder or foot spray.\n";
        cout << "Precautions: Keep feet clean and dry.\n";
        cout << "Quantity: Apply powder or spray daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "chafing")) {
        cout << "Prescription: Apply an anti-chafing balm or cream.\n";
        cout << "Precautions: Wear loose-fitting clothing and avoid friction.\n";
        cout << "Quantity: Apply balm as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "minor abrasions")) {
        cout << "Prescription: Clean the wound and apply an antiseptic.\n";
        cout << "Precautions: Keep the wound covered and change the bandage regularly.\n";
        cout << "Quantity: Apply antiseptic 2-3 times daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "razor burns")) {
        cout << "Prescription: Apply soothing lotion or aloe vera gel.\n";
        cout << "Precautions: Avoid shaving over the affected area until healed.\n";
        cout << "Quantity: Apply lotion after shaving or as needed.\n";
    }

     else if (bruteForceStringMatch(symptom, "overplucked eyebrows")) {
        cout << "Prescription: Use castor oil or eyebrow serum to promote growth.\n";
        cout << "Precautions: Avoid over-plucking in the future.\n";
        cout << "Quantity: Apply serum daily until hair regrowth is visible.\n";
    }
    else if (bruteForceStringMatch(symptom, "mild hair thinning")) {
        cout << "Prescription: Consider biotin supplements or a hair growth treatment.\n";
        cout << "Precautions: Avoid harsh chemical treatments and excessive heat styling.\n";
        cout << "Quantity: Follow supplement instructions or use treatment as directed.\n";
    }
    else if (bruteForceStringMatch(symptom, "calluses")) {
        cout << "Prescription: Use a pumice stone or foot file.\n";
        cout << "Precautions: Moisturize the feet regularly after exfoliation.\n";
        cout << "Quantity: Use pumice stone 2-3 times a week.\n";
    }
    else if (bruteForceStringMatch(symptom, "corns")) {
        cout << "Prescription: Apply salicylic acid or corn pads.\n";
        cout << "Precautions: Avoid tight footwear and protect the corn from pressure.\n";
        cout << "Quantity: Follow instructions on the corn pad packaging.\n";
    }
    else if (bruteForceStringMatch(symptom, "cracked heels")) {
        cout << "Prescription: Use a thick moisturizing cream or foot balm.\n";
        cout << "Precautions: Keep feet clean and wear soft socks at night.\n";
        cout << "Quantity: Apply cream at least twice a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "split nails")) {
        cout << "Prescription: Moisturize nails and use a nail-strengthening treatment.\n";
        cout << "Precautions: Avoid exposing nails to harsh chemicals.\n";
        cout << "Quantity: Apply treatment 2-3 times a week.\n";
    }
    else if (bruteForceStringMatch(symptom, "discolored nails")) {
        cout << "Prescription: Nail treatment for discoloration or consult a dermatologist.\n";
        cout << "Precautions: Keep nails clean and avoid using harsh nail products.\n";
        cout << "Quantity: Follow the treatment's instructions or consult a specialist.\n";
    }
    else if (bruteForceStringMatch(symptom, "fungal nail infections")) {
        cout << "Prescription: Antifungal nail lacquer or oral antifungal medication.\n";
        cout << "Precautions: Keep nails clean and dry, avoid sharing nail tools.\n";
        cout << "Quantity: Apply antifungal treatment daily, as prescribed.\n";
    }
    else if (bruteForceStringMatch(symptom, "wrinkles")) {
        cout << "Prescription: Use a retinol-based serum or moisturizer.\n";
        cout << "Precautions: Apply sunscreen daily to prevent further damage.\n";
        cout << "Quantity: Apply serum once daily, preferably at night.\n";
    }
    else if (bruteForceStringMatch(symptom, "crow�s feet")) {
        cout << "Prescription: Consider eye creams containing peptides or retinol.\n";
        cout << "Precautions: Avoid squinting and use sunglasses in bright sunlight.\n";
        cout << "Quantity: Apply cream around eyes twice a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "age spots")) {
        cout << "Prescription: Use a brightening serum or sunscreen with SPF 30 or higher.\n";
        cout << "Precautions: Avoid sun exposure and reapply sunscreen regularly.\n";
        cout << "Quantity: Apply serum and sunscreen daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "puffy cheeks")) {
        cout << "Prescription: Reduce salt intake and drink plenty of water.\n";
        cout << "Precautions: Avoid sleeping on your face and manage allergies if necessary.\n";
        cout << "Quantity: Follow hydration and diet tips as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "dull skin")) {
        cout << "Prescription: Exfoliate and use a brightening moisturizer.\n";
        cout << "Precautions: Avoid excessive sun exposure and hydrate your skin.\n";
        cout << "Quantity: Exfoliate 2-3 times a week, use moisturizer daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "dehydrated skin")) {
        cout << "Prescription: Use a hydrating serum and apply moisturizer regularly.\n";
        cout << "Precautions: Drink plenty of water and avoid harsh skincare products.\n";
        cout << "Quantity: Use serum and moisturizer daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "skin tags")) {
        cout << "Prescription: Consult a dermatologist for removal options.\n";
        cout << "Precautions: Avoid trying to remove the skin tag yourself.\n";
        cout << "Quantity: Follow the dermatologist's guidance.\n";
    }
    else if (bruteForceStringMatch(symptom, "stretch marks")) {
        cout << "Prescription: Use creams containing hyaluronic acid or retinoids.\n";
        cout << "Precautions: Keep skin moisturized and avoid weight fluctuations.\n";
        cout << "Quantity: Apply cream once or twice a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "minor pigmentation")) {
        cout << "Prescription: Use a pigmentation treatment containing vitamin C.\n";
        cout << "Precautions: Avoid sun exposure and wear sunscreen.\n";
        cout << "Quantity: Apply treatment twice a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "freckles")) {
        cout << "Prescription: Use a brightening serum or sunscreen.\n";
        cout << "Precautions: Wear sunscreen regularly and avoid excessive sun exposure.\n";
        cout << "Quantity: Apply sunscreen daily and brightening serum as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "melasma")) {
        cout << "Prescription: Consult a dermatologist for topical treatments.\n";
        cout << "Precautions: Apply sunscreen and avoid sun exposure.\n";
        cout << "Quantity: Follow dermatologist's treatment plan.\n";
    }
    else if (bruteForceStringMatch(symptom, "red nose")) {
        cout << "Prescription: Use a soothing cream or aloe vera gel.\n";
        cout << "Precautions: Avoid harsh products and protect the skin from irritation.\n";
        cout << "Quantity: Apply soothing cream 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "temporary jaw pain")) {
        cout << "Prescription: Consider using a warm compress or anti-inflammatory medication.\n";
        cout << "Precautions: Avoid excessive chewing or clenching of the jaw.\n";
        cout << "Quantity: Apply compress for 10-15 minutes and take medication as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "lockjaw (mild)")) {
        cout << "Prescription: Seek medical attention if symptoms persist or worsen.\n";
        cout << "Precautions: Avoid stress and jaw strain.\n";
        cout << "Quantity: Follow medical advice.\n";
    }
    else if (bruteForceStringMatch(symptom, "dry mouth (temporary)")) {
        cout << "Prescription: Stay hydrated and use saliva substitutes.\n";
        cout << "Precautions: Avoid alcohol and caffeine, which can worsen dryness.\n";
        cout << "Quantity: Drink water regularly and use substitutes as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "metallic taste in mouth")) {
        cout << "Prescription: Rinse with a saline solution or drink plenty of water.\n";
        cout << "Precautions: Avoid strong flavors that may exacerbate the taste.\n";
        cout << "Quantity: Rinse as needed or drink water frequently.\n";
    }
    else if (bruteForceStringMatch(symptom, "teeth sensitivity to cold")) {
        cout << "Prescription: Use toothpaste for sensitive teeth.\n";
        cout << "Precautions: Avoid extremely cold foods and drinks.\n";
        cout << "Quantity: Use sensitive toothpaste regularly.\n";
    }
    else if (bruteForceStringMatch(symptom, "teeth grinding")) {
        cout << "Prescription: Consider using a mouth guard at night.\n";
        cout << "Precautions: Manage stress and avoid caffeine before bedtime.\n";
        cout << "Quantity: Use mouth guard nightly if recommended.\n";
    }
    else if (bruteForceStringMatch(symptom, "chapped gums")) {
        cout << "Prescription: Use a moisturizing mouth rinse or gel.\n";
        cout << "Precautions: Avoid harsh toothpaste and brush gently.\n";
        cout << "Quantity: Use gel or rinse 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "overbrushing damage")) {
        cout << "Prescription: Switch to a soft-bristled toothbrush and use toothpaste for sensitive teeth.\n";
        cout << "Precautions: Brush gently and avoid brushing too often.\n";
        cout << "Quantity: Brush twice a day with gentle strokes.\n";
    }
    else if (bruteForceStringMatch(symptom, "puffy eyelids")) {
        cout << "Prescription: Use a cold compress or an eye cream with caffeine.\n";
        cout << "Precautions: Get adequate sleep and reduce salt intake.\n";
        cout << "Quantity: Apply compress or cream 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "temporary blurry vision")) {
        cout << "Prescription: Rest your eyes and consider using lubricating eye drops.\n";
        cout << "Precautions: Take frequent breaks from screens and ensure proper lighting.\n";
        cout << "Quantity: Use eye drops as needed and rest eyes regularly.\n";
    }
    else if (bruteForceStringMatch(symptom, "dry eyelids")) {
        cout << "Prescription: Use a gentle moisturizing cream or eye ointment.\n";
        cout << "Precautions: Avoid harsh cleansers and rubbing the eyes.\n";
        cout << "Quantity: Apply moisturizer 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "eyelash irritation")) {
        cout << "Prescription: Use a soothing eye gel or saline solution.\n";
        cout << "Precautions: Avoid wearing mascara until the irritation subsides.\n";
        cout << "Quantity: Use soothing gel 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "eyebrow dandruff")) {
        cout << "Prescription: Use a gentle dandruff shampoo or eyebrow scrub.\n";
        cout << "Precautions: Keep eyebrows clean and avoid harsh products.\n";
        cout << "Quantity: Use dandruff shampoo every few days or scrub once a week.\n";
    }
    else if (bruteForceStringMatch(symptom, "eye watering due to wind")) {
        cout << "Prescription: Use lubricating eye drops or wear protective glasses.\n";
        cout << "Precautions: Avoid prolonged exposure to windy conditions.\n";
        cout << "Quantity: Use drops as needed, wear glasses when exposed to wind.\n";
    }
    else if (bruteForceStringMatch(symptom, "eyelid twitching")) {
        cout << "Prescription: Take magnesium supplements or relax your eye muscles.\n";
        cout << "Precautions: Avoid excessive caffeine and reduce stress.\n";
        cout << "Quantity: Take supplements as directed, manage stress daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "eye redness from rubbing")) {
        cout << "Prescription: Use lubricating eye drops or a cold compress.\n";
        cout << "Precautions: Avoid rubbing the eyes and consider allergy relief.\n";
        cout << "Quantity: Apply eye drops 2-3 times a day, cold compress as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "ear itching from earphones")) {
        cout << "Prescription: Clean earphones regularly and use over-the-counter ear drops.\n";
        cout << "Precautions: Avoid wearing earphones for extended periods.\n";
        cout << "Quantity: Apply ear drops as needed and clean earphones before use.\n";
    }
    else if (bruteForceStringMatch(symptom, "pain from tight headphones")) {
        cout << "Prescription: Take breaks from wearing headphones and use cushioned ear pads.\n";
        cout << "Precautions: Adjust the headphones to avoid excessive pressure.\n";
        cout << "Quantity: Take breaks every hour and adjust the fit of headphones.\n";
    }
    else if (bruteForceStringMatch(symptom, "ear pain from cold weather")) {
        cout << "Prescription: Use ear muffs or a warm hat to protect ears from the cold.\n";
        cout << "Precautions: Avoid direct exposure to wind and cold temperatures.\n";
        cout << "Quantity: Wear ear protection whenever outside in cold weather.\n";
    }
    else if (bruteForceStringMatch(symptom, "temporary tinnitus")) {
        cout << "Prescription: Avoid exposure to loud noises and consider using white noise.\n";
        cout << "Precautions: Reduce stress and avoid loud music or environments.\n";
        cout << "Quantity: Use white noise if needed, protect ears from loud sounds.\n";
    }
    else if (bruteForceStringMatch(symptom, "sudden loud noise sensitivity")) {
        cout << "Prescription: Avoid noisy environments and consider wearing ear protection.\n";
        cout << "Precautions: Limit exposure to loud sounds and reduce stress.\n";
        cout << "Quantity: Use ear protection in noisy areas and avoid sudden loud noises.\n";
    }
    else if (bruteForceStringMatch(symptom, "mild ear infection from swimming")) {
        cout << "Prescription: Use ear drops for ear infections or consult a doctor.\n";
        cout << "Precautions: Keep ears dry and avoid swimming until fully healed.\n";
        cout << "Quantity: Use drops as prescribed, consult a doctor if needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "neck stiffness")) {
        cout << "Prescription: Apply heat or cold compress and consider gentle neck stretches.\n";
        cout << "Precautions: Maintain proper posture and avoid prolonged sitting.\n";
        cout << "Quantity: Apply compress as needed, stretch 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "shoulder strain")) {
        cout << "Prescription: Rest the shoulder and apply an ice pack.\n";
        cout << "Precautions: Avoid lifting heavy objects or doing strenuous activities.\n";
        cout << "Quantity: Rest shoulder for a few days, apply ice for 15-20 minutes.\n";
    }
    else if (bruteForceStringMatch(symptom, "lower back discomfort")) {
        cout << "Prescription: Apply heat or cold compress and consider gentle back exercises.\n";
        cout << "Precautions: Avoid heavy lifting and maintain good posture.\n";
        cout << "Quantity: Use compresses as needed, do stretches daily.\n";
    }

    else if (bruteForceStringMatch(symptom, "spine misalignment")) {
        cout << "Prescription: Consult a chiropractor or physical therapist.\n";
        cout << "Precautions: Maintain good posture and avoid heavy lifting.\n";
        cout << "Quantity: Follow therapist's advice for exercises and treatments.\n";
    }
    else if (bruteForceStringMatch(symptom, "hip pain")) {
        cout << "Prescription: Rest and apply an ice pack to the hip.\n";
        cout << "Precautions: Avoid excessive walking and lifting.\n";
        cout << "Quantity: Rest and ice for 15-20 minutes, 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "joint fatigue")) {
        cout << "Prescription: Consider taking glucosamine or other joint supplements.\n";
        cout << "Precautions: Avoid overexertion and engage in gentle exercises.\n";
        cout << "Quantity: Follow the supplement instructions for daily intake.\n";
    }
    else if (bruteForceStringMatch(symptom, "overused wrist strain")) {
        cout << "Prescription: Rest the wrist and use a wrist brace.\n";
        cout << "Precautions: Avoid repetitive wrist motions and heavy lifting.\n";
        cout << "Quantity: Wear a wrist brace and rest as much as possible.\n";
    }
    else if (bruteForceStringMatch(symptom, "knuckle cracking discomfort")) {
        cout << "Prescription: Apply a cold compress and perform gentle stretches.\n";
        cout << "Precautions: Avoid cracking your knuckles frequently.\n";
        cout << "Quantity: Apply cold compress for 10-15 minutes, 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "temporary carpal tunnel symptoms")) {
        cout << "Prescription: Consider wearing a wrist splint during sleep.\n";
        cout << "Precautions: Take regular breaks from typing or repetitive hand motions.\n";
        cout << "Quantity: Wear the splint during the night or as advised by a doctor.\n";
    }
    else if (bruteForceStringMatch(symptom, "finger joint stiffness")) {
        cout << "Prescription: Perform finger stretches and use a hand massager.\n";
        cout << "Precautions: Avoid overexerting the hands and keep joints warm.\n";
        cout << "Quantity: Do gentle stretches 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "big toe pain")) {
        cout << "Prescription: Rest the foot and wear comfortable shoes.\n";
        cout << "Precautions: Avoid high heels or tight shoes.\n";
        cout << "Quantity: Rest the foot, wear appropriate footwear.\n";
    }
    else if (bruteForceStringMatch(symptom, "heel pain")) {
        cout << "Prescription: Use heel pads or orthotics in your shoes.\n";
        cout << "Precautions: Avoid prolonged standing and excessive walking.\n";
        cout << "Quantity: Use heel pads daily and rest when possible.\n";
    }
    else if (bruteForceStringMatch(symptom, "elbow joint cracking")) {
        cout << "Prescription: Apply a warm compress and do light stretching exercises.\n";
        cout << "Precautions: Avoid overexertion and repetitive elbow motions.\n";
        cout << "Quantity: Apply compress for 10-15 minutes and stretch daily.\n";
    }
    else if (bruteForceStringMatch(symptom, "wrist cracking")) {
        cout << "Prescription: Stretch the wrist and apply a warm compress.\n";
        cout << "Precautions: Avoid overuse and strain on the wrist.\n";
        cout << "Quantity: Stretch 2-3 times a day and use compress as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "mild finger swelling")) {
        cout << "Prescription: Elevate the hand and apply a cold compress.\n";
        cout << "Precautions: Avoid tight rings and prolonged use of hands.\n";
        cout << "Quantity: Apply cold compress 10-15 minutes, 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "upper back strain")) {
        cout << "Prescription: Rest the back and use a heating pad.\n";
        cout << "Precautions: Avoid heavy lifting and practice good posture.\n";
        cout << "Quantity: Apply heat for 15-20 minutes, 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "jaw stiffness from chewing")) {
        cout << "Prescription: Apply a warm compress to the jaw and avoid hard foods.\n";
        cout << "Precautions: Avoid clenching and grinding teeth.\n";
        cout << "Quantity: Use compress for 10-15 minutes, 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "mild shoulder clicking")) {
        cout << "Prescription: Perform shoulder stretches and apply a warm compress.\n";
        cout << "Precautions: Avoid heavy lifting and excessive shoulder movements.\n";
        cout << "Quantity: Do stretches daily and apply compress as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "excessive sweating in armpits")) {
        cout << "Prescription: Use an antiperspirant or seek advice for prescription options.\n";
        cout << "Precautions: Wear breathable fabrics and avoid spicy foods.\n";
        cout << "Quantity: Apply antiperspirant daily, as directed.\n";
    }
    else if (bruteForceStringMatch(symptom, "sweaty forehead")) {
        cout << "Prescription: Use an absorbent forehead powder or antiperspirant.\n";
        cout << "Precautions: Keep cool and avoid overheating.\n";
        cout << "Quantity: Apply powder as needed to control sweat.\n";
    }
    else if (bruteForceStringMatch(symptom, "chilled fingers")) {
        cout << "Prescription: Keep hands warm with gloves and ensure proper circulation.\n";
        cout << "Precautions: Avoid prolonged exposure to cold temperatures.\n";
        cout << "Quantity: Wear gloves in cold environments and warm hands regularly.\n";
    }
    else if (bruteForceStringMatch(symptom, "chilled toes")) {
        cout << "Prescription: Wear warm socks and shoes, and keep feet elevated.\n";
        cout << "Precautions: Avoid walking barefoot in cold conditions.\n";
        cout << "Quantity: Wear warm socks and keep feet warm regularly.\n";
    }
    else if (bruteForceStringMatch(symptom, "temporary circulation issues")) {
        cout << "Prescription: Elevate your legs and avoid sitting for long periods.\n";
        cout << "Precautions: Keep your legs moving and avoid tight clothing.\n";
        cout << "Quantity: Elevate legs and take short breaks throughout the day.\n";
    }
    else if (bruteForceStringMatch(symptom, "swollen fingers from heat")) {
        cout << "Prescription: Cool the hands with cold water or a compress.\n";
        cout << "Precautions: Avoid extreme heat and reduce salt intake.\n";
        cout << "Quantity: Apply cold compress 10-15 minutes, 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "temporary hot flashes")) {
        cout << "Prescription: Stay cool with light clothing and drink cold fluids.\n";
        cout << "Precautions: Avoid spicy foods and hot environments.\n";
        cout << "Quantity: Drink cold water and use a fan or cooling gel as needed.\n";
    }
    else if (bruteForceStringMatch(symptom, "red cheeks from exertion")) {
        cout << "Prescription: Rest and hydrate to cool down.\n";
        cout << "Precautions: Avoid vigorous activities in hot weather.\n";
        cout << "Quantity: Drink water and rest until redness subsides.\n";
    }
    else if (bruteForceStringMatch(symptom, "puffy face from sleeping")) {
        cout << "Prescription: Drink water and elevate your head while sleeping.\n";
        cout << "Precautions: Avoid salty foods before bed.\n";
        cout << "Quantity: Drink water in the morning and sleep with head elevated.\n";
    }
    else if (bruteForceStringMatch(symptom, "bruised knees")) {
        cout << "Prescription: Apply ice to reduce swelling and use a bandage if needed.\n";
        cout << "Precautions: Avoid further injury and protect the bruised area.\n";
        cout << "Quantity: Apply ice for 10-15 minutes, 2-3 times a day.\n";
    }
    else if (bruteForceStringMatch(symptom, "twisted wrist")) {
        cout << "Prescription: Rest the wrist and apply a cold compress.\n";
        cout << "Precautions: Avoid using the wrist and keep it elevated.\n";
        cout << "Quantity: Apply cold compress for 10-15 minutes, 2-3 times a day.\n";
    }
     else {
        cout << "Symptom not recognized. Please consult a doctor.\n";
    }
}




using namespace std;

int module4() {
    string symptom;

    cout << "Based on analysis, what are you suffering from: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, symptom);

    giveAdvice(symptom);
    cout << "Get well soon... Have a Great day!!" << endl;

    return 0;
}


int main()
{
    int choice=0;
    do
    {
    std::cout << "-----PATIENT SYMPTOM ANALYSIS AND SELF DIAGNOSIS----" << std::endl;
std::cout << "1. MODULE 1 -- PATIENT AMBIGUOUS SYMPTOM" << std::endl;
std::cout << "2. MODULE 2 -- LIMITED MEDICAL AWARENESS " << std::endl;
std::cout << "3. MODULE 3 -- NEAREST MEDICAL FACILITY " << std::endl;
std::cout << "4. MODULE 4 -- SELF DIAGNOSIS " << std::endl;
std::cout << "5. EXIT " << std::endl;
     cout<<"\nEnter your choice:\t";
     cin>>choice;

     switch(choice)
     {
         case 1:module1();
                break;
         case 2:module2();
                break;
         case 3:module3();
                break;
         case 4:module4();
                break;
         default:cout<<"ENTER VALID CHOICE";
                break;


     }

    }while(choice!=5);

    return 0;
}

