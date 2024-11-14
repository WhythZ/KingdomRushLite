#ifndef _ANIMATION_HPP_
#define _ANIMATION_HPP_

#include <vector>
//����ص�����
#include <functional>
//����֧��
#include <SDL.h>
//�����ʱ��
#include "../Timer/Timer.hpp"

//�������ŵ�Ĭ��֡���
#define FRAME_INTERVAL 0.1

class Animation
{
private:
	Timer timer;                                     //��ʱ������֡��������
	std::function<void()> trigger;                   //�������Ž����󴥷��Ļص�����
	bool isLoop = true;                              //�Ƿ�ѭ�����Ŷ���

	SDL_Texture* srcSpriteSheetTexture = nullptr;    //ʹ�õ�ԴSpriteSheetͼƬ
	std::vector<SDL_Rect> srcSpriteSheetRects;       //�ü�SpriteSheet��ȡ�������֡ͼƬ

	size_t frameIdx = 0;                             //�������ŵ���֡������
	int frameWidth = 0, frameHeight = 0;             //ÿ֡��������Ŀ��
	SDL_Texture* dstAnimTexture = nullptr;           //ÿ֡����ʵ�ʲ��ŵ�����ͼƬ
	SDL_Rect* animRect = nullptr;                    //ָ������ͼƬ��Ⱦ�ڴ����ϵ�λ��

public:
	Animation();
	~Animation() = default;

	//���ö����زģ����Ե��Ĳ�������������б��ȡ�����ԴSpriteSheetͼƬ����������λ����������֡ͼƬ�������е����ɲ��־�������������������֡
	void SetAnimFrames(SDL_Texture*, int, int, const std::vector<int>&);
	void SetAnimEndTrigger(std::function<void()>);   //���ö������Ž����Ļص�����
	void SetLoop(bool);                              //���ö����Ƿ�ѭ������
	void SetFrameInterval(double);                   //����ÿ֡�������ŵ�ʱ����������ʱ����waitTime

	//���ö������ţ���������Ⱦ������������Ⱦ��λ�ã�Rect�����϶��㣩����������ת�Ƕȣ������ӵ��������淽����ת��
	void OnRender(SDL_Renderer*, const SDL_Point&, double);
	void OnUpdate(double);                           //������֡����
	void Restart();                                  //���ö�������
};

Animation::Animation()
{
	//���ü�ʱ���ļ��ʱ�䴥��������ÿ֡���������Ա�����Զ�δ���
	timer.SetOneShot(false);

	//����֡������˴�����Ĭ��֡���
	this->SetFrameInterval(FRAME_INTERVAL);

	//ʹ�������������ü�ʱ��ʹ�õĻص�������ÿ֡���������󶼻ᴥ���ú�����
	//�ڲ�Ƕ����ÿ��Animation��������Ļص����������ڶ������һ֡���ź󴥷��ú�����
	timer.SetTimeOutTrigger(
		//[&]�������ò����ⲿ�������������ǲ����б��������Ǻ�����
		[&]()
		{
			//����֡����
			frameIdx++;
			//���֡�����Ƿ񳬹�������������Χ
			if (frameIdx >= srcSpriteSheetRects.size())
			{
				//��ѭ�����ţ��򷵻ص�һ֡
				if (isLoop)
					frameIdx = 0;
				//��֮�����䱣�������һ֡
				else
				{
					frameIdx = (srcSpriteSheetRects.size() - 1);
					//����ص��������ڣ��͵��øú���
					if (trigger)
						trigger();
				}
			}
		}
	);
}

void Animation::SetAnimFrames(SDL_Texture* _spriteSheet, int _horizontalNum, int _verticalNum, const std::vector<int>& _idxList)
{
	//�洢���ڲü���ԴSpriteSheet
	srcSpriteSheetTexture = _spriteSheet;
	
	//��ѯ����¼SpriteSheet�Ŀ��
	int _srcWidth, _srcHeight;
	SDL_QueryTexture(srcSpriteSheetTexture, nullptr, nullptr, &_srcWidth, &_srcHeight);

	//����֡����ͼƬ�Ŀ��
	frameWidth = _srcWidth / _horizontalNum;
	frameHeight = _srcHeight / _verticalNum;

	//ͨ������������б�ȷ�����ڽ�ȡSpriteSheet�����SDL_Rect�б�
	//�ȸ����б�����ͬ�ĳߴ�
	srcSpriteSheetRects.resize(_idxList.size());
	//������������������и�����ľ���
	for (int i = 0; i < _idxList.size(); i++)
	{
		//����SpriteSheet��3x3�ģ���ô����2��ʾ��1�е�3��������4��ʾ��2�е�2��
		int _idx = _idxList[i];
		int _x = _idx % _horizontalNum;
		int _y = _idx / _verticalNum;

		//��ȡ�ü�����ľ���֡����ͼƬ
		SDL_Rect _rect = {
			//��Ӧÿ֡ͼƬ�����϶�������
			_x * frameWidth,_y * frameHeight,
			//��֡ͼƬ�Ŀ��
			frameWidth,frameHeight
		};

		//����ȡ�ľ��θ����б���ȥ
		srcSpriteSheetRects[i] = _rect;
	}
}

void Animation::SetAnimEndTrigger(std::function<void()> _callBack)
{
	trigger = _callBack;
}

void Animation::SetLoop(bool _loop)
{
	isLoop = _loop;
}

void Animation::SetFrameInterval(double _time)
{
	timer.SetWaitTime(_time);
}

void Animation::OnRender(SDL_Renderer* _renderer, const SDL_Point& _dstPos, double _angle = 0)
{
	//����Ϸ���ڵĲ��о��Σ�����ȷ����Ⱦ��Ŀ��λ��
	//���Rect�Ǿ�̬�ģ�����OnRender��Ƶ�����ã����Ծ�̬�ľֲ����������ڼ����½����������Ŀ�����
	//����Animation������ô˺���ʱ����ͬһ��Rect������������Ҫȷ��ʵʱ���£������з�����
	static SDL_Rect _dstPosRect;
	_dstPosRect =
	{
		_dstPos.x, _dstPos.y,
		frameWidth, frameHeight
	};

	//�����Ǿ�̬�ģ�����д��ֻ���ڳ�ʼ����ʱ�򱻸�ֵһ�Σ����󶼲��ᱻ���£����Իᵼ�¹��ﶯ��ԭ��̤��
	//static SDL_Rect _dstPosRect =
	//{
	//	_dstPos.x,_dstPos.y,
	//	frameWidth,frameHeight
	//};
	//std::cout << "_dstPosRect.x/y=(" << _dstPosRect.x << "," << _dstPosRect.y << ")\n";

	//SDL_RenderCopy�Ĳ������װ棺��Ⱦ����Դ���ʡ�Դ���ʵĲ��о��Ρ�Ŀ����Ⱦλ�ã��������ڵĲ��о��Σ�����ת�Ƕȡ���ת�����ĵ㣨Ĭ�ϼ������ģ�������ת����ö��
	SDL_RenderCopyEx(_renderer, srcSpriteSheetTexture, &srcSpriteSheetRects[frameIdx], &_dstPosRect, _angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

void Animation::OnUpdate(double _delta)
{
	timer.OnUpdate(_delta);
}
 
void Animation::Restart()
{
	//��ʱ�����¿�ʼ��ʱ
	timer.Restart();
	//֡��������
	frameIdx = 0;
}

#endif