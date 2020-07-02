#include"jumpnrun/spell/SpellName2Text.h"
#include<assert.h>

using namespace jnr;
using namespace spl;

std::string SpellName2Text::Name(const std::string& str)
{
	if (str == "arrow")
	{
		return "エルフィンアロー";
	}
	if (str == "explosion")
	{
		return "爆破陣";
	}
	else if (str == "bubble")
	{
		return "バブルウォール";
	}
	else if (str == "elf")
	{
		return "サモン・エルフ";
	}
	else if (str == "circlecancel")
	{
		return "スペルキャンセル";
	}
	else if (str == "darkwave")
	{
		return "ダークウェーブ";
	}
	else if (str == "gas")
	{
		return "毒ガス";
	}
	else if (str == "lifegain20")
	{
		return "ヒール Lv. 1";
	}
	else if (str == "lifegain30")
	{
		return "ヒール Lv. 2";
	}
	else if (str == "bullet")
	{
		return "マグナム";
	}
	else if (str == "frog")
	{
		return "サモン・トード";
	}
	else if (str == "bomb")
	{
		return "ボム";
	}
	else if (str == "starshoot")
	{
		return "シューティングスター";
	}
	else if (str == "mushroom")
	{
		return "サモン・キノコ";
	}
	else if (str == "cannon")
	{
		return "サモン・キャノン";
	}
	else if (str == "dashattack")
	{
		return "ダッシュアタック";
	}
	else if (str == "slimeball")
	{
		return "スライムボール";
	}
	else if (str == "barrier")
	{
		return "バリア";
	}
	else if (str == "laser")
	{
		return "魔導レーザー";
	}
	else if (str == "homingmissile")
	{
		return "魔導ミサイル";
	}
	else if (str == "knight")
	{
		return "サモン・ナイト";
	}
	else if (str == "life2mana20")
	{
		return "血占術";
	}
	else if (str == "thunder")
	{
		return "エレキショット";
	}
	else if (str == "homingfire")
	{
		return "鬼火";
	}
	else if (str == "flame")
	{
		return "フレイム";
	}
	else if (str == "bubblestream")
	{
		return "バブルストリーム";
	}
	else if (str == "sword")
	{
		return "ブレード";
	}
	else if (str == "sword3")
	{
		return "ブレード３";
	}
	else if (str == "shotgun")
	{
		return "ショットガン";
	}
	else if (str == "whip")
	{
		return "ウィップ";
	}
	else if (str == "muscle")
	{
		return "パンプアップ";
	}
	else if (str == "dirtcube")
	{
		return "土ブロック";
	}
	else if (str == "leafshoot")
	{
		return "リーフショット";
	}
	else if (str == "sonicboom")
	{
		return "ソニックブーム";
	}
	else if (str == "ghost")
	{
		return "サモン・ゴースト";
	}
	else if (str == "manaflower")
	{
		return "魔力草";
	}
	else if (str == "starstream")
	{
		return "スターストリーム";
	}
	else if (str == "shuffle")
	{
		return "ブレインストーム";
	}
	else if (str == "thunderbolt")
	{
		return "スパーク";
	}
	else if (str == "scatterurchin")
	{
		return "シーマイン";
	}
	else if (str == "speedup")
	{
		return "スピードアップ";
	}
	else if (str == "trapexp")
	{
		return "トラップボム";
	}
	else if (str == "bombox")
	{
		return "ボムボックス";
	}
	else if (str == "wing")
	{
		return "ワンダーウィング";
	}
	else if (str == "quake")
	{
		return "クエイク";
	}
	else if (str == "hardpunch")
	{
		return "マジカルスマッシュ";
	}
	else if (str == "concentrate")
	{
		return "コンセントレート";
	}
	else if (str == "mindcontrol")
	{
		return "ブレインウォッシュ";
	}
	else if (str == "gasbomb")
	{
		return "毒瓶";
	}
	else if (str == "angel")
	{
		return "サモン・エンジェル";
	}
	else if (str == "mphprate")
	{
		return "賢者の石";
	}
	else if (str == "circlebarrier")
	{
		return "サンクチュアリ";
	}
	assert(0);
	return "？？？";
}

std::string SpellName2Text::Text(const std::string& str)
{
	if (str == "arrow")
	{
		return "矢をを発射して\n前方の敵に攻撃する。";
	}
	if (str == "explosion")
	{
		return "一定時間後に爆発する\n魔方陣を設置。";
	}
	else if (str == "bubble")
	{
		return "敵の攻撃を打ち消す泡で\n身を守る。";
	}
	else if (str == "circlecancel")
	{
		return "設置された魔方陣を打ち消す。";
	}
	else if (str == "darkwave")
	{
		return "波動で周囲を攻撃。\n敵にあたるとMPを吸収する。";
	}
	else if (str == "elf")
	{
		return "エルフを召喚する。";
	}
	else if (str == "gas")
	{
		return "毒ガスを発生させる\n魔方陣を設置。";
	}
	else if (str == "lifegain20")
	{
		return "ライフポイントを\n少しだけ回復。";
	}
	else if (str == "lifegain30")
	{
		return "ライフポイントを\nそこそこの量回復。";
	}
	else if (str == "bullet")
	{
		return "銃弾を発射して\n前方の敵に攻撃する。";
	}
	else if (str == "frog")
	{
		return "カエルを召喚する。";
	}
	else if (str == "bomb")
	{
		return "爆弾を投げつける。\n爆風は敵も味方も巻き込む。";
	}
	else if (str == "explosion")
	{
		return "一定時間後に爆発する魔方陣を設置。";
	}
	else if (str == "starshoot")
	{
		return "星形弾を発射して\n前方の敵に攻撃。";
	}
	else if (str == "mushroom")
	{
		return "キノコを召喚する。";
	}
	else if (str == "cannon")
	{
		return "キャノン砲を召喚する。";
	}
	else if (str == "dashattack")
	{
		return "前方へ突進攻撃。";
	}
	else if (str == "slimeball")
	{
		return "スライム弾を前方に発射する。\n当たると敵の動きを遅くする。";
	}
	else if (str == "barrier")
	{
		return "一度だけダメージを防ぐ\nバリアを張る。";
	}
	else if (str == "laser")
	{
		return "敵を貫通するレーザーを発射。";
	}
	else if (str == "homingmissile")
	{
		return "敵を追尾するミサイルを発射。";
	}
	else if (str == "knight")
	{
		return "騎士を召喚する。";
	}
	else if (str == "life2mana20")
	{
		return "ライフを少しだけ\nマナに変換する。";
	}
	else if (str == "thunder")
	{
		return "雷属性の攻撃。\n敵を麻痺させる。";
	}
	else if (str == "homingfire")
	{
		return "敵を追跡する火炎弾を\n前方へ発射。";
	}
	else if (str == "flame")
	{
		return "炎で前方へ攻撃する。";
	}
	else if (str == "bubblestream")
	{
		return "敵の攻撃を打ち消す\n泡弾を多数発射。";
	}
	else if (str == "sword")
	{
		return "剣を発射。\n近くの敵へ向かっていく。";
	}
	else if (str == "sword3")
	{
		return "剣を３つ発射。\n近くの敵へ向かっていく。";
	}
	else if (str == "shotgun")
	{
		return "当たると弾ける\n銃弾を発射する。";
	}
	else if (str == "whip")
	{
		return "鞭で攻撃。敵を引き寄せる。";
	}
	else if (str == "muscle")
	{
		return "物理攻撃力を少しアップ。";
	}
	else if (str == "dirtcube")
	{
		return "土でできたブロックをおく。\nもろい。";
	}
	else if (str == "leafshoot")
	{
		return "植物属性の攻撃。\n葉っぱ２枚を発射。";
	}
	else if (str == "sonicboom")
	{
		return "超音波攻撃。\n一度だけ貫通する。";
	}
	else if (str == "ghost")
	{
		return "ゴーストを召喚する。";
	}
	else if (str == "manaflower")
	{
		return "花を植える。\n育つと魔法石をだす。";
	}
	else if (str == "starstream")
	{
		return "星形弾を発射して\n前方の敵に連続攻撃。";
	}
	else if (str == "shuffle")
	{
		return "カードをシャッフルして引き直す。";
	}
	else if (str == "thunderbolt")
	{
		return "雷属性の全方位攻撃。\n当たった敵を麻痺させる。";
	}
	else if (str == "scatterurchin")
	{
		return "機雷を４つ設置する。\n触れた敵はダメージを受ける。";
	}
	else if (str == "speedup")
	{
		return "移動速度をアップ。";
	}
	else if (str == "trapexp")
	{
		return "敵や味方が入ると爆発する\nトラップを設置する。";
	}
	else if (str == "trapexp") 
	{
		return "敵が入ると爆発する\nトラップを設置する。";
	}
	else if (str == "bombox")
	{
		return "爆弾ブロックを設置する。\n攻撃が当たると爆発する。";
	}
	else if (str == "wing")
	{
		return "翼を装備する。\n羽ばたきにはMPを消費する。";
	}
	else if (str == "quake")
	{
		return "地面を揺らして攻撃。\n飛んでいる相手には効かない。";
	}
	else if (str == "hardpunch")
	{
		return "威力の高い\n打撃攻撃を繰り出す。";
	}
	else if (str == "concentrate")
	{
		return "MPの回復速度をアップ。";
	}
	else if (str == "mindcontrol")
	{
		return "一番近くの敵を\n味方に変更する。";
	}
	else if (str == "gasbomb")
	{
		return "フラスコを発射。\n周りに毒ガスを発生させる。";
	}
	else if (str == "angel")
	{
		return "天使を召喚する。";
	}
	else if (str == "mphprate")
	{
		return "MPが足りない時\n代わりにHPを支払うことが\nできるようになる。";
	}
	else if (str == "circlebarrier")
	{
		return "敵の攻撃を防ぐ\n魔方陣を設置する。";
	}
	assert(0);
	return "？？？";
}