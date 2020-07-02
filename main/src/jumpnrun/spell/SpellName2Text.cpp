#include"jumpnrun/spell/SpellName2Text.h"
#include<assert.h>

using namespace jnr;
using namespace spl;

std::string SpellName2Text::Name(const std::string& str)
{
	if (str == "arrow")
	{
		return "�G���t�B���A���[";
	}
	if (str == "explosion")
	{
		return "���j�w";
	}
	else if (str == "bubble")
	{
		return "�o�u���E�H�[��";
	}
	else if (str == "elf")
	{
		return "�T�����E�G���t";
	}
	else if (str == "circlecancel")
	{
		return "�X�y���L�����Z��";
	}
	else if (str == "darkwave")
	{
		return "�_�[�N�E�F�[�u";
	}
	else if (str == "gas")
	{
		return "�ŃK�X";
	}
	else if (str == "lifegain20")
	{
		return "�q�[�� Lv. 1";
	}
	else if (str == "lifegain30")
	{
		return "�q�[�� Lv. 2";
	}
	else if (str == "bullet")
	{
		return "�}�O�i��";
	}
	else if (str == "frog")
	{
		return "�T�����E�g�[�h";
	}
	else if (str == "bomb")
	{
		return "�{��";
	}
	else if (str == "starshoot")
	{
		return "�V���[�e�B���O�X�^�[";
	}
	else if (str == "mushroom")
	{
		return "�T�����E�L�m�R";
	}
	else if (str == "cannon")
	{
		return "�T�����E�L���m��";
	}
	else if (str == "dashattack")
	{
		return "�_�b�V���A�^�b�N";
	}
	else if (str == "slimeball")
	{
		return "�X���C���{�[��";
	}
	else if (str == "barrier")
	{
		return "�o���A";
	}
	else if (str == "laser")
	{
		return "�������[�U�[";
	}
	else if (str == "homingmissile")
	{
		return "�����~�T�C��";
	}
	else if (str == "knight")
	{
		return "�T�����E�i�C�g";
	}
	else if (str == "life2mana20")
	{
		return "����p";
	}
	else if (str == "thunder")
	{
		return "�G���L�V���b�g";
	}
	else if (str == "homingfire")
	{
		return "�S��";
	}
	else if (str == "flame")
	{
		return "�t���C��";
	}
	else if (str == "bubblestream")
	{
		return "�o�u���X�g���[��";
	}
	else if (str == "sword")
	{
		return "�u���[�h";
	}
	else if (str == "sword3")
	{
		return "�u���[�h�R";
	}
	else if (str == "shotgun")
	{
		return "�V���b�g�K��";
	}
	else if (str == "whip")
	{
		return "�E�B�b�v";
	}
	else if (str == "muscle")
	{
		return "�p���v�A�b�v";
	}
	else if (str == "dirtcube")
	{
		return "�y�u���b�N";
	}
	else if (str == "leafshoot")
	{
		return "���[�t�V���b�g";
	}
	else if (str == "sonicboom")
	{
		return "�\�j�b�N�u�[��";
	}
	else if (str == "ghost")
	{
		return "�T�����E�S�[�X�g";
	}
	else if (str == "manaflower")
	{
		return "���͑�";
	}
	else if (str == "starstream")
	{
		return "�X�^�[�X�g���[��";
	}
	else if (str == "shuffle")
	{
		return "�u���C���X�g�[��";
	}
	else if (str == "thunderbolt")
	{
		return "�X�p�[�N";
	}
	else if (str == "scatterurchin")
	{
		return "�V�[�}�C��";
	}
	else if (str == "speedup")
	{
		return "�X�s�[�h�A�b�v";
	}
	else if (str == "trapexp")
	{
		return "�g���b�v�{��";
	}
	else if (str == "bombox")
	{
		return "�{���{�b�N�X";
	}
	else if (str == "wing")
	{
		return "�����_�[�E�B���O";
	}
	else if (str == "quake")
	{
		return "�N�G�C�N";
	}
	else if (str == "hardpunch")
	{
		return "�}�W�J���X�}�b�V��";
	}
	else if (str == "concentrate")
	{
		return "�R���Z���g���[�g";
	}
	else if (str == "mindcontrol")
	{
		return "�u���C���E�H�b�V��";
	}
	else if (str == "gasbomb")
	{
		return "�ŕr";
	}
	else if (str == "angel")
	{
		return "�T�����E�G���W�F��";
	}
	else if (str == "mphprate")
	{
		return "���҂̐�";
	}
	else if (str == "circlebarrier")
	{
		return "�T���N�`���A��";
	}
	assert(0);
	return "�H�H�H";
}

std::string SpellName2Text::Text(const std::string& str)
{
	if (str == "arrow")
	{
		return "����𔭎˂���\n�O���̓G�ɍU������B";
	}
	if (str == "explosion")
	{
		return "��莞�Ԍ�ɔ�������\n�����w��ݒu�B";
	}
	else if (str == "bubble")
	{
		return "�G�̍U����ł������A��\n�g�����B";
	}
	else if (str == "circlecancel")
	{
		return "�ݒu���ꂽ�����w��ł������B";
	}
	else if (str == "darkwave")
	{
		return "�g���Ŏ��͂��U���B\n�G�ɂ������MP���z������B";
	}
	else if (str == "elf")
	{
		return "�G���t����������B";
	}
	else if (str == "gas")
	{
		return "�ŃK�X�𔭐�������\n�����w��ݒu�B";
	}
	else if (str == "lifegain20")
	{
		return "���C�t�|�C���g��\n���������񕜁B";
	}
	else if (str == "lifegain30")
	{
		return "���C�t�|�C���g��\n���������̗ʉ񕜁B";
	}
	else if (str == "bullet")
	{
		return "�e�e�𔭎˂���\n�O���̓G�ɍU������B";
	}
	else if (str == "frog")
	{
		return "�J�G������������B";
	}
	else if (str == "bomb")
	{
		return "���e�𓊂�����B\n�����͓G���������������ށB";
	}
	else if (str == "explosion")
	{
		return "��莞�Ԍ�ɔ������閂���w��ݒu�B";
	}
	else if (str == "starshoot")
	{
		return "���`�e�𔭎˂���\n�O���̓G�ɍU���B";
	}
	else if (str == "mushroom")
	{
		return "�L�m�R����������B";
	}
	else if (str == "cannon")
	{
		return "�L���m���C����������B";
	}
	else if (str == "dashattack")
	{
		return "�O���֓ːi�U���B";
	}
	else if (str == "slimeball")
	{
		return "�X���C���e��O���ɔ��˂���B\n������ƓG�̓�����x������B";
	}
	else if (str == "barrier")
	{
		return "��x�����_���[�W��h��\n�o���A�𒣂�B";
	}
	else if (str == "laser")
	{
		return "�G���ђʂ��郌�[�U�[�𔭎ˁB";
	}
	else if (str == "homingmissile")
	{
		return "�G��ǔ�����~�T�C���𔭎ˁB";
	}
	else if (str == "knight")
	{
		return "�R�m����������B";
	}
	else if (str == "life2mana20")
	{
		return "���C�t����������\n�}�i�ɕϊ�����B";
	}
	else if (str == "thunder")
	{
		return "�������̍U���B\n�G��Ⴢ�����B";
	}
	else if (str == "homingfire")
	{
		return "�G��ǐՂ���Ή��e��\n�O���֔��ˁB";
	}
	else if (str == "flame")
	{
		return "���őO���֍U������B";
	}
	else if (str == "bubblestream")
	{
		return "�G�̍U����ł�����\n�A�e�𑽐����ˁB";
	}
	else if (str == "sword")
	{
		return "���𔭎ˁB\n�߂��̓G�֌������Ă����B";
	}
	else if (str == "sword3")
	{
		return "�����R���ˁB\n�߂��̓G�֌������Ă����B";
	}
	else if (str == "shotgun")
	{
		return "������ƒe����\n�e�e�𔭎˂���B";
	}
	else if (str == "whip")
	{
		return "�ڂōU���B�G�������񂹂�B";
	}
	else if (str == "muscle")
	{
		return "�����U���͂������A�b�v�B";
	}
	else if (str == "dirtcube")
	{
		return "�y�łł����u���b�N�������B\n���낢�B";
	}
	else if (str == "leafshoot")
	{
		return "�A�������̍U���B\n�t���ςQ���𔭎ˁB";
	}
	else if (str == "sonicboom")
	{
		return "�����g�U���B\n��x�����ђʂ���B";
	}
	else if (str == "ghost")
	{
		return "�S�[�X�g����������B";
	}
	else if (str == "manaflower")
	{
		return "�Ԃ�A����B\n��Ɩ��@�΂������B";
	}
	else if (str == "starstream")
	{
		return "���`�e�𔭎˂���\n�O���̓G�ɘA���U���B";
	}
	else if (str == "shuffle")
	{
		return "�J�[�h���V���b�t�����Ĉ��������B";
	}
	else if (str == "thunderbolt")
	{
		return "�������̑S���ʍU���B\n���������G��Ⴢ�����B";
	}
	else if (str == "scatterurchin")
	{
		return "�@�����S�ݒu����B\n�G�ꂽ�G�̓_���[�W���󂯂�B";
	}
	else if (str == "speedup")
	{
		return "�ړ����x���A�b�v�B";
	}
	else if (str == "trapexp")
	{
		return "�G�□��������Ɣ�������\n�g���b�v��ݒu����B";
	}
	else if (str == "trapexp") 
	{
		return "�G������Ɣ�������\n�g���b�v��ݒu����B";
	}
	else if (str == "bombox")
	{
		return "���e�u���b�N��ݒu����B\n�U����������Ɣ�������B";
	}
	else if (str == "wing")
	{
		return "���𑕔�����B\n�H�΂����ɂ�MP�������B";
	}
	else if (str == "quake")
	{
		return "�n�ʂ�h�炵�čU���B\n���ł��鑊��ɂ͌����Ȃ��B";
	}
	else if (str == "hardpunch")
	{
		return "�З͂̍���\n�Ō��U�����J��o���B";
	}
	else if (str == "concentrate")
	{
		return "MP�̉񕜑��x���A�b�v�B";
	}
	else if (str == "mindcontrol")
	{
		return "��ԋ߂��̓G��\n�����ɕύX����B";
	}
	else if (str == "gasbomb")
	{
		return "�t���X�R�𔭎ˁB\n����ɓŃK�X�𔭐�������B";
	}
	else if (str == "angel")
	{
		return "�V�g����������B";
	}
	else if (str == "mphprate")
	{
		return "MP������Ȃ���\n�����HP���x�������Ƃ�\n�ł���悤�ɂȂ�B";
	}
	else if (str == "circlebarrier")
	{
		return "�G�̍U����h��\n�����w��ݒu����B";
	}
	assert(0);
	return "�H�H�H";
}