#include "pch.h"
#include "TestFramework.h"

#pragma warning( disable: 4189 )

DEFINE_TEST_CLASS( Getter )
DEFINE_TEST_CLASS( ConditionalJudgement )
DEFINE_TEST_CLASS( BasicCalc )
DEFINE_TEST_CLASS( Filtering )
DEFINE_TEST_CLASS( BasicOperation )
DEFINE_TEST_CLASS( Conversion )

#ifdef __cplusplus_winrt
int main( ::Platform::Array<::Platform::String^>^ /*args*/ )
#else
int main( int /*argc*/, char* /*args*/[] )
#endif
{
	REGISTER_TEST_CLASS( Getter )
	REGISTER_TEST_CLASS( ConditionalJudgement )
	REGISTER_TEST_CLASS( BasicCalc )
	REGISTER_TEST_CLASS( Filtering )
	REGISTER_TEST_CLASS( BasicOperation )
	REGISTER_TEST_CLASS( Conversion )

	TestFramework::Run();
	TestFramework::Wait();
	return 0;
}