#include "map.hpp"
#include <iostream>
//define number of the case
Road::Road(Box *rBox)
{
    RoadBox = rBox;
}

Road::~Road()
{
}

Map::~Map()
{
}
//Constructor Map
Map::Map()
{
    mIdMap = 1;

    //Init Variable for the map creator
    mVariableCreator.isInit = false;
    mVariableCreator.isStartSet = false;
    mVariableCreator.isEndSet = false;
    mVariableCreator.checkPathNotValid = false;
    mVariableCreator.valToSet = 'r';
    mVariableCreator.frameCounter = 0;

    MapGeneration(Choosemap());
    mIsMapCleared = false;
}

void Map::Reset(int idMap)
{
    mIdMap = 1;
    roads.clear();
    mTabBox.clear();

    mIdMap = idMap;

    //Init Variable for the map creator
    mVariableCreator.isInit = false;
    mVariableCreator.isStartSet = false;
    mVariableCreator.isEndSet = false;
    mVariableCreator.checkPathNotValid = false;
    mVariableCreator.valToSet = 'r';
    mVariableCreator.frameCounter = 0;

    MapGeneration(Choosemap());
    mIsMapCleared = false;
}

void Map::ClearMap()
{
    if(!mIsMapCleared)
    {
        roads.clear();
        mTabBox.clear();

        mIdMap = 0;

        //Init Variable for the map creator
        mVariableCreator.isInit = false;
        mVariableCreator.isStartSet = false;
        mVariableCreator.isEndSet = false;
        mVariableCreator.checkPathNotValid = false;
        mVariableCreator.valToSet = 'r';
        mVariableCreator.frameCounter = 0;
        mIsMapCleared = true;
    }


}

int Map::GetIdMap()
{
    return mIdMap;
}

int Map::GetStartRoad(const char* levelDesign)
{
    int startId = -1;
    //Search for the element 's' which is the start of the road
    for (int i = 0; i < constNumberMaxBox; i++)
    {
        if (levelDesign[i] == 's')
        {
            startId = i;
            break;
        }
    }
    return startId;
}

//Generate the map
void Map::MapGeneration(const char *levelDesign)
{

    // Init Tab of Box and Road
    mTabBox.clear();
    mTabBox.resize(constNumberMaxBox);

    // Counter of box
    // Init all boxes , for each row in map then each column
    for (float i = 0; i < constBoxColumn; i++)
    {
        for (float j = 0; j < constBoxRow; j++)
        {
            int tileId = j * constBoxColumn + i;
            Box &curBox = mTabBox[tileId];
            //Init all value of Box using constSizeBox to set the right value (each box is a square)
            curBox.pBox = {i * constSizeBox, j * constSizeBox, constSizeBox, constSizeBox};
            curBox.centerBox = {(curBox.pBox.x + constSizeBox / 2), (curBox.pBox.y + constSizeBox / 2)};
            // Check if the pos of the box is a road or not using the char* leveldesign
            curBox.tmpRoad = GetRoad(levelDesign[tileId]);
            curBox.isFree = !curBox.tmpRoad;

            //no scenary on grass for an empty map
            if (mIdMap != 0)
            {
                if (curBox.isFree)
                {
                    int randomValue = rand();
                    //We set at 10 the random value for scenary on tiles
                    const int constRandScenary = 10;
                    if (randomValue % constRandScenary == 0)
                    {
                        //Get a random tile used as scenary //!\\ Only usable with this tilesheet
                        curBox.scenaryId = 130 + rand() % 8;
                        curBox.hasScenary = true;
                    }
                }
            }
        }
    }

    //If not in map creator initialize the first value of the list
    if (mIdMap != 0)
    {
        bool isPathValid = true;

        int startId = GetStartRoad(levelDesign);

        mTabBox[startId].hasScenary = true;
        mTabBox[startId].scenaryId = 64;

        //Initialize and set all values
        //way is used to change the incrementation to follow the road
        int way = 1;

        //Set the start position of the enemies
        roads.push_back(Road(&mTabBox[startId]));
        //Set them at the left of the box to increase realism accuracy
        startPosition = {mTabBox[startId].pBox.x, mTabBox[startId].centerBox.y};
        int counter = startId + way;

        //Set the list of the road using leveldesign
        for (int id = counter; id < constNumberMaxBox; id += way)
        {
            Box& curBox = mTabBox[id];
            switch(levelDesign[id])
            {
                case 'r' : 
                {
                    break;
                }
                case 'f' :
                {
                    //Set way at -1 to turn to the left 
                    way = -1 ;
                    curBox.hasScenary = true;
                    curBox.scenaryId = 21;
                    break;
                }
                case 'i' :
                {
                    //Set way at 1 to turn to the right 
                    way = 1;
                    curBox.hasScenary = true;
                    curBox.scenaryId = 20;
                    break;
                }
                case 'd' :
                {
                    //Set way at 22 to turn down
                    way = 22;
                    curBox.hasScenary = true;
                    curBox.scenaryId = 22;
                    break;
                }
                case 'u' :
                {
                    //Set way at -22 to turn up
                    way = -22;
                    curBox.hasScenary = true;
                    curBox.scenaryId = 19;
                    break;
                }
                case 'e' :
                {
                    //Insane value to break the loop
                    way += 10000000000;
                    curBox.hasScenary = true;
                    curBox.scenaryId = 15;
                    break;
                }
                default :
                    break;
            } 

            //Add new element
            if(isPathValid)
                roads.push_back(Road(&mTabBox[id]));
            else
                break; 
        }
    }
}

bool Map::DrawMapChoice(Resource& resource)
{
    DrawTexturePro(resource.playerUI , {456 , 62 , 352 , 143} , {100 , 10 , constScreenWidth , constScreenHeight - 50} , {0,0}, 0, WHITE);

    DrawTexturePro(resource.mapOnePreview , {0,0, 1408 , 1025} , {225, 200, 500 , 400} , {0,0}, 0 , WHITE);
    DrawTexturePro(resource.mapTwoPreview , {0,0, 1408 , 1025} , {875, 200, 500 , 400} , {0,0}, 0 , WHITE);

    if(ButtonS({400 , constScreenHeight - 350 } , {405 , constScreenHeight - 345} , 35 , 100 , 50 , "PLAY" , BLACK , BLACK, BROWN , BROWN))
    {
        mIdMap = 1;
        return true;
    }

    if(ButtonS({constScreenWidth - 300 , constScreenHeight - 350}, {constScreenWidth - 295 , constScreenHeight - 345} , 35 , 100 , 50 , "PLAY" , BLACK , BLACK, BROWN , BROWN))
    {
        mIdMap = 2;
        return true;
    }

    return false;
    
}

//get an int as index and get the good map
const char *Map::Choosemap()
{
    // Creation of the map with a char*
    // value :   r = road , ' ' = grass , 'f' = left , 'i' = right , 'u' = up , 'd' = down , 's' is for the start of the road and 'e' for the end
    //By convention 's' should be on the left side and 'e' on the right side
    switch(mIdMap)
    {
        case 1:
        {
            const char* map = 
            "                      "
            "    irrrrrrrrd        "
            "    r        r        "
            "    r        r        "
            "srrrrrrd     r        "    
            "    r  r     r        "    
            "    r  r     r      ie"    
            "    urrf     r     iu "    
            "             r    iu  "    
            "             r   iu   "    
            "             r  iu    "    
            "    drrrrrrrrf  r     "    
            "    r           r     "     
            "    r           r     "
            "    irrrrrrrrrrru     "    
            "                      ";
            return map;
        }
        case 2:
        {
            const char* map = 
            "                      "
            "  irrrrrd   drrf      "
            "  r     r   r  r      "     
            "  r     r   r  r      "    
            "srrrrd  r   irrrrrrrre"
            "  r  r  r      r      "
            "  r  r  r      r      "
            "  urrf  r      r      "
            "        r      r      "
            "        r      r      "
            "     irrrrrrrrrrrrd   "    
            "     r  r      r  r   "    
            "     r  r      r  r   "    
            "     urrf      urrf   "      
            "                      "       
            "                      ";
            return map;
                        
        }

    }
    return " ";
}

vector2D Map::GetTileGrass(int i, int j, int tileId)
{
    //Raw values are used in GetPoseTile function, it's used to get the right assets from the tilesheet from just an id

    //Get the tile Id at every position around the current tile
    int northId = (j - 1) * constBoxColumn + (i);
    int northWestId = (j - 1) * constBoxColumn + (i - 1);
    int northEastId = (j - 1) * constBoxColumn + (i + 1);

    int southId = (j + 1) * constBoxColumn + (i);
    int southEastId = (j + 1) * constBoxColumn + (i + 1);
    int southWestId = (j + 1) * constBoxColumn + (i - 1);

    int eastId = j * constBoxColumn + (i + 1);
    int westId = j * constBoxColumn + (i - 1);

    //Verification for border of the map

    //If we are on the first row
    bool checkNorth =   (tileId >= constBoxColumn)                        && mTabBox[northId].tmpRoad;
    //If we are on the last row
    bool checkSouth =   ((j % constBoxRow) != (constBoxRow - 1))          && mTabBox[southId].tmpRoad;
    //If we are on the first column
    bool checkWest =    ((i % constBoxColumn) != 0)                       && mTabBox[westId].tmpRoad;
    //If we are on the last column
    bool checkEast =    ((i % constBoxColumn) != (constBoxColumn - 1))    && mTabBox[eastId].tmpRoad;
    
    bool checkSouthEast = (((j % constBoxRow) != (constBoxRow - 1)) && ((i % constBoxColumn) != (constBoxColumn - 1)))  && mTabBox[southEastId].tmpRoad;
    bool checkSouthWest = (((j % constBoxRow) != (constBoxRow - 1)) && ((i % constBoxColumn) != 0))                     && mTabBox[southWestId].tmpRoad ;
    bool checkNorthEast = ((tileId >= constBoxColumn)               && ((i % constBoxColumn) != (constBoxColumn - 1)))  && mTabBox[northEastId].tmpRoad;;
    bool checkNorthWest = ((tileId >= constBoxColumn)               && ((i % constBoxColumn) != 0))                     && mTabBox[northWestId].tmpRoad;;


    vector2D tileToSpawn = {-1, -1};
    //Succession of if to check wich adjacent box are road, to make the path smoother
    //check north and adjacent boxes
    if (checkNorth        &&
        checkWest         &&
        checkEast         &&
        checkSouth)
    {
        tileToSpawn = GetPosTile(49);
    }
    else if (checkNorth)
    {
        if  (checkWest)
        {
            tileToSpawn = GetPosTile(72);
        }
        else if (checkEast)
        {
            tileToSpawn = GetPosTile(73);
        }
        else if (checkSouth)
        {
            tileToSpawn = GetPosTile(1);
        }
        else
        {
            tileToSpawn = GetPosTile(116);
        }
    }
    //Check south and adjacent boxes
    else if (checkSouth)
    {
        if  (checkWest)
        {
            tileToSpawn = GetPosTile(95);
        }
        else if (checkEast)
        {
            tileToSpawn = GetPosTile(96);
        }
        else
        {
            tileToSpawn = GetPosTile(70);
        }
    }
    //Check only adjacent
    else if  (checkWest)
    {
        if (checkEast)
        {
            tileToSpawn = GetPosTile(25);
        }
        else
        {
            tileToSpawn = GetPosTile(94);
        }
    }
    else if ( checkEast)
    {
        tileToSpawn = GetPosTile(92);
    }
    //Check diagonal boxes
    else if (checkSouthWest)
    {
        if (checkNorthEast)
        {
            tileToSpawn = GetPosTile(46);
        }
        else
        {
            tileToSpawn = GetPosTile(71);
        }
    }
    else if (checkSouthEast)
    {
        if (checkNorthWest)
        {
            tileToSpawn = GetPosTile(48);
        }
        else
        {
            tileToSpawn = GetPosTile(69);
        }
    }
    else if (checkNorthEast)
    {
        tileToSpawn = GetPosTile(115);
    }
    else if (checkNorthWest)
    {
        tileToSpawn = GetPosTile(117);
    }
    return tileToSpawn;
}

//This function draw the map
void Map::ShowCadrillage(Resource &mapText)
{
    //Go through the list and draw the box
    for (int i = 0; i < constBoxColumn; i++)
    {
        for (int j = 0; j < constBoxRow; j++)
        {
            int tileId = j * constBoxColumn + i;

            Box &curBox = mTabBox[tileId];
            //If the box is a road we draw a road texture
            if (curBox.tmpRoad)
            {
                vector2D tileToSpawn = GetPosTile(93);
                Rectangle frameRec = {tileToSpawn.x, tileToSpawn.y, (float)mapText.tilesheet.width / 23, (float)mapText.tilesheet.height / 13};
                DrawTextureRec(mapText.tilesheet, frameRec, {curBox.pBox.x, curBox.pBox.y}, WHITE);
            }
            //If the box is not we draw grass
            else
            {
                vector2D tileToSpawn = GetTileGrass(i, j, tileId);

                //if none we just draw regular grass
                if (tileToSpawn == vector2D{-1, -1})
                {
                    tileToSpawn = GetPosTile(119);
                }
                else
                {
                    curBox.hasScenary = false;
                }

                Rectangle frameRec = {tileToSpawn.x, tileToSpawn.y, (float)mapText.tilesheet.width / constTileSheetWidth, (float)mapText.tilesheet.height / constTileSheetHeight};
                DrawTextureRec(mapText.tilesheet, frameRec, {curBox.pBox.x, curBox.pBox.y}, WHITE);
            }

            if (curBox.hasScenary)
            {
                vector2D tmpVec= GetPosTile(curBox.scenaryId);
                Vector2 srcPos = {tmpVec.x , tmpVec.y};
                Vector2 dstPos = { (tileId % constBoxColumn) * 64.f, (tileId / constBoxColumn) * 64.f };
                DrawTextureRec(mapText.tilesheet, { srcPos.x, srcPos.y, 64.f, 64.f }, { dstPos.x , dstPos.y }, WHITE );
            }
        }
    }

}

//Function to find the box your at with x and y coordonate
int Map::FindBox(float mousePosX, float mousePosY)
{

    //Go trough the list
    for (int i = 0; i < constBoxColumn; i++)
    {
        for (int j = 0; j < constBoxRow; j++)
        {
            //Check x and y position if true we return the number of the box
            if (mousePosX >= mTabBox[i + j * constBoxColumn].pBox.x && mousePosX < mTabBox[i + j * constBoxColumn].pBox.x + constSizeBox &&
                mousePosY >= mTabBox[i + j * constBoxColumn].pBox.y && mousePosY < mTabBox[i + j * constBoxColumn].pBox.y + constSizeBox)
            {
                return i + j * constBoxColumn;
            }
        }
    }
    return -1;
}

//This function check the char in the array and return true if it's a road
bool Map::GetRoad(char lvlDsgn)
{
    switch (lvlDsgn)
    {
    case 'r':
        return true;
    case 'i':
        return true;
    case 'f':
        return true;
    case 'u':
        return true;
    case 'd':
        return true;
    case 's':
        return true;
    case 'e':
        return true;
    case ' ':
        return false;
    default:
        return false;
    }
}

//Get the position of the tile on the tilesheet using the id
vector2D Map::GetPosTile(int tileId)
{
    vector2D tilePose = {(float)(tileId % constTileSheetWidth) * constSizeBox, (float)(tileId / constTileSheetWidth) * constSizeBox};
    return tilePose;
}

bool Map::MapCreator(Resource& resource, bool& shouldQuit)
{
    bool isRoadValid = VerifRoad();

    mIdMap = 0;

    if (!mVariableCreator.isInit)
    {
        mVariableCreator.isInit = true;
        for (char &a : mVariableCreator.levelDesign)
        {
            a = ' ';
        }
    }

    //Init drawing
    ClearBackground(WHITE);
    ShowCadrillage(resource);

    //Handle button
    vector2D mouse = {GetMousePosition().x, GetMousePosition().y};
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        vector2D mouse = {GetMousePosition().x, GetMousePosition().y};
        int id = FindBox(mouse.x, mouse.y);
        if (id != -1 && mVariableCreator.levelDesign[id] == ' ')
        {
            mVariableCreator.levelDesign[id] = mVariableCreator.valToSet;

            if (mVariableCreator.valToSet == 's')
            {
                mVariableCreator.isStartSet = true;
            }
            else if (mVariableCreator.valToSet == 'e')
            {
                mVariableCreator.isEndSet = true;
            }
            mVariableCreator.valToSet = 'r';
        }
    }
    //Erase
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        int id = FindBox(mouse.x, mouse.y);
        if (id != -1)
        {
            if (mVariableCreator.levelDesign[id] == 's')
            {
                mVariableCreator.isStartSet = false;
            }
            else if (mVariableCreator.levelDesign[id] == 'e')
            {
                mVariableCreator.isEndSet = false;
            }
            mVariableCreator.levelDesign[id] = ' ';
        }
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        mVariableCreator.isInit = false;
        mVariableCreator.isEndSet = false;
        mVariableCreator.isStartSet = false;
    }

    // Const value for buttons
    const int width = 80;
    const int height = 60;
    const Color textColor = BLACK;
    const int textSize = 20;
    const Color recHoverColor = LIGHTGRAY;

    //DRAW Element for creation
    {
        static bool warning = false;
        //Full shop
        DrawRectangle(constScreenWidth, 0, 200, constScreenHeight, BROWN);

        //Button Start
        Color classicButtonColor = BLUE;

        Color startSet = BLUE;
        if (mVariableCreator.isStartSet)
        {
            startSet = RED;
        }
        if(ButtonS({constScreenWidth + 55, 125 },{constScreenWidth + 60, 145 } , textSize, width ,height  ,  "START" , textColor , textColor , startSet , recHoverColor) && !mVariableCreator.isStartSet )
        {
            mVariableCreator.valToSet = 's';
        }

        //Button End
        Color endSet = BLUE;
        if (mVariableCreator.isEndSet)
        {
            endSet = RED;
        }
        if(ButtonS({constScreenWidth + 55, 225 },{constScreenWidth + 75, 245 } , textSize, width ,height  ,  "END" , textColor , textColor , endSet , recHoverColor) && !mVariableCreator.isEndSet )
        {
            mVariableCreator.valToSet = 'e';
        }

        //Button Right
        if(ButtonS({constScreenWidth + 55, 325 },{constScreenWidth + 70, 345 } , textSize, width ,height  ,  "Right" , textColor , textColor , classicButtonColor , recHoverColor))
        {
            mVariableCreator.valToSet = 'i';
        }

        //Button Left
        if(ButtonS({constScreenWidth + 55, 425 },{constScreenWidth + 75, 445 } , textSize, width ,height  , "Left" , textColor , textColor , classicButtonColor , recHoverColor ))
        {
            mVariableCreator.valToSet = 'f';
        }

        //Button Up
        if(ButtonS({constScreenWidth + 55, 525 }, {constScreenWidth + 80, 545 } , textSize, width ,height  ,"Up", textColor , textColor , classicButtonColor , recHoverColor ))
        {
            mVariableCreator.valToSet = 'u';
        }

        //Button Down
        if(ButtonS({constScreenWidth + 55, 625 } , {constScreenWidth + 70, 645 } , textSize, width ,height  ,"Down", textColor , textColor , classicButtonColor , recHoverColor )) 
        {
            mVariableCreator.valToSet = 'd';
        }

        if (ButtonS({constScreenWidth + 55, 725}, {constScreenWidth + 70, 745}, 20, 80, 60, "QUIT", BLACK, BLACK, RED, LIGHTGRAY))
        {
            warning = true;
        }
        if (warning == true)
        {
            DrawText("IF YOU QUIT YOU WILL LOSE YOUR MAP", constScreenWidth / 10, constScreenHeight / 2 - 90, 50, RED);
            if (ButtonS({constScreenWidth / 2 - 200, constScreenHeight / 2}, {constScreenWidth / 2 - 185, constScreenHeight / 2 + 15}, 25, 100, 50, "QUIT", BLACK, BLACK, BLUE, LIGHTGRAY))
            {
                warning = false;
                shouldQuit = true;
                return false;
            }
            if (ButtonS({constScreenWidth / 2 , constScreenHeight / 2}, {constScreenWidth / 2 + 15, constScreenHeight / 2 + 15}, 25, 100, 50, "STAY", BLACK, BLACK, BLUE, LIGHTGRAY))
            {
                warning = false;
                return false;
            }
        }

        if (ButtonS({constScreenWidth + 55, 825}, {constScreenWidth + 70, 845}, 20, 80, 60, "PLAY", BLACK, BLACK, GREEN, LIGHTGRAY))
        {
            if (isRoadValid)
            {
                mIdMap = 3;
                MapGeneration(mVariableCreator.levelDesign);
                return true;
            }
            else
            {
                mVariableCreator.checkPathNotValid = true;
            }
        }
    }

    if (mVariableCreator.checkPathNotValid)
    {
        DrawRectangle(constScreenWidth / 2 - 100, constScreenHeight / 2 - 100, 400, 75, BLACK);
        DrawText("INVALID PATH", constScreenWidth / 2 - 90, constScreenHeight / 2 - 90, 50, RED);
        mVariableCreator.frameCounter += GetFrameTime();

        if (mVariableCreator.frameCounter >= 1)
        {
            mVariableCreator.frameCounter = 0;
            mVariableCreator.checkPathNotValid = false;
        }
    }

    //Generate Map
    MapGeneration(mVariableCreator.levelDesign);

    HighlightCurTile();

    return false;
}

bool Map::VerifRoad()
{
    std::vector<Road> testRoads;

    int startId = GetStartRoad(mVariableCreator.levelDesign);

    if (startId == -1)
    {
        return false;
    }

    //init first tile
    mTabBox[startId].hasScenary = true;
    mTabBox[startId].scenaryId = 64;

    //Initialize and set all values
    //way is used to change the incrementation to follow the road
    int way = 1;

    //Set the start position of the enemies
    testRoads.push_back(Road(&mTabBox[startId]));
    //Set them at the left of the box
    startPosition = {0, mTabBox[startId].centerBox.y};
    int counter = startId + way;

    //Create a fake road to make sure it works
    int verifLength = 0;
    for (int id = counter; id < constNumberMaxBox; id += way)
    {
        //Security for no infinite loop
        if (verifLength > constNumberMaxBox)
        {
            testRoads.clear();
            return false;
        }
        verifLength++;

        Box& curBox = mTabBox[id];
        switch(mVariableCreator.levelDesign[id])
        {
            //If no road 
            case ' ' :
            {
                return false;
            }
            case 'f' :
            {
                //Set way at -1 to turn to the left 
                way = -1 ;
                curBox.hasScenary = true;
                curBox.scenaryId = 21;
                break;
            }
            case 'i' :
            {
                //Set way at 1 to turn to the right 
                way = 1;
                curBox.hasScenary = true;
                curBox.scenaryId = 20;
                break;
            }
            case 'd' :
            {
                //Set way at 22 to turn down
                way = 22;
                curBox.hasScenary = true;
                curBox.scenaryId = 22;
                break;
            }
            case 'u' :
            {
                //Set way at -22 to turn up
                way = -22;
                curBox.hasScenary = true;
                curBox.scenaryId = 19;
                break;
            }
            case 'e' :
            {
                //Insane value to break the loop
                way += 10000000000;
                curBox.hasScenary = true;
                curBox.scenaryId = 15;
                break;
            }
            default :
                break;
        } 

        //Add new element
        testRoads.push_back(Road(&mTabBox[id]));
    }

    //Check way is insane so we did find and 'e'
    if (way < 10000000)
    {
        testRoads.clear();
        return false;
    }

    return true;
}

void Map::HighlightCurTile()
{
    m_Rectangle recHighlight;
    int currentBox = FindBox((float)GetMouseX(), (float)GetMouseY());
    if (currentBox != -1)
    {
        recHighlight = mTabBox[currentBox].pBox;
        DrawRectangleLines(recHighlight.x, recHighlight.y, recHighlight.width, recHighlight.height, GRAY);
    }
}