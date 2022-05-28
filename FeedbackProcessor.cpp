#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

#define MAX_NUM_FEEDBACK_QUESTIONS 10
const string feedbackFname = "Database/User Feedback.txt";

void openInput(ifstream &, string);
void displayResult(vector<int>);
void openInput(ifstream &fname, string filepath)
{
    fname.open(filepath.c_str());
    if (fname.fail()) // check for a successful open
    {
        cout << "\nFailed to open the file named " << filepath << " for read"
             << "\n Please check that this file exists"
             << endl;
        exit(1);
    }
    return;
}
vector<int> feedbackProcessor(ifstream &inFeedbackDataBase) {
    if (inFeedbackDataBase.is_open()) {
        vector<int> scoreList;
        string line;
        string myString;
        string tempString;
        while(getline(inFeedbackDataBase, line)) 
        {
            int oddQuestionScore = 0;
            int evenQuestionScore = 0;
            stringstream ss(line);
            for (int i = 1; i <= MAX_NUM_FEEDBACK_QUESTIONS; i++) 
            {
                getline(ss, tempString, ','); //read question
                if (i % 2 == 1) {
                    getline(ss,myString,',');
                    int score = stoi(myString);
                    oddQuestionScore += (score);
                } else {
                    getline(ss,myString,',');
                    int score = stoi(myString);
                    evenQuestionScore += (score);
                }
            }
            int SUS_score = (oddQuestionScore + (20 - evenQuestionScore)) * 2.5;
            scoreList.push_back(SUS_score);
        }
        return scoreList;
    }
    else {
        cout << "\nFailed to open the file named User Feedback.txt for read \n Please check that this file exists"
             << endl;
        exit(1);
    }
}
void displayResult(vector<int> scoreList) {
    int sum = 0;
    if (scoreList.size() > 0) 
    {
        for (int i = 0; i < scoreList.size(); i++) {
            cout<<"Score "<<i+1<<" : "<<scoreList[i]<<"\n";
            sum += scoreList[i];
        }
        cout<<"------------------------------------\n";
        int average = sum / scoreList.size();
        cout<<"SUS Score Average: "<< average<<"\n";
        cout<<"\n______________SCALE_______________\n";
        cout<<"--0-(worst imaginable)-----10------20------30-(poor)-----40------50-(ok)------60------70-(good)-----80-(excellent)-----90-(best imaginable)-----100--\n";
        cout<<"Your Software's Acceptability level: ";
        if      (average > 90) {cout<< "BEST IMAGINABLE";}
        else if (average > 80) {cout<< "EXECELLENT";}
        else if (average > 70) {cout<< "GOOD";}
        else if (average > 50) {cout<< "OK";}
        else if (average > 30)   {cout<< "POOR";}
        else if (average > 0)  {cout<< "WORST IMAGINABLE";}
    } else {
        cout<<"Data is missing - Unable to calculate SUS score\n";
    }

}
int main() {
    vector<int> SUS_scoreList;
    ifstream inFeedbackDataBase;
    openInput(inFeedbackDataBase,feedbackFname);
    SUS_scoreList = feedbackProcessor(inFeedbackDataBase);
    displayResult(SUS_scoreList);
}

