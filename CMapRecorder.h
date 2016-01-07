#ifndef CMAPRECORDER_H
#define CMAPRECORDER_H

#include <map>

class CMapRecorder : public CSingleton< CMapRecorder >
{
	friend class CSingleton< CMapRecorder >;

	struct stObject
	{
		short mid;
		float x, y, z;
		float rx, ry, rz;
		float d;
	};

private:
	bool m_bRecord;
	float m_fRecDistance;
	std::multimap< short, stObject > m_mmObjects;
public:
	void Process();
	static bool IsAlmostEqual( float f1, float f2, float epsilon = 0.001f )
	{
		return fabs( f1 - f2 ) < epsilon;
	}
private:
	CMapRecorder();
	~CMapRecorder();

	static void CMDStartRecording( PCHAR szCmd );
	static void CMDStopRecording( PCHAR szCmd );
	static void CMDSave( PCHAR szCmd );
	static void CMDReset( PCHAR szCmd );
	static void CMDHelp( PCHAR szCmd );
};

#endif