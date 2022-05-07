#pragma once

#include "inventory_item.h"

class CPhysicItem;
class CEntityAlive;

class CEatableItem : public CInventoryItem {
private:
	typedef CInventoryItem	inherited;

protected:
	CPhysicItem		*m_physic_item;

public:
							CEatableItem				();
	virtual					~CEatableItem				();
	virtual	DLL_Pure*		_construct					();
	virtual CEatableItem	*cast_eatable_item			()	{return this;}

	virtual void			Load						(LPCSTR section);
	virtual bool			Useful						() const;

	virtual BOOL			net_Spawn					(CSE_Abstract* DC);

	virtual void			OnH_B_Independent			(bool just_before_destroy);
	virtual void			OnH_A_Independent			();
	virtual	void			UseBy						(CEntityAlive* npc);
	virtual void			save						(NET_Packet &output_packet);
	virtual void			load						(IReader &input_packet);
	virtual	bool			Empty						()						{return PortionsNum()==0;};
			int				PortionsNum					()	const				{return m_iPortionsNum;}

	IC		u32				GetPortionsNum				()	const				{return m_iPortionsNum;}
			void			SetPortionsNum				(u32 value)				{m_iPortionsNum = value;}
protected:	
	//������� ��� �������� ���� �� ��������� ������
	float					m_fHealthInfluence;
	float					m_fPowerInfluence;
	float					m_fSatietyInfluence;
	float					m_fRadiationInfluence;
	float					m_fMaxPowerUpInfluence;
	float					m_fThirstInfluence;
	float					m_fIntoxicationInfluence;
	float					m_fSleepenessInfluence;
	//���������� ��� �� ���-�� ���������
	float					m_fWoundsHealPerc;

	//���������� ������ ���, 
	//-1 - ������ ���� � ������ �� ������ (���� �� �������� ������� � ����)
	u32						m_iPortionsNum;
};
