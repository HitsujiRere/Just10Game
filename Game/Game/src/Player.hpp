
#pragma once

# include "Common.hpp"
# include "Cell.hpp"
# include "CellField.hpp"
# include "PlayerKeySet.hpp"

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

// �v���C���[
class Player
{
private:
	// ���Ƃ��Z����1���[�v
	Array<Cell> dropCells1LoopCells;
	// ���Ƃ��Z���̎��̃X�^�b�N
	Array<Cell> dropCellStack = Array<Cell>();

	// fieldJust10Times�Ȃǂ̍X�V
	// �Ԃ�l�͏�������̂����邩�ǂ���
	bool updatedField();

public:
	Player(Array<int32> _dropCells1LoopNum = { 0, 1, 1, 1, 1, 1 });
	//Player(const Player& another);
	//Player(Player&& another);

	Player& operator=(const Player& another);

	// �t�B�[���h
	CellField field;
	// �t�B�[���h�̃Z����Just10�̗v�f�ƂȂ��Ă����
	Grid<int32> just10times;
	// �Z���̈ړ���
	Grid<Point> fieldMoveTo;

	// �t�B�[���h�i�w�i�j�̐F
	Color fieldColor = ColorF(0.8);

	// ���Ƃ��Z���̈ړ��c�莞��
	double dropCellTimer = 0.1;
	// ���Ƃ��Z���̈ړ�����
	const double dropCellCoolTime = 0.1;
	// ���Ƃ��t�B�[���h��x
	int32 dropCellFieldX = 0;
	// ���Ƃ��Z����1���[�v��Number�̐�
	Array<int32> dropCells1LoopNum = Array<int32>
	{
		0, 1, 1, 1, 1, 1,
	};

	// �z�[���h�Z��
	Cell holdCell = Cell(CellType::Empty);

	// ����ł��邩�ǂ���
	bool canOperate = true;
	// ���Ƃ��Z���𗎉��ł��邩�ǂ���
	bool canDrop = true;

	// Just10�����ҋ@���Ԃ��ǂ���
	bool isDeletingTime = false;
	// Just10�����ҋ@�c�莞��
	double deletingTimer = 0.0;
	// Just10�����ҋ@�ő厞��
	const double deletingCoolTime = 1.0;

	// �Z�������ҋ@���Ԃ��ǂ���
	bool isMovingTime = false;
	// Just10�����ҋ@�c�莞��
	double movingTimer = 0.0;
	// Just10�����ҋ@����
	const double movingCoolTime = 0.5;

	// �������o�c�莞��
	double stateTimer = 0.0;
	// �������o�ő厞��
	const double stateCoolTime = 2.0;

	// ���s
	BattleState state = BattleState::playing;

	// �X�R�A
	int32 score = 0;
	// �X�R�A�v�Z�֐�
	std::function<int32(int32 dc, int32 combo)> scoreFunc
		= [](int32 d, int32 c) {return static_cast<int32>(ceil(d * (c / 2.0 + 1.0))); };
	// �R���{��
	int32 combo = 0;
	// �I�W���}�𑗂�c�莞��
	double sendObstructTimer = 0.0;
	// �I�W���}�𑗂�ҋ@����
	const double sendObstructCoolTime = 4.0;

	// ������I�W���}
	int32 obstructsMaked = 0;
	// �ۗ����̎󂯂��I�W���}�̐�
	int32 obstructsSentSum = 0;
	// �ۗ����̎󂯂��I�W���}�̗�ʐ�
	Array<int32> obstructsSentCntArray;

	// �^����_���[�W����
	double atkRate = 1.0;
	// �^������_���[�W����
	double defRate = 1.0;

	// ���Ƃ��Z�����쐬����
	void makeDropCells(int32 min);

	// ���Ƃ��Z�����擾����
	// num��dropCellStack�̃T�C�Y�𒴂��Ă�����ǉ�����
	inline const Cell& getDropCell(int32 num)
	{
		makeDropCells(num);

		return dropCellStack.at(num);
	}

	// ���Ƃ��Z�����擾����
	inline const Cell& getDropCellConst(int32 num) const
	{
		return dropCellStack.at(num);
	}

	// �X�V����
	// �Ԃ�l�͍쐬�����I�W���}
	int32 update(PlayerKeySet keySet);

	void draw(Point fieldPos, Size cellDrawSize) const;

	// �I�W���}�𑗂�
	void sendObstructs(double sent_obstructs);
};
