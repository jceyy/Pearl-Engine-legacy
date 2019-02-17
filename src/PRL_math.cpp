/* PI constants */

const double pi = 3.141592653589793;
const double pi_neg=-3.141592653589793;
const double three_pi_2=4.712388980384689; //3*pi/2
const double two_pi=6.283185307179586;
const double two_pi_neg=-6.283185307179586;
const double pi_2=1.570796326794897; // pi/2
const double pi_2_neg=-1.570796326794897;

/* Taylor constants, used for interpolation */
// these numbers represent the taylor polynomial approximation in pi/2
// for several degrees plus an interpolation constant of the highest degree.

// Cosine
const long double alpha = 0.40528473456935108578; // deg 2
const long double beta = 0.03838665120972617986; // deg 4
const long double gamma = 0.00132934019384949401; // deg 6
const long double delta = 0.00002413417706299239; // deg 8
const long double epsilon = 0.00000027049118139781; // deg 10
const long double zeta = 0.00000000205966141520; // deg 12

const long double fact2 = 0.5;
const long double fact4 = 0.04166666666666666667;
const long double fact6 = 0.00138888888888888889;
const long double fact8 = 0.00002480158730158730;
const long double fact10 = 0.00000027557319223986;

// Exponential: powers of 2 (p2_1 is already the 8th one, p2_13 the 20th one)
const double p2_1 = 16.0;
const double p2_2 = 256.0;
const double p2_3 = 65536.0;

/* General Mathematic Tools */

long double pow10(int power)
{
    long double i(1.0);
    if (power == 0)
    {
        return i;
    }
    else if (power > 0)
    {
        for (int j(0); j < power; j++)
            i *= 10.0;
        return i;
    }
    else
    {
        for (int j(0); j < -power; j++)
            i *= 10.0;
        return 1.0/i;
    }
}

float modulo_pos(float x, float y)
{
    if (y == 0.0f || x == 0.0f)
        return x;
    else if (y > 0.0f)
    {
        if (x > 0.0f)
            return x - ((int)(x/y)) * y;
        else
            return x + ((int)(-x/y) + 1) * y;
    }
    else
        return x;

}

double modulo_pos(double x, double y)
{
    if (y == 0.0 || x == 0.0)
        return x;
    else if (y > 0.0)
    {
        if (x > 0.0)
            return x - ((int)(x/y)) * y;
        else
            return x + ((int)(-x/y) + 1) * y;
    }
    else
        return x;

}

/* Absolute value of a number */

int absVal(int x)
{
    if (x < 0)
        return (-1) * x;
    return x;
}

long long absVal(long long  x)
{
    if (x < 0)
        return (-1) * x;
    return x;
}

float absVal(float x)
{
    if (x < 0.0f)
        return (-1) * x;
    return x;
}

double absVal(double x)
{
    if (x < 0.0)
        return (-1) * x;
    return x;
}

long double absVal(long double x)
{
    if (x < 0.0)
        return (-1) * x;
    return x;
}

/* Sign of a number */

int sign(int x)
{
    if (x < 0)
        return -1;
    else
        return 1;
}

int sign(long long x)
{
    if (x < 0)
        return -1;
    else
        return 1;
}

int sign(float x)
{
    if (x < 0.0f)
        return -1;
    else
        return 1;
}

int sign(double x)
{
    if (x < 0.0)
        return -1;
    else
        return 1;
}

int sign(long double x)
{
    if (x < 0.0)
        return -1;
    else
        return 1;
}

/* Trigonometric Functions Approximation */

/* Cosines */

float cos1 (float x) // Degree 2 function
{
    x = modulo_pos(x, (float)two_pi);
	if (x <= pi_2)
		return 1 - x*x*alpha;
	else if (x <= three_pi_2)
	{
		return (x - pi)*(x - pi)*alpha - 1;
	}
	else
        return 1 - (x - two_pi)*(x - two_pi)*alpha;
}

float cos2 (float x) // Degree 4 function
{
	x = modulo_pos(x, (float)two_pi);
	if (x <= pi_2)
		return 1 - x*x*(fact2 - x*x*beta);
	else if (x <= three_pi_2)
	{
	    x = x-pi;
		return x*x*(fact2 - x*x*beta) - 1;
	}
	else
    {
        x = x - two_pi;
        return 1 - x*x*(fact2 - x*x*beta);
    }
}

float cos3 (float x) // Degree 6 function
{
	x = modulo_pos(x, (float)two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - gamma*x));
	}
	else if (x <= three_pi_2)
	{
	    x=(x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - gamma*x)) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - gamma*x));
    }
}

float cos4 (float x) // Degree 8 function
{
	x = modulo_pos(x, (float)two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*delta)));
	}
	else if (x <= three_pi_2)
	{
	    x = (x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - x*(fact6 - x*delta))) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*delta)));
    }
}

float cos5 (float x) // Degree 10 function
{
	x = modulo_pos(x, (float)two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - epsilon*x))));
	}
	else if (x <= three_pi_2)
	{
	    x = (x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - epsilon*x)))) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - epsilon*x))));
    }
}

float cos6 (float x) // Degree 12 function
{
	x = modulo_pos(x, (float)two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - x*(fact10 - x*zeta)))));
	}
	else if (x <= three_pi_2)
	{
	    x = (x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - x*(fact10 - x*zeta))))) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - x*(fact10 - x*zeta)))));
    }
}

double cos1 (double x) // Degree 2 function
{
    x = modulo_pos(x, two_pi);
	if (x <= pi_2)
		return 1 - x*x*alpha;
	else if (x <= three_pi_2)
	{
		return (x - pi)*(x - pi)*alpha - 1;
	}
	else
        return 1 - (x - two_pi)*(x - two_pi)*alpha;
}

double cos2 (double x) // Degree 4 function
{
	x = modulo_pos(x, two_pi);
	if (x <= pi_2)
		return 1 - x*x*(fact2 - x*x*beta);
	else if (x <= three_pi_2)
	{
	    x = x-pi;
		return x*x*(fact2 - x*x*beta) - 1;
	}
	else
    {
        x = x - two_pi;
        return 1 - x*x*(fact2 - x*x*beta);
    }
}

double cos3 (double x) // Degree 6 function
{
	x = modulo_pos(x, two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - gamma*x));
	}
	else if (x <= three_pi_2)
	{
	    x=(x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - gamma*x)) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - gamma*x));
    }
}

double cos4 (double x) // Degree 8 function
{
	x = modulo_pos(x, two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*delta)));
	}
	else if (x <= three_pi_2)
	{
	    x = (x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - x*(fact6 - x*delta))) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*delta)));
    }
}

double cos5 (double x) // Degree 10 function
{
	x = modulo_pos(x, two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - epsilon*x))));
	}
	else if (x <= three_pi_2)
	{
	    x = (x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - epsilon*x)))) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - epsilon*x))));
    }
}

double cos6 (double x) // Degree 12 function
{
	x = modulo_pos(x, two_pi);
	if (x <= pi_2)
    {
        x = x*x;
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - x*(fact10 - x*zeta)))));
	}
	else if (x <= three_pi_2)
	{
	    x = (x - pi)*(x - pi);
        return x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - x*(fact10 - x*zeta))))) - 1;
	}
	else
    {
        x = (x - two_pi)*(x - two_pi);
        return 1 - x*(fact2 - x*(fact4 - x*(fact6 - x*(fact8 - x*(fact10 - x*zeta)))));
    }
}

/* Sines */

float sin1 (float x) // Deg 2 function
{
    return cos1((float) pi_2 - x);
}

float sin2 (float x) // Deg 4 function
{
    return cos2((float) pi_2 - x);
}

float sin3 (float x) // Deg 6 function
{
    return cos3((float) pi_2 - x);
}

float sin4 (float x) // Deg 8 function
{
    return cos4((float) pi_2 - x);
}

float sin5 (float x) // Deg 10 function
{
    return cos5((float) pi_2 - x);
}

float sin6 (float x) // Deg 12 function
{
    return cos6((float) pi_2 - x);
}

double sin1 (double x) // Deg 2 function
{
    return cos1(pi_2 - x);
}

double sin2 (double x) // Deg 4 function
{
    return cos2(pi_2 - x);
}

double sin3 (double x) // Deg 6 function
{
    return cos3(pi_2 - x);
}

double sin4 (double x) // Deg 8 function
{
    return cos4(pi_2 - x);
}

double sin5 (double x) // Deg 10 function
{
    return cos5(pi_2 - x);
}

double sin6 (double x) // Deg 12 function
{
    return cos6(pi_2 - x);
}


/* Exponentials Functions Approximations */

float expon1(float x)
{
    x = 1.0f + x/((float) p2_1);
    x*=x; x*=x; x*=x; x*=x;
    return x;
}

float expon2(float x)
{
    x = 1.0f + x/((float) p2_2);
    x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x;
    return x;
}

float expon3(float x)
{
    x = 1.0f + x/((float) p2_3);
    x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x;
    x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x;
    return x;
}

double expon1(double x)
{
    x = 1.0 + x/p2_1;
    x*=x; x*=x; x*=x; x*=x;
    return x;
}

double expon2(double x)
{
    x = 1.0 + x/p2_2;
    x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x;
    return x;
}

double expon3(double x)
{
    x = 1.0 + x/p2_3;
    x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x;
    x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x; x*=x;
    return x;
}
