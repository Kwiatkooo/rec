#ifndef _GTASA_H
#define _GTASA_H

#include <cmath>

namespace GTASA
{
	enum MemoryAddress
	{
		MA_D3DD9,
		MA_WINDOWHANDLE,
		MA_MENUACTIVE,
		MA_SCREENWIDTH,
		MA_SCREENHEIGHT,
		MA_PLAYER_PED,

		MA_TOTAL
	};

	const unsigned int Memory[ MA_TOTAL ] = 
	{
		0xC97C28, //MA_D3DD9
		0xC8CF88, //MA_WINDOWHANDLE
		0xBA67A4, //MA_MENUACTIVE
		0xC17044, //MA_SCREENWIDTH
		0xC17048, //MA_SCREENHEIGHT
		0xB7CD98  //MA_PLAYER_PED
	};

	class Manager
	{
	public:
		template < typename T >
		__inline static T GetAddressValue( MemoryAddress ma )
		{
			return (T)Memory[ ma ];
		}

		template < typename T >
		__inline static T GetValueAt( MemoryAddress ma )
		{
			return ( *(T *)Memory[ ma ] );
		}

		template < typename T >
		__inline static T* GetPointer( MemoryAddress ma )
		{
			return (T *)Memory[ ma ];
		}

		template < typename T >
		__inline static T* GetPointerAt( MemoryAddress ma )
		{
			return ( *(T **)Memory[ ma ] );
		}

		template < typename T >
		__inline static T GetValueAtRAW( int raw )
		{
			return ( *(T *)raw );
		}

		template < typename T >
		__inline static T* GetPointerRAW( int raw )
		{
			return (T *)raw;
		}

		template < typename T >
		__inline static T* GetPointerAtRAW( int raw )
		{
			return ( *(T **)raw );
		}
	};

	class CPed
	{
	private:
		char m_idk[0x14];
		void *m_pMat;
	public:
		__inline float* GetPos()
		{
			return (float *)( (char *)m_pMat + 0x30 );
		}

		float GetDistanceTo( float *vec )
		{
			float* pos = GetPos();
			float diff[] =
			{
				vec[0] - pos[0],
				vec[1] - pos[1],
				vec[2] - pos[2]
			};
			return sqrtf( diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2] );
		}
	};
};

#endif