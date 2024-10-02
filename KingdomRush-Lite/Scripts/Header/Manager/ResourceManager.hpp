#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include "Manager.hpp"

//ͼ����Դ��
enum SpriteResPool
{
	Player,                      //��ҿ��ƽ�ɫ��֡����
	VFX_Flash_Up,                //��Ҽ������磬������
	VFX_Flash_Down,              //��Ҽ������磬������
	VFX_Flash_Left,              //��Ҽ������磬������
	VFX_Flash_Right,             //��Ҽ������磬������
	VFX_Impact_Up,               //
	VFX_Impact_Down,             //
	VFX_Impact_Left,             //
	VFX_Impact_Right,            //
	VFX_Explode,                 //

	Tile_TileSet,                //��Ƭ�ز�ͼ��
	Tile_Home,                   //����Ƭ
	Tile_Spawner,                //�������ɵ���Ƭ

	Tower_Archer,                //�����ַ�����
	Tower_Axeman,                //Ͷ���ַ�����
	Tower_Gunner,	             //ǹ���ַ�����
	Bullet_Arrow,                //����Ͷ����
	Bullet_Axe,                  //����Ͷ����
	Bullet_Shell,                //�ڵ�Ͷ����

	Enemy_Slime,                 //ʷ��ķ֡����
	Enemy_KingSlime,             //��ʷ��ķ֡����
	Enemy_Skeleton,              //���ñ�֡����
	Enemy_Goblin,                //�粼��֡����
	Enemy_PriestGoblin,          //��ʦ�粼��֡����
	Enemy_Slime_Sketch,          //ʷ��ķ��ɫ��Ӱ�������ܻ���Ч
	Enemy_KingSlime_Sketch,      //��ʷ��ķ��ɫ��Ӱ
	Enemy_Skeleton_Sketch,       //���ñ���ɫ��Ӱ
	Enemy_Goblin_Sketch,         //�粼�ְ�ɫ��Ӱ
	Enemy_PriestGoblin_Sketch,   //��ʦ�粼�ְ�ɫ��Ӱ

	UI_CoinDrop,                 //����Ľ��
	UI_CoinOwned,                //��ҽ���ܶ�
	UI_HealthBar,                //�������ֵ

	UI_Avatar_Player,            //���ͷ��
	UI_Avatar_Home,              //��ͷ��
	
	UI_SelectCursor,             //ѡ�ж������Ч
	UI_Place_Idle,               //���÷�����������UI
	UI_Place_HoveredTop,         //ָ�������ڷ��������ϲ�ʱ�ķ�����Ч
	UI_Place_HoveredLeft,        //ָ�������ڷ���������ʱ�ķ�����Ч
	UI_Place_HoveredRight,       //ָ�������ڷ��������Ҳ�ʱ�ķ�����Ч
	UI_Upgrade_Idle,             //����������������UI
	UI_Upgrade_HoveredTop,       //ָ�����������������ϲ�ʱ�ķ�����Ч
	UI_Upgrade_HoveredLeft,      //ָ������������������ʱ�ķ�����Ч
	UI_Upgrade_HoveredRight,     //ָ�����������������Ҳ�ʱ�ķ�����Ч

	UI_Text_Win,                 //ʤ��������ʾ
	UI_Text_Loss,                //ʧ��������ʾ
	UI_GameOver                  //ʧ�ܽ���
};

//��Ƶ��Դ��
enum AudioResPool
{
	BGM_Main,                    //����������

	SFX_PlayerSkill_Flash,       //��Ҽ�����Ч
	SFX_PlayerSkill_Impact,      //��Ҽ�����Ч

	SFX_Pick_Coin,               //Ӳ�Ҽ�ʰ��Ч
	SFX_HomeHurt,                //�ұ�������Ч

	SFX_Tower_Place,             //������������Ч
	SFX_Tower_Upgrade,           //������������Ч

	SFX_Arrow_Shoot_1,           //��ʸͶ����Ч1
	SFX_Arrow_Shoot_2,           //��ʸͶ����Ч2
	SFX_Arrow_Hit_1,             //��ʸ������Ч1
	SFX_Arrow_Hit_2,             //��ʸ������Ч2
	SFX_Arrow_Hit_3,             //��ʸ������Ч3

	SFX_Axe_Shoot,               //��ͷͶ����Ч
	SFX_Axe_Hit_1,               //��ͷ������Ч1
	SFX_Axe_Hit_2,               //��ͷ������Ч2
	SFX_Axe_Hit_3,               //��ͷ������Ч3

	SFX_Shell_Shoot,             //ը��Ͷ����Ч
	SFX_Shell_Hit,               //ը��������Ч

	SFX_Win,                     //��Ϸʤ����Ч
	SFX_Loss,                    //��Ϸʧ����Ч
};

//������Դ��
enum FontResPool
{
	Pixel_CN,                   //��Ϸ�����壨�����ģ�
};

//��Ϸ�ʲ�������
class ResourceManager :public Manager<ResourceManager>
{
	friend class Manager<ResourceManager>;

private:


public:
};

#endif
