
#pragma once

#ifndef MY_CELL_FIELD_INCLUDED
#	define MY_CELL_FIELD_INCLUDED
#endif

#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include "Cell.hpp"

// �Z����u���t�B�[���h
class CellField
{
private:
	// �t�B�[���h
	Grid<Cell> Field;

	// Just10�̗v�f�̉�
	Grid<int32> Just10Times;

	// �t�B�[���h������������
	void clearField(Size size);

	// Just10Times���X�V����
	void updateJust10Times();
		

public:
	// 0*0��Empty�ŏ�����
	CellField();

	// width*height��Empty�ŏ�����
	CellField(int32 width, int32 height);

	// size��Empty�ŏ�����
	CellField(Size size);

	// Grid���擾����
	const Grid<Cell>& getGrid() const;

	// �t�B�[���h��Just10�̗v�f�ƂȂ��Ă���񐔂�Grid��Ԃ�
	Grid<int32> getJust10Times() const;

	// �t�B�[���h�̃T�C�Y���擾����
	Size size() const;

	// ������̃f�[�^���擾����
	Grid<Point> getFallTo() const;

	// ������Ɉړ�������
	void moveCells(Grid<Point> FallTo);

	// (x,0)�ɃZ����u��
	// �Ԃ�l�͂ł������ǂ���
	bool pushCell(const Cell& cell, int32 x);

	// isDelete�̗v�f��0�ȊO�Ȃ�폜
	// �Ԃ�l�͏������Z���̐�
	int32 deleteCells(Grid<int> isDelete);

	// �t�B�[���h��`��
	void draw(Point fieldPos, Size cellSize, Color backcolor = Color(0, 0)) const;

	// �t�B�[���h��`��
	void draw(Point fieldPos, Size cellSize, 
		std::function<Point(Point, int32)> posFunc,
		std::function<Color(Point, int32)> colorFunc,
		std::function<Color(Point, int32)> backcolorFunc) const;

	// �����_���Ȑ����̃Z���̃t�B�[���h��Ԃ�
	// �������ŏ㕔�͋�ɂȂ�
	static CellField RandomField(Size size = Size(0, 0), int32 maxNumber = 9, bool existsEmpty = false, bool existsObstruct = false);
};
