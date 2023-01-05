#pragma once
#include <raylib.h>
#include <vector>
#include "setup.hpp"

struct Box
{
    m_Rectangle pBox;
    vector2D centerBox;
    bool tmpRoad;
    bool isFree;
    bool hasScenary;
    int scenaryId;
};

struct Road
{
    Box* RoadBox;

    Road() = default;
    ~Road();
    Road(Box* rBox );
};

struct ValueMapCreator
{
    bool isInit ;
    bool isStartSet;
    bool isEndSet;
    bool checkPathNotValid;

    char levelDesign[353];
    char valToSet;
    
    float frameCounter;
};

class Map
{
private:
    int mIdMap;
    bool mIsMapCleared;
    
    ValueMapCreator mVariableCreator;

    bool VerifRoad();
    vector2D GetTileGrass(int i , int j, int tileId);

public:
    //Road* head;
    std::vector<Road> roads;
    //Box* mTabBox;
    std::vector<Box> mTabBox;
    vector2D startPosition;

    Map();
    ~Map();


    void Reset(int idMap = 1);
    void ShowCadrillage(Resource& mapText);
    void MapGeneration(const char* levelDesign);
    void ClearMap();
    void HighlightCurTile();

    const char* Choosemap();

    int FindBox(float mousePosX, float mousePosY);
    int GetStartRoad(const char* levelDesign);
    int GetIdMap();

    vector2D GetPosTile(int tileId);

    bool DrawMapChoice(Resource& resource);
    bool GetRoad(char lvlDsgn );
    bool MapCreator(Resource& resource, bool& shouldQuit);
};

 