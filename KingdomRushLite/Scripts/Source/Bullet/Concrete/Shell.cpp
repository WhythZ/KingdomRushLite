#include "../../../Header/Bullet/Concrete/Shell.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header//Manager/Concrete/AudioManager.h"

Shell::Shell()
{
	//设置子弹类型
	type = BulletType::Shell;

	//根据每帧动画的图片文件实际尺寸设置
	size.x = 48; size.y = 48;

	//设置炮弹的爆炸范围
	damageRadius = (explodeSize.x + explodeSize.y) / 2;

	#pragma region SetAnimation
	//获取炮弹飞行的图片资源并设置动画帧
	static SDL_Texture* _animFlyingSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Shell)->second;
	static const std::vector<int> _animFlyingSpritesIdices = { 0, 1 };
	animFlying.SetAnimFrames(_animFlyingSprites, 2, 1, _animFlyingSpritesIdices);
	animFlying.SetLoop(true);

	//获取炮弹爆炸的动画
	static SDL_Texture* _animExplodingSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Shell_Explode)->second;
	static const std::vector<int> _animExplodingSpritesIdices = { 0, 1, 3, 4, 5 };
	animExploding.SetAnimFrames(_animExplodingSprites, 5, 1, _animExplodingSpritesIdices);
	//炮弹动画并非循环，炸完就没了
	animExploding.SetLoop(false);
	//让炮弹动画结束时调用使炮弹消失的回调函数
	animExploding.SetOnAnimFinished(
		[&]()
		{
			Invalidate();
		}
	);
	#pragma endregion
}

void Shell::OnUpdate(double _delta)
{
	//如果当前炮弹未爆炸，即处于可碰撞的状态，则只调用父类方法
	if (CanCollide())
	{
		//包括炮弹飞行时的动画更新
		Bullet::OnUpdate(_delta);
		return;
	}

	#pragma region ExplosionAnimUpdate
	//否则说明炮弹当前处于爆炸后状态，此时需更新爆炸动画
	animExploding.OnUpdate(_delta);
	#pragma endregion
}

void Shell::OnRender(SDL_Renderer* _renderer)
{
	//如果当前炮弹未爆炸，即处于可碰撞的状态，则只调用父类方法
	if (CanCollide())
	{
		//包括炮弹飞行时的动画渲染
		Bullet::OnRender(_renderer);
		return;
	}

	#pragma region ExplosionAnimRender
	//否则说明炮弹当前处于爆炸后状态，此时需渲染爆炸动画，由于爆炸动画帧的尺寸不同于子弹尺寸，故需要重新确定渲染位置
	static SDL_Point _point;
	_point.x = (int)(position.x - explodeSize.x / 2);
	_point.y = (int)(position.y - explodeSize.y / 2);
	animExploding.OnRender(_renderer, _point);
	#pragma endregion
}

void Shell::OnCollide(Enemy* _enemy)
{
	//父类原方法Bullet::OnCollide(_enemy);会直接使得子弹消失，而炮弹还需要播放爆炸动画后再消失
	DisableCollide();

	//播放命中音效
	AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Shell_Hit);
}