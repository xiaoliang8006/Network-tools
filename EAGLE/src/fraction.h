#ifndef _FRACTION_H_
#define _FRACTION_H_

#include <istream>

class CFraction
{
	friend std::istream& operator>>( std::istream&in, CFraction &frac);
	friend std::ostream& operator<<( std::ostream&out, CFraction &frac);
	friend bool operator==(CFraction &lf, CFraction &rf );
	friend bool operator!=(CFraction &lf, CFraction &rf );
	friend bool operator<(CFraction &lf, CFraction &rf );

	friend CFraction operator+( CFraction &lf, CFraction &rf );
	friend CFraction operator-( CFraction &lf, CFraction &rf );
	friend CFraction operator*( CFraction &lf, CFraction &rf );
	friend CFraction operator/( CFraction &lf, CFraction &rf );

friend bool operator>(CFraction &lf, CFraction &rf );
public:
	CFraction();
	CFraction( int nu, int de );
	CFraction( const CFraction & frac );
	~CFraction();

public:
	void reduct();
	void reciprocal();
	CFraction & operator+=( CFraction &frac );
	CFraction & operator-=( CFraction &frac );
	CFraction & operator*=( CFraction &frac );
	CFraction & operator/=( CFraction &frac );
	CFraction & operator=( const CFraction &frac );

	int get_numerator();
	int get_denominator();

private:
	static int gcd( int a, int b );

	static void swap( int &a, int &b );

private:
	int numerator;

	int denominator;
};

CFraction operator+( CFraction &lf, CFraction &rf );
CFraction operator-( CFraction &lf, CFraction &rf );
CFraction operator*( CFraction &lf, CFraction &rf );
CFraction operator/( CFraction &lf, CFraction &rf );


#endif
