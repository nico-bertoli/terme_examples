#pragma once
#include <terme/entities/static_collider.h>

namespace SpaceInvaders
{
    class ShieldPart : public terme::Collider
    {
        //---------------------------------------------------------- Fields
    private:
        terme::Model model_ = CreteModelUsingChar('=', 1, 1);

        //---------------------------------------------------------- Methods
    public:
        using Collider::Collider;

    protected:
        bool CanExitScreenSpace() const override { return false; }
        double GetGravityScale() const override { return 0; }
        terme::TerminalColor GetColor() const override { return terme::color::kGreen; }
        void InitModel() override { SetModel(model_); }
        void OnCollisionEnter(std::shared_ptr<Collider>other, terme::Direction collision_dir)override;
    };
}