// quiz.cpp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Question {
    string q,a,b,c,d;
    char correct; // 'A'..'D'
};
vector<Question> qs;

bool loadQuiz(const string& path){
    ifstream f(path);
    if(!f) return false;
    qs.clear();
    string line;
    while (true){
        Question Q;
        if(!getline(f,Q.q)) break;
        if(Q.q.empty()) continue;     // skip blank lines
        getline(f,Q.a); getline(f,Q.b);
        getline(f,Q.c); getline(f,Q.d);
        string corr; if(!getline(f,corr)) break;
        Q.correct = toupper(corr[0]);
        getline(f,line);              // read the blank separator
        qs.push_back(Q);
    }
    return !qs.empty();
}

char askOne(const Question& Q){
    cout << "\n" << Q.q << "\n";
    cout << "A) " << Q.a << "\nB) " << Q.b << "\nC) " << Q.c << "\nD) " << Q.d << "\n";
    cout << "Your answer (A/B/C/D): ";
    char ch; cin >> ch; ch = toupper(ch);
    while (ch<'A' || ch>'D'){ cout<<"Enter A/B/C/D: "; cin>>ch; ch=toupper(ch); }
    return ch;
}

char grade(double p){
    if (p>=90) return 'A';
    if (p>=75) return 'B';
    if (p>=60) return 'C';
    if (p>=40) return 'D';
    return 'F';
}

int main(){
    string path = "quiz.txt";
    cout << "Quiz file [press Enter for quiz.txt]: ";
    string in; 
    // read whole line; user may just press Enter
    getline(cin, in);
    if(!in.empty()) path = in;

    if(!loadQuiz(path)){
        cout << "Could not load questions from '" << path
             << "'. Make sure the file is in the same folder.\n";
        return 0;
    }

    // shuffle questions for variety
    random_device rd; mt19937 rng(rd());
    shuffle(qs.begin(), qs.end(), rng);

    int correct=0;
    for (auto &Q: qs){
        char ans = askOne(Q);
        if (ans==Q.correct){ cout<<"Correct!\n"; correct++; }
        else cout<<"Wrong. Correct is "<<Q.correct<<"\n";
    }
    double perc = (qs.empty()?0 : (correct*100.0/qs.size()));
    cout << "\nScore: " << correct << "/" << qs.size()
         << "  (" << fixed << setprecision(2) << perc << "%)"
         << "  Grade: " << grade(perc) << "\n";
}
