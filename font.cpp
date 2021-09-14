//フォント処理のソースファイル


//-ヘッダーファイル-

#include"game.h"
#include"font.h"


//-グローバル関数-

//フォントデータを管理
FONT fontdef;		//デフォルトのフォント

FONT sampleFont1;	//サンプルフォント(PCにありそうなフォント)
FONT sampleFont2;	//サンプルフォント(フォントファイルから読み込んだフォント)
FONT sampleFont3;	//サンプルフォント(フォントファイルから読み込んだフォント)


//-関数-

/// <summary>
/// フォントの読み込み
/// </summary>
/// <returns>読み込めたらTRUE/読み込めなかったらFALSE</returns>
BOOL FontAdd(VOID)
{
	//フォントを一時的に読み込み(WinAPI)
	if (AddFontResourceEx(FONT_JIYU_PATH, FR_PRIVATE, NULL) == 0)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), FONT_JIYU_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}
	//フォントを一時的に読み込み(WinAPI)
	if (AddFontResourceEx(FONT_MUKASI_PATH, FR_PRIVATE, NULL) == 0)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), FONT_MUKASI_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// フォント作成
/// </summary>
BOOL FontCreate(VOID)
{
	//デフォルトフォントの情報を初期化
	SetFont(
		&fontdef,			//設定するフォント
		DEF_FONT_NAME,		//フォントの名前をコピー
		DEF_FONT_SIZE,		//フォントのサイズ
		DEF_FONT_THINCK,	//フォントの太さ
		DEF_FONT_TYPE		//フォントのタイプ
	);

	//デフォルトフォントのハンドル作成
	if (FontCreateHandle(&fontdef) == FALSE)
	{ return FALSE; }

	//デフォルトフォントを全体に反映
	SetDefaultFont(fontdef);

	//サンプルフォント1の情報を設定
	SetFont(
		&sampleFont1,
		FONT_MSGOTH_NAME,
		32,
		5,
		DX_FONTTYPE_ANTIALIASING
	);

	//フォントハンドル1を作成
	if (FontCreateHandle(&sampleFont1) == FALSE)
	{ return FALSE; }


	//サンプルフォント2の情報を設定
	SetFont(
		&sampleFont2,
		FONT_JIYU_NAME,
		64,
		1,
		DX_FONTTYPE_ANTIALIASING
	);

	//フォントハンドル2を作成
	if (FontCreateHandle(&sampleFont2) == FALSE)
	{ return FALSE; }

	//サンプルフォント3の情報を設定
	SetFont(
		&sampleFont3,
		FONT_MUKASI_NAME,
		32,
		5,
		DX_FONTTYPE_ANTIALIASING
	);

	//フォントハンドル3を作成
	if (FontCreateHandle(&sampleFont3) == FALSE)
	{ return FALSE; }

	return TRUE;
}

/// <summary>
/// フォントハンドルを作成
/// </summary>
/// <param name="f">フォント構造体</param>
BOOL FontCreateHandle(FONT* f)
{
	//ハンドルを作成(?)
	f->handle = CreateFontToHandle(
		f->Name,
		f->Size,
		f->Thinck,
		f->Type
	);

	//ハンドルが読み込めていなかったら
	if (f->handle == -1) 
	{ return FALSE; }

	return TRUE;
}

/// <summary>
/// フォントを削除
/// </summary>
VOID FontDelete(VOID)
{
	//デフォルトフォント削除
	DeleteFontToHandle(fontdef.handle);

	DeleteFontToHandle(sampleFont1.handle);	//サンプルフォント削除
	DeleteFontToHandle(sampleFont2.handle);	//サンプルフォント削除
	DeleteFontToHandle(sampleFont3.handle);	//サンプルフォント削除

	return;
}

/// <summary>
/// 因数のフォント構造体へ設定する
/// </summary>
/// <param name="f">設定するフォント構造体</param>
/// <param name="name">フォント名</param>
/// <param name="size">フォントのサイズ</param>
/// <param name="thinck">フォントの太さ</param>
/// <param name="type">フォントのサイズ</param>
VOID SetFont(FONT* f, const char* name, int size, int thinck, int type)
{
	strcpy_sDx(f->Name, STR_MAX, name);	//フォントの名前をコピー
	f->Size = size;						//フォントのサイズ
	f->Thinck = thinck;					//フォントの太さ
	f->Type = type;						//フォントのタイプ
	return;
}

/// <summary>
/// フォントを画面全体に反映させる
/// </summary>
/// <param name="f">設定するフォント構造体</param>
VOID SetDefaultFont(FONT f)
{
	ChangeFont(f.Name);				//フォント名を設定
	SetFontSize(f.Size);			//フォントのサイズを設定
	SetFontThickness(f.Thinck);		//フォントの太さを設定
	ChangeFontType(f.Type);			//フォントのタイプを設定

	return;
}

/// <summary>
/// フォントの削除
/// </summary>
VOID FontRemove(VOID)
{
	//一時的に読み込んだフォントを削除(WinAPI)
	RemoveFontResourceEx(FONT_JIYU_PATH, FR_PRIVATE, NULL);
	RemoveFontResourceEx(FONT_MUKASI_PATH, FR_PRIVATE, NULL);

	return;
}