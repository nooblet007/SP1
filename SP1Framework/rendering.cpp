#include "game.h"
#include "map.h"
#include "Framework\console.h"
#include <iostream>
#include <fstream>
#include <string>
using std::cout;
using std::endl;
using std::string;
PMAP MapRender;
PMAP Push;

const char block = 219;
const char pblock = 254;
const char grass = 58;
const char lava = 247;
const char trap = 59;
const char treasure = 36;
const char switches = 47;
const char boost = 175;
const char X = 88;
const char Y = 89;
const char Z = 90;

char arr[12][12];

void createMap(COORD charLocation, bool blind, int range, player &user, Console &g_Console)
{
	MapRender = load_map("stage1.txt");
    //render the game

    //render test screen code (not efficient at all)
    //const WORD colors[] =   {
	   //                     0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
	   //                     0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
	   //                     };
	string line;
	int temprange = range;
	int Xoffset = 0;
	int Yoffset = 0;
	//gotoXY(40, 20);
	//cout << "Coordinates :" << charLocation.X << " " << charLocation.Y;
	if(blind == 1)
	{
		//Set Offset when player is near the Edge of the screen
		if(charLocation.X < temprange)
		{
			Xoffset = temprange - charLocation.X;
		}
		if(charLocation.Y < temprange)
		{
			Yoffset = temprange - charLocation.Y;
		}

		for(int i=0;i < range*2+1-Yoffset;++i)
		{
			//Cout Char Elements Before the player (X-axis)
			if(charLocation.Y+temprange-i < MapRender->nrow && charLocation.X-temprange < MapRender->ncol)
			{
			// && charLocation.Y-temprange+offset >-1  charLocation.X-temprange+offset >-1
			//if(charLocation.X != 0)
			//{
			for(int j = 0;j < range-Xoffset;++j)
			{
				line = MapRender->data[charLocation.Y+temprange-i];
				arr[i][j] = line[charLocation.X - temprange+Xoffset+j];
				if(line[charLocation.X-temprange+Xoffset+j] == 'W')
				{
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, block, 0xf1);
				}else if(line[charLocation.X-temprange+Xoffset+j] == 'D')
				{
					//colour(0xf7);
					//cout << trap;
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, trap, 0xf1);
				}else if(line[charLocation.X-temprange+Xoffset+j] == 'X' && user.switch1 == 0)
				{
					//colour(0xf1);
					//cout << "X";
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, X, 0xf1);
				}
				else if(line[charLocation.X-temprange+Xoffset+j] == 'Y' && user.switch2 == 0)
				{
					//colour(0xf1);
					//cout << "Y";
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, Y, 0xf1);
				}
				else if(line[charLocation.X-temprange+Xoffset+j] == 'Z' && user.switch3 == 0)
				{
					//colour(0xf1);
					//cout << "Z";
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, Z, 0xf1);
				}
				else if(line[charLocation.X-temprange+Xoffset+j] == '1' && user.switch1 == 0)
				{
					//colour(0xf8);
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, switches, 0xf8);
				}
				else if(line[charLocation.X-temprange+Xoffset+j] == '2' && user.switch2 == 0)
				{
					//colour(0xf8);
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, switches, 0xf8);
				}
				else if(line[charLocation.X-temprange+Xoffset+j] == '3' && user.switch3 == 0)
				{
					//colour(0xf8);
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, switches, 0xf8);
				}
				else if(line[charLocation.X-temprange+Xoffset+j] == '=')
				{
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, lava, 0xfc);
				}
				else if (line[charLocation.X-temprange+Xoffset+j] == 'T' && user.TTaken == 0)
				{
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, treasure, 0xf6);
				}
				else if (line[charLocation.X-temprange+Xoffset+j] == 'I' && user.TTaken == 0)
				{
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, boost, 0xf2);
				}
				else{
					g_Console.writeToBuffer(charLocation.X-temprange+Xoffset+j, charLocation.Y+temprange-i, grass, 0xf1);
				}
			}
			//Cout Char Elements After the player (Y-axis)
			for(int k = 0;k < range+1;++k)
			{
				if(charLocation.X == 0)
				{
					line = MapRender->data[charLocation.Y+temprange-i];
				}
				arr[i][range+k] = line[charLocation.X+k];
				if(line[charLocation.X+k] == 'W')
				{
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, block, 0xf1);
				}else if(line[charLocation.X+k] == 'D')
				{
					//colour(0xf7);
					//cout << trap;
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, trap, 0xf1);
				}
				else if(line[charLocation.X+k] == '1' && user.switch1 == 0)
				{
					//colour(0xf8);
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, switches, 0xf1);
				}
				else if(line[charLocation.X+k] == '2' && user.switch2 == 0)
				{
					//colour(0xf8);
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, switches, 0xf1);
				}
				else if(line[charLocation.X+k] == '3' && user.switch3 == 0)
				{
					//colour(0xf8);
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, switches, 0xf1);
				}
				else if(line[charLocation.X+k] == 'X'&& user.switch1 == 0)
				{
					//colour(0xf1);
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, X, 0xf1);
				}
				else if(line[charLocation.X+k] == 'Y' && user.switch2 == 0)
				{
					//colour(0xf1);
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, Y, 0xf1);
				}
				else if(line[charLocation.X+k] == 'Z' && user.switch3 == 0)
				{
					//colour(0xf1);
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, Z, 0xf1);
				}
				else if(line[charLocation.X+k] == '=')
				{
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, lava, 0xfc);
				}
				else if (line[charLocation.X+k] == 'T' && user.TTaken == 0)
				{
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i,treasure, 0xf6);
				}
				else if (line[charLocation.X+k] == 'I' && user.TTaken == 0)
				{
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i, boost, 0xf2);
				}
				else if(charLocation.X+k < 51){
					g_Console.writeToBuffer(charLocation.X+k,charLocation.Y+temprange-i,grass, 0xf1);
				}

			}
			}
		}
		//Arr RADAR
		//for(int i = 0; i< range*2; ++i)
		//{
		//	for(int j = 0; j < range*2; ++j)
		//	{
		//		colour(0x03);
		//		gotoXY(50+j,16+i);
		//		if(i == range && j == range)
		//		{
		//			arr[i][j] = 'A';
		//		}
		//		cout << arr[range*2-i][j];
		//	}
		//}
	}else
	{
		//Cout the whole map
		for(int i=0;i<MapRender->nrow;++i)
		{
			gotoXY(0, i);
			for(int j=0; j<MapRender->ncol; ++j)
			{
				if(MapRender->data[i][j] == 'W')
				{
					colour(1);
				cout << block;
				}else if(MapRender->data[i][j] == 'D')
				{
					colour(0xff);
					cout << trap;
				}
				else if(MapRender->data[i][j] == '1' && user.switch1 == 0)
				{
					colour(8);
					cout << "/";
				}
				else if(MapRender->data[i][j] == '2' && user.switch2 == 0)
				{
					colour(8);
					cout << "/";
				}
				else if(MapRender->data[i][j] == '3' && user.switch3 == 0)
				{
					colour(0xf8);
					cout << "/";
				}
				else if(MapRender->data[i][j] == 'X'&& user.switch1 == 0)
				{
					colour(0xf1);
					cout << "X";
				}
				else if(MapRender->data[i][j] == 'Y' && user.switch2 == 0)
				{
					colour(0xf1);
					cout << "Y";
				}
				else if(MapRender->data[i][j] == 'Z' && user.switch3 == 0)
				{
					colour(0xf1);
					cout << "";
				}
				else if(MapRender->data[i][j] == '=')
				{
					colour(0x0C);
					cout << lava;
				}
				else if (MapRender->data[i][j] == 'T' && user.TTaken == 0)
				{
					cout << treasure;
				}
				else{
					colour(0xf7);
					cout << grass;
				}
			}
		}
	}
}
void blockp(COORD &charLocation, COORD &blocks, COORD &lastknown, int range, Console &g_Console)
{
	//COUT BLOCK IF IN RANGE
	if((charLocation.X - range-1 < blocks.X && charLocation.X + range+1 > blocks.X) && (charLocation.Y - range-1 < blocks.Y && charLocation.Y + range+1 > blocks.Y))
	{
		g_Console.writeToBuffer(blocks.X, blocks.Y, pblock, 0xf1);
	}
	if(charLocation.X == blocks.X - 1 && charLocation.Y == blocks.Y)
	{
		if(MapRender->data[blocks.Y][blocks.X+1] != 'W')
		{
			g_Console.writeToBuffer(51, 11, "You can push the block! LEFT", 0xf1);
			lastknown.X = charLocation.X;
			lastknown.Y = charLocation.Y;
		}else
		{
			g_Console.writeToBuffer(51, 11, "You can't push the block anymore..", 0xf1);
		}
	}else if(charLocation.X == blocks.X + 1 && charLocation.Y == blocks.Y)
	{
		if(MapRender->data[blocks.Y][blocks.X-1] != 'W')
		{
			g_Console.writeToBuffer(51, 11, "You can push the block! RIGHT", 0xf1);
			lastknown.X = charLocation.X;
			lastknown.Y = charLocation.Y;
		}else
		{
			g_Console.writeToBuffer(51, 11, "You can't push the block anymore..", 0xf1);
		}
	}else if(charLocation.X == blocks.X && charLocation.Y == blocks.Y +1)
	{
		if(MapRender->data[blocks.Y+1][blocks.X] != 'W')
		{
			g_Console.writeToBuffer(51, 11, "You can push the block! BOTTOM", 0xf1);
			lastknown.X = charLocation.X;
			lastknown.Y = charLocation.Y;
		}else
		{
			g_Console.writeToBuffer(51, 11, "You can't push the block anymore..", 0xf1);
		}
	}else if(charLocation.X == blocks.X && charLocation.Y == blocks.Y -1)
	{
		if(MapRender->data[blocks.Y+1][blocks.X] != 'W')
		{
			g_Console.writeToBuffer(51, 11, "You can push the block! TOP", 0xf1);
			lastknown.X = charLocation.X;
			lastknown.Y = charLocation.Y;
		}else
		{
			g_Console.writeToBuffer(51, 11, "You can't push the block anymore..", 0xf1);
		}
	}else if(charLocation.X != blocks.X || charLocation.Y != blocks.Y){
			lastknown.X = charLocation.X;
			lastknown.Y = charLocation.Y;
	}else if(charLocation.X == blocks.X && charLocation.Y == blocks.Y)
	{
		gotoXY(51, 11);
			g_Console.writeToBuffer(51, 11, "You are standing on the box..", 0xf1);
	}
	if(charLocation.X == blocks.X && charLocation.Y == blocks.Y)
	{
		if(lastknown.X == charLocation.X -1 && lastknown.Y == charLocation.Y)
		{
			if(MapRender->data[blocks.Y][blocks.X +1] != 'W')
			{
				if(MapRender->data[blocks.Y][blocks.X] != '=')
				{
					charLocation.X -=1;
					blocks.X +=1;
				}
			}
		}
		if(lastknown.X == charLocation.X +1 && lastknown.Y == charLocation.Y)
		{
			if(MapRender->data[blocks.Y][blocks.X -1] != 'W')
			{
				if(MapRender->data[blocks.Y][blocks.X] != '=')
				{
					charLocation.X +=1;
					blocks.X -=1;
				}
			}
		}
		if((lastknown.X == charLocation.X) && (lastknown.Y == charLocation.Y-1))
		{
			if(MapRender->data[blocks.Y+1][blocks.X] != 'W')
			{
				if(MapRender->data[blocks.Y][blocks.X] != '=')
				{
					charLocation.Y -=1;
					blocks.Y +=1;
				}
			}
		}
		if((lastknown.X == charLocation.X) && (lastknown.Y == charLocation.Y+1))
		{
			if(MapRender->data[blocks.Y-1][blocks.X] != 'W')
			{
				if(MapRender->data[blocks.Y][blocks.X] != '=')
				{
				charLocation.Y +=1;
				blocks.Y -=1;
				}
			}
		}
	}
	//colour(0x0f);
	//gotoXY(51, 16);
	//cout << "Block X: " << blocks.X << " " << blocks.Y;
	//gotoXY(51, 20);
	//cout << "LastCoord: " << lastknown.X << " " << lastknown.Y;
	//gotoXY(51, 18);
	//cout << "CharCoord: " << charLocation.X << " " << charLocation.Y;
}