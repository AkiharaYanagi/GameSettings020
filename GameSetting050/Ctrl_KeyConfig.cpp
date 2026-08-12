//=================================================================================================
//
//	キーコンフィグ
//
//=================================================================================================

//-------------------------------------------------------------------------------------------------
// ヘッダファイルのインクルード
//-------------------------------------------------------------------------------------------------
#include "Ctrl_KeyConfig.h"

//-------------------------------------------------------------------------------------------------
// 定義
//-------------------------------------------------------------------------------------------------
namespace GAME
{
	const Array < String > Ctrl_KeyConfig::keynames =
	{
		U"↑",
		U"↓",
		U"←",
		U"→",
		U"ボタン1 (弱攻撃)",
		U"ボタン2 (中攻撃)",
		U"ボタン3 (強攻撃)",
		U"ボタン4 (特大攻撃)",
		U"ボタン5 (必殺技)",
		U"ボタン6 (投げ)",
		U"ボタン7 (ポーズ)",
		U"ボタン8 (リセット)",
	};

	const int32 Ctrl_KeyConfig::NUM_STG = 24;
	const int32 Ctrl_KeyConfig::NUM_ROW = 12;
	const int32 Ctrl_KeyConfig::NUM_CLM = 2;

	const int32 Ctrl_KeyConfig::P = 20;
	const int32 Ctrl_KeyConfig::B = P * 3 + 10;
	const int32 Ctrl_KeyConfig::W = 340;

	const int32 Ctrl_KeyConfig::BX0 = 20;
	const int32 Ctrl_KeyConfig::BX1 = 420;


	const char32_t * Ctrl_KeyConfig::FILE_NAME = U"keyconfig.dat";



	Ctrl_KeyConfig::Ctrl_KeyConfig ()
	{
		//初期化
		device_inputs.clear();
		di_strs.clear ();

		for ( int i = 0; i < NUM_STG; ++ i )
		{
			device_inputs.push_back ( DeviceInput () );
			di_strs.push_back ( U"-" );
		}
	}

	Ctrl_KeyConfig::~Ctrl_KeyConfig ()
	{
	}


	void Ctrl_KeyConfig::Load ()
	{
		//デフォルトファイルが存在するかどうか
		if ( FileSystem::Exists ( FILE_NAME ) )
		{
			Load ( s3d::FilePath ( FILE_NAME ) );
		}
		else
		{
			Init ();
			strDiag.assign (U"対象が読み込めないため初期値で開始しました.");
		}

		//文字列に反映
		for ( int i = 0; i < NUM_STG; ++ i )
		{
			di_strs [i].assign ( DeviceInput_ToString ( device_inputs [ i ] ) ) ;
		}

		//カレントディレクトリの取得
		wchar_t CRNT [ MAX_PATH ];
		::GetCurrentDirectory ( MAX_PATH, CRNT );
		s3d::String crnt_path = Unicode::FromWstring ( CRNT );

		crnt_path.append ( U"\\" ) += FILE_NAME;
		filePath.assign ( crnt_path );

	}


	void Ctrl_KeyConfig::Init ()
	{
		device_inputs [ 0 ].SetKeyboard ( SIK_UP );
		device_inputs [ 1 ].SetKeyboard ( SIK_DOWN );
		device_inputs [ 2 ].SetKeyboard ( SIK_LEFT );
		device_inputs [ 3 ].SetKeyboard ( SIK_RIGHT );
		device_inputs [ 4 ].SetKeyboard ( SIK_Z );
		device_inputs [ 5 ].SetKeyboard ( SIK_X );
		device_inputs [ 6 ].SetKeyboard ( SIK_C );
		device_inputs [ 7 ].SetKeyboard ( SIK_V );
		device_inputs [ 8 ].SetKeyboard ( SIK_B );
		device_inputs [ 9 ].SetKeyboard ( SIK_N );
		device_inputs [ 10 ].SetKeyboard ( SIK_M );
		device_inputs [ 11 ].SetKeyboard ( SIK_COMMA );
		device_inputs [ 12 ].SetKeyboard ( SIK_HOME );
		device_inputs [ 13 ].SetKeyboard ( SIK_END );
		device_inputs [ 14 ].SetKeyboard ( SIK_DELETE );
		device_inputs [ 15 ].SetKeyboard ( SIK_PAGEDN );
		device_inputs [ 16 ].SetKeyboard ( SIK_A );
		device_inputs [ 17 ].SetKeyboard ( SIK_S );
		device_inputs [ 18 ].SetKeyboard ( SIK_D );
		device_inputs [ 19 ].SetKeyboard ( SIK_F );
		device_inputs [ 20 ].SetKeyboard ( SIK_G );
		device_inputs [ 21 ].SetKeyboard ( SIK_H );
		device_inputs [ 22 ].SetKeyboard ( SIK_J );
		device_inputs [ 23 ].SetKeyboard ( SIK_K );

		selected_index = 0;
	}


	void Ctrl_KeyConfig::Move ()
	{

		//入力の取得
		DeviceInput di = SivInput::Inst()->PushInput ();
		INPUT_DEVICE_TYPE idt = di.GetType();

		//入力が有効な場合
		if ( idt != NODEVICE )
		{
			DeviceInput _di = SivInput::Inst()->PushInput ();
			//保存
			device_inputs [ selected_index ] = di;
			di_strs [ selected_index ] = DeviceInput_ToString ( di );

			//選択を次へ
			selected_index ++;
			if ( selected_index >= NUM_STG ) { selected_index = 0; }
		}

		//保存ボタン
		if ( s3d::SimpleGUI::Button ( U"保存", s3d::Vec2 ( 80, 400 ) ) )
		{
			//固定ディレクトリ
			Save ( s3d::FilePath ( FILE_NAME ) );
#if 0

			//指定ディレクトリ
			wchar_t CRNT [ MAX_PATH ];
			::GetCurrentDirectory ( MAX_PATH, CRNT );
			s3d::String crnt_path = Unicode::FromWstring ( CRNT );

			s3d::Optional < FilePath > path =
				s3d::Dialog::SaveFile (
					{ s3d::FileFilter::AllFiles() }
					, s3d::FilePathView ( crnt_path += U"\\keyconfig.dat" )
					//					, s3d::FilePathView ( U"D:\\Game\\" )
				);

			if ( path )
			{
				Save ( path );
			}

#endif // 0
		}

		//読込ボタン
		if ( s3d::SimpleGUI::Button ( U"読込", s3d::Vec2 ( 200, 400 ) ) )
		{
			//固定ディレクトリ
			Load ( s3d::FilePath ( FILE_NAME ) );
			ResetDiStr ();
#if 0
			s3d::Optional < FilePath > path = s3d::Dialog::OpenFile ( { s3d::FileFilter::AllFiles() } );
			ctrl_KeyConfig.Load ( path );
#endif // 0
		}

		//デフォルトボタン
		if ( s3d::SimpleGUI::Button ( U"デフォルト" , s3d::Vec2 ( 320, 400 ) ) )
		{
			Init ();
			ResetDiStr ();
		}

		//保存して閉じるボタン
		if ( s3d::SimpleGUI::Button ( U"保存して閉じる" , s3d::Vec2 ( 80, 445 ) ) )		{
			SaveAndClose ( FILE_NAME );
		}


		//------------------------------------------
		// マウス入力
		
		//マウスで位置選択
		mouse_selected_index = GetMousePos ();

		//範囲外でないとき
		if ( mouse_selected_index != -1 )
		{
			//左クリック時
			if ( s3d::MouseL.down () )
			{
				//各対象範囲にいるとき、選択位置を更新
				selected_index = (size_t)mouse_selected_index;
			}
		}
	}


	void Ctrl_KeyConfig::Draw ()
	{
		//説明の表示
		font ( U"キーボードかコントローラの入力を設定してください" ).draw ( 120, 0 );
		font ( U"※マウスクリックで現在の設定位置を選択できます" ).draw ( 120, P );

		//プレイヤーの表示
		int player_base_y = 4 + (int32)P * 2;
		Rect { BX0, 4 + player_base_y, W, P }.draw ( ColorF { 0.5, 0.15, 0.15 } );
		font ( U"プレイヤー1" ).draw ( BX0, player_base_y );
		Rect { BX1, 4 + player_base_y, W, P }.draw ( ColorF { 0.15, 0.15, 0.6 } );
		font ( U"プレイヤー2" ).draw ( BX1, player_base_y );

#if 0
		//選択位置の表示
		int x = 0;
		int y = 0;
		if ( selected_index < NUM_ROW )
		{
			x = 20;
			y = 35 + (int)selected_index * 20;
		}
		else
		{
			x = 420;
			y = 35 + ( (int)selected_index - NUM_ROW ) * 20;
		}
		Rect { x, y, 300, 20 }.draw ( ColorF { 0.2, 0.2, 0.2 } );


		//マウス位置の表示
		int mouse_x = 0;
		int mouse_y = 0;
		if ( mouse_selected_index < NUM_ROW )
		{
			mouse_x = 20;
			mouse_y = 35 + (int)mouse_selected_index * 20;
		}
		else
		{
			mouse_x = 420;
			mouse_y = 35 + ( (int)mouse_selected_index - NUM_ROW ) * 20;
		}
		Rect { mouse_x, mouse_y, 300, 20 }.draw ( ColorF { 0.2, 0.2, 0.5 } );
#endif // 0

		//マウス位置の表示
		if ( mouse_selected_index != -1 )
		{
			//範囲外は描画しない
			DrawPosRect ( (int)mouse_selected_index, ColorF { 0.05, 0.2, 0.2 } );
		}

		//選択位置の表示
		DrawPosRect ( (int)selected_index, ColorF { 0.4, 0.4, 0.4 } );


		//キー名
		//入力種類の表示
		
		for ( size_t i = 0; i < NUM_STG; ++ i )
		{
			String str = U"";

			double dx = 0;
			double dy = 0;
			if ( i < NUM_ROW )
			{
				str = keynames [ i ];
				dx = 20;
				dy = B + P * (double)i;
			}
			else
			{
				str = keynames [ i - NUM_ROW ];
				dx = 420;
				dy = B + P * (double)(i - NUM_ROW);
			}

			font ( str ).draw ( dx, dy );
			font ( di_strs [ i ] ).draw ( dx + 200, dy );
			
		}



		//test
#if 0
		const GMPD & gmpd = SivInput::Inst()->GetPadState ();
		const GamePadInputStore & gmpdStore = SivInput::Inst()->GetStore ();
#endif // 0
		DeviceInput di = SivInput::Inst()->PushInput ();
		GamePadInput gpi = di.GetPad ();

#if 0
		if ( di.GetType () != NODEVICE )
		{
			font( U"GamePadInput={}"_fmt( gpi.GetInputType () ) ).draw(100, 370);
		}
		font( U"GamePadInput={}"_fmt( di.GetType () ) ).draw(100, 370);
#endif // 0

#if 0
		font( U"axes[0]={}"_fmt( gmpd.axes[AXIS_X] ) ).draw(500, 300);
		font( U"axes[1]={}"_fmt( gmpd.axes[AXIS_Y] ) ).draw(500, 320);
		if ( gmpd.axes.size () > 2 )
		{
		font( U"axes[2]={}"_fmt( gmpd.axes[AXIS_Z] ) ).draw(500, 340);
		font( U"axes[3]={}"_fmt( gmpd.axes[3] ) ).draw(500, 360);
		font( U"axes[4]={}"_fmt( gmpd.axes[4] ) ).draw(500, 380);
		font( U"axes[5]={}"_fmt( gmpd.axes[5] ) ).draw(500, 400);
		font( U"pre_axes[2]={}"_fmt( gmpdStore.GetAxisZ () ) ).draw(100, 340);
		}

		font( U"pre_axes[0]={}"_fmt( gmpdStore.GetAxisX () ) ).draw(100, 300);
		font( U"pre_axes[1]={}"_fmt( gmpdStore.GetAxisY () ) ).draw(100, 320);

		font( U"PovD8={}"_fmt( gmpdStore.PovD8 () ) ).draw(100, 360);

#endif // 0

#if 0

		font( U"povUp={}"_fmt( gmpd.povUp.pressed() ) ).draw(300, 300);
		font( U"povDown={}"_fmt( gmpd.povDown.pressed() ) ).draw(300, 320);
		font( U"povLeft={}"_fmt( gmpd.povLeft.pressed() ) ).draw(300, 340);
		font( U"povRight={}"_fmt( gmpd.povRight.pressed() ) ).draw(300, 360);

		font( U"povD8={}"_fmt( gmpd.povD8() ) ).draw(300, 380);


		font( U"povUp={}"_fmt( gmpd.povUp.pressed() ) ).draw(100, 300);
		font( U"povDown={}"_fmt( gmpd.povDown.pressed() ) ).draw(100, 320);
		font( U"povLeft={}"_fmt( gmpd.povLeft.pressed() ) ).draw(100, 340);
		font( U"povRight={}"_fmt( gmpd.povRight.pressed() ) ).draw(100, 360);

		font( U"povD8={}"_fmt( gmpd.povD8() ) ).draw(100, 380);
#endif // 0


		//保存ファイル
		//指定なしのときはデフォルト(カレントディレクトリ＋"keyconfig.dat")

		font ( filePath ).draw ( 20, 500 );
		font ( strDiag ).draw ( 20, 500 + P );
	}


	s3d::String Ctrl_KeyConfig::GamePadInput_ToString ( GamePadInput gpi )
	{
		PAD_INPUT_TYPE pit = gpi.GetInputType ();

		s3d::String str = U"P{}_"_fmt ( gpi.GetID () );

		switch ( pit )
		{
		case PIT_AXIS:
			str += U"{}"_fmt ( gpi.GetStrAxis() );
		break;

		case PIT_POINT_OF_VIEW:
			str += U"{}"_fmt ( gpi.GetStrPov () );
		break;

		case PIT_BUTTON:
			str += U"BTN_";
			str += U"{}"_fmt ( gpi.GetButtonID() );
		break;

		case PIT_NO_DATA:		str += U"NO_DATA";	break;

		default:break;
		}

		return str;
	}

	s3d::String Ctrl_KeyConfig::DeviceInput_ToString ( DeviceInput di )
	{
		INPUT_DEVICE_TYPE idt = di.GetType ();

		s3d::String str;
		switch ( idt )
		{
		case KEYBOARD:	str = U"Key_{}"_fmt( di.GetKeyName () );		break;
		case GAMEPAD:	str = GamePadInput_ToString ( di.GetPad() );	break;
		case MOUSE:		str = Unicode::Widen ( STR(MOUSE) );	break;
		default:break;
		}

		return str;
	}



	void Ctrl_KeyConfig::Save ( s3d::Optional < FilePath > path )
	{
		s3d::BinaryWriter bw ( path.value() );

		for ( size_t i = 0; i < NUM_STG; ++ i )
		{
			s3d::String str = device_inputs [ i ].ToString ();
			s3d::Logger << str;
			device_inputs [ i ].Save ( bw );
#if 0

			DeviceInput di = device_inputs [ i ];
			bw.write ( (uint8)di.GetType () );
			bw.write ( (uint8)( di.GetPad ().GetID() ) );
			bw.write ( (uint8)( di.GetPad ().GetInputType() ) );
			bw.write ( (uint8)( di.GetPad ().GetButtonID() ) );
			bw.write ( (uint8)( di.GetPad ().GetLever() ) );
			bw.write ( (uint8)di.GetKey () );

#endif // 0
		}

		strDiag.assign(U"保存しました.");
	}

	void Ctrl_KeyConfig::SaveAndClose ( s3d::Optional < FilePath > path )
	{
		Save ( path );
		s3d::System::Exit ();	//終了
	}

	void Ctrl_KeyConfig::Load ( s3d::Optional < FilePath > path )
	{
		s3d::BinaryReader br ( path.value() );

		for ( size_t i = 0; i < NUM_STG; ++ i )
		{
#if 0

			uint8 device_type = 0;
			br.read ( device_type );

			uint8 pad_id = 0;
			br.read ( pad_id );

			uint8 input_type = 0;
			br.read ( input_type );

			uint8 btn = 0;
			br.read ( btn );

			uint8 lever = 0;
			br.read ( lever );

			uint8 key = 0;
			br.read ( key );


			INPUT_DEVICE_TYPE idt = (INPUT_DEVICE_TYPE)device_type;
			GamePadInput gpi;
			switch ( idt )
			{
			case KEYBOARD:
				device_inputs [ i ].SetKeyboard ( (KEY_NAME)key );
			break;

			case GAMEPAD:
				gpi.Set ( pad_id, (PAD_INPUT_TYPE)input_type, btn, (LEVER_DIR)lever );
				device_inputs [ i ].SetPad ( gpi );
			break;

			default:break;
			}

#endif // 0
			DeviceInput di;
			di.Load ( br );
			device_inputs [ i ] = di;

			s3d::String str = device_inputs [ i ].ToString ();
			s3d::Logger << str;
		}

		strDiag.assign (U"読み込みました.");
	}


	void Ctrl_KeyConfig::ResetDiStr ()
	{
		for ( size_t i = 0; i < di_strs.size (); ++ i )
		{
			DeviceInput di = device_inputs [ i ];
			di_strs [ i ] = DeviceInput_ToString ( di );
		}
	}


	//位置の矩形を表示
	void Ctrl_KeyConfig::DrawPosRect ( int32 index, ColorF clrF )
	{
		//選択位置の表示
		int32 x = 0;
		int32 y = 0;
		if ( index < NUM_ROW )
		{
			x = 20;
			y = 4 + B + index * P;
		}
		else
		{
			x = 420;
			y = 4 + B + ( index - NUM_ROW ) * P;
		}
		Rect { x, y, W, P }.draw ( clrF );

	}

	int32 Ctrl_KeyConfig::GetMousePos ()
	{
		s3d::Point cursorPt = s3d::Cursor::Pos ();
//		s3d::Print ( cursorPt );
		s3d::int32 cx = cursorPt.x;
		s3d::int32 cy = cursorPt.y;

		//左側
		if ( BX0 < cx && cx < BX0 + W )
		{
			for ( int32 n = 0; n < NUM_ROW; ++ n )
			{
				if ( B + P * n <= cy && cy < B + P * (n + 1) ) { return n; }
			}
		}
		//右側
		else if ( BX1 < cx && cx < BX1 + W )
		{
			for ( int32 n = 0; n < NUM_ROW; ++ n )
			{
				if ( B + P * n <= cy && cy < B + P * (n + 1) ) { return n + NUM_ROW; }
			}
		}

		//範囲外
		return -1;
	}


}	//namespace GAME

