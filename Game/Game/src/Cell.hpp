
#pragma once

#ifndef MY_CELL_INCLUDED
#	define MY_CELL_INCLUDED
#endif // !MY_CELL_INCLUDED

#include <Siv3D.hpp> // OpenSiv3D v0.4.2

// �Z���̃^�C�v
enum class CellType
{
    // ������1�̃Z��
    Cell1 = 1,
    // ������2�̃Z��
    Cell2,
    // ������3�̃Z��
    Cell3,
    // ������4�̃Z��
    Cell4,
    // ������5�̃Z��
    Cell5,
    // ������6�̃Z��
    Cell6,
    // ������7�̃Z��
    Cell7,
    // ������8�̃Z��
    Cell8,
    // ������9�̃Z��
    Cell9,
    // ������10�̃Z��
    Cell10,

    // ��̃Z��
    Empty = 11,
    
    // �I�W���}�Z��
    Obstruct = 12,
    
    // �֎~�Z��
    No = 13,
};

// �Z��
class Cell
{
private:
    // �Z���̃^�C�v���Ƃ̃e�N�X�`��
    static Array<Texture> typesTexture;

    // �e�N�X�`�������[�h����
    static void loadTypesTexture();

    // �Z���̃^�C�v
    CellType type;

public:
    // Empty�ŏ���������
    // �����loadTypesTexture()�����s����
    Cell();

    // �w�肵���^�C�v�ŏ���������
    // �����loadTypesTexture()�����s����
    Cell(CellType _type);

    // �Z���̃^�C�v���擾����
    inline CellType getType() const
    {
        return type;
    }
    
    // �Z���̃^�C�v�ɑΉ������e�N�X�`���̃R�s�[��Ԃ�
    // Textures�ɑ��݂��Ȃ��ꍇ�͋�̃e�N�X�`����Ԃ�
    const Texture getTexture() const
    {
        int32 number = static_cast<int32>(type);

        if (number < typesTexture.size())
        {
            return typesTexture.at(number);
        }
        else
        {
            return Texture();
        }
    }

    // �����_���ȃ^�C�v�̃Z����Ԃ�
    static Cell& RandomTypeCell(int32 maxTypeNumber = 9, bool hasEmpty = false, bool hasObstruct = false);

    // �Z���̃^�C�v���Ƃ̃e�N�X�`����Ԃ�
    static inline const Array<Texture>& getTypesTexture()
    {
        return typesTexture;
    }

    // �w�肵���^�C�v�̃e�N�X�`����Ԃ�
    static inline const Texture getTexture(CellType type)
    {
        int32 number = static_cast<int32>(type);

        if (number < typesTexture.size())
        {
            return typesTexture.at(number);
        }
        else
        {
            return Texture();
        }
    }

    inline operator String() const
    {
        return Format(static_cast<int32>(type));
    }
};
