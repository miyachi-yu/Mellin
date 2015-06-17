/*--------------------------------------------------------
Mellin Inversion and Mellin transfomarion 
Date : 2007/04/18
Auther : nkobayashi (GL Integral: Sugiya)
Software development : GNU g++

----------------------------------------------------------*/

#include <iostream>
#include <cmath>
#include <complex>

#include <iomanip>

#include "test.cc"


//f(n)->f(x)Mellin Inversion 
double f4(double z,double x)
{


	//--transform parameter--
	double c=2.2;
	double phi=0.75*M_PI;


	complex<double> e(cos(phi),sin(phi));

	double s=imag(e* pow(x,-c-z*e)
		 * beta(c + z*e) //f(n)=beta(n)
		 );

 	return s;
}

//Mellin transformed f(n)->f(x)
double f3(double z,double x)
{


	//--transform parameter--
	double c=2.3;
	double phi=0.5*M_PI;


	complex<double> e(cos(phi),sin(phi));

	double s=imag(e* pow(x,-c-z*e)
		 * fhat(c + z*e) //f(n)
		 );
	
 	return s;
}


//Gaussian Quadrature

double GL(double (*func)(double,double),double xx)
{
//--data set--------------------------------------------------
	int n=100;
	double a=0.;
	double b=10.;
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
		 ss=ss+c[i]*func(x,xx);
		}
	}
	s=ss*hx;
	return s;	
}


//Heaviside Step Function
double H(double x)
{
	if(x<0){ return 0;}
	else if(x==0){ return 0.5;}
	else {return 1;}

}


int main()
{


	double b=-2.1;
	double c=0.1;
	double x;

	double A,B,C,err1,err2,err3,err4;
	double h;
	h=pow(10,0.1);

  std::cout 
    << std::setw(12) << "x\t"
    << std::setw(12) << "f(x)\t"
    << std::setw(12) << " M.M-1{f(x)} \t"
    << std::setw(12) << " M-1{f^(s)} \t"
    << std::setw(12) << "rel.err1 \t"
    << std::setw(12) << "rel.err2 \t"
    << std::setw(12) << "abs.err1 \t"
    << std::setw(12) << "abs.err2"
    << std::endl;


	for(x=1E-5;x<1.;x*=h)
	{	

		A=pow(x,b)*pow(1.-x,c);
		B=GL(f3,x)/M_PI;
		C=GL(f4,x)/M_PI;	 
		err1=fabs(1-B/A);	
		err2=fabs(1-C/A);
		err3=fabs(B-A);
		err4=fabs(C-A);

    std::cout 
      << std::setw(12) << x << '\t'
      << std::setw(12) << A << "\t"
      << std::setw(12) << B << "\t"
      << std::setw(12) << C << "\t"
      << std::setw(12) << err1 << "\t"
      << std::setw(12) << err2 << "\t"
      << std::setw(12) << err3 << "\t"
      << std::setw(12) << err4
      << std::endl;
	
	}



	return 0;
}
