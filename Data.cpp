//-�w�b�_�[�t�@�C���ǂݍ���-
#include "game.h"
#include "Data.h"
#include "shape.h"	//�}�`�̏���


//-�O���[�o���֐�-
CHARA_DATA enemy[ENEMY_MAX];	//�G�f�[�^

SCORE_DATA score_data;
SCORE_DATA score_dataInit =		//�X�R�A�f�[�^
{ SCORE_DATA_PATH,"NAME1",-1,"NAME2",-1,"NAME3",-1 };//�X�R�A�f�[�^�������p

//-�֐�-

/// <summary>
/// CSV�t�@�C����ǂݍ���
/// </summary>
/// <param name="path">CSV�t�@�C���̃p�X</param>
/// <param name="chara">�`�����f�[�^�\���̂̐擪�A�h���X</param>
/// <param name="DataMax">CSV�t�@�C���̍ő吔</param>
/// <param name="IsHeader">CSV�t�@�C���̈�s�ڂ��w�b�_�Ȃ�TRUE</param>
/// <returns>TRUE�͓ǂݍ��߂�/FALSE�͎��s����</returns>
BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader)
{
	FILE* fp;				//�t�@�C���|�C���^
	char GetChar = '\0';	//�擾���镶��
	int LoopCnt = 0;		//���[�v�J�E���^
	errno_t FileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	int IsEOF = NULL;		//�t�@�C���̏I��肩�`�F�b�N

	//�t�@�C����ǂݍ��݃��[�h(r)�ŊJ��
	FileErr = fopen_s(&fp, path, "r");
	if (FileErr != NULL)
	{
		//�ǂݍ��݃G���[
		MessageBox(GetMainWindowHandle(), path, CSV_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}


	//����ɓǂݍ��߂��牺��

	//CSV�̈�s�ڂ��w�b�_�̎�
	if (IsHeader == TRUE)
	{
		//�ꕶ�������o���A���s���q���g�Ɉ�s�ڂ�ǂݔ�΂�
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//���ۂɃf�[�^���i�[����
	while (IsEOF != EOF)	//End Of File(�t�@�C���̍Ō�)�łȂ��Ƃ��J��Ԃ�
	{
		//CSV�̃f�[�^�̈�s����ǂݎ��
		IsEOF = fscanf_s(
			fp,
			CSV_CHARA_FORMAT,
			&(chara + LoopCnt)->No,
			&(chara + LoopCnt)->Name[0], STR_MAX,
			&(chara + LoopCnt)->HP,
			&(chara + LoopCnt)->ATK,
			&(chara + LoopCnt)->DEF,
			&(chara + LoopCnt)->MAX_HP,
			&(chara + LoopCnt)->MAX_ATK,
			&(chara + LoopCnt)->MAX_DEF
			);

		//���̃f�[�^��
		LoopCnt++;
	}
	
	//�f�[�^�̐����Ⴄ�Ƃ�
	if (LoopCnt - 1 != DataMax)
	{
		//�ǂݍ��݃G���[
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);
		return FALSE;
	}

	fclose(fp);		//�t�@�C�������

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^��ǂݍ���
/// </summary>
/// <param name="path">�X�R�A�f�[�^�̃p�X</param>
/// <param name="score">�X�R�A�f�[�^�\���̂̐擪�A�h���X</param>
/// <param name="IsHeader">CSV�t�@�C���̈�s�ڂ��w�b�_�Ȃ�TRUE</param>
/// <returns>�ǂݍ��߂��Ȃ�TRUE</returns>
BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader = FALSE)
{
	FILE* fp;					//�t�@�C���|�C���^
	char GetChar = '\0';		//�擾���镶��
	errno_t FileErr = NULL;		//�t�@�C���ǂݍ��݃G���[���m�F
	int IsEOF = NULL;			//�t�@�C���̏I��肩�`�F�b�N

	//�p�X�̃R�s�[
	SetScoreDataPath(path);

	FileErr = fopen_s(&fp, path, "r");	//�t�@�C����ǂݍ��݃��[�h�ŊJ��
	if (FileErr != NULL)
	{
		//�X�R�A�f�[�^�쐬
		if (CreateScoreData() == FALSE) { return FALSE; }

		//�X�R�A�X�e�[�^�X�쐬����Ă���͂�

		FileErr = fopen_s(&fp, path, "r");
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), path, SCORE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
	}

	//-����ɓǂݍ��߂��Ȃ�-

	//CSV�̈�s�ڂ��w�b�_�̎�
	if (IsHeader == TRUE)
	{
		//1���������o���A��s�ڂ�ǂݔ�΂�
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//�f�[�^���i�[����
	while (IsEOF != EOF)	//End Of File �łȂ��Ƃ��J��Ԃ�
	{
		//CSV�̃f�[�^��1�s���ǂݎ��
		IsEOF = fscanf_s(
			fp,
			SCORE_DATA_FORMAT,	//�f�[�^���u1�s=1��v�ŏ�����Ă���
			//�������ǂݍ��ނƂ��́A���̈����ŕ�����ő吔���w�肷��
			&(score)->Name1, STR_MAX,
			&(score)->Score1,
			&(score)->Name2, STR_MAX,
			&(score)->Score2,
			&(score)->Name3, STR_MAX,
			&(score)->Score3
		);
	}

	fclose(fp);	//�t�@�C�������
}

/// <summary>
/// �X�R�A�f�[�^�쐬
/// </summary>
BOOL CreateScoreData(VOID)
{
	//�Ȃ�������X�R�A�f�[�^�����
	FILE* crefp;
	errno_t creFileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//�t�@�C�����������݃��[�h�ŊJ��
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�w�b�_����������
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//����̃X�R�A�f�[�^����������
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(crefp);	//�X�R�A�f�[�^�����

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�ۑ�
/// </summary>
BOOL SaveScoreData(VOID)
{
	//�Ȃ�������X�R�A�f�[�^�����
	FILE* crefp;
	errno_t creFileErr = NULL;	//�t�@�C���ǂݍ��݃G���[���m�F
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//�t�@�C�����������݃��[�h�ŊJ��
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//�w�b�_����������
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//�X�R�A�f�[�^����������
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(crefp);	//�X�R�A�f�[�^�����

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// �X�R�A�f�[�^�̍폜
/// </summary>
/// <param name="path">�X�R�A�f�[�^�̃p�X</param>
BOOL DeleteScoreData(VOID)
{
	//�t�@�C���폜
	if (remove(GetScoreDataPath()) != 0)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_ERR_TITLE, MB_OK);
	}
	else
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_TITLE, MB_OK);
	}

	//�X�R�A�f�[�^���쐬
	return CreateScoreData();
}

/// <summary>
/// �X�R�A�f�[�^�̃p�X��Ԃ�
/// </summary>
const char* GetScoreDataPath(VOID)
{
	return score_data.path;
}

/// <summary>
/// �X�R�A�f�[�^�̃p�X��ݒ肷��
/// </summary>
VOID SetScoreDataPath(const char* path)
{
	strcpyDx(score_data.path, path);
	return;
}