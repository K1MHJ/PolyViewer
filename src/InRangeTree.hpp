#pragma once

#include <set>
#include <vector>
#include <list>
#include <map>

using namespace std;

typedef uint16_t WORD;
typedef uint32_t DWORD;

template <class T>
class CCell;
template< class T>
class OBJECT_FOR_TREE
{
public:
	CCell<T> *m_pCell;
	T *m_pObject;
    OBJECT_FOR_TREE<T>* m_spPre;
	OBJECT_FOR_TREE<T>* m_spNext;
	int m_id;

public:
	OBJECT_FOR_TREE( int id = 0 ){
		m_id = id;
		m_pCell = NULL;
		m_pObject = NULL;
		m_spPre = NULL;
		m_spNext = NULL;
	}

	virtual ~OBJECT_FOR_TREE()
	{
	}

public:
	bool Remove(){
		if(!m_pCell)
			return false;
		if(!m_pCell->OnRemove( this ))
			return false;
		if(m_spPre != 0)
		{
			m_spPre->m_spNext = m_spNext;
		}
		if(m_spNext != 0)
		{
			m_spNext->m_spPre = m_spPre;
		}
		m_spPre = 0;
		m_spNext = 0;
		m_pCell = NULL;
		return true;
	}
};
#define INRANGELIST_REALLOCSIZE	100
template < class T >
class InRangeList {
public:
	InRangeList() : root_( 0 ), pos_( 0 ), mallocSize_( 0 ) {
		root_ = (T**)malloc( 0 );
	}
	~InRangeList() {
		reflesh();
	}
	size_t getSize() {
		return pos_;
	}
	T** getRootPtr() {
		return root_;
	}
	void resetPos() {
		pos_ = 0;
	}
	void write( T* obj1 ) {
		if ( pos_ >= mallocSize_ ) {
			root_ = (T**)realloc( root_, sizeof (T*) * ( mallocSize_ + INRANGELIST_REALLOCSIZE ) );
			mallocSize_ += INRANGELIST_REALLOCSIZE;
		}
		root_[pos_++] = obj1;
	}
	void reflesh() {
		if ( root_ ) {
			free( root_ );
		}
	}
private:
	T**		root_;
	size_t	pos_;
	size_t	mallocSize_;
};

#define CINRANGETREEMANAGER_MAXLEVEL	9
template <class T>
class CInRangeTreeManager
{
protected:
	unsigned int m_uiDim;
	CCell<T> **ppCellArray;
	WORD *pCellDepth;
	unsigned int m_iPow[CINRANGETREEMANAGER_MAXLEVEL+1];
	float m_fW;
	float m_fH;
	float m_fLeft;
	float m_fBottom;
	float m_fUnit_W;
	float m_fUnit_H;
	DWORD m_dwCellNum;
	unsigned int m_uiLevel;
	InRangeList<T> m_InRangeList;
	InRangeList<CCell<T>> m_InCellList;

public:
	CInRangeTreeManager()
	{
		m_uiLevel = 0;
		m_fW = 0.0f;
		m_fH = 0.0f;
		m_fLeft = 0.0f;
		m_fBottom = 0.0f;
		m_fUnit_W = 0.0f;
		m_fUnit_H = 0.0f;
		m_dwCellNum = 0;
		ppCellArray = NULL;
		m_uiDim = 0;

		int i;
		m_iPow[0] = 1;
		for(i=1;i<CINRANGETREEMANAGER_MAXLEVEL+1;i++)
			m_iPow[i] = m_iPow[i-1]<<2;
	}
	virtual ~CInRangeTreeManager()
	{
		DWORD i;
		for(i=0; i<m_dwCellNum; i++){
			if(ppCellArray[i]!=NULL)
				delete ppCellArray[i];
		}
		delete[] ppCellArray;
	}
	bool Init( unsigned int Level, float left, float bottom, float right, float top )
	{
		if(Level>=CINRANGETREEMANAGER_MAXLEVEL)
			return false;
		m_dwCellNum = (m_iPow[Level+1]-1)/3;
		ppCellArray = new CCell<T>*[m_dwCellNum];
		pCellDepth = new WORD[m_dwCellNum];
		memset(ppCellArray, 0, sizeof(CCell<T>*) * m_dwCellNum);
		WORD* pCD = pCellDepth;
		for(unsigned int i = 0; i < Level; i++)
		{
			for(int j = 0;j<pow(4,i);j++){
				*pCD = i; 
				pCD += 1;
			}
		}

		m_fLeft = left;
		m_fBottom = bottom;
		m_fW = right - left;
		m_fH = top - bottom;
		m_fUnit_W = m_fW/(1<<Level);
		m_fUnit_H = m_fH/(1<<Level);
		m_uiLevel = Level;
		return true;
	}
	bool Regist( float left, float bottom, float right, float top, OBJECT_FOR_TREE<T> *spOFT )
	{
		if(right < m_fLeft || m_fLeft + m_fW < left || top < m_fBottom || m_fBottom + m_fH < bottom)
			return false;
		DWORD Elem = GetMortonNumber( left, bottom, right, top );
		if(Elem < m_dwCellNum ){
			if( !ppCellArray[Elem] )
				CreateNewCell(Elem);
			return ppCellArray[Elem]->Push(spOFT);
		}
		return false;
	}
	DWORD GetAllInRangeObjs(float left, float bottom, float right, float top, 
							InRangeList<T>** inRangeObjList)
	{
		if(right < m_fLeft || m_fLeft + m_fW < left || top < m_fBottom || m_fBottom + m_fH < bottom)
			return 0;
		if(left < m_fLeft) left = m_fLeft;
		if(bottom < m_fBottom) bottom = m_fBottom;
		if(m_fLeft + m_fW < right) right = m_fLeft + m_fW;
		if(m_fBottom+m_fH < top) top = m_fBottom + m_fH;
		DWORD Elem = GetMortonNumber( left, bottom, right, top );
		if(Elem >= m_dwCellNum )
			return 0;
		m_InRangeList.resetPos();
		if( ppCellArray[Elem] == NULL )
			return 0;
		GetInRangeList( Elem );
		GetInRangeUpperList( (Elem - 1) >> 2 );
		*inRangeObjList = &m_InRangeList;
		return (DWORD)m_InRangeList.getSize();
	}
	DWORD GetAllInRangeCells(float left, float bottom, float right, float top, 
							InRangeList<CCell<T>>** inRangeCellList)
	{
		if(right < m_fLeft || m_fLeft + m_fW < left || top < m_fBottom || m_fBottom + m_fH < bottom)
			return 0;
		if(left < m_fLeft) left = m_fLeft;
		if(bottom < m_fBottom) bottom = m_fBottom;
		if(m_fLeft + m_fW < right) right = m_fLeft + m_fW;
		if(m_fBottom+m_fH < top) top = m_fBottom + m_fH;

		DWORD Elem = GetMortonNumber( left, bottom, right, top );
		if(Elem >= m_dwCellNum )
			return 0;
		m_InCellList.resetPos();
		if( ppCellArray[Elem] == NULL )
			return 0;
		GetInRangeList( Elem );
		GetInRangeUpperList( (Elem - 1) >> 2 );
		*inRangeCellList = &m_InCellList;
		return (DWORD)m_InCellList.getSize();
	}
protected:
	bool GetInRangeList( DWORD Elem )
	{
		OBJECT_FOR_TREE<T>* spOFT1 = ppCellArray[Elem]->GetFirstObj();
		while( spOFT1 != NULL )
		{
			m_InCellList.write(ppCellArray[Elem]);
			m_InRangeList.write( spOFT1->m_pObject );
			spOFT1 = spOFT1->m_spNext;
		}
		DWORD i, NextElem;
		for(i=0; i<4; i++){ 
			NextElem = (Elem << 2) + 1 + i;
			if( NextElem < m_dwCellNum && ppCellArray[NextElem] ){
				GetInRangeList( NextElem );
			}
		}
		return true;
	}
	bool GetInRangeUpperList( DWORD Elem )
	{
		if(0 > Elem || Elem >= m_dwCellNum || ppCellArray[Elem] == NULL){
			return false;
		}
		OBJECT_FOR_TREE<T>* spOFT1 = ppCellArray[Elem]->GetFirstObj();
		while( spOFT1 != NULL )
		{
			m_InCellList.write(ppCellArray[Elem]);
			m_InRangeList.write( spOFT1->m_pObject );
			spOFT1 = spOFT1->m_spNext;
		}
		DWORD NextElem;
		NextElem = (Elem - 1) >> 2;
		if(0 <= NextElem && NextElem < m_dwCellNum && ppCellArray[NextElem] ){
			GetInRangeUpperList( NextElem );
		}
		return true;
	}
	bool CreateNewCell( DWORD Elem )
	{
		while( !ppCellArray[Elem] )
		{
			ppCellArray[Elem] = new CCell<T>;
			ppCellArray[Elem]->m_id = Elem;
			ppCellArray[Elem]->m_width  = m_fUnit_W * (1<<(m_uiLevel - pCellDepth[Elem]));
			ppCellArray[Elem]->m_height = m_fUnit_H * (1<<(m_uiLevel - pCellDepth[Elem]));
			Elem = (Elem-1) >> 2;
			if(Elem >= m_dwCellNum) break;
		}
		return true;
	}
	DWORD GetMortonNumber( float left, float bottom, float right, float top ) const
	{
		DWORD LT = GetPointElem(left, bottom);
		DWORD RB = GetPointElem(right, top );
		DWORD Def = RB ^ LT;
		unsigned int HiLevel = 0;
		unsigned int i;
		for(i=0; i<m_uiLevel; i++)
		{
			DWORD Check = (Def>>(i*2)) & 0x3;
			if( Check != 0 )
				HiLevel = i+1;
		}
		DWORD SpaceNum = RB>>(HiLevel*2);
		DWORD AddNum = (m_iPow[m_uiLevel-HiLevel]-1)/3;
		SpaceNum += AddNum;

		if(SpaceNum > m_dwCellNum)
			return 0xffffffff;

		return SpaceNum;
	}

	DWORD BitSeparate32( DWORD n ) const
	{
		n = (n|(n<<8)) & 0x00ff00ff;
		n = (n|(n<<4)) & 0x0f0f0f0f;
		n = (n|(n<<2)) & 0x33333333;
		return (n|(n<<1)) & 0x55555555;
	}
	WORD Get2DMortonNumber( WORD x, WORD y ) const
	{
		return (WORD)(BitSeparate32(x) | (BitSeparate32(y)<<1));
	}
	DWORD GetPointElem( float pos_x, float pos_y ) const
	{
		return Get2DMortonNumber( (WORD)((pos_x-m_fLeft)/m_fUnit_W), (WORD)((pos_y-m_fBottom)/m_fUnit_H) );
	}
};
template <class T>
class CCell
{
public:
	OBJECT_FOR_TREE<T>* m_spLatest;
	DWORD m_id;
	float m_width;
	float m_height;
	CCell() : m_spLatest( 0 )
	{
	}

	virtual ~CCell()
	{
		if( m_spLatest != NULL )
			ResetLink( m_spLatest );
	}

	void ResetLink( OBJECT_FOR_TREE<T> *spOFT )
	{
		if( spOFT->m_spNext != 0 ) {
			ResetLink( spOFT->m_spNext );
		}
		spOFT = 0;
	}

	bool Push( OBJECT_FOR_TREE<T> *spOFT )
	{
		if( spOFT == 0 ) return false;
		if( spOFT->m_pCell == this ) return false;
		if( m_spLatest == 0 ){
			m_spLatest = spOFT;
		}
		else
		{
			spOFT->m_spNext = m_spLatest;
			m_spLatest->m_spPre = spOFT;
			m_spLatest = spOFT;
		}
		spOFT->m_pCell = this;
		return true;
	}

	OBJECT_FOR_TREE<T> *GetFirstObj() const
	{
		return m_spLatest;
	}
	bool OnRemove( OBJECT_FOR_TREE<T> *pRemoveObj )
	{
		if( m_spLatest == pRemoveObj ){
			if( m_spLatest != NULL )
				m_spLatest = m_spLatest->m_spNext;
		}
		return true;
	}
};