#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct adventureGame{
    //Members for user input and command history
    char command[1000]; // Allocate memory for the command typed by the user
    char lastCommand[1000]; // Allocate memory for the lastCommand to copy the comment typed by the user

    //Members for room information
    const char *rooms[13]; // Declare an array of 13 char*'s holding the room descriptions.
    int room; // The room the user is in. Initialized to start in room#0

    //Look Array
    const char* lookArray[13]; //Get memory for an array of 13 char*'s

    //Move Array
    const char* moveArray[13][4];// Get memory for an array of 13x4 char *’s
    int moveToArray[13][4];// memory for destination rooms

    //Monster Array
    const int N_monster;
    const char *monsterArray[7];
    int monsterStatusArray[7];
    int monsterLoc[13];
    int monsterXP[7];

    //Objects or Artifacts array
    const int N_artifact;
    const char *artifact[9]; // Array of artifact names
    int artifactLoc[13]; // Array to represent the location of each artifact
    int artifactXP[9]; // Array to track XP for each artifact
    int artifactXPScore[9]; // Array to track XP score for each artifact (Use in Final Score)

    //Player Health Status(HP) and Experience Points(XP)
    int playerHP; // Player HP
    int playerXP; // Player XP


} adventureGame;

// Function to replace newline characters in the command
void replaceNewLine(adventureGame *ag) ;

// Function to heal the player with a specified amount, ensuring the health limit
void healPlayerWithLimit(adventureGame *ag, int amount) ;

// Function to display help information in the game
void processHelp();

// Function to print the final score (XP) player gained
void printFinalScore(adventureGame *ag);

// Function to handle monster block south
void handleMonsterBlockS(adventureGame *ag, int monsterIndex, int specialRoom, const char *message);

// Function to handle monster block east
void handleMonsterBlockE(adventureGame *ag, int monsterIndex, int specialRoom, const char *message);

// Function to handle movement
void handleMovement(adventureGame *ag, int specialRoom, int direction, const char *message);

// Function to handle lock movement
void handleLockMovement(adventureGame *ag, int roomIndex, int direction, const char *message);

// Function to handle picking up an artifact and gain XP
void pickUpArtifact(adventureGame *ag, int artifactIndex) ;


