//-ヘッダーファイル読み込み-
#include "game.h"
#include "Data.h"
#include "shape.h"	//図形の処理


//-グローバル関数-
CHARA_DATA enemy[ENEMY_MAX];	//敵データ

SCORE_DATA score_data;
SCORE_DATA score_dataInit =		//スコアデータ
{ SCORE_DATA_PATH,"NAME1",-1,"NAME2",-1,"NAME3",-1 };//スコアデータ初期化用

//-関数-

/// <summary>
/// CSVファイルを読み込む
/// </summary>
/// <param name="path">CSVファイルのパス</param>
/// <param name="chara">チャラデータ構造体の先頭アドレス</param>
/// <param name="DataMax">CSVファイルの最大数</param>
/// <param name="IsHeader">CSVファイルの一行目がヘッダならTRUE</param>
/// <returns>TRUEは読み込めた/FALSEは失敗した</returns>
BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader)
{
	FILE* fp;				//ファイルポインタ
	char GetChar = '\0';	//取得する文字
	int LoopCnt = 0;		//ループカウンタ
	errno_t FileErr = NULL;	//ファイル読み込みエラーを確認
	int IsEOF = NULL;		//ファイルの終わりかチェック

	//ファイルを読み込みモード(r)で開く
	FileErr = fopen_s(&fp, path, "r");
	if (FileErr != NULL)
	{
		//読み込みエラー
		MessageBox(GetMainWindowHandle(), path, CSV_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}


	//正常に読み込めたら下へ

	//CSVの一行目がヘッダの時
	if (IsHeader == TRUE)
	{
		//一文字ずつ取り出し、改行をヒントに一行目を読み飛ばす
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//実際にデータを格納する
	while (IsEOF != EOF)	//End Of File(ファイルの最後)でないとき繰り返す
	{
		//CSVのデータの一行分を読み取る
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

		//次のデータへ
		LoopCnt++;
	}
	
	//データの数が違うとき
	if (LoopCnt - 1 != DataMax)
	{
		//読み込みエラー
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);
		return FALSE;
	}

	fclose(fp);		//ファイルを閉じる

	return TRUE;
}

/// <summary>
/// スコアデータを読み込む
/// </summary>
/// <param name="path">スコアデータのパス</param>
/// <param name="score">スコアデータ構造体の先頭アドレス</param>
/// <param name="IsHeader">CSVファイルの一行目がヘッダならTRUE</param>
/// <returns>読み込めたならTRUE</returns>
BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader = FALSE)
{
	FILE* fp;					//ファイルポインタ
	char GetChar = '\0';		//取得する文字
	errno_t FileErr = NULL;		//ファイル読み込みエラーを確認
	int IsEOF = NULL;			//ファイルの終わりかチェック

	//パスのコピー
	SetScoreDataPath(path);

	FileErr = fopen_s(&fp, path, "r");	//ファイルを読み込みモードで開く
	if (FileErr != NULL)
	{
		//スコアデータ作成
		if (CreateScoreData() == FALSE) { return FALSE; }

		//スコアステータス作成されているはず

		FileErr = fopen_s(&fp, path, "r");
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), path, SCORE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
	}

	//-正常に読み込めたなら-

	//CSVの一行目がヘッダの時
	if (IsHeader == TRUE)
	{
		//1文字ずつ取り出し、一行目を読み飛ばす
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//データを格納する
	while (IsEOF != EOF)	//End Of File でないとき繰り返す
	{
		//CSVのデータを1行分読み取る
		IsEOF = fscanf_s(
			fp,
			SCORE_DATA_FORMAT,	//データが「1行=1列」で書かれている
			//文字列を読み込むときは、次の因数で文字列最大数を指定する
			&(score)->Name1, STR_MAX,
			&(score)->Score1,
			&(score)->Name2, STR_MAX,
			&(score)->Score2,
			&(score)->Name3, STR_MAX,
			&(score)->Score3
		);
	}

	fclose(fp);	//ファイルを閉じる
}

/// <summary>
/// スコアデータ作成
/// </summary>
BOOL CreateScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* crefp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//ファイルを書き込みモードで開く
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ヘッダを書き込み
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//からのスコアデータを書き込み
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(crefp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// スコアデータ保存
/// </summary>
BOOL SaveScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* crefp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//ファイルを書き込みモードで開く
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ヘッダを書き込み
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//スコアデータを書き込み
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(crefp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// スコアデータの削除
/// </summary>
/// <param name="path">スコアデータのパス</param>
BOOL DeleteScoreData(VOID)
{
	//ファイル削除
	if (remove(GetScoreDataPath()) != 0)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_ERR_TITLE, MB_OK);
	}
	else
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_TITLE, MB_OK);
	}

	//スコアデータを作成
	return CreateScoreData();
}

/// <summary>
/// スコアデータのパスを返す
/// </summary>
const char* GetScoreDataPath(VOID)
{
	return score_data.path;
}

/// <summary>
/// スコアデータのパスを設定する
/// </summary>
VOID SetScoreDataPath(const char* path)
{
	strcpyDx(score_data.path, path);
	return;
}