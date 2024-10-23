#include "lab.h"


//-----------replaceNewLine()--------------------//
void replaceNewLine(adventureGame *ag) {
    for (int i=0;ag->command[i]!='\0'; i++) {// Iterate over every char in the command
        if (ag->command[i]=='\n') { // Search for newline characters
            ag->command[i]='\0'; // replace them with end-of-string char.
            return; // We return after first newline character is found
        }
    }
}

//-------------processLook()-------------------//
int processLook(adventureGame *ag) {
    // Display the player's current health points (HP) and experience points (XP)
    printf("Player HP: %d, XP: %d .\n",ag->playerHP,ag->playerXP);

     // Display the room description using the lookArray based on the player's current room
    printf("%s\n",ag->lookArray[ag->room]);

    // Process any monster in the room
    for (int i=0;i<ag->N_monster;i++) {
        if (ag->monsterLoc[i] == ag->room){
            //Check if the monster is alive.
            if (ag->monsterStatusArray[i]>0) {
                // Display the live monster and its current HP in the room
                printf("There is a %s here. HP: %d .\n",ag->monsterArray[i],ag->monsterStatusArray[i]);
            }
            else if (ag->monsterStatusArray[i]==0) { // Check if the monster is dead.
            // Display a message indicating a dead monster in the room
            printf("There is a dead %s here\n",ag->monsterArray[i]);
            }
        }
    }

    // Process any artifact that may be in the room
    for (int i=0;i<ag->N_artifact;i++) {
        if (ag->artifactLoc[i]==ag->room) printf("You see %s here.\n",ag->artifact[i]);
    }
    return(1);
}

// Function to handle blocking movement in the south direction based on monster status
void handleMonsterBlockS(adventureGame *ag, int monsterIndex, int specialRoom, const char *message){
    if (ag->room == specialRoom && ag->moveToArray[specialRoom][1] == -1) {
        // Check if the monster is still alive
        if (ag->monsterStatusArray[monsterIndex] > 0) {
            printf(message);
			return;
        }
    }
}

// Function to handle blocking movement in the east direction based on monster status
void handleMonsterBlockE(adventureGame *ag, int monsterIndex, int specialRoom, const char *message){
    if (ag->room == specialRoom && ag->moveToArray[specialRoom][2] == -1) {
        // Check if the monster is still alive
        if (ag->monsterStatusArray[monsterIndex] > 0) {
            printf(message);
            return;
        }
    }
}

// Function to handle movement
void handleMovement(adventureGame *ag, int roomIndex, int direction, const char *message){
    if (ag->moveToArray[roomIndex][direction] == -1) {
        printf("%s", message);
        return;
    }
    printf("%s\n", ag->moveArray[ag->room][direction]);// Print details where u move to
    ag->room = ag->moveToArray[ag->room][direction];// Change room index to the new room
    processLook(ag);// Call function to print details of the new room
}

// Function to handle lock movement
void handleLockMovement(adventureGame *ag, int roomIndex, int direction, const char *message){
     // Check if the player is in the specified room and the movement in the given direction is not allowed
	if (ag->room == roomIndex && ag->moveToArray[roomIndex][direction] == -1) {
        printf(message);
        return;
    }
}

//------------processMovement()-------------------//
int processMovement(adventureGame *ag) {
    if(strcmp(ag->command,"north")==0 || strcmp(ag->command,"n")==0) {
        handleMovement(ag,ag->room,0,"Unable to move north.\n");
        return(1);
    }

   if (strcmp(ag->command,"south")==0 || strcmp(ag->command,"s")==0) {
        //Print a message indicating the need to kill the werewolf before enter the courtyard.
        handleMonsterBlockS(ag,1,4,"You need to kill the werewolf before entering the courtyard.\n");
        //Print a message indicating the need to kill the zombie before enter the laundry room.
        handleMonsterBlockS(ag,0,2,"You need to kill the zombie before entering the laundry room.\n");
        // Print the message indicating the need for the bronze key to unlock the door.
        if (ag->monsterStatusArray[1] <= 0) { // Check if the monster is dead
        handleLockMovement(ag,4,1,"The door to the Courtyard is locked. You need the bronze key.\n");
        }
        handleMovement(ag,ag->room,1,"Unable to move sourth.\n");
        return(1);
    }

    if (strcmp(ag->command,"east")==0 || strcmp(ag->command,"e")==0) {
        // Print a message indicating the need to kill the zombie before entering the dining room.
       handleMonsterBlockE(ag, 0, 2, "You need to kill the zombie before entering the dining room.\n");
        // Print a message indicating the need for the copper key to enter the study room
        handleLockMovement(ag,11,2,"The door to the Study Room is locked. You need the copper key.\n");
        handleMovement(ag,ag->room,2,"Unable to move east.\n");
        return(1);
    }

    if (strcmp(ag->command,"west")==0 || strcmp(ag->command,"w")==0) {
        handleMovement(ag,ag->room,3,"Unable to move west.\n");
        return(1);
    }

    return(0);// Command was not recognized by this function
}

//--------------processKillMonster()-------------//
int processKillMonster(adventureGame *ag) {
    char *monster = &ag->command[5]; // The sixth character of the line is where the name of the monster begins.

    // Flag to check if a monster is found in the room
    int monsterFound = 0;

   // Process any monster in the room
    for (int i=0;i<ag->N_monster;i++) {
        // Check if the monster is in the list of monsterArray
        if (strcasecmp(monster,ag->monsterArray[i])==0 || strcmp(ag->command,"killMonster")==0 ||
            strcmp(ag->command,"killmonster")==0 || strcmp(ag->command,"km")==0) {
            // Check if a monster is in the current room
            if (ag->monsterLoc[i]==ag->room){
                // Check if the monster is already dead
                if (ag->monsterStatusArray[i] <= 0) {printf("The %s is already dead\n",ag->monsterArray[i]);}
                // Check if the monster is alive
                if (ag->monsterStatusArray[i] > 0) {
                    monsterFound = 1;  // Set the flag to true

                    // Generate random numbers (integer between 1 to 2) for player and monster damage
                    int playerDamage = 1+(2.0*rand())/(RAND_MAX+1.0);
                    int monsterDamage = 1+(2.0*rand())/(RAND_MAX+1.0);

                    // Adjust player damage based on artifacts
                    // Werewolf is injured more with the silver spear
                    if(ag->artifactLoc[7]==-1 && ag->monsterLoc[1]==ag->room){
                        playerDamage *= 4;
                    }
                    // Diamond cross injures vampires( Dracula and Bride of Dracula) more
                    else if((ag->artifactLoc[6]==-1) && (ag->monsterLoc[3]==ag->room || ag->monsterLoc[5]==ag->room)){
                        playerDamage *= 5;
                    }
                    // Sword has minimal impact on the vampires( Dracula and Bride of Dracula)
                    else if(ag->artifactLoc[2]==-1 && (ag->monsterLoc[3]==ag->room || ag->monsterLoc[5]==ag->room)){
                        playerDamage *= 1.5;
                    }
                    // Having the sword increases the player damage
                    else if(ag->artifactLoc[2]==-1 ){
                        playerDamage *= 2;
                    }

                    // Update player and monster health
                    ag->playerHP -= monsterDamage;
                    ag->monsterStatusArray[i] -= playerDamage;

                    // Display the results of the encounter
                    printf("You dealt %d damage. Monster dealt %d damage. \n",playerDamage,monsterDamage);
                    printf("Player HP: %d, %s HP: %d .\n",ag->playerHP,ag->monsterArray[i],ag->monsterStatusArray[i]);

                    // If the zombie died, change the moveToArray to go east to dining room, south to laundry room
                    if(ag->monsterStatusArray[0]<=0){
                       ag->moveToArray[2][2] = 3; // Go east to Dining Room
                       ag->moveToArray[2][1] = 7; // Go south to Laundry Room
                    }

                    // Check if the player died
                    if(ag->playerHP <= 0){
                        printf("You died! Game over.\n");
                        printFinalScore(ag); // Print the final score before exiting
                        exit (0); // End the game
                    }

                    // Check if the monster is defeated
                    if(ag->monsterStatusArray[i] <= 0){
                        printf("You slay the %s!\n",ag->monsterArray[i]);
                        // Award XP for defeating a monster
                        ag->playerXP += ag->monsterXP[i];
                        printf("You gained %d XP!\n", ag->monsterXP[i]);
                    }
                }
            }
        }
    }

    // Print the message if no monsters are found in the room
    if (!monsterFound) {
        printf("There is no monster here.\n");
    }
    return(1);
}


// Function to check if the player's inventory is full
int isInventoryFull(adventureGame *ag) {
    // Track the number of available slots in the player's inventory
    int inventoryCount = 0; // Initializes the variable to 0
    for (int i = 0; i < ag->N_artifact; i++) { // iterate over each artifact in the list
        if (ag->artifactLoc[i] == -1) {
            inventoryCount++;
        }
    }
    return (inventoryCount == 3);
}

// Function to handle picking up an artifact and gain XP
void pickUpArtifact(adventureGame *ag, int artifactIndex) {
    printf("You pick up the %s.\n", ag->artifact[artifactIndex]);
    ag->artifactLoc[artifactIndex] = -1;

    // XP for picking up the artifact
    if (ag->artifactXP[artifactIndex] > 0 && ag->artifactLoc[artifactIndex] == -1) {
        // Award XP for getting an artifact
        ag->playerXP += ag->artifactXP[artifactIndex];
        printf("You gained %d XP!\n", ag->artifactXP[artifactIndex]);

        // Mark the artifact as obtained to prevent XP gain on subsequent pickups
        ag->artifactXP[artifactIndex] = -1;
    }
}

//-------------processGet()-----------//
int processGet(adventureGame *ag) {
    char *item = &ag->command[4]; // The fifth character of the line is where the name of the item begins.

    // Check if the player's inventory is full
    if (isInventoryFull(ag)) {
        printf("Your inventory is full. You cannot carry more items.\n");
        return (1);
    }

    // Iterate over each artifact in the list to find the desired item
    for (int i=0;i<ag->N_artifact;i++) {
        if (strcasecmp(item,ag->artifact[i])==0 ) { // The object being gotten is in the list of objects
             // Check if the desired item is in the current room
            if (ag->artifactLoc[i]==ag->room) {
                // Check if the Dracula is being killed to pick up the copper key.
                if(strcasecmp(item,"copper key")==0 && ag->monsterStatusArray[3] > 0){
                    printf("You need to kill Dracula to pick up the copper key.\n");
                }
                // Check if the Lizard-man is being killed to pick up the diamond cross.
                else if(strcasecmp(item,"diamond cross")==0 && ag->monsterStatusArray[4] > 0){
                    printf("You need to kill Lizard man to pick up the diamond cross.\n");
                }
                // Check if the Dragon is being killed to pick up the Golden Chalice.
                else if(strcasecmp(item,"golden chalice")==0 && ag->monsterStatusArray[6] > 0){
                    printf("You need to kill Dragon to pick up the Golden Chalice.\n");
                }
                else{pickUpArtifact(ag,i);}
                return(1);
            } else {
                printf("You cannot see any %s here or you have already obtained it.\n",item);
                return(1);
            }
        }
    }

    // If the desired item is not found in the current room
    printf("There is not any %s here.\n",item);
    return(1);
}

//------------printInventory-----------//
int printInventory(adventureGame *ag) {
    int i=0,j=0;
    printf("You are carrying:\n");

     // Iterate over each artifact in the list
    for (i=0,j=0;i<ag->N_artifact;i++) {
        // Check if the artifact is in the player's inventory
        if (ag->artifactLoc[i]==-1) {
            printf("%2d %s\n",++j,ag->artifact[i]);  // Print the artifact
        }
    }
    // If the player is not carrying anything, print "Nothing"
    if (j==0) printf("Nothing\n");

    return(1);
}

//-----------processDrop--------------//
int processDrop(adventureGame *ag) {
    char *item = &ag->command[5]; // The sixth character of the line is where the name of the item begins (after "drop ").

    // Iterate over each artifact in the player's inventory
    for (int i = 0; i < ag->N_artifact; i++) {
        if (strcasecmp(item, ag->artifact[i]) == 0) {
            // Check if the object is in the player's inventory (-1 indicates it's in the inventory)
            if (ag->artifactLoc[i] == -1) {
                // Print a message indicating the player dropped the item
                printf("You drop the %s.\n", item);
                // Update the item's location to the current room
                ag->artifactLoc[i] = ag->room;
                return (1);
            } else {
                // Print a message indicating the player is not carrying the item
                printf("You are not carrying any %s.\n", item);
                return (1);
            }
        }
    }
    // Print a message indicating the player doesn't have the specified item to drop
    printf("There is not any %s here.\n", item);
    return (1);
}

// Function for healing with a specified amount
void healPlayerWithLimit(adventureGame *ag, int amount) {
    printf(" and restore %d hit-points.\n", amount);
    ag->playerHP += amount;

    // Ensure player HP does not exceed the maximum (12)
    if (ag->playerHP > 12) {
        ag->playerHP = 12;
    }
}

//-----processHealing()-----------//
int processHealing(adventureGame *ag){

    // Check if the command is "eat food" and the food artifact is in the player's inventory
    if (strcmp(ag->command, "eat food") == 0 && ag->artifactLoc[0] == -1 ){
        printf("You %s",ag->command);
        healPlayerWithLimit(ag,5); // Call the function to restore 5 hit-points
        ag->artifactLoc[0]=-2; // -2 indicates the object is being used
        return(1);
    }
    // Check if the command is "drink elixir" and the elixir artifact is in the player's inventory
    else if (strcmp(ag->command, "drink elixir")==0 && ag->artifactLoc[5] == -1) {
        printf("You %s",ag->command);
        healPlayerWithLimit(ag,10); // Call the function to restore 10 hit-points
        ag->artifactLoc[5]=-2; // -2 indicates the object is being used
        return(1);
    }
    // Check if the command is "use medpack" and the medpack artifact is in the player's inventory
    else if (strcmp(ag->command, "use medpack")==0 && ag->artifactLoc[1] == -1) {
        printf("You %s",ag->command);
        healPlayerWithLimit(ag,10); // Call the function to restore 10 hit-points
        ag->artifactLoc[1]=-2; // -2 indicates the object is being used
        return(1);
    }

    return(0);
}

//-------unlockDoor()-----//
int processUnlockDoor(adventureGame *ag){

    // Check if the player is in Kitchen and the door to the Courtyard is locked and the werewolf is dead
    if (ag->room == 4 && ag->moveToArray[4][1] == -1 &&ag->monsterStatusArray[1]<=0){
         // Check if the player has the bronze key in the inventory
        if(ag->artifactLoc[4]== -1 ){
                    // Unlock the door to the Courtyard with the bronze key
                    printf("You unlock the door to the Courtyard with the bronze key.\n");
                    // Update the destination room after unlocking
                    ag->room = 9;
                    ag->moveToArray[4][1]=9; //  Updating the destination room index in the moveToArray
                    ag->artifactLoc[4]=-2; //-2 indicate the key has been used
                    return(1);
                }
    }

   // Check if the player is in Game Room and the door to the Study Room is locked
    if (ag->room == 11 && ag->moveToArray[11][2] == -1 ){
            // Check if the player has the copper key in the inventory
            if(ag->artifactLoc[3]== -1){
                // Unlock the door to the Study Room with the copper key
                printf("You unlock the door to the Study Room with the copper key.\n");
                // Update the destination room after unlocking
                ag->room = 12;
                ag->moveToArray[11][2]=12; //  Updating the destination room index in the moveToArray
                ag->artifactLoc[3]=-2; //-2 indicate the key has been used
                return(1);
            }
    }
    // If none of the conditions are met, print a message indicating that the door cannot be unlocked
    printf("You attempt to unlock a door, but it seems you're in the wrong place or the door is already unlocked.\n");
    return (1);

}

//-----------AutoReply-----------//
void AutoReply(adventureGame *ag){
    // Check if the current command is empty (just Enter pressed)
    if (strcmp(ag->command,"\n")==0){
         // Copy the last command if the current command is just a newline
        strncpy(ag->command,ag->lastCommand,sizeof(ag->command)-1);
        ag->command[strlen(ag->command)-1]='\0'; //Remove the newline character
        printf(ag->lastCommand);
    }else if (ag->command[0] != '\0' && ag->command[0] != '\n') {
        // Copy the current command to lastCommand only if it's not an empty line
        strcpy(ag->lastCommand, ag->command);
    }
}

//------FinalScore-------//
void printFinalScore(adventureGame *ag){
    printf("\n===============Score====================\n");
    printf("--------------------------------------\n");
    printf("XP Gained from defeating monsters\n");
    // Iterate over each monster to print XP gained
    for (int i = 0; i < ag->N_monster; i++) {
		// Check if the monster is dead
        if (ag->monsterStatusArray[i] <= 0) {
            printf("Defeated %-20s : %d XP\n", ag->monsterArray[i], ag->monsterXP[i]);
        }
    }

    printf("\n--------------------------------------\n");
    printf("XP Gained from picking up artifacts\n");
    // Iterate over each artifact to print XP gained
    for (int i = 0; i < ag->N_artifact; i++) {
		// Check the artifact has been picked up or used
        if (ag->artifactXP[i] == -1 || ag->artifactXP[i] == -2) {
            printf("Picked up %-20s: %d XP\n", ag->artifact[i], ag->artifactXPScore[i]);
        }
    }
    printf("\n--------------------------------------\n");
    printf("Your Final Score:  %d / 400.\n",ag->playerXP);
    printf("\n========================================\n");

}

//----------processHelp()-------//
void processHelp(){
    printf(
    "Command                 Example(s)\n"
    "===================================================================================\n"
    "Movement commands       north, n, south, s, east, e, west, w\n"
    "Look command            look,l\n"
    "Fight commands          killMonster,killmonster,km\n"
    "Alternate               kill zombie, kill werewolf, kill dragon\n"
    "Get commands            get diamond cross, get sword, get Sword, get elixir\n"
    "Drop commands           drop copper key, drop food, drop golden chalice.\n"
    "Inventory command       inventory, i: list what the player is carrying\n"
    "Healing commands        eat food, drink elixir, use medpack\n"
    "Unlocking door          unlock door\n"
    "Exit game               exit\n"
    "===================================================================================\n"
    );
    printf("Tips: Vampires fear the diamond cross, while werewolves are afraid of the silver spear.\n");
}

//--------------processCommand()-------------//
int processCommand(adventureGame *ag) {

    AutoReply(ag);
    replaceNewLine(ag);


    // Handle the exit command
    if (strcmp(ag->command,"exit")==0) {
        printf("You exit the game.\n");
        printFinalScore(ag); // Print the final score before exiting
        return(0);
    }

    //If the player wins
    // Check if the player has the Golden Chalice and is in the starting room
    if (ag->artifactLoc[8]==-1 && ag->room==0){
        printf("Congratulations! You have obtained the Golden Chalice. You are victorious!\n");
        printFinalScore(ag); // Print the final score before exiting
        exit(0); // End the game
    }

    // Process the look command.
    if (strncmp(ag->command,"look",4)==0 || strcmp(ag->command,"l")==0) {processLook(ag); return(1);}
    // Process the killMonster command.
    if (strcmp(ag->command,"killMonster")==0 || strcmp(ag->command,"killmonster")==0 ||
        strcmp(ag->command,"km")==0 || strncmp(ag->command,"kill ",5)==0 )
        {processKillMonster(ag); return(1);}
    // Getting object
    if (strncmp(ag->command,"get ",4)==0) {processGet(ag);return(1);}
    // Process the movement command.
    if (processMovement(ag)) return(1);
    // Printing an inventory
    if(strcmp(ag->command,"inventory")==0 || strcmp(ag->command,"i")==0) {printInventory(ag);return(1);}
    // Dropping Objects
    if (strncmp(ag->command, "drop ", 5) == 0) {processDrop(ag);return (1);}
    // Healing
    if(processHealing(ag))return(1);
    // Unlock door
    if(strcmp(ag->command,"unlock door")==0){processUnlockDoor(ag);return(1);}
    // Help command
    if(strcmp(ag->command,"help")==0){processHelp();return(1);}

    // If none of the recognized commands matched
    printf("Unknown command: %s\n",ag->command);
    return(1);

}
