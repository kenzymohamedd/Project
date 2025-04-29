#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include <limits>
using namespace std;

const int SIZE = 5, N = 10 , Max_Exercise = 10; // review later N will increase

struct Measurements {
    float weight, height, bmi;
};

struct Workout {
    string workoutID, workoutName, exercises[SIZE];
    int duration, exerciseCount;
};

struct Client {
    string clientId, cUsername, cPassword;
    Workout workoutplans[SIZE];
    float progresslogs[SIZE] = { -1 };
    Measurements measure;
};

struct Trainer {
    string TrainerID, tUsername, tPassword;
    Client client[N];
};

// Global arrays for Clients and Trainers
Client clients[N];
Trainer trainers[N];
int nextClientID = 1; // ID for clients
int nextTrainerID = 101; // ID for trainers
int clientCount = 0;
int trainerCount = 0;

const string CLIENT_FILE = "clients.txt";
const string TRAINER_FILE = "trainers.txt";

void mainMenu() {
    cout << "1. Register as Client\n";
    cout << "2. Register as Trainer\n";
    cout << "3. Login\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

void readPassword(string& password, char& pass) {
    password.clear();
    do {
        pass = _getch();
        if (pass == 13) {
            break;
        }
        else if (pass == 8) {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password += pass;
            cout << "*";
        }
    } while (true);
    cout << endl;
}
//***
bool isUsernameTaken(char choiceOfRegister, const std::string& userName) {
    if (choiceOfRegister == 'c' || choiceOfRegister == 'C') {
        for (int i = 0; i < clientCount; i++) {
            if (clients[i].cUsername == userName) {
                return true;
            }
        }
    }
    else if (choiceOfRegister == 't' || choiceOfRegister == 'T') {
        for (int i = 0; i < trainerCount; i++) { 
            if (trainers[i].tUsername == userName) {
                return true;
            }
        }
    }
    return false;
} 

void loadData() {
    ifstream clientFile("clients.txt");
    ifstream trainerFile("trainers.txt");

    if (clientFile.is_open()) {
        while (clientFile >> clients[clientCount].clientId >> clients[clientCount].cUsername >> clients[clientCount].cPassword) {
            clientCount++;
        }
        clientFile.close();
    }


    if (trainerFile.is_open()) {
        while (trainerFile >>trainers[trainerCount].TrainerID>> trainers[trainerCount].tUsername >> trainers[trainerCount].tPassword) {
            trainerCount++;
        }
        trainerFile.close();
    }
}
Workout workouts[SIZE] = {
{"W001", "Full Body Blast", {"Squats", "Push-ups", "Pull-ups", "Plank", "Lunges"}, 60, 5 },
{ "W002", "Cardio Power", {"Running", "Jumping Jacks", "Burpees", "Mountain Climbers"}, 45, 4 },
{ "W003", "Strength Focus", {"Bench Press", "Deadlifts", "Overhead Press", "Rows"}, 75, 4 },
{ "W004", "Leg Day",  {"Squats", "Lunges", "Leg Press", "Calf Raises"}, 55, 4 },
{ "W005", "Core Circuit", {"Crunches", "Leg Raises", "Russian Twists", "Plank"}, 30, 4 }
};
int workoutCount = 5; 


// Function to find if the username is unique
Client* findClient(const string& id) {
    int left = 0;
    int right = clientCount - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (clients[mid].clientId == id) {
            return &clients[mid];
        }
        else if (clients[mid].clientId < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return nullptr; // Client not found
}

Trainer* findTrainerById(const string& id) {
	int left = 0, right = trainerCount - 1; 
	while (left <= right) {
		int mid = left + (right - left) / 2;
		if (trainers[mid].TrainerID == id) {
			return &trainers[mid];
		}
		else if (trainers[mid].TrainerID < id) {
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}   
	return nullptr; // Trainer not found
}
// Functions to find clients and trainers ID using binary search K

//Fatma

void registerClient() {
    char choiceOfRegister = 'c';
    if (clientCount >= N) {
        cout << "Maximum number of clients reached.\n";
        return;
    }

    Client& newClient = clients[clientCount];

    cout << "--- Register as Client ---" << endl;

    do {
        cout << "Username: ";
        cin >> newClient.cUsername;
        if (newClient.cUsername.empty()) {
            cout << "Error: Username cannot be empty. Please try again.\n";
            continue;
        }
        if (isUsernameTaken(choiceOfRegister, newClient.cUsername)) {
            cout << "This username is taken. Choose another one.\n";
        }
        else {
            break;
        }
    } while (true);

    cout << "Password: ";
    char pass;
    readPassword(newClient.cPassword, pass);
    if (newClient.cPassword.empty()) {
        cout << "Error: Password cannot be empty. Registration failed.\n";
        return;
    }

    ofstream clientFile(CLIENT_FILE, ios::app);
    if (clientFile.is_open()) {
        clientFile << newClient.clientId << '\n' << newClient.cUsername << '\n' << newClient.cPassword << endl;
        clientFile.close();
        cout << "Client registered successfully!\n";
		cout << "Your ID is: " << nextClientID << endl;
		clients[clientCount].clientId = to_string(nextClientID++);
        clientCount++;
        cout << "clientCount: " << clientCount;
    }
    else {
        cerr << "Error: Unable to open client file for writing." << endl;
    }
}


void registerTrainer() {
	char choiceOfRegister = 't';
    if (trainerCount >= N) {
        cout << "Maximum number of trainers reached.\n";
        return;
    }

    Trainer& newTrainer = trainers[trainerCount];
    char pass;

    cout << "--- Register as Trainer ---" << endl;

    do {
        cout << "Username: ";
        cin >> newTrainer.tUsername;
        if (newTrainer.tUsername.empty()) {
            cout << "Error: Username cannot be empty. Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (isUsernameTaken(choiceOfRegister, newTrainer.tUsername)) {
            cout << "This username is taken. Choose another one.\n";
        }
        else {
            break;
        }
    } while (true);

    cout << "Password: ";
    readPassword(newTrainer.tPassword, pass);
    if (newTrainer.tPassword.empty()) {
        cout << "Error: Password cannot be empty. Registration failed.\n";
        return;
    }

    ofstream trainerFile(TRAINER_FILE, ios::app);
    if (trainerFile.is_open()) {
        trainerFile <<newTrainer.TrainerID <<'\n' << newTrainer.tUsername << '\n' << newTrainer.tPassword << endl;
		trainerFile.close();
        cout << "Trainer registered successfully!\n";
		cout << "Your ID is: " << nextTrainerID << endl;
		trainers[trainerCount].TrainerID = to_string(nextTrainerID++);
        trainerCount++;
    }
    else {
        cerr << "Error: Unable to open trainer file for writing." << endl;
    }
}

string loginUser() {
    string username, password;
    string storedUsername, storedPassword;
    char pass;

    cout << "--- Login ---" << endl;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    readPassword(password, pass);

    for (int i = 0; i < clientCount; i++) {
        if (clients[i].cUsername == username && clients[i].cPassword == password) {
            cout << "Client login successful!\n";
            return "client";
        }
    }
    for (int i = 0; i < trainerCount; i++) {
        if (trainers[i].tUsername == username && trainers[i].tPassword == password) {
            cout << "Logged in successfully as Trainer!\n";
            return "trainer";
        }
    }

    cout << "Login failed. Invalid username or password.\n";
    return "";
}


//Kariman Functions
void deleteWorkout() {
    string id;
    cout << "Enter workout ID to delete: ";
    cin >> id;

    bool found = false;

    for (int i = 0; i < workoutCount; ++i) {
        if (workouts[i].workoutID == id) {
            char confirm;//Confirm deletion
            cout << "Are you sure you want to delete workout \"" << workouts[i].workoutName << "\"? (Y/N): ";
            cin >> confirm;

            if (confirm == 'Y' || confirm == 'y') {

                for (int j = i; j < workoutCount - 1; ++j) {
                    workouts[j] = workouts[j + 1];
                }
                workoutCount--;
                found = true;
                cout << "Workout deleted successfully.\n";
            }
            else {
                cout << "Deletion cancelled.\n";
            }

            break;
        }
    }

    if (!found) {
        cout << "Workout not found.\n";
    }
}
void editWorkout() {

    string id;
    cout << "Enter workout ID to edit: ";
    cin >> id;

    for (int i = 0;i < workoutCount;i++) {
        if (workouts[i].workoutID == id) {
            cout << "Editing workout:" << workouts[i].workoutName << endl;

            cout << "Enter new workout name :";
            cin >> workouts[i].workoutName;

            cout << "Enter new number of exercises :";
            cin >> workouts[i].exerciseCount;
            for (int j = 0;j < workouts[i].exerciseCount;j++) {
                cout << "Exercises name " << (j + 1) << ":";
                cin >> workouts[i].exercises[j];
            }
            cout << "Enter new duration:";
            cin >> workouts[i].duration;

            cout << "workout done!\n";

            return;
        }
    }
    cout << "workout not found";

}
void listAllWorkouts() {
    if (workoutCount == 0) {
        cout << "No workouts available.\n";
        return;
    }
    for (int i = 0;i < workoutCount;i++) {
        cout << "\nworkout ID :" << workouts[i].workoutID << endl;
        cout << "workout name :" << workouts[i].workoutName << endl;
        cout << "Exercises:\n";
        for (int j = 0; j < workouts[i].exerciseCount; ++j) {
            cout << "  - " << workouts[i].exercises[j] << endl;
        }
        cout << "duration in minutes:" << workouts[i].duration << endl;
    }
}
//_________________________
void unassignWorkoutFromClient(Trainer MyTrainer[10]) {
    string ClientID;
    string workID;
    bool CIDFOUND = false;
    bool WIDFOUND = false;
    cout << "Enter The Client's ID: ";
    cin >> ClientID;
    cout << "Enter The Workout's ID: ";
    cin >> workID;
    for (int i = 0;i < 10;i++) {
        for (int j = 0;j < 10;j++) {
            if (ClientID == MyTrainer[i].client[j].clientId) {
                CIDFOUND = true;
                for (int n = 0;n < SIZE;n++) {
                    if (workID == MyTrainer[i].client[j].workoutplans[n].workoutID) {
                        WIDFOUND = true;
                        MyTrainer[i].client[j].workoutplans[n].workoutName = "";
                        MyTrainer[i].client[j].workoutplans[n].workoutID = "";
                        for (int k = 0;k < N;k++) {
                            MyTrainer[i].client[j].workoutplans[n].exercises[k] = "";
                        }
                        MyTrainer[i].client[j].workoutplans[n].duration = 0;
                        MyTrainer[i].client[j].workoutplans[n].exerciseCount = 0;
                        cout << "Workout Deleted Successfully! \n";
                        break;
                    }

                }
            }
        }
    }
}


// Kamila clients
string ClientMenu() {
    string choice;
    while (true) {
        cout << "\t\t\t\t\tClient Workout Tracking Menu\n";
        cout << "\t\t\t\t\t****************************\n";
        cout << "Press" << endl;
        cout << "1. View Assigned Workouts" << endl;
        cout << "2. log Completed Workout" << endl;
        cout << "3. View Workout History" << endl;
        cout << "4. View Workout Progress Summary" << endl;
		cout << "5. Add Measurement" << endl;
		cout << "6. View Measurements" << endl; 
		cout << "0. Exit" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;
        if (choice >= "0" && choice <= "6") {
            break;
        }
        else {
            cout << "Invalid choice! Please enter a number between 1 and 4." << endl;
        }
    }
    return choice;
}


void viewAssignedWorkouts(Client& currentClient) {
    cout << "\t***Your Assigned Workouts***\n\n";

    int foundWorkouts = 0;

    for (int i = 0; i < SIZE; i++) {
        if (currentClient.workoutplans[i].workoutID != "") {
            foundWorkouts++;
            cout << foundWorkouts << ". Workout #" << foundWorkouts << ":" << endl;
            cout << "ID : " << currentClient.workoutplans[i].workoutID << endl;
            cout << "Name : " << currentClient.workoutplans[i].workoutName << endl;
            cout << "Duration : " << currentClient.workoutplans[i].duration << endl;
            if (currentClient.workoutplans[i].exerciseCount > 0) {
                cout << "Exercises :" << endl;
                for (int j = 0; j < currentClient.workoutplans[i].exerciseCount; j++) {
                    cout << "- " << currentClient.workoutplans[i].exercises[j] << endl;
                }
            }
            cout << "----------------------------------------" << endl;
        }
    }
    if (foundWorkouts == 0) {
        cout << "No workouts found! Ask your trainer to assign some." << endl;
    }
}

void logCompletedWorkout(Client& currentClient) {
    cout << "\t***Log Completed Workout***\n\n";

    viewAssignedWorkouts(currentClient);
    cout << endl;

    char ans = 'y';
    do {
        int workoutNumber;
        cout << "Enter the number of the Workout you completed: ";
        cin >> workoutNumber;

        int realIndex = -1;
        int count = 0;
        for (int i = 0; i < SIZE; i++) {
            if (currentClient.workoutplans[i].workoutID != "") {
                count++;
                if (count == workoutNumber) {
                    realIndex = i;
                    break;
                }
            }
        }
        if (realIndex == -1) {
            cout << "Invalid workout number!" << endl;
            continue;
        }
        cout << endl;

        float percent;
        do {
            cout << "How much of " << currentClient.workoutplans[realIndex].workoutName << " did you complete (0-100%)? ";
            cin >> percent;
            cout << endl;

            if (percent < 0 || percent > 100) {
                cout << "Invalid input! Please enter a number between 0 and 100." << endl;
            }
        } while (percent < 0 || percent > 100);

        for (int i = 0; i < SIZE; i++) {
            if (currentClient.progresslogs[i] == -1) {
                currentClient.progresslogs[i] = percent;
                cout << "Progress logged: You completed " << percent << "% of " << currentClient.workoutplans[realIndex].workoutName << "." << endl;
                break;
            }
        }
        cout << "Do you want to log another workout? (Y/N)";
        cin >> ans;
        while (ans != 'Y' && ans != 'y' && ans != 'N' && ans != 'n') {
            cout << "Invalid input! Please enter 'Y' for yes or 'N' for no." << endl;
            cout << "Enter again: ";
            cin >> ans;
        }
    } while (ans == 'y' || ans == 'Y');
}

void viewWorkoutHistory(Client& currentClient) {
    cout << "\t***Workout History***\n\n";

    bool hasHistory = false;
    for (int i = 0; i < SIZE; i++) {
        if (currentClient.workoutplans[i].workoutID != "" && currentClient.progresslogs[i] != -1) {
            hasHistory = true;
            cout << "Workout " << i + 1 << ": " << currentClient.workoutplans[i].workoutName << endl;
            cout << "Completed: " << currentClient.progresslogs[i] << " %" << endl;
            cout << "----------------------------------------" << endl;
        }
    }
    if (hasHistory == false) {
        cout << "No workout history found. Please log some completed workouts!" << endl;
    }
}

void summarizeWorkoutProgress(Client& currentClient) {
    cout << "\t***Workout Progress Summary***\n\n";

    float sum = 0.0;
    int loggedCount = 0;
    int totalAssigned = 0;
    int fullyComplete = 0;
    for (int i = 0; i < SIZE; i++) {
        if (currentClient.workoutplans[i].workoutID != "") {
            totalAssigned++;
            if (currentClient.progresslogs[i] != -1) {
                loggedCount++;
                sum += currentClient.progresslogs[i];
                if (currentClient.progresslogs[i] == 100.0) {
                    fullyComplete++;
                }
            }
        }
    }
    cout << "Total Workouts Completed: " << fullyComplete << " out of " << totalAssigned << endl;

    cout << "----------------------------------------" << endl;

    if (loggedCount == 0) {
        cout << "Average Progress : 0% (No progress logged yet)" << endl;
        cout << "It looks like you haven't logged any progress yet. Let's get started and make that first step today!" << endl;
    }
    else {
        cout << "Average Progress: " << (sum / loggedCount) << " %" << endl;
        if (fullyComplete == totalAssigned && totalAssigned != 0) {
            cout << "Fantastic job! You've completed all your workouts with full dedication! Keep it up!" << endl;
        }
        else {
            cout << "You're making excellent progress! Keep pushing towards your goal, you're on the right track." << endl;
        }
    }
    cout << "----------------------------------------" << endl;
}

//Laila clients measurement

void addMeasurement(Client& client) {
    cout << "\nAdd your new Measurements " << client.clientId << "):\n";
    cout << "Enter Weight (in kg): ";
    cin >> client.measure.weight;
    cout << "Enter Height (in meters): ";
    cin >> client.measure.height;

    if (client.measure.height > 0) {
        client.measure.bmi = client.measure.weight / (client.measure.height * client.measure.height);
        cout << "BMI calculated: " << client.measure.bmi << "\n";
    }
    else {
        cout << "Invalid height entered. BMI not calculated.\n";
    }
    cout << "Measurement added successfully!\n";
}

void viewMeasurements(const Client& client) {
    cout << "\nMeasurement History for Client (ID: " << client.clientId << "):\n";
    cout << "Weight: " << client.measure.weight << " kg\n";
    cout << "Height: " << client.measure.height << " m\n";
    cout << "BMI: " << client.measure.bmi << "\n";
}


void clientActions() {
    string loggedinID, answer, clientChoice;
	cout << "Enter your ID to proceed: \n";
	cin >> loggedinID;
	Client* currentClient = findClient(loggedinID);
    do {
        clientChoice = ClientMenu();
        if (clientChoice == "1") {
            viewAssignedWorkouts(*currentClient);
        }
        else if (clientChoice == "2") {
            logCompletedWorkout(*currentClient);
        }
        else if (clientChoice == "3") {
            viewWorkoutHistory(*currentClient);
        }
        else if (clientChoice == "4") {
            summarizeWorkoutProgress(*currentClient);
        }
        else if (clientChoice == "5") {
            addMeasurement(*currentClient);
        }
        else if (clientChoice == "6") {
            viewMeasurements(*currentClient);
        }
		else if (clientChoice == "0") {
			cout << "Exiting the client menu.\n";
			break;
		}
		else {
			cout << "Invalid choice! Please try again.\n";
		}
         cout << "Do you want to do another operation? (Y/N) " << endl;
         cin >> answer;
         while (answer != "Y" && answer != "y" && answer != "N" && answer != "n") {
            cout << "Invalid input! Please enter 'Y' for yes or 'N' for no." << endl;
            cout << "Enter again: ";
            cin >> answer;
            }
            cout << "\t\t ------------------------------------------------------\n";
        } while (answer == "Y" || answer == "y");

        }
   


void viewAssignedClients(Trainer trainers[10]) {

    cout << "To Show Your Assigned Clients Please Enter Your ID: ";
    string IDToBeFound;
    bool IDFound = false;
    cin >> IDToBeFound;
    for (int i = 0;i < 10;i++) {
        cout << "Searching for ID: " << IDToBeFound << endl;
        cout << "Comparing with Trainer ID: " << trainers[i].TrainerID << endl;
        //will comment
        if (IDToBeFound == trainers[i].TrainerID) {
            IDFound = true;
            cout << "Your CLients:(Name,ID,Weight,Height,bmi,progress,Workout name,Workout ID,Exercises,Count,Duration) \n";
            for (int j = 0;j < 10;j++) {
                cout << trainers[i].client[j].cUsername << '\t';
                cout << trainers[i].client[j].clientId << '\t';
                cout << trainers[i].client[j].measure.weight << '\t';
                cout << trainers[i].client[j].measure.height << '\t';
                cout << trainers[i].client[j].measure.bmi << '\t';
                cout << trainers[i].client[j].progresslogs[j] << '\t';
                cout << trainers[i].client[j].workoutplans[j].workoutName << '\t';
                cout << trainers[i].client[j].workoutplans[j].workoutID << '\t';
                cout << trainers[i].client[j].workoutplans[j].exercises[j] << '\t';
                cout << trainers[i].client[j].workoutplans[j].exerciseCount << '\t';
                cout << trainers[i].client[j].workoutplans[j].duration << '\n';
            }
			cout << "----------------------------------------\n";
            break;
        }
    }

    if (!IDFound) {
        cout << "ID NOT FOUND \n";
    }
}



void getClientsSummary(Trainer trainers[N]) {
    cout << "To Show A Summary of Your Assigned Clients (Their Name and Progress) Please Enter Your ID: ";
    string inputID;
	cin >> inputID;
    Trainer* currentTrainer = findTrainerById(inputID);
    if (currentTrainer != nullptr) {
            cout << "Your CLients: \n";
            for (int j = 0;j < 10;j++) {
                cout << "Client Name:" << currentTrainer->client[j].cUsername << endl;
                cout << "Client Progress:" << currentTrainer->client[j].progresslogs[0] << endl;
            }
        }
    }



void trainerActions() {
        cout << "\n--- Trainer Actions ---" << endl;
        char trainerChoice;
        while (true) {
            cout << "Would you like to:\n";
            cout << "1. View Your Clients\n";
            cout << "2. Get a Summary of Your Clients' Progress\n";
            cout << "3. Add Workout to Client\n";
            cout << "4. Remove Workout from Client\n";
            cout << "5. Edit  Workout\n";
            cout << "6. Delete  Workout\n";
            cout << "7. List All  Workouts\n";
            cout << "0. Go back to the Main Menu\n";
            cout << "Enter your choice: ";
            cin >> trainerChoice;
            cout << "~ ~ ~ ~ ~ ~ ~ ~ \n";

            switch (trainerChoice) {
            case '1': {
                viewAssignedClients(trainers);
                cout << "~ ~ ~ ~ ~ ~ ~ ~ \n";
                break;
            }
            case '2': {
                getClientsSummary(trainers);
                cout << "~ ~ ~ ~ ~ ~ ~ ~ \n";
                break;
            }
			case '3': {
				cout << "Add Workout to Client\n";
				
				break;
			}
            case '4': {
                cout << "Remove Workout from Client\n";
				unassignWorkoutFromClient(trainers);    
                break;
            }
            case'5': {
				cout << "Edit Workout\n";
				editWorkout();
				break;
            }
            case '6': {
                cout << "Delete Workout\n";
                deleteWorkout();
                break;
            }
            case '7': {
                cout << "List All Workouts\n";
                listAllWorkouts();
                break;
            }
            case '0':
                cout << "Returning to the main menu.\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

void takeChoice() {
    int choice;
    mainMenu();
    do {
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            choice = -1;
        }

        switch (choice) {
        case 1: {
            registerClient();
            break;
        }
        case 2: {
            registerTrainer();
            break;
        }
        case 3: {
            string loggedInAs = loginUser();
            if (!loggedInAs.empty()) {
                cout << "Welcome!\n";
                if (loggedInAs == "client") {
                    clientActions();
                }
                else if (loggedInAs == "trainer") {
                    trainerActions();
                }
            }
            break;
        }
        case 0: {
            cout << "Exiting the application. Goodbye!\n";
            break;
        }
        default: {
            cout << "Invalid choice please try again\n";
            break;
        }
        }
    } while (choice != 0);
}


int main() {
    loadData();
    // Initialized workouts as they aren't in a file
    cout << "--- Welcome to our fitness app ---" << endl;
	takeChoice();

    return 0;
}
