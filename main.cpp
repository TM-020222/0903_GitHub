#define _CRT_SECURE_NO_WARNINGS
//�w�b�_�t�@�C���ǂݍ���
#include"game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//����̍\����
struct MOVIE
{
	int handle = -1;		//����̃n���h��(�Ǘ��ԍ�)
	char path[255];			//����̏ꏊ(�p�X)

	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int volume = 255;		//�{�����[�� 0-255

	//BOOL IsDraw = FALSE;	//���悪�`��ł���H
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;		//����̃n���h��(�Ǘ��ԍ�)
	char path[255];			//����̏ꏊ(�p�X)

	int volume = -1;		//�{�����[�� 0-255
	int playtype = -1;		//BGM or SE
};

//�摜�̍\����
struct IMAGE
{
	int handle = -1;		//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];			//�摜�̏ꏊ(�p�X)
	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	BOOL IsDraw = FALSE;	//�摜���`��ł���H
};

//�L�����N�^�̍\����
typedef struct CHARACTOR
{
	IMAGE img;

	int speed = 1;			//�ړ����x

	RECT coll;				//�����蔻��̗̈�(�l�p)
}CHARA;

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	// �� �̃Q�[���̃V�[��

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn  = FALSE;	//�t�F�[�h�C��

//�V�[���؂�ւ�
int fadeTimeMill = 2000;						//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�C���̃J�E���^
int fadeInCntMax = 0;				//�t�F�[�h�C���̃J�E���^MAX 0?

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID Over(VOID);		//�Q�[���I�[�o�[���
VOID OverProc(VOID);	//�Q�[���I�[�o�[���(����)
VOID OverDraw(VOID);	//�Q�[���I�[�o�[���(�`��)

BOOL GameLoad(VOID);	//�Q�[���f�[�^�̃��[�h

//���y�̃��[�h
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playtype);

//�摜�̃��[�h
BOOL LoadImg(IMAGE* image, const char* path);

VOID GameInit(VOID);	//�Q�[���̏�����
VOID TitleInit(VOID);	//�^�C�g���̏�����

VOID CollUpdate(CHARA* chara);	//�����蔻��̗̈���X�V

BOOL CubeCollision(RECT A, RECT B);		//��`�Ƌ�`�̓����蔻��

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���
//DxLib��DirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	//Log.txt���o�͂��Ȃ�

	ChangeWindowMode(TRUE);			//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);	//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//�E�B���h�E�̉𑜓x��ݒ�

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//�E�B���h�E�̑傫����ݒ�

	SetBackgroundColor(255, 255, 255);
	//�f�t�H���g�̔w�i�̐F

	SetWindowIconID(GAME_ICON_ID);
	//�A�C�R���t�@�C����ǂݍ���

	SetWindowStyleMode(GAME_WINDOW_BAR);
	//�E�B���h�E�o�[�̏��

	SetWaitVSyncFlag(TRUE);
	//�f�B�X�v���C�̐���������L���ɂ���

	SetAlwaysRunFlag(TRUE);
	//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���̓ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();	//DxLib�I��
		return -1;		//�����I��
	}

	//�Q�[���̏�����
	GameInit();

	//�^�C�g���̏�����
	TitleInit();

	while (1)
	{
		//�L�[�������ꂽ�Ƃ�
		/*if (CheckHitKeyAll() != 0) { break; }*/

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0) { break; }

		//��ʏ���
		if (ClearDrawScreen() != 0) break;
		
		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) break;

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
			OldGameScene = GameScene;

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		case GAME_SCENE_OVER:	//�Q�[���I�[�o�[���
			Over();
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();
		
		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��;
	}

	//�I���Ƃ��̏���

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE,�ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	return TRUE;
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playtype">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playtype)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path);					//�p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path);	//���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ��������́A�G���[������
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			audio->path,					//���b�Z�[�W�{��
			TEXT("���y�ǂݍ��݃G���[�I"),	//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);

		return FALSE;
	}
	//���̑��̐ݒ�
	audio->volume = volume;
	audio->playtype = playtype;

	return TRUE;
}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name=""></param>
BOOL LoadImg(IMAGE* image, const char* path)
{
	//�摜�̓ǂݍ���
	strcpyDx(image->path, path);					//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);	//���y�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ��������́A�G���[������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			image->path,					//���b�Z�[�W�{��
			TEXT("�摜�ǂݍ��݃G���[�I"),	//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);

		return FALSE;
	}
	//���̑��̐ݒ�
	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	return TRUE;
}



/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	

	return;
}

/// <summary>
/// �^�C�g���̉摜��������
/// </summary>
/// <param name=""></param>
VOID TitleInit(VOID)
{

}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	//�v���C�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	//�G���h�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE && GAME_DEBUG == TRUE)
	{

		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		return;
	}

	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		
	}
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		
	}
	
	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i�����`��

	
	DrawString(0, 0, "�v���C���", GetColor(0,0,0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	//�^�C�g���V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g���̏�����
		TitleInit();

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	
	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//����
	ChangeDraw();	//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else	//�t�F�[�h�C���������I�������
		{
			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C���I��
		}
	}
	
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else	//�t�F�[�h�A�E�g�������I�������
		{
			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;			//�t�F�[�h�A�E�g�I��
		}
	}


	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			// �v���C ��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();			// �G���h ��ʂ̕`��
		break;
	case GAME_SCENE_OVER:
		OverDraw();
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntInit) * 255);	//0-255
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);	//0-255
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0),TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 16, "�؂�ւ����", GetColor(128, 128, 128));
	return;
}

/// <summary>
/// �Q�[���I�[�o�[���
/// </summary>
VOID Over(VOID)
{
	OverProc();	//����
	OverDraw();	//�`��

	return;
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̏���
/// </summary>
VOID OverProc(VOID)
{
	//�^�C�g���V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g���̏�����
		TitleInit();

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	
	return;
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID OverDraw(VOID)
{
	DrawString(0, 0, "�Q�[���I�[�o�[���", GetColor(0, 0, 0));
	//�t�F�[�h�C��
	
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARA* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

BOOL CubeCollision(RECT A,RECT B)
{
	if (A.left<B.right			//��`A�̍���X���W < ��`B�̉E��X���W����
		&& A.right>B.left		//��`A�̉E��X���W > ��`B�̍���X���W����
		&& A.top<B.bottom		//��`A�̏��Y���W < ��`B�̉���Y���W����
		&& A.bottom>B.top)		//��`A�̉���Y���W > ��`B�̏��Y���W
	{
		//�������Ă���Ƃ�
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ��Ƃ�
		return FALSE;
	}
}