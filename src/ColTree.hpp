#pragma once

#include <set>
#include <vector>
#include <list>
#include <map>

using namespace std;

typedef uint16_t WORD;
typedef uint32_t DWORD;

namespace IKD
{
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
#define COLLISIONLIST_REALLOCSIZE	100
template < class T >
class CollisionList {
public:
	CollisionList() : root_( 0 ), pos_( 0 ), mallocSize_( 0 ) {
		root_ = (T**)malloc( 0 );
	}
	~CollisionList() {
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

	void wright( T* obj1, T*obj2 ) {
		if ( pos_ >= mallocSize_ ) {
			root_ = (T**)realloc( root_, sizeof (T*) * ( mallocSize_ + COLLISIONLIST_REALLOCSIZE ) );
			mallocSize_ += COLLISIONLIST_REALLOCSIZE;
		}
		root_[ pos_++ ] = obj1;
		root_[ pos_++ ] = obj2;
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

#define CLINER4TREEMANAGER_MAXLEVEL		9
template <class T>
class CLiner4TreeManager
{
protected:
	unsigned int m_uiDim;
	CCell<T> **ppCellAry;
	unsigned int m_iPow[CLINER4TREEMANAGER_MAXLEVEL+1];
	float m_fW;
	float m_fH;
	float m_fLeft;
	float m_fTop;
	float m_fUnit_W;
	float m_fUnit_H;
	DWORD m_dwCellNum;
	unsigned int m_uiLevel;
	CollisionList< T > m_ColList;

public:
	CLiner4TreeManager()
	{
		m_uiLevel = 0;
		m_fW = 0.0f;
		m_fH = 0.0f;
		m_fLeft = 0.0f;
		m_fTop = 0.0f;
		m_fUnit_W = 0.0f;
		m_fUnit_H = 0.0f;
		m_dwCellNum = 0;
		ppCellAry = NULL;
		m_uiDim = 0;

		int i;
		m_iPow[0] = 1;
		for(i=1;i<CLINER4TREEMANAGER_MAXLEVEL+1;i++)
			m_iPow[i] = m_iPow[i-1]*4;
	}

	virtual ~CLiner4TreeManager()
	{
		DWORD i;
		for(i=0; i<m_dwCellNum; i++){
			if(ppCellAry[i]!=NULL)
				delete ppCellAry[i];
		}
		delete[] ppCellAry;
	}

	bool Init( unsigned int Level, float left, float top, float right, float bottom )
	{
		if(Level>=CLINER4TREEMANAGER_MAXLEVEL)
			return false;
		m_dwCellNum = (m_iPow[Level+1]-1)/3;
		ppCellAry = new CCell<T>*[m_dwCellNum];
		memset(ppCellAry, 0, sizeof(CCell<T>*)*m_dwCellNum);

		m_fLeft = left;
		m_fTop = top;
		m_fW = right - left;
		m_fH = bottom - top;
		m_fUnit_W = m_fW/(1<<Level);
		m_fUnit_H = m_fH/(1<<Level);

		m_uiLevel = Level;

		return true;
	}
	bool Regist( float left, float top, float right, float bottom, OBJECT_FOR_TREE<T> *spOFT )
	{
		DWORD Elem = GetMortonNumber( left, top, right, bottom );
		if(Elem < m_dwCellNum ){
			if( !ppCellAry[Elem] )
				CreateNewCell(Elem);
			return ppCellAry[Elem]->Push(spOFT);
		}
		return false;
	}

	DWORD GetAllCollisionList( CollisionList<T>** colList )
	{
		m_ColList.resetPos();
		if( ppCellAry[0] == NULL )
			return 0;
		list<T*> ColStac;
		GetCollisionList( 0, ColStac );
		*colList = &m_ColList;
		return (DWORD)m_ColList.getSize();
	}
protected:
	bool GetCollisionList( DWORD Elem, list<T*> &ColStac )
	{
		OBJECT_FOR_TREE<T>* spOFT1 = ppCellAry[Elem]->GetFirstObj();
		while( spOFT1 != 0 )
		{
			OBJECT_FOR_TREE<T>* spOFT2 = spOFT1->m_spNext;
			while( spOFT2 != 0 ){
				m_ColList.wright( spOFT1->m_pObject, spOFT2->m_pObject );
				spOFT2 = spOFT2->m_spNext;
			}
            for(auto const& it : ColStac){
				m_ColList.wright( spOFT1->m_pObject, it );
			}
			spOFT1 = spOFT1->m_spNext;
		}


		bool ChildFlag = false;
		DWORD ObjNum = 0;
		DWORD i, NextElem;
		for(i=0; i<4; i++){ 
			NextElem = Elem*4+1+i;
			if( NextElem<m_dwCellNum && ppCellAry[Elem*4+1+i] ){
				if(!ChildFlag){
					spOFT1 = ppCellAry[Elem]->GetFirstObj();
					while( spOFT1 != 0 ){
						ColStac.push_back( spOFT1->m_pObject );
						ObjNum++;
						spOFT1 = spOFT1->m_spNext;
					}
				}
				ChildFlag = true;
				GetCollisionList( Elem*4+1+i, ColStac );
			}
		}
		if( ChildFlag ){
			for(i=0; i<ObjNum; i++)
				ColStac.pop_back();
		}
		return true;
	}
	bool CreateNewCell( DWORD Elem )
	{
		while( !ppCellAry[Elem] )
		{
			ppCellAry[Elem] = new CCell<T>;
			Elem = (Elem-1)>>2;
			if(Elem>=m_dwCellNum) break;
		}
		return true;
	}
	DWORD GetMortonNumber( float left, float top, float right, float bottom )
	{
		DWORD LT = GetPointElem(left, top);
		DWORD RB = GetPointElem(right, bottom );
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

	DWORD BitSeparate32( DWORD n )
	{
		n = (n|(n<<8)) & 0x00ff00ff;
		n = (n|(n<<4)) & 0x0f0f0f0f;
		n = (n|(n<<2)) & 0x33333333;
		return (n|(n<<1)) & 0x55555555;
	}
	WORD Get2DMortonNumber( WORD x, WORD y )
	{
		return (WORD)(BitSeparate32(x) | (BitSeparate32(y)<<1));
	}
	DWORD GetPointElem( float pos_x, float pos_y )
	{
		return Get2DMortonNumber( (WORD)((pos_x-m_fLeft)/m_fUnit_W), (WORD)((pos_y-m_fTop)/m_fUnit_H) );
	}
};
template <class T>
class CCell
{
protected:
	OBJECT_FOR_TREE<T>* m_spLatest;

public:
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

	OBJECT_FOR_TREE<T> *GetFirstObj()
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

}
