
#pragma once

#ifndef MY_CELL_INCLUDED
#	define MY_CELL_INCLUDED
#endif // !MY_CELL_INCLUDED

#include <Siv3D.hpp> // OpenSiv3D v0.4.2

// �Z���̃^�C�v���Ƃ̐���
enum class CellTypeNumber
{
	Cell1 = 1,
	Cell2,
	Cell3,
	Cell4,
	Cell5,
	Cell6,
	Cell7,
	Cell8,
	Cell9,
	Cell10,
	Empty = 11,
	Obstruct = 12,
};


// �Z���i�P�ʃu���b�N�j
class Cell
{
private:
	// �Z���̐������Ƃ̃e�N�X�`��
	static Array<Texture> Textures;

	// �Z���̃e�N�X�`�������[�h����
	static void loadTextures();

	// �Z���̐���
	int32 Number;

public:
	// Empty�ŏ�����
	// �����LoadTextures()�����s����
	Cell();

	// number�ŏ�����
	// �����LoadTextures()�����s����
	Cell(int number);

	// �������擾����
	int32 getNumber() const;

	// ���݂̃Z���̃e�N�X�`���̃R�s�[��Ԃ�
	// Textures�ɑ��݂��Ȃ��ꍇ�͋�̃e�N�X�`����Ԃ�
	Texture getTexture() const;

	// �����_���Ȑ����̃Z����Ԃ�
	static Cell getRandomCell(int32 maxNumber = 9, bool existsEmpty = false, bool existsObstruct = false);
};
