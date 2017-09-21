//�n�[�g��`���悤�ȓ���������
#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>

//�E�B���h�E�T�C�Y
#define MIN_X 0
#define MIN_Y 0
#define MAX_X 640
#define MAX_Y 480

//��ʂ̒��S���W
#define CENTER_X ((MIN_X + MAX_X)/2)
#define CENTER_Y ((MIN_Y + MAX_Y)/2)

//�p�x�̌v�Z(����rad)
#define OMEGA( t ) (t * M_PI / 180)

//��ʓ��ɍő�1000���̒e
#define MAX_BULLET 2000

//#define BLAST_FIG_NUM 16//��������A�j���̉摜��
//int BlastHandle[BLAST_FIG_NUM];//�����摜�n���h��
int t;//����
int bullet_img;//�e�̉摜

//���@
struct Player
{
	int x;                   
	int y;
	int img;
	double range;//�����蔻��̔��a
};

struct Player player;

//�G
struct Enemy
{
	int x;//���W
	int y;
	int img;//�摜
};

struct Enemy enemy;

//�X�̒e
struct Bullet
{
	double x;//���W
	double y;
	double speed;
	double angle;
	double range ;//�����蔻��̔��a
	bool isExist;//���݂�����true�A���Ȃ�������false
	int img;//�摜
};

struct Bullet bullet[MAX_BULLET];//MAX_BULLET��Bullet


//������
void Init()
{
	int i;

	t=0;//���ԏ�����

	//���@��K���ȍ��W��
	player.x = CENTER_X;
	player.y = CENTER_Y * 1.5 ;

	player.range = 5;

	//�G��K���ȍ��W��
	enemy.x=CENTER_X;
	enemy.y=CENTER_Y/2;

	//�e��S�ď�����
	for(i = 0; i < MAX_BULLET; i++)
	{
		bullet[i].isExist = false;
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].speed = 0;
		bullet[i].angle = 0;
		bullet[i].range = 0;
	}

}

//�摜�E���t�@�C����ǂݍ��ފ֐�
void LoadData()
{
	player.img = LoadGraph("player.png");
	enemy.img = LoadGraph("enemy.png");
	bullet_img = LoadGraph("bullet.png");
//	LoadDivGraph( "bomb0.png", BLAST_FIG_NUM, 8, 2, 768/8, 192/2, BlastHandle );//�����摜�o�^ 640x480bomb0.png
}

//���@�̈ړ�
void MovePlayer()
{
	const int speed = 4;

	if( CheckHitKey(KEY_INPUT_LEFT) )
		player.x -= speed;
	if( CheckHitKey(KEY_INPUT_RIGHT) )
		player.x += speed;
	if( CheckHitKey(KEY_INPUT_UP) )
		player.y -= speed;
	if( CheckHitKey(KEY_INPUT_DOWN) )
		player.y += speed;

	if( player.x < MIN_X )
		player.x = MIN_X;
	else if( player.x > MAX_X )
		player.x = MAX_X;
	if( player.y < MIN_Y )
		player.y = MIN_Y;
	else if( player.y > MAX_Y)
		player.y = MAX_Y;

}

//���@�̕\��
void DrawPlayer()
{
	DrawRotaGraphF( (float)player.x, (float)player.y, 1.0, 0, player.img, TRUE ) ;
}

//���@�_���̊p�x
double TargetAnglePlayer(double x, double y)
{	
	return atan2( player.y-y, player.x-x );
}

//�e���̐���

//�����e(x,y:���˒n�_, speed:���x, angle:�p�x)
void MakeBullet(double x, double y, double speed, double angle, double range)
{
	int i; 

	for( i = 0; i < MAX_BULLET; i++)
	{
		//bullet[i]���g�p����Ă��Ȃ���΃p�����[�^�ݒ��
		if( !bullet[i].isExist )
			break;
	}
	if ( i == MAX_BULLET )//����󂢂ĂȂ�
		return;
	bullet[i].isExist = true;
	//���˒n�_�̍��W
	bullet[i].x = x;
	bullet[i].y = y;
	bullet[i].angle = angle;//���ˊp�x
	bullet[i].speed = speed;//���x
	bullet[i].range = range;
	bullet[i].img = bullet_img;//�摜�̑��
}


//�e�̈ړ�
void MoveBullet()
{
	double x, y;
	double angle;
	int i;

	//���˒��̒e��S�Ē��ׂ�

    for(i = 0; i < MAX_BULLET; i++)
	{
	    if( !bullet[i].isExist )
	        continue;

        x = bullet[i].x;
		y = bullet[i].y;

        angle = bullet[i].angle;

        //�p�xangle������speed���i�߂�
		x += bullet[i].speed * cos( angle );
		y += bullet[i].speed * sin( angle );

        //�e����ʊO�ɏo���ꍇ�A�e������
        if( x < MIN_X || x > MAX_X || y < MIN_Y || y > MAX_Y)
		    bullet[i].isExist = false;
            
        bullet[i].x = x;
        bullet[i].y = y;
    }

}


//way�e(way:�������ɑł�, angle:��`�̊p�x, main_angle:��`���ǂ̕�����������)
void MakeWayBullet(double x, double y, double speed, int way, double wide_angle, double main_angle, double range)
{
	int i; 
    
	double w_angle;

    for( i = 0; i < way; i++)
    {
		w_angle = main_angle + i * wide_angle / ( way - 1 ) - wide_angle / 2;//���ˊp�x
		if(i%3==0)
			MakeBullet(x,y,speed,w_angle,range);
		else
			MakeBullet(x,y,speed,-w_angle,range);
    }
    
}

//�G�̍s��
void ActionEnemy()
{
	const int fire = 5;
	const double speed = 1.0;
	const double wide_angle = OMEGA(180);
	const int way = 2;
	const double angle = OMEGA(180);
	const double range = 4;

	//�G�̓���(���͌Œ�)
	//	enemy.x = CENTER_X;
	//	enemy.y = CENTER_Y/2;
 
	//fire��̃��[�v���Ƃɒe�𔭎�
	//if( t % fire == 0 )
	//	MakeBullet(enemy.x, enemy.y, speed, TargetAnglePlayer(enemy.x,enemy.y), range);
	if( t % fire == 0 )
			MakeWayBullet(enemy.x, enemy.y, speed, way, wide_angle, OMEGA(t)+180, range);
}

//�e�̕\��
void DrawBullet()
{
	double x,y,angle;
	int img;

	int i;

	//���˒��̒e��S�Ē��ׂ�
	for(i = 0; i < MAX_BULLET; i++)
	{
	    if( !bullet[i].isExist )
	        continue;

        x = bullet[i].x ;
        y = bullet[i].y ;

        angle = bullet[i].angle ;

        img = bullet[i].img ;

        //�e�̕\��
        DrawRotaGraphF( (float)x, (float)y, 1.0, angle, img, TRUE ) ;
        
    }
}

//�G�̕\��
void DrawEnemy()
{
	DrawRotaGraph( enemy.x, enemy.y, 1.0, 0.0, enemy.img, TRUE ) ;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow )
{
	// �^�C�g���� �ύX
	SetMainWindowText( "�����e" ) ;
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode( TRUE ) ;
	// �c�w���C�u��������������
	if( DxLib_Init() == -1 )		// �c�w���C�u��������������
		return -1 ;			// �G���[���N�����璼���ɏI��

	// �`����ʂ𗠂ɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	LoadData();
	Init();

	//���C�����[�v Esc�L�[�ŏI��
	while(!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{

		ClearDrawScreen();
		
		//���@�̈ړ�
		MovePlayer();

		//�G�̍s��
		ActionEnemy();

		//�e�̈ړ�
		MoveBullet();

		//�G�̕\��
		DrawEnemy();

		//���@�̕\��
		DrawPlayer();

		//�e�̕\��
		DrawBullet();

		t++;//���Ԃ�i�߂�

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}
	return 0;
}
