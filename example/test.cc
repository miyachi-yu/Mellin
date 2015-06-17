#include <complex>
#include <cmath>
#include <iostream>
#include "cgamma.cpp"



using namespace std; 


double ff(double x)
{

	double b=-2.1;
	double c=0.1;
	double t;
	t=exp(x);
	return pow(t,b)*pow(1.-t,c); 
}

double GLInt(double (*func)(double,complex<double>),complex<double> z)
{

//--data set--------------------------------------------------
	int n=2000;
	double a=-100.;
	double b=-0.000001;
//--weight data -----------------------------------------------
	int m=5;	
	double t[]={-0.906180,-0.538469,0.000000,0.538469,0.906180};	
	double c[]={ 0.236926, 0.478628,0.568888,0.478628,0.236926};
//--Gauss integration-------------------------------------------
	double x0,x;
	double s=0.0;
	double ss=0.0;
	double dx=(b-a)/n;
	double hx=dx/2.0;
	for (int k =0 ; k <= n-1 ;k++)
	{
	  x0=a+k*dx+hx;
		for (int i=0; i < m ;i++)
		{
		 x = x0 +t[i]*hx;
		 ss=ss+c[i]*func(x,z);
		}
	}
	s=ss*hx;
	return s;	
}

double real_f(double x,complex<double> s)
{
	double v,w,z;
	v=real(s);
	w=imag(s);
	z=exp(v*x)*cos(w*x)*ff(x);
	return z; 
}

double imag_f(double x,complex<double> s)
{

	double v,w,z;
	v=real(s);
	w=imag(s);
	z=exp(v*x)*sin(w*x)*ff(x);
	return z;
}

complex<double> fhat(complex<double> s)
{
	complex<double> z( GLInt(real_f,s), GLInt(imag_f,s) );
	return z;	
}

complex<double> beta(complex<double> s)
{
	double b=-2.1;
	double c=0.1;
	return cgamma(s+b,0)*cgamma(c+1.,0)/cgamma(s+b+c+1.,0);	
}

