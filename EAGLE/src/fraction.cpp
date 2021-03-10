/***************************************************************************\
 *   Copyright (C) 2007 by Huawei Shen                                     *
 *   shenhuawei@software.ict.ac.cn                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
\***************************************************************************/

#include "fraction.h"

using namespace std;

CFraction::CFraction()
{
	numerator = 0;
	denominator = 1;
}

CFraction::CFraction( int nu, int de )
{
	numerator = nu;
	denominator = de;
}

CFraction::CFraction( const CFraction &frac )
{
	numerator = frac.numerator;
	denominator = frac.denominator;
}

CFraction::~CFraction()
{
}

void CFraction::swap( int &a, int &b )
{
	a ^= b;
	b ^= a;
	a ^= b;
}

int CFraction::gcd( int a, int b )
{
	if ( a == 0 )
		return b;

	if ( 0 == b )
		return a;

	if ( a < 0 )
		a = -a;

	if ( b < 0 )
		b = -b;

	if ( a < b )
		swap( a, b );

	int c;

	for ( c = a % b; c > 0; c = a % b )
	{
		a = b;
		b = c;
	}

	return b;
}

int CFraction::get_numerator()
{
	return numerator;
}

int CFraction::get_denominator()
{
	return denominator;
}

void CFraction::reduct()
{
	int gcd_num;
	
	gcd_num = gcd( denominator, numerator );

	denominator /= gcd_num;
	numerator /= gcd_num;
}

void CFraction::reciprocal()
{
	swap( denominator, numerator );
}

istream & operator>>( istream &in, CFraction &frac )
{
	in >> frac.numerator >> frac.denominator;

	return in;
}

ostream & operator<<( ostream &out, CFraction &frac )
{
	frac.reduct();
	out << frac.numerator << "/" << frac.denominator;

	return out;
}

CFraction & CFraction::operator+=( CFraction & frac )
{
	reduct();
	frac.reduct();

	int ld = denominator;
	int rd = frac.denominator;
	int cd = gcd(ld, rd);
	int ll = rd / cd;
	int rl = ld / cd;

	denominator *= ll;
	numerator = numerator * ll + frac.numerator * rl;
	reduct();

	return *this;
}

CFraction & CFraction::operator-=( CFraction &frac )
{
	reduct();
	frac.reduct();

	int ld = denominator;
	int rd = frac.denominator;
	int cd = gcd(ld, rd);
	int ll = rd / cd;
	int rl = ld / cd;

	denominator *= ll;
	numerator = numerator * ll - frac.numerator * rl;
	reduct();

	return *this;

}

CFraction & CFraction::operator*=( CFraction &frac )
{
	denominator *= frac.denominator;
	numerator *= frac.numerator;

	reduct();

	return *this;
}

CFraction & CFraction::operator/=( CFraction &frac )
{
	reciprocal();
	*this *= frac;
	reciprocal();

	return *this;
}

CFraction & CFraction::operator=( const CFraction &frac )
{
	denominator = frac.denominator;
	numerator = frac.numerator;

	return *this;
}

CFraction operator+(CFraction &lo, CFraction &ro )
{
	CFraction result;

	int ld = lo.denominator;
	int rd = ro.denominator;
	int cd = CFraction::gcd(ld, rd);
	int ll = rd / cd;
	int rl = ld / cd;

	result.denominator = ld * ll;
	result.numerator = lo.numerator * ll + ro.numerator * rl;

	result.reduct();

	return result;
}

CFraction operator-(CFraction &lo, CFraction &ro )
{
	CFraction result;

	int ld = lo.denominator;
	int rd = ro.denominator;
	int cd = CFraction::gcd(ld, rd);
	int ll = rd / cd;
	int rl = ld / cd;

	result.denominator = ld * ll;
	result.numerator = lo.numerator * ll - ro.numerator * rl;

	result.reduct();

	return result;
}

CFraction operator*(CFraction &lo, CFraction &ro )
{
	CFraction result;

	result.denominator = lo.denominator * ro.denominator;
	result.numerator = lo.numerator * ro.numerator;

	result.reduct();

	return result;
}

CFraction operator/(CFraction &lo, CFraction &ro )
{
	CFraction result;

	result = lo;
	result.reciprocal();
	result *= ro;
	result.reciprocal();

	return result;
}

bool operator == ( CFraction &lf, CFraction &rf )
{
	lf.reduct();
	rf.reduct();

	return ( (lf.denominator == rf.denominator) && (lf.numerator == rf.numerator) );
}

bool operator != ( CFraction &lf, CFraction &rf )
{
	return !(lf == rf);
}

bool operator < ( CFraction &lf, CFraction &rf )
{
	lf.reduct();
	rf.reduct();

	int ld = lf.denominator;
	int rd = rf.denominator;
	int cd = CFraction::gcd( ld, rd );
	int ll = rd / cd;
	int rl = ld / cd;

	return ( lf.numerator*ll < rf.numerator * rl );
}

bool operator > ( CFraction &lf, CFraction &rf )
{
	lf.reduct();
	rf.reduct();

	int ld = lf.denominator;
	int rd = rf.denominator;
	int cd = CFraction::gcd( ld, rd );
	int ll = rd / cd;
	int rl = ld / cd;

	return ( lf.numerator*ll > rf.numerator * rl );
}
