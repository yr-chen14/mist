#include <mist/config/integer.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::integer< 100 > v1( 1000000000 ), v2( 1000000000 );
//	mist::integer< 10, short, 100 > v1( 900 ), v2( 1000 );


	cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << endl;
	cout << " 1. Operator A + B = \t"; cout << ( v1 + v2 ) << endl;
	cout << " 2. Operator A - B = \t"; cout << ( v1 - v2 ) << endl;
	cout << " 3. Operator A * B = \t"; cout << ( v1 * v2 ) << endl;
	cout << " 3. Operator A * 2 = \t"; cout << ( v1 *  2 ) << endl;
	cout << " 3. Operator 2 * B = \t"; cout << ( 2  * v2 ) << endl;
	//cout << " 4. Operator A / B = \t"; cout << ( v1 / v2 ) << endl;

	//cout << " 5. Operator A % B = \t"; cout << ( v1 % v2 ) << endl;
	//cout << " 6. Operator A | B = \t"; cout << ( v1 | v2 ) << endl;
	//cout << " 7. Operator A & B = \t"; cout << ( v1 & v2 ) << endl;
	//cout << " 8. Operator A ^ B = \t"; cout << ( v1 ^ v2 ) << endl;

	//cout << " 9. Operator A == B = \t"; cout << ( v1 == v2 ) << endl;
	//cout << "10. Operator A != B = \t"; cout << ( v1 != v2 ) << endl;
	//cout << "11. Operator A <  B = \t"; cout << ( v1 <  v2 ) << endl;
	//cout << "12. Operator A <= B = \t"; cout << ( v1 <= v2 ) << endl;
	//cout << "13. Operator A >  B = \t"; cout << ( v1 >  v2 ) << endl;
	//cout << "14. Operator A >= B = \t"; cout << ( v1 >= v2 ) << endl;

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;

	return( 0 );
}
