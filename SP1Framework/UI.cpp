#include <iostream>

#include "UI.h"
#include "game.h"
#include "map.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::cin;
// UI THINGY

vector<int>highscorePoints;
vector<string>highscoreNames;
vector<string>highscore;
int firstLine = 0;

//Background
void background( Console & g_Console )
{
    for (int y_axis = 0; y_axis < 28; ++y_axis)
    {
        for (int x_axis = 50; x_axis < 80; ++x_axis)
        {
            g_Console.writeToBuffer(x_axis, y_axis, " ", 01);
        }
    }
}


//Divider
void divider(Console & g_Console)
{
    for (int i = 0; i < 28; ++i)
    {
		g_Console.writeToBuffer(50, i, "|", 0);
    }
}



//Timer Notification
void timer(double g_dElapsedTime, Console & g_Console, player & user)
{
    //Timer
    g_Console.writeToBuffer(61, 1, "Timer: ", 10);

    renderFramerate();

    //Time Limit
    g_Console.writeToBuffer(54, 3, "Time Limit: ", 10);

    std::ostringstream strs;
    strs << user.timelimit; 
    string timelimit = strs.str();
    timelimit += " secs";
    g_Console.writeToBuffer(66, 3, timelimit, 10);

    //Timer Notification
    if (user.timelimit - g_dElapsedTime <= 60 && user.timelimit - g_dElapsedTime >= 59.8)
    {
        writeLog("You have a minute left!", g_dElapsedTime);
    }
}
    

//Lives & Health
void lives( player & user, Console &g_Console )
{
	g_Console.writeToBuffer(61, 5, "Lives: ", 10);

    for (int count = 0; count < user.lives; ++count)
    {
		g_Console.writeToBuffer(55 + count*4, 6, "|<3|", 43);
    }

    g_Console.writeToBuffer(61, 7, "Health: ", 10);
    for (int count = 0; count < user.health; ++count)
    {
		g_Console.writeToBuffer(55 + count*4, 8, "|HP|", 43);
    }
}

//Render Inventory
void renderInventory( player & user , Console & g_Console , double &boostcd , double g_dElapsedTime )
{
    g_Console.writeToBuffer(59 , 9, "Inventory: ", 10);

    for (int i = 0; i < user.inventoryitems.size(); ++i)
    {
        if (user.inventory[i] == 't')
        {
            g_Console.writeToBuffer(57, 10 + i , user.inventoryitems[i] , 10);

            if (g_dElapsedTime >= boostcd && user.boost == 1 && user.inventoryitems[user.select] == "Boost")
            {
                for (int find_boost = 0; find_boost <  user.inventoryitems.size(); ++find_boost)
                {
                    if (user.inventoryitems[find_boost] == "Boost")
                    {
                        g_Console.writeToBuffer(62, 10 + find_boost , "(R)" , 10);
                    }
                }
            }
        }

        /*if (g_dElapsedTime >= boostcd && user.boost == 1 && user.inventoryitems[user.select] == "Boost")
        {
            g_Console.writeToBuffer(62, 10 + i , "(R)" , 10);
        }*/
    }
    
}

//Points / Treasure
void point( player & user , Console & g_Console )
{
    g_Console.writeToBuffer(59, 16 , "Treasures:" , 10);

    //if (user.TTaken == 1)
    //{
        if (user.points == 1)
        {
            g_Console.writeToBuffer(70, 16 , "1" , 10);
        }
        else if (user.points == 2)
        {
            g_Console.writeToBuffer(70, 16 , "2" , 10);
        }
        else if (user.points == 3)
        {
            g_Console.writeToBuffer(70, 16 , "3" , 10);
        }
        else if (user.points == 4)
        {
            g_Console.writeToBuffer(70, 16 , "4" , 10);
        }
		else if (user.points == 0)
		{
			g_Console.writeToBuffer(70, 16 , "0" , 10);
		}
}

//Calc. Final Score
void calculateFinal ( player & user , double Endtime )
{
    user.final_score = 0;
    if (user.stage1 == 1)
    {
        user.final_score += 1000;
        for (int time_score = 0; time_score <= (user.timelimit - Endtime); time_score += 60)
        {
            user.final_score += 50;
        }

        if (user.stage2 == 1)
        {
            user.final_score += 1000;
            for (int time_score = 0; time_score <= (user.timelimit - Endtime); time_score += 60)
            {
                user.final_score += time_score;
            }
            if (user.stage3 == 1)
            {
                user.final_score += 1000;
                for (int time_score = 0; time_score <= (user.timelimit - Endtime); time_score += 60)
                {
                    user.final_score += time_score;
                }
            }

        }
    }

    user.final_score += (user.points*500);
	if(user.difficulty == 3)
		user.final_score *=0.5;
	else if(user.difficulty == 1)
		user.final_score *=1.5;
	else if(user.difficulty == 2)
		user.final_score *= 2;
}

//Final Score
void finalscore( Console & g_Console , player & user , double Endtime , COORD c )
{
    calculateFinal ( user , Endtime);

    std::ostringstream strs;
    strs << user.final_score; 
    string final = strs.str();

    g_Console.writeToBuffer(c, final, 0xf9);
}

//Selector
void selector( player & user , Console & g_Console )
{
    g_Console.writeToBuffer(59, 13 , "Item selected: ", 10);

    if (user.inventory[user.select] == 't')
    {
        g_Console.writeToBuffer(59, 14 , user.inventoryitems[user.select] , 10);
    }
    else
    {
        g_Console.writeToBuffer(59, 14 , "No item" , 10);
    }
}

//Read Log
void readLog ( Console & g_Console , double g_dElapsedTime)
{
    vector<string>logText;
    std::fstream log ("log.txt");

    string line;

    g_Console.writeToBuffer(58, 17 , "Notifications:" , 10);
    g_Console.writeToBuffer(51, 18 , "-----------------------------" , 10);

    int i = 0;
    if (log.is_open())
    {
        while (getline (log,line))
        {
            logText.push_back(line);

            /*g_Console.writeToBuffer(52, 23 - i , line , 10);
            ++i;

            if(i == 5)
            {
                i = 0;
            }*/ 
        }
        log.close();
    }
   
    for(int counter = 0; ( counter + firstLine ) < logText.size(); ++counter)
    {
        g_Console.writeToBuffer(52, 23 - counter , logText[counter + firstLine] , 10);

        if (counter == 5)
        {
            counter = -1;
            ++firstLine;
        }
    }
 /* std::ostringstream strs;
    strs << logText.size();
    string str;
    str = strs.str();

    g_Console.writeToBuffer(52, 25, str, 10);*/
}

//Write Log
void writeLog ( string line , double time) 
{
    std::fstream log ("log.txt", std::fstream::app);

    log << line << " @" << static_cast<int>(time) << "secs" << "\n";

    log.close();

    //Check for 5th line
    /*std::fstream checklog ("log.txt");
    string last_line;
    int num = 0;
    while (getline (checklog,last_line))
    {
        ++num; 
        if (num == 5)
        {
            std::fstream log ("log.txt", std::fstream::app);

            log << "-----------------------------";

            log.close();
        }
    }*/
}

void highscoreWrite ( player & user , Console & g_Console )
{
    //Take Current Stats from Text File
    std::fstream highscorePointsValues ("highscorePoints.txt");
    string line;
    if (highscorePointsValues.is_open())
    {
        while (getline (highscorePointsValues,line))
        {
            int points;
            std::istringstream ( line ) >> points;
            highscorePoints.push_back(points);
        }
    }
    highscorePointsValues.close();
    //Truncate Points
    std::ofstream T_highscorePointsValues;
    T_highscorePointsValues.open("highscorePoints.txt", std::fstream::trunc);
    T_highscorePointsValues.close();


    std::fstream highscoreNameValues ("highscoreNames.txt");
    if (highscoreNameValues.is_open())
    {
        while (getline (highscoreNameValues,line))
        {
            highscoreNames.push_back(line);
        }
    }
    highscoreNameValues.close();
    //Truncate Names
    std::ofstream T_highscoreNameValues;
    T_highscoreNameValues.open("highscoreNames.txt", std::fstream::trunc);
    T_highscoreNameValues.close();

    //Get current Combined Text Files
    std::fstream OldHighscore ("highscore.txt");
    if (OldHighscore.is_open())
    {
        while (getline (OldHighscore,line))
        {
            highscore.push_back(line);
        }
    }
    OldHighscore.close();
    //Truncate Old Combined File
    std::ofstream T_OldHighscore;
    T_OldHighscore.open("highscore.txt", std::fstream::trunc);
    T_OldHighscore.close();

    //Input Current Highscore Values
    //cin >> name;
    user.name += " ";

    highscorePoints.push_back(user.final_score);
    highscoreNames.push_back(user.name);

    /*std::ostringstream strs;
    strs << name;
    strs << user.final_score; 
    string final = strs.str();

    highscore.push_back(final);*/
    
    //Arrange in order
    for (int counter = 0; counter < highscorePoints.size(); ++counter)
    {
        for (int SecondCounter = counter + 1; SecondCounter < highscorePoints.size(); ++SecondCounter)
        {
            if (highscorePoints[SecondCounter] >= highscorePoints[counter])
            {
                int temp = highscorePoints[counter];
                highscorePoints[counter] = highscorePoints[SecondCounter];
                highscorePoints[SecondCounter] = temp;

                string tempName = highscoreNames[counter];
                highscoreNames[counter] = highscoreNames[SecondCounter];
                highscoreNames[SecondCounter] = tempName;
            }
        }
    }

    int counter = 0;

    while (counter < highscorePoints.size())
    {
        //Write to Combined Text File
        std::fstream highscore ("highscore.txt", std::fstream::app);

        std::ostringstream strs;
        strs << highscoreNames[counter];
        strs << highscorePoints[counter]; 
        string final = strs.str();
        highscore << final << "\n";
        
        highscore.close();

        //Write Back to separate Text Files 
        //Points
        std::fstream highscorePointsValueNew ("highscorePoints.txt", std::fstream::app);

        std::ostringstream points;
        points << highscorePoints[counter]; 
        string finalPoints = points.str();

        highscorePointsValueNew << finalPoints << "\n";
        highscorePointsValueNew.close();

        //Names
        std::fstream highscoreNamesValueNew ("highscoreNames.txt", std::fstream::app);

        std::ostringstream names;
        names << highscoreNames[counter]; 
        string finalNames = names.str();

        highscoreNamesValueNew << finalNames << "\n";
        highscoreNamesValueNew.close();

        ++counter;
    }

    user.wroteHighScore = 1;
}

void highscoreRead ( player & user , Console & g_Console )
{
    std::fstream highscore ("highscore.txt");
    //highscore.open("highscore.txt");

    string line;

    int i = 1;

    g_Console.writeToBuffer(30, 15 , "LEADERBOARDS" , 0xf9);

    if (highscore.is_open())
    {  
        while (getline (highscore,line))
        {
            std::ostringstream strs;
            strs << i;
            string rank = strs.str();
            g_Console.writeToBuffer(20, 15 + i , rank , 0xf9);
            g_Console.writeToBuffer(30, 15 + i , line , 0xf9);
            ++i;
            if(i == 6)
            {
                break;
            }
        }
        highscore.close();
    }
}

void highscoreBoard ( player & user , Console & g_Console )
{
    std::fstream highscore ("highscore.txt");
    //highscore.open("highscore.txt");

    string line;

    int i = 1;

    g_Console.writeToBuffer(30, 5 , "LEADERBOARDS" , 0xfc);

    if (highscore.is_open())
    {  
        while (getline (highscore,line))
        {
            std::ostringstream strs;
            strs << i;
            string rank = strs.str();
            g_Console.writeToBuffer(20, 7 + i , rank , 0xf9);
            g_Console.writeToBuffer(30, 7 + i , line , 0xf9);
            ++i;
            if(i == 6)
            {
                break;
            }
        }
        highscore.close();
    }
}
