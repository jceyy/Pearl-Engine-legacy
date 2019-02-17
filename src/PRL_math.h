#ifndef PRL_MATH_H_INCLUDED
#define PRL_MATH_H_INCLUDED

//! Value of pi.
const double pi = 3.141592653589793;

//! Give the corresponding power of 10.
long double pow10(int power);

//! Give the positive modulo of 2 numbers.
/*!
It is a classic modulo, returning always positive values.
For instance, -12 % 5 would not be -2 but +3. Please note that a negative y value make the function return x untouched.
*/
double modulo_pos(double x, double y);
//! Give the positive modulo of 2 numbers.
/*!
It is a classic modulo, returning always positive values.
For instance, -12 % 5 would not be -2 but +3. Please note that a negative y value make the function return x untouched.
*/
float modulo_pos(float x, float y);


//! Absolute value of a number.
int absVal(int x);
//! Absolute value of a number.
long long absVal(long long x);
//! Absolute value of a number.
float absVal(float x);
//! Absolute value of a number.
double absVal(double x);
//! Absolute value of a number.
long double absVal(long double x);

//! Sign of a number.
/*!
This function returns +1 if the provided number is positive and -1 if it is negative.
*/
int sign(int x);
//! Sign of a number.
/*!
This function returns +1 if the provided number is positive and -1 if it is negative.
*/
int sign(long long x);
//! Sign of a number.
/*!
This function returns +1 if the provided number is positive and -1 if it is negative.
*/
int sign(float x);
//! Sign of a number.
/*!
This function returns +1 if the provided number is positive and -1 if it is negative.
*/
int sign(double x);
//! Sign of a number.
/*!
This function returns +1 if the provided number is positive and -1 if it is negative.
*/
int sign(long double x);

//! The fastest cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 2.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double cos1(double x);
//!  Extremely fast cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 4.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double cos2(double x);
//!  Very fast cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 6.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double cos3(double x);
//!  Fast cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 8.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double cos4(double x);
//!  Quite precise cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 10.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double cos5(double x);
//!  Most precise cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 12.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double cos6(double x);




//! The fastest cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 2.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float cos1(float x);
//!  Extremely fast cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 4.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 4.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float cos2(float x);
//!  Very fast cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 6.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 6.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float cos3(float x);
//!  Fast cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 8.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 8.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float cos4(float x);
//!  Quite precise cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 10.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 10.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float cos5(float x);
//!  Most precise cosine approximation.
/*!
The cosine is approximated with a modified Taylor expansion of degree 12.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 12.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float cos6(float x);



//! The fastest sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 2.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double sin1 (double x);
//! Extremely sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 4.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 4.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double sin2 (double x);
//! Very fast sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 6.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 6.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double sin3 (double x);
//! Fast sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 8.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 8.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double sin4 (double x);
//! Quite precise sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 10.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 10.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double sin5 (double x);
//! Most precise sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 12.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 12.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
double sin6 (double x);




//! The fastest sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 2.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 2.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float sin1 (float x);
//! Extremely sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 4.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 4.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float sin2 (float x);
//! Very fast sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 6.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 6.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float sin3 (float x);
//! Fast sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 8.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 8.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float sin4 (float x);
//! Quite precise sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 10.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 10.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float sin5 (float x);
//! Most precise sine approximation.
/*!
The sine is defined as sin(x) = cos(pi/2 - x), using the cosine approximation of degree 12.
In order to make the Taylor expansion continuous, it is modified with an extra interpolation of degree 12.
The higher the degree, the better the precision, but with higher precision comes with slower computation.
*/
float sin6 (float x);

//! Fastest exponential approximation.
/*!
Using the formula exp(x) = lim|n->inf (1 + x/n)^n, this approximation uses n = 16;
Even though it is quite accurate with negative numbers, the true exponential function's huge growth rate make it quickly inaccurate for positive numbers!
*/
double expon1(double x);
//! Fast exponential approximation.
/*!
Using the formula exp(x) = lim|n->inf (1 + x/n)^n, this approximation uses n = 256;
Even though it is quite accurate with negative numbers, the true exponential function's huge growth rate make it quickly inaccurate for positive numbers!
*/
double expon2(double x);
//! Most precise approximation.
/*!
Using the formula exp(x) = lim|n->inf (1 + x/n)^n, this approximation uses n = 65536;
Even though it is quite accurate with negative numbers, the true exponential function's huge growth rate make it quickly inaccurate for positive numbers!
*/
double expon3(double x);



//! Fastest exponential approximation.
/*!
Using the formula exp(x) = lim|n->inf (1 + x/n)^n, this approximation uses n = 16;
Even though it is quite accurate with negative numbers, the true exponential function's huge growth rate make it quickly inaccurate for positive numbers!
*/
float expon1(float x);
//! Fast exponential approximation.
/*!
Using the formula exp(x) = lim|n->inf (1 + x/n)^n, this approximation uses n = 256;
Even though it is quite accurate with negative numbers, the true exponential function's huge growth rate make it quickly inaccurate for positive numbers!
*/
float expon2(float x);
//! Most precise approximation.
/*!
Using the formula exp(x) = lim|n->inf (1 + x/n)^n, this approximation uses n = 65536;
Even though it is quite accurate with negative numbers, the true exponential function's huge growth rate make it quickly inaccurate for positive numbers!
*/
float expon3(float x);


#endif // PRL_MATH_H_INCLUDED
