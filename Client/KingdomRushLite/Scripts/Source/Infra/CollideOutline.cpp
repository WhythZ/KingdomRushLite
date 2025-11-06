#include "../../Header/Infra/CollideOutline.h"

void CollideOutline::DrawBoxOutline(SDL_Renderer* _renderer, SDL_Color _color, SDL_Point _center, SDL_Point _size, double _angle)
{
    //设置绘制颜色
    SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);

    //计算未旋转前的四个角点（相对于中心）
    SDL_FPoint corners[4] = {
        { -_size.x / 2.0f, -_size.y / 2.0f },
        {  _size.x / 2.0f, -_size.y / 2.0f },
        {  _size.x / 2.0f,  _size.y / 2.0f },
        { -_size.x / 2.0f,  _size.y / 2.0f }
    };

    //旋转并转换到屏幕坐标，多一个点用于闭合图形
    SDL_FPoint rotated[5];
    float rad = static_cast<float>(_angle * M_PI / 180.0);
    float cos_angle = cosf(rad);
    float sin_angle = sinf(rad);

    for (int i = 0; i < 4; ++i) {
        rotated[i].x = _center.x + corners[i].x * cos_angle - corners[i].y * sin_angle;
        rotated[i].y = _center.y + corners[i].x * sin_angle + corners[i].y * cos_angle;
    }
    //闭合路径
    rotated[4] = rotated[0];

    //绘制线条（使用浮点版本更精确）
    SDL_RenderDrawLinesF(_renderer, rotated, 5);
}