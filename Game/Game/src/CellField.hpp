
#pragma once

#ifndef MY_CELL_FIELD_INCLUDED
#	define MY_CELL_FIELD_INCLUDED
#endif

# include <Siv3D.hpp> // OpenSiv3D v0.4.2
//# include "Common.hpp"
# include "Cell.hpp"

// �Z����u���t�B�[���h
class CellField
{
private:

	// �t�B�[���h�̓����T�C�Y
	Size size;

	// �t�B�[���h�̕`��T�C�Y
	Size drawsize;

	// �t�B�[���h
	Grid<Cell> field;

	// Just10�̗v�f�̉�
	Grid<int32> just10Times;

	// �t�B�[���h������������
	void clearField();

	// Just10Times���X�V����
	void updateJust10Times();

public:

	// �T�C�Y��(0,0)��CellType��Empty�ŏ���������
	CellField();

	// �T�C�Y��(drawWidth,drawHeight)��CellType��Empty�ŏ�����
	CellField(int32 drawWidth, int32 drawHeight);

	// �T�C�Y��CellType��Empty�ŏ�����
	CellField(Size _drawsize);

	// �t�B�[���h���擾����
	inline const Grid<Cell>& getField() const
	{
		return field;
	}

	// �t�B�[���h��Just10�̗v�f�ƂȂ��Ă���񐔂�Grid��Ԃ�
	inline const Grid<int32>& getJust10Times() const
	{
		return just10Times;
	}

	// �t�B�[���h�̓����T�C�Y���擾����
	inline Size getSize() const
	{
		return size;
	}

	// �t�B�[���h�̕`��T�C�Y���擾����
	inline Size getDrawsize() const
	{
		return drawsize;
	}

	// drawsize�O�̃o�b�N�t�B�[���h������������
	void clearBackField();

	// �Z�����Z�b�g����
	void setCell(Cell& cell, int32 x, int32 y);

	// �Z�����Z�b�g����
	void setCell(Cell& cell, Point p);

	// ������̃f�[�^���擾����
	Grid<Point> getFallTo() const;

	// �s��floats�̕���������̃f�[�^���擾����
	Grid<Point> getFloatTo(Array<int32> floats) const;

	// ������Ɉړ�������
	void moveCells(Grid<Point> FallTo);

	// (x,0)�ɃZ����u��
	// �Ԃ�l�͂ł������ǂ���
	bool pushTopCell(const Cell& cell, int32 x);

	// isDelete�̗v�f��0�ȊO�Ȃ�폜
	// �Ԃ�l�͏������Z���̐�
	int32 deleteCells(Grid<int> isDelete);

	// �t�B�[���h��`��
	void draw(Point fieldPos, Size cellSize, Color backcolor = Color(0, 0)) const;

	// �t�B�[���h��`��
	void draw(Point fieldPos, Size cellSize, 
		std::function<Point(Point, CellType)> posFunc,
		std::function<Color(Point, CellType)> colorFunc) const;

	// �����_���Ȑ����̃Z���̃t�B�[���h��Ԃ�
	// �������ŏ㕔�͋�ɂȂ�
	static CellField RandomField(Size drawsize = Size(0, 0), int32 maxNumber = 9, bool hasEmpty = false, bool hasObstruct = false);
};
