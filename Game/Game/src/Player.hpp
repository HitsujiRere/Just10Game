#pragma once

# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"

// �����̏��
enum class BattleState
{
	// ������
	playing,
	// ����
	win,
	// ����
	lose,
	// ��������
	tie,
};

// �v���C���[�̃L�[�Z�b�g
class PlayerKeySet
{
public:
	KeyGroup moveL = KeyGroup();
	KeyGroup moveR = KeyGroup();
	KeyGroup drop = KeyGroup();
	KeyGroup hold = KeyGroup();
	KeyGroup changeCell = KeyGroup();
	KeyGroup toRandom = KeyGroup();
	KeyGroup toEmpty = KeyGroup();

	PlayerKeySet();

	PlayerKeySet(KeyGroup _moveL, KeyGroup _moveR, KeyGroup _drop, KeyGroup _hold,
		KeyGroup _changeCell = KeyGroup(), KeyGroup _toRandom = KeyGroup(),
		KeyGroup _toEmpty = KeyGroup())
		: moveL(_moveL)
		, moveR(_moveR)
		, drop(_drop)
		, hold(_hold)
		, changeCell(_changeCell)
		, toRandom(_toRandom)
		, toEmpty(_toEmpty)
	{
	}
};

// �v���C���[
class Player
{
private:
	// ���Ƃ��Z����1���[�v
	Array<Cell> dropCells1LoopCells;
	// ���Ƃ��Z���̎��̃X�^�b�N
	Array<Cell> dropCells = Array<Cell>();

	// fieldJust10Times�Ȃǂ̍X�V
	// �Ԃ�l�͏�������̂����邩�ǂ���
	bool updatedField();

public:
	Player(Array<int32> _dropCells1LoopNum = { 0, 1, 1, 1, 1, 1 });

	// �f�o�b�O�p��Print�����邩�ǂ���
	bool debugPrint = false;

	// �t�B�[���h�̑傫��
	const Size fieldSize = Size(6, 12);
	// �t�B�[���h
	CellField field = CellField(fieldSize);
	// �t�B�[���h�̃Z����Just10�̗v�f�ƂȂ��Ă����
	Grid<int32> just10Times = Grid<int32>(fieldSize);
	// �Z���̈ړ���
	Grid<Point> fieldMoveTo = Grid<Point>(fieldSize);

	// �t�B�[���h�i�w�i�j�̐F
	Color fieldColor = ColorF(0.8);

	// ���Ƃ��Z���̈ړ�����
	const double dropCellCoolTime = 0.1;
	// ���Ƃ��Z���̈ړ��c�莞��
	double dropCellTimer = 0.1;
	// ���Ƃ��t�B�[���h��x
	int32 dropCellFieldX = 0;
	// ���Ƃ��Z����1���[�v��Number�̐�
	const Array<int32> dropCells1LoopNum = Array<int32>
	{
		0, 1, 1, 1, 1, 1,
	};

	// �z�[���h�Z��
	Cell holdCell = Cell((int32)CellTypeNumber::Empty);

	// ����ł��邩�ǂ���
	bool canOperate = true;
	// ���Ƃ��Z���𗎉��ł��邩�ǂ���
	bool canDrop = true;

	// Just10�����ҋ@���Ԃ��ǂ���
	bool isDeletingTime = false;
	// Just10�����ҋ@�c�莞��
	double deletingTimer = 0.0;
	// Just10�����ҋ@����
	const double deletingCoolTime = 1.0;

	// �Z�������ҋ@���Ԃ��ǂ���
	bool isFallingTime = false;
	// Just10�����ҋ@�c�莞��
	double fallingTimer = 0.0;
	// Just10�����ҋ@����
	const double fallingCoolTime = 0.5;

	// �������o�c�莞��
	double loseTimer = 0.0;
	// �������o����
	const double loseCoolTime = 2.0;

	// �X�R�A
	int32 m_score = 0;

	// ���s
	int32 state = (int32)BattleState::playing;

	// ���Ƃ��Z�����擾����
	Cell& getDropCell(int32 n);

	// ���Ƃ��Z�����擾����
	// num�𒴂��Ă��ǉ����Ȃ�
	const Cell& getDropCellConst(int32 num) const;

	void update(PlayerKeySet keySet);

	void draw(Point fieldPos, Size cellDrawSize) const;
};

class PlayerData
{
public:
	Player player;
	PlayerKeySet keySet;
	Point fieldPos;

	PlayerData()
	{
	}

	PlayerData(Player _player, PlayerKeySet _keySet, Point _fieldPos)
		: player(_player)
		, keySet(_keySet)
		, fieldPos(_fieldPos)
	{
	}
};
