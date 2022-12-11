#pragma once

namespace Tmpl8 {
    class Camera
    {
    public:
        Camera();
        ~Camera();

        void Update();
        void SetPosition(vec2 position);
        void SetRotation(float angle);

    private:
        vec2 m_position;
        float m_rotation;
    };
} // namespace Tmpl8