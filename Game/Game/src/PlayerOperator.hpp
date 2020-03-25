
# pragma once

# include <Siv3D.hpp>

// �v���C���[�̑���
class PlayerOperator
{
public:
	virtual bool isMoveL() const;

	virtual bool isMoveR() const;

	virtual bool isHold() const;

	virtual bool isDrop() const;

	virtual bool isDecide() const;
};
