#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include<algorithm>

using namespace std;
using namespace std::chrono;


// Define constants for simulation
const int MAX_OVERS = 10;
const int MAX_BALLS_PER_OVER = 6;

// Function to clear the console screen 
void clearScreen() {
    this_thread::sleep_for(3s);
    system("cls");
}

int option = 1;

class Player {
public:
    string name;

    int runsScored = 0;
    int ballsFaced = 0;
    int wicketsTaken = 0;
    int ballsBowled = 0;
    int battingOrder;
    
    Player* nextBatsman;
    Player* nextBowler;

    Player(const string playerName,int order, Player* nextBatsman, Player* nextBowler)
        : name(playerName),battingOrder(order),
        nextBatsman(nullptr), nextBowler(nullptr) {
        this->nextBatsman = nextBatsman;
        this->nextBowler = nextBowler; 
    }
    ~Player() {}

    string getName() {
        return name;
    }
    

    // Function to calculate and return the player's batting score
    int calculateBattingScore() {
        // Score between 0 and 6.
        int randomScore = rand() % 7;
        return randomScore;
    }

    // Function to calculate and return the player's bowling score
    int calculateBowlingScore() {
        // Wicket-taking score between 0 and 2.
        int randomScore = rand() % 3;
        return randomScore;
    }
};

class Team {
public:
    string name;
    Player* captain;
    Player* nextPlayer;
    int teamscore;
    

    Team(string teamName) : name(teamName), captain(nullptr) {
        Player* tempplayer = new Player("sds", 0, nullptr, nullptr);
        nextPlayer = tempplayer;
    }
    ~Team(){}

    string getTeamName() {
        return name;
    }

    // Add methods to add players, set captains, openers, etc.
    void addPlayer(string name, int battingorder) {
        if (nextPlayer-> getName() == "sds") {
            Player* newplayer = new Player(name,battingorder,nullptr, nullptr);
            nextPlayer = newplayer;
        }
        else if (nextPlayer->nextBatsman == nullptr) {
            Player* newplayer = new Player(name, battingorder,nullptr, nextPlayer);
            nextPlayer->nextBatsman = newplayer;
        }
        else {
            Player* tempPlayer = nextPlayer;
            while (tempPlayer->nextBatsman != nullptr) {
                tempPlayer = tempPlayer->nextBatsman;
            }
            Player* newplayer = new Player(name, battingorder,nullptr, tempPlayer);
            tempPlayer->nextBatsman = newplayer;
        }
    }
    // Function to set the captain
    void setCaptain(Player* player) {
        captain = player;
    }

    // Function to display team players
    void displayPlayers() {
        cout << "Players of Team " << name << ":" << endl;
        Player* player = nextPlayer;
      
        while (player != nullptr) {  
            if(player==captain){
                cout << player->getName() <<"(Captain)"<< endl;
            }
            else {
                cout << player->getName() << endl;
            }
            player = player->nextBatsman;
        }
        
    }
};

class Scoreboard {
private:
    int score = 0;
    int overs = 0;
    int wickets = 0;
    int balls = 0;

    Team* battingTeam;
    Team* bowlingTeam;

    Player* striker;
    Player* nonStriker;
    Player* bowler;

public:
    Scoreboard(Team* teamA, Team* teamB) : battingTeam(teamA), bowlingTeam(teamB) {
        striker = battingTeam->nextPlayer;
        nonStriker = battingTeam->nextPlayer->nextBatsman;
        Player* temp = bowlingTeam->nextPlayer;
        while (temp->nextBatsman != nullptr) {
            temp = temp->nextBatsman;
        }
        bowler = temp;
    }
    ~Scoreboard() {}

    int getOvers() const {
        return overs;
    }
    int getBalls() {
        return balls;
    }


    // Function to switch striker and non-striker
    void switchStrikers(Player* outedbatman) {
        if (outedbatman == striker) {
            striker = nonStriker;
            nonStriker = nonStriker->nextBatsman;
        }
        else {
            nonStriker = nonStriker->nextBatsman;
        }
    }

    // Function to switch bowlers
    void switchBowler() {
        bowler = bowler->nextBowler;
    }

    void swapStriker() {
        Player* temp = striker;
        striker = nonStriker;
        nonStriker = temp;
    }
    void showBatsmans() {
        cout << "Striker  " << striker->getName() << "\tNon Striker  " << nonStriker->getName() << endl;
    }
    void showBowler() {
        cout << "Now bowling " << bowler->getName() << endl;
    }

    void endOfFirstInnings() {
        // Swap two teams
        Team* temp = battingTeam;
        battingTeam = bowlingTeam;
        bowlingTeam = temp;

        // Reset batsmen and baller for team 1
        striker = battingTeam->nextPlayer;
        nonStriker = battingTeam->nextPlayer->nextBatsman;

        // Find the last player in team 2 to set as the baller
        Player* temphead = bowlingTeam->nextPlayer;
        while (temphead->nextBatsman != nullptr) {
            temphead = temphead->nextBatsman;
        }
        bowler = temphead;

    }

    void play() {
        cout << "--------------------Cricket match starts!--------------------" << endl<<endl;

        for (overs = 1; overs <= MAX_OVERS; overs++) {
            cout << "\nOver " << overs << " begins!" << endl;

            for (balls = 1; balls <= 6; balls++) {
                cout << "\nBall " << balls << "..." << endl;
                this_thread::sleep_for(3s);

                int random_event = rand() % 9; // Generate a random number between 0 and 6

                // Simulate a ball delivery
                if (random_event == 0) {
                    cout << "Wide Ball!" << endl;
                    score += 1; // Increase the score by 1
                    battingTeam->teamscore++; // Increase the team score by 1
                }
                else if (random_event == 5) {
                    cout << "No Ball!" << endl;
                    score += 1; // Increase the score by 1
                    battingTeam->teamscore++; // Increase the team score by 1
                }
                else {
                    // A regular ball
                    if (random_event == 6) {
                        cout << "It's a Six!" << endl;
                        score += 6; // Increase the score by 6
                        battingTeam->teamscore+=6; // Increase the team score by 6
                    }
                    else if (random_event == 7) {
                        cout << "It's a Four!" << endl;
                        score += 4; // Increase the score by 4
                        battingTeam->teamscore += 4; // Increase the team score by 4
                    }
                    else if (random_event == 8) { // Check if the batsman is out
                        cout << "Batsman " << striker->getName() << " is out!" << endl;
                        wickets += 1;
                        striker->nextBatsman = nullptr;

                        // Get the next batsman
                        switchStrikers(striker);
                    }
                    else {
                        cout << "Batsman plays the ball..." << endl;
                        score += random_event; // Increase the score by the runs scored
                        battingTeam->teamscore += random_event; // Increase the team score
                    }

                    // Update the batsman's statistics
                    striker->runsScored += random_event;
                    striker->ballsBowled += 1;
                }

                if (random_event != 0 && random_event != 5) {
                    // Update the overs, wickets, and swap the batsmen if needed
                    if (random_event % 2 != 0 && random_event != 7) {
                        swapStriker();
                    }
                    striker->ballsBowled += 1;

                    if (balls == 6) {
                        swapStriker();
                    }
                } 

                // Show the current scoreboard
                cout << "Score: " << score << "/" << wickets << " in " << overs << " overs" << endl;
                showBatsmans();

                if (wickets == 10) {
                    cout << "All out!" << endl;
                    break; // All out
                }

                // Show the current baller
                showBowler();

                if (overs == MAX_OVERS && balls == 6) {
                    cout << "\n\nFirst Innings Ended!" << endl;
                    cout << "Score: " << score << "/" << wickets << " in " << overs << " overs" << endl;
                    break; // End of the first innings
                }
            }

            if (wickets == 10) {
                cout << "\n\nFirst Innings Ended!" << endl;
                cout << "Score: " << score << "/" << wickets << " in " << overs << " overs" << endl;
                break; // End of the first innings
            }
        }

        cout << "\n\n*********Innings Break!*********" << endl;
        cout << "Runs to win : " << score+1  << " in " << --overs << " overs" << endl;

        // Reset the scoreboard for the second innings
        score = 0;
        balls = 0;
        overs = 0;
        wickets = 0;

        // Switch teams for the second innings
        endOfFirstInnings();

        for (overs = 1; overs <= MAX_OVERS; overs++) {
            cout << "\nOver " << overs << " begins!" << endl;

            for (balls = 1; balls <= 6; balls++) {
                cout << "\nBall " << balls << "..." << endl;
                this_thread::sleep_for(3s);

                int random_event = rand() % 9; // Generate a random number between 0 and 6

                // Simulate a ball delivery
                if (random_event == 0) {
                    cout << "Wide Ball!" << endl;
                    score += 1; // Increase the score by 1
                    battingTeam->teamscore++; // Increase the team score by 1
                }
                else if (random_event == 5) {
                    cout << "No Ball!" << endl;
                    score += 1; // Increase the score by 1
                    battingTeam->teamscore++; // Increase the team score by 1
                }
                else {
                    // A regular ball
                    if (random_event == 6) {
                        cout << "It's a Six!" << endl;
                        score += 6; // Increase the score by 6
                        battingTeam->teamscore+=6; // Increase the team score by 6
                    }
                    else if (random_event == 7) {
                        cout << "It's a Four!" << endl;
                        score += 4; // Increase the score by 4
                        battingTeam->teamscore += 4; // Increase the team score by 4
                    }
                    else if (random_event == 8) { // Check if the batsman is out
                        cout << "Batsman " << striker->getName() << " is out!" << endl;
                        wickets += 1;
                        if (wickets == 10) {
                            cout << "All out!" << endl;
                            break; // All out
                        }
                        striker->nextBatsman = nullptr;

                        // Get the next batsman
                        switchStrikers(striker);
                    }
                    else {
                        cout << "Batsman plays the ball..." << endl;
                        score += random_event; // Increase the score by the runs scored
                        battingTeam->teamscore += random_event; // Increase the team 
                    }

                    // Update the batsman's statistics
                    striker->runsScored += random_event;
                    striker->ballsBowled += 1;
                }

                
                if (random_event != 0 && random_event != 5) {
                    // Update the overs, wickets, and swap the batsmen if needed
                    if (random_event % 2 != 0 && random_event != 7) {
                        swapStriker();
                    }
                    striker->ballsBowled += 1;

                    if (balls == 6) {
                        swapStriker();
                    }
                } 

                // Show the current scoreboard
                cout << "Score: " << score << "/" << wickets << " in " << overs << " overs" << endl;
                showBatsmans();

                

                // Show the current baller
                showBowler();

                if (overs == MAX_OVERS && balls == 6) {
                    cout << "\nSecond Innings Ended!" << endl;
                    cout << "Score: " << score << "/" << wickets << " in " << overs << " overs" << endl;
                    cout << endl << endl;
                    if (battingTeam->teamscore == bowlingTeam->teamscore) { // Determine the match result
                        cout << "\nThe match ended in a draw!" << endl;
                    }
                    break; // End of the second innings
                }
        }
            // Determine the match result
            
            if (battingTeam->teamscore > bowlingTeam->teamscore) {
                cout << "\nTeam  " << battingTeam->getTeamName() << " WINS!!!" << endl;
                break;
            }
            
        
        }
        if (battingTeam->teamscore < bowlingTeam->teamscore) {
            cout << "\nTeam  " << bowlingTeam->getTeamName() << " WINS!!!" << endl;
            option--;
        }
        std::cout<< "\n-------------------Match Over!-------------------" << endl;
        
    }
};


class Commentator {
public:
    // Function to generate commentary based on events
    static string generateCommentary() {
        switch (option) {
        case 0:
            return "Sri Lanka has emerged victorious in this thrilling match with an exceptional batting performance. They've played brilliantly to secure a well-deserved win!";
        case 1:
            return "Australia has clinched victory with some sensational bowling! They've managed to defend their target admirably, showing great skill and determination on the field.";
        default:
            return " ";
        }
    }
};

class Match{

public:
// Function to simulate a cricket match
void simulateCricketMatch(Team* teamA, Team* teamB) {
    Scoreboard scoreboard(teamA, teamB);
    Commentator commentator;

        string commentary = commentator.generateCommentary();

        // Display scoreboard and commentary
        scoreboard.play();
        cout << "Commentary: " << commentary << endl;

        // Pause for a moment before the next ball
        this_thread::sleep_for(seconds(2));
        //clearScreen();  // Clear the screen for the next ball
}

void textInput(string textline, Team* teamA,Team* teamB) {
    string up = textline;
    std::transform(up.begin(), up.end(), up.begin(), ::toupper);
    string command, team_name, player_name;
    int battingorder;
    stringstream geek(up);
    geek >> command >> team_name;
    if (command == "CREATE") {
        if (teamA->name == "0") {
            teamA->name = team_name;
        }
        else {
            teamB->name = team_name;
        }
    }
    else if (command == "ADD") {
        geek >> player_name >> battingorder;

        Team* tempteam=new Team("hi");
        if (team_name == teamA->name) {
            tempteam = teamA;
        }
        else if (team_name == teamB->name) {
            tempteam = teamB;
        }
        tempteam->addPlayer(player_name,battingorder);
    }
}
};

int main() {

    // Create two teams
    Team* team1 = new Team("0");
    Team* team2 = new Team("0");
    Match match;

    fstream myfile;
    myfile.open("inputs.txt", ios::in);
    if (myfile.is_open()) {
        
        string line;
        while (getline(myfile, line)) {
            
            match.textInput(line,team1,team2);
        }
    }
    else {
        cout << "File not found";
    }
    
     //Simulate the cricket match
    match.simulateCricketMatch(team1, team2);

    return 0;
}
