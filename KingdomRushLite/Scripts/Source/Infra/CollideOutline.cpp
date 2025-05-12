#include "../../Header/Infra/CollideOutline.h"

void CollideOutline::DrawBoxOutline(SDL_Renderer* _renderer, SDL_Color _color, SDL_Point _center, SDL_Point _size, double _angle)
{
    //���û�����ɫ
    SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);

    //����δ��תǰ���ĸ��ǵ㣨��������ģ�
    SDL_FPoint corners[4] = {
        { -_size.x / 2.0f, -_size.y / 2.0f },
        {  _size.x / 2.0f, -_size.y / 2.0f },
        {  _size.x / 2.0f,  _size.y / 2.0f },
        { -_size.x / 2.0f,  _size.y / 2.0f }
    };

    //��ת��ת������Ļ���꣬��һ�������ڱպ�ͼ��
    SDL_FPoint rotated[5];
    float rad = static_cast<float>(_angle * M_PI / 180.0);
    float cos_angle = cosf(rad);
    float sin_angle = sinf(rad);

    for (int i = 0; i < 4; ++i) {
        rotated[i].x = _center.x + corners[i].x * cos_angle - corners[i].y * sin_angle;
        rotated[i].y = _center.y + corners[i].x * sin_angle + corners[i].y * cos_angle;
    }
    //�պ�·��
    rotated[4] = rotated[0];

    //����������ʹ�ø���汾����ȷ��
    SDL_RenderDrawLinesF(_renderer, rotated, 5);
}