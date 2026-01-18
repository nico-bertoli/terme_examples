#pragma once
#include <terme/entities/collider.h>	
#include <terme/general/vector2_int.h>

namespace Pong
{
    class PongBar;
    class PongLevel;

    class PongBall : public terme::Collider
    {
        //---------------------------------------------------------- Fields
    public:
        nbkit::Event<> on_goal;
    private:
        PongLevel* level_;
        double y_speed_;
        double x_speed_;
        bool is_first_launch_;
        static inline const terme::Model kModel = terme::Model(1, { '@' });

        //---------------------------------------------------------- Methods
    public:
        PongBall(PongLevel* level, int x_pos, int y_pos, double y_speed);
        terme::TerminalColor GetColor() const override { return terme::color::kGreen; }

    protected:
        bool CanExitScreenSpace() const override { return false; }
        double GetGravityScale() const override { return 0; }
        void OnCollisionEnter(std::shared_ptr<Collider> other, terme::Direction collision_dir) override;
        void Update() override;
        void InitModel() override { SetModel(kModel); }

    private:
        void HandleBarCollision(std::shared_ptr<PongBar> colliding_bar);
    };
}