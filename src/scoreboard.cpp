#include <algorithm>
#include <fstream>
#include "scoreboard.hpp"

Scoreboard::Scoreboard()
{
    std::ifstream scoreboardFile("gameScore.bin");

    std::string name;
    int score;
    while (scoreboardFile >> name >> score) // Failed if name and score is not parsed
    {
        mEntries.push_back({name, score});
    }

    scoreboardFile.close();
}

Scoreboard::~Scoreboard()
{
    mEntries.clear();
}

bool compareScore(const ScoreEntry &a, const ScoreEntry &b)
{
    return a.score > b.score;
}

//Draw the name creation return true if the name has been written
int Scoreboard::UpdateAndDraw(const Game &game, Resource& resource)
{
    ClearBackground(BLACK);
    if (mShowBoard)
    {
        return ShowScoreboard(resource);
    }
    else
    {
        //Instruction text
        DrawText("ENTER YOUR TAG", constScreenWidth / 3 + 120, constScreenHeight / 3 - 100, 40, MAROON);
        DrawText("3 CHARACTER ONLY MAJ", constScreenWidth / 3 + 60, constScreenHeight / 3 + 100, 40, MAROON);

        //Raylib core to get input text
        HandleTextInput();

        //Draw the name
        DrawText(mNamePlayer.c_str(), constScreenWidth / 2 + 40, constScreenHeight / 3, 40, MAROON);

        //Draw a blinking underscore after the text
        // increment the time counter
        ShowBlinkingCursor();

        if ((mNamePlayer.length() == (size_t)mMaxInputChar) && IsKeyPressed(KEY_ENTER))
        {
            // Add entry
            mEntries.push_back({mNamePlayer, game.GetRound()});

            // Sort them
            std::sort(mEntries.begin(), mEntries.end(), compareScore);

            SaveScore();
            mShowBoard = true;
        }
        return 0;
    }
}

void Scoreboard::HandleTextInput()
{
    int key = GetCharPressed();
    while (key > 0)
    {
        // Only allow key between 65 and 90 from raylib enum
        // So only alphabetics maj character can be entered
        if (((key >= 65) && (key <= 90)) && (mNamePlayer.length() < (size_t)mMaxInputChar))
        {
            mNamePlayer += key;
        }

        key = GetCharPressed(); // Check next character in the queue
    }
    //Remove last character
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        //Security if user try to delete a character that doesn't exist
        if (mNamePlayer.empty() == false)
        {
            mNamePlayer.pop_back();
        }
    }
}

void Scoreboard::ShowBlinkingCursor()
{
    mBlinkFramesCounter += GetFrameTime();
    if (mBlinkFramesCounter >= 0.8 && mNamePlayer.size() < 3)
    {
        DrawText("_", constScreenWidth / 2 + 40 + MeasureText(mNamePlayer.c_str(), 40), constScreenHeight / 3, 40, MAROON);
        if (mBlinkFramesCounter >= 1)
            mBlinkFramesCounter = 0;
    }
}

//Save the score in the gameScore file
void Scoreboard::SaveScore()
{
    std::ofstream scoreboardFile;
    scoreboardFile.open("gameScore.bin");

    for (const ScoreEntry &entry : mEntries)
    {
        scoreboardFile << entry.name << " ";
        scoreboardFile << entry.score << '\n';
    }

    scoreboardFile.close();
    mNamePlayer.clear();
}

int Scoreboard::ShowScoreboard(Resource& resource)
{

    std::string line;
    int drawCounter = 1;
    DrawText("SCOREBOARD", constScreenWidth / 3 + 200, constScreenHeight / 3 - 150, 40, MAROON);
    for (size_t i = 0; i < mEntries.size(); i++)
    {
        DrawText(TextFormat("%d : %s %d", drawCounter, mEntries[i].name.c_str(), mEntries[i].score), constScreenWidth / 3 + 200, constScreenHeight / 3 - 100 + (drawCounter * 50), 40, MAROON);
        drawCounter++;
    }

    DrawTexturePro(resource.home, {48, 30, 602, 538}, {(constScreenWidth + 200) * 0.5 - 350 - 150, 250 + 150, 150, 150}, {0, 0}, 0, WHITE);
    DrawTexturePro(resource.quit, {50, 0, 600, 600}, {(constScreenWidth + 200) * 0.5 + 200 + 140, 250 + 150, 150, 150}, {0, 0}, 0, WHITE);

    if (InRec(GetMouseX(),GetMouseY(), (constScreenWidth + 200) * 0.5 - 350 - 150, 250 + 150, 150, 150) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return 1;
    }
    if (InRec(GetMouseX(),GetMouseY(), (constScreenWidth + 200) * 0.5 + 200 + 140, 250 + 150, 150, 150) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return 2;
    }

    return 0;
}