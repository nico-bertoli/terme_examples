#pragma once

#include <terme/entities/level.h>

namespace Pong
{
    class PongLevel : public terme::Level
    {
        //---------------------------------------------------------- Methods
    private:
        size_t score_player_1_ = 0;
        size_t score_player_2_ = 0;
        //---------------------------------------------------------- Methods
    public:
        int GetWorldSizeX() const override { return 50; }
        int GetWorldSizeY() const override { return 35; }
        int GetScreenPadding() const override { return 2; }
        void LoadInSimulation() override;
        void IncreaseP1Score();
        void IncreaseP2Score();

    protected:
        double ShowGameOverScreenDelay() const override { return 0.5; }
        void OnPostGameOverDelayEnded() override;
        void OnGameOver() override;

    private:
        void RefreshHeader();
    };
}