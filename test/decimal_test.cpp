#include <mist/config/decimal.h>
#include <iostream>


int main( int argc, char *argv[] )
{
	using namespace std;

//	mist::decimal< 100 > v1( "2.1" ), v2( "1.1" );
//	mist::decimal< 10 > v1( "2.0" ), v2( "1.1" );
	mist::decimal< 20 > v1( 0.25 ), v2( 3.14159265358979323846264338327950288419716940 );


	cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << endl;
	cout << " 1. Operator A   + B   = \t"; cout << ( v1  + v2  ) << endl;
	cout << " 2. Operator A   + 3.5 = \t"; cout << ( v1  + 3   ) << endl;
	cout << " 3. Operator 3.5 + B   = \t"; cout << ( 3.5 + v2  ) << endl;
	cout << " 4. Operator A   - B   = \t"; cout << ( v1  - v2  ) << endl;
	cout << " 5. Operator A   - 3.5 = \t"; cout << ( v1  - 3.5 ) << endl;
	cout << " 6. Operator 3.5 - B   = \t"; cout << ( 3.5 - v2  ) << endl;
	cout << " 7. Operator A   * B   = \t"; cout << ( v1  * v2  ) << endl;
	cout << " 8. Operator A   * 2.5 = \t"; cout << ( v1  *  2  ) << endl;
	cout << " 9. Operator 2   * B   = \t"; cout << ( 2.5 * v2  ) << endl;
	cout << "10. Operator A   / B   = \t"; cout << ( v1  / v2  ) << endl;
	cout << "11. Operator A   / 2.5 = \t"; cout << ( v1  / 2.5 ) << endl;

	//cout << "12. Operator A % B = \t"; cout << ( v1 % v2 ) << endl;
	//cout << "13. Operator A | B = \t"; cout << ( v1 | v2 ) << endl;
	//cout << "14. Operator A & B = \t"; cout << ( v1 & v2 ) << endl;
	//cout << "15. Operator A ^ B = \t"; cout << ( v1 ^ v2 ) << endl;

	cout << "16. Operator A == B = \t"; cout << ( v1 == v2 ) << endl;
	cout << "17. Operator A != B = \t"; cout << ( v1 != v2 ) << endl;
	cout << "18. Operator A <  B = \t"; cout << ( v1 <  v2 ) << endl;
	cout << "19. Operator A <= B = \t"; cout << ( v1 <= v2 ) << endl;
	cout << "20. Operator A >  B = \t"; cout << ( v1 >  v2 ) << endl;
	cout << "21. Operator A >= B = \t"; cout << ( v1 >= v2 ) << endl;

	cout << "22. sqrt( A )       = \t"; cout << std::sqrt( v1 ) << endl;
	cout << "22. PAI             = \t"; cout << mist::decimal<10 >::pai( ) << endl;

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;

	return( 0 );
}