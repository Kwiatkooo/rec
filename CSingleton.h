#ifndef CSINGLETON_H
#define CSINGLETON_H

template < class T >
class CSingleton
{
private:
	static T *g_pInstance;
public:
	static void Start()
	{
		if ( g_pInstance == NULL )
		{
			g_pInstance = new T;
		}
	}
	static T* Instance()
	{
		return g_pInstance;
	}
	static void Shutdown()
	{
		if ( g_pInstance != NULL )
		{
			delete g_pInstance;
			g_pInstance = NULL;
		}
	}
protected:
	CSingleton() {};
	~CSingleton() {};
private:
	CSingleton( T const& );
	void operator=( T const& );
};

template < class T >
T* CSingleton< T >::g_pInstance = NULL;

#endif