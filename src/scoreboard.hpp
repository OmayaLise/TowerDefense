#include "game.hpp"

struct ScoreEntry
{
    std::string name;
    int score;
};

class Scoreboard
{
private:
    const int mMaxInputChar = 3;
    std::string mNamePlayer;
    float mBlinkFramesCounter = 0.f;

    bool mShowBoard = false; // False = inputting name, true = show boards

    void HandleTextInput();
    void ShowBlinkingCursor();
    void SaveScore();
    int ShowScoreboard(Resource& resource);

    std::vector<ScoreEntry> mEntries;

public:
    Scoreboard();
    ~Scoreboard();
    int UpdateAndDraw(const Game& myGame, Resource& resource);
};