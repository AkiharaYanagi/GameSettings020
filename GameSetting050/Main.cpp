# include <Siv3D.hpp> // Siv3D v0.6.16
#include "Game.h"
#include "Ctrl_KeyConfig.h"


using namespace GAME;

void Init ();


void Main()
{
	//タイトルバー
	s3d::Window::SetTitle ( U"剣撃クロスゾーン：キーコンフィグ" );

	//キーコンフィグ　コントロール
	Ctrl_KeyConfig		ctrl_KeyConfig;
	ctrl_KeyConfig.Load ();

	//========================================
	//メインループ
	bool init = F;

	while ( System::Update() )
	{
		//1回のみ初期化
		if ( ! init ) { Init (); init = T; }

#if 0
		//入力の更新
		SivInput::Inst()->Update ();

		//動作
		ctrl_KeyConfig.Move ();

		//描画
		ctrl_KeyConfig.Draw ();

		//入力の保存
		SivInput::Inst()->Store ();
#endif // 0
	}
	//========================================
}

void Init()
{
	//中央
	s3d::Window::Centering();
}
