/*
  Usage: use  -FF to get Xml data 
*/
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <Utility/Arguments.hh>
#include <QCD/PdfErrMatrix.hh>
#include <Fragmentation/Evolution.hh>

#include <TMatrix.h>
#include <TAxis.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TPDF.h>
#include <TPostScript.h>

#define NUM 2
using namespace std;

//set groval invariant
TMatrix m(NUM,NUM),minv(NUM,NUM),I(NUM,NUM);
QCD::PdfErrMatrix matrix;
vector< int > ind( 2 );

//get error Matrix parameter
//int set_Matrix(int argc ,char* argv[], vector < vector < double > > matrix); 
void set_Matrix( ) throw( int ) ; 

//caluculate invart
void inv();

//display matrix     
void display();

//draw correlation
void draw();

//draw correlation as table format
void table_draw();

//draw pick up correlation as table format
void pickup_draw(int x1 ,int x2);

void pickup_draw2(int x1 ,int x2);

//**********************************************
//******************main************************
//**********************************************
int main(int argc, char* argv[])
{
     Utility::Arguments& args = Utility::Arguments::ref( argc, argv );

     TApplication TA( "browser", &argc, argv ); //needed if you use root
     
     // ----------------  create PdfErrMatrix  ------------------ //
     try{ 
	  set_Matrix();
	 	  
     } catch ( int error ) {
	  ARG_ERR << "catch error (" << error << ")" << endl;
	  return 1;
     }


     //set syle
     gROOT->SetStyle("Pub");

     TF2 *f0 = new TF2("f0","[0]*x^2+[1]*x*y+[2]*y^2",-1,1,-1,1);
     //draw();
     //table_draw();
     //pickup_draw(0,0);
     pickup_draw2(1,7);
     return 0;     
}



//**********************************************
//************FUNCTIONS*************************
//**********************************************


//get a data & matrix size from xml file
//int set_Matrix(int argc ,char*argv[] , vector < vector < double > > matrix){
void set_Matrix() throw( int ) { 
     
     Utility::Arguments& args = Utility::Arguments::ref();
     try {
	  //  cout << "yoshi -1" << endl;
	  // Prepare PDF
	  Fragmentation::Evo pdf( args );

//	  cout << "yoshi 0" << endl;
	  
	  // load Error matrix from pdf
	  QCD::PdfParametrization* ipdf = pdf.initialDistribution();

	  //cout << "yoshi 1" << endl;

	  matrix.load( ipdf );

	  //cout << "yoshi 2" << endl;

	  cout << matrix << endl;

	  //cout << "yoshi 3" << endl;
	  
	  // if no parameters are free, just exit the program
	  if( matrix.size() == 0 ){
	       cerr << "Error matrix size is 0" << endl;
	       throw 1;	       
	  }
	  
     }    
     
//error massage   
     catch( int err ){
	  
	  switch( err ) {
	  case 1:
	       cerr 
		    << "No error matrix defined!!\n"
		    << "check PDF XML file\n"
		    << endl;
	       break;
	       
	  default:
	       cerr << "Catch error " << err << endl; break;
	  }
     }
}

/////////////////////////////////////////////////////////////////////////////
//caluculate invart
void inv(){ 
     minv = m;
     minv.Invert();
     I =m*minv;
}

////////////////////////////////////////////////////////////////////////////
//display matrix
void display(){
     m.Print();
     minv.Print();
     I.Print();
}



///////////////////////////////////////////////////////////////////////////
//draw correlation
void draw(){
     
     //   TPostScript eps("error_correlation.eps");
     TPDF pdf( "error_correlation.pdf" );
     
     //Set Canvas
     TCanvas *c1 = new TCanvas( "c1", "Error_Correlation" );
     c1->Draw();

     TF2 *f0 = new TF2("f0","[0]*x^2+[1]*x*y+[2]*y^2",-1,1,-1,1);
     f0->SetTitle( "" );
     
     //set Contour level
     double level[] = {0.5 ,1.0};
     f0 -> SetContour( 2, level);

     int ndim = matrix.size();


     
     for( int i = 0; i < ndim - 1; i++ ){
	  for( int j = i + 1; j < ndim - 1; j++ ){
	       


	       if( matrix[ i ][ i ] == 0.0 ||
		   matrix[ j ][ j ] == 0.0 ) continue;
	       
	       
	       ind[ 0 ] = i;
	       ind[ 1 ] = j;
	       
	       for( int i0 = 0; i0 < 2; i0++ )
		    for( int i1 = 0; i1 < 2; i1++ )
			 m( i0, i1 ) = matrix[ ind[ i0 ] ][ ind[ i1 ] ];
	       
	       inv();
	       display();
	       
	       double offdiag = sqrt( m( 0, 0 ) * m( 1, 1 ) );
	       f0->SetParameters( minv( 0, 0 ) * m( 0, 0 ),
				  ( minv( 0, 1 ) +minv( 1, 0 ) ) * offdiag,
				  minv( 1, 1 ) * m( 1, 1 ) );
	       
	       f0->GetXaxis()->SetTitle( matrix.info()[ ind[ 0 ] ].c_str() );
	       f0->GetYaxis()->SetTitle( matrix.info()[ ind[ 1 ] ].c_str() );
	       
	       f0-> Draw();
	       c1->Update();

/*	       char* str = "Error_Correlation_"
		            + matrix.info()[ ind[ 0 ] ].c_str()
			    +  "_" +
		    matrix.info()[ ind[ 1 ] ].c_str() +
		    ".eps";
*/       
//	       c1->Print( "Error_Correlation.eps" );
	  }     
     }
     c1->Print( "Error_Correlation.png" );
//     c1->Print( "Error_Correlation.eps");
     pdf.Close();
//     eps.Close();
}


///////////////////////////////////////////////////////////////////////////
//draw correlation as table form
void table_draw(){
     

     //TPDF pdf( "error_correlation.pdf" );
     


     int ndim = matrix.size() ,k = 0;


     //((ndim-1)-1-i)*(ndim-1)+(ndim-1)-1-j-i+1
 

     //Set Canvas
     TCanvas *c1 = new TCanvas( "c1", "Error_Correlation" );
     c1->Divide( ndim -1, ndim -1 );


     TF2 *f0 = new TF2("f0","[0]*x^2+[1]*x*y+[2]*y^2",-1,1,-1,1);
     f0->SetTitle( "" );
//     f0->SetTitle( "Correlation" );
     

//set Contour level
     double level[] = {0.5 ,1.0};
     f0 -> SetContour( 2, level);

     for( int i = 0; i < ndim - 1; i++ ){
	  for( int j = 0; j < ndim - 1 -i; j++ ){
	       
	       k = ( ndim - 1 )*( ndim - 1 )-( ndim - 1 )*i-j-i;
	      
	       c1 -> cd(k);


	       if( matrix[ i ][ i ] == 0.0 ||
		   matrix[ j ][ j ] == 0.0 ) continue;



	       ind[ 0 ] = i;
	       ind[ 1 ] = j;
	       
	       for( int i0 = 0; i0 < 2; i0++ )
		    for( int i1 = 0; i1 < 2; i1++ )
			 m( i0, i1 ) = matrix[ ind[ i0 ] ][ ind[ i1 ] ];
	       
	       inv();
	       display();
	       
	       double offdiag = sqrt( m( 0, 0 ) * m( 1, 1 ) );
	       f0->SetParameters( minv( 0, 0 ) * m( 0, 0 ),
				  ( minv( 0, 1 ) +minv( 1, 0 ) ) * offdiag,
				  minv( 1, 1 ) * m( 1, 1 ) );
	       
	       f0->GetXaxis()->SetTitle( matrix.info()[ ind[ 0 ] ].c_str() );
	       f0->GetYaxis()->SetTitle( matrix.info()[ ind[ 1 ] ].c_str() );
	       
	       //Change pad
	       //((ndim-1)-1-i)*(ndim-1)+(ndim-1)-1-j-i+1	       
	      
	       f0-> Draw();
	       c1->Update();
	  }     
     }
     c1->Print( "Error_Correlation.png" );

     //pdf.Close();
}


///////////////////////////////////////////////////////////////////////////
//draw pick up correlation as table form
void pickup_draw(int x1 ,int x2){     

     //TPDF pdf( "error_correlation.pdf" );
     


     int ndim = matrix.size() ,k = 0 ,l = 0;


     //((ndim-1)-1-i)*(ndim-1)+(ndim-1)-1-j-i+1
 

     //Set Canvas
     TCanvas *c1 = new TCanvas( "c1", "Error_Correlation" );
     int tdim =2;
     if(tdim > ndim){
	  cout << "table size is not proper" << endl;
	  //return 0;
     }
     c1->Divide( tdim ,tdim );


     TF2 *f0 = new TF2("f0","[0]*x^2+[1]*x*y+[2]*y^2",-1,1,-1,1);
     f0->SetTitle( "" );
//     f0->SetTitle( "Correlation" );
     

//set Contour level
     double level[] = {0.5 ,1.0};
     f0 -> SetContour( 2, level);


//prepare to draw
     for( int i = 0; i < tdim; i++ ){
	  for( int j = 0; j < tdim ; j++ ){

	       l=tdim*tdim -tdim*i -j;
//	       l=i*tdim+j+1;
	       c1 -> cd( l );

	       if( matrix[ x1+i ][ x1+i ] == 0.0 ||
	          matrix[ x2+j ][ x2+j ] == 0.0 ) continue;
	       x1=1;
	       x2=0;


	       ind[ 0 ] =  x1+i;
	       ind[ 1 ] =  x2+j;

	       
	       cout<<"////////////////////////////"<<endl;	       
	       cout<<"ind0 :"<<ind[0]<<endl
		   <<"ind1 :"<<ind[1]<<endl;
	       for( int i0 = 0; i0 < 2; i0++ )
		    for( int i1 = 0; i1 < 2; i1++ )
			 m( i0, i1 ) = matrix[ ind[ i0 ] ][ ind[ i1 ] ];
	       
	       inv();

	       display();
	       
	       double offdiag = sqrt( m( 0, 0 ) * m( 1, 1 ) );
	       f0->SetParameters( minv( 0, 0 ) * m( 0, 0 ),
				  ( minv( 0, 1 ) +minv( 1, 0 ) ) * offdiag,
				  minv( 1, 1 ) * m( 1, 1 ) );
	       
	       f0->GetXaxis()->SetTitle( matrix.info()[ ind[ 0 ] ].c_str() );
	       f0->GetYaxis()->SetTitle( matrix.info()[ ind[ 1 ] ].c_str() );
	       
	       //k = ( ndim - 1 )*( ndim - 1 )-( ndim - 1 )*i-j-i;
	       //cout<<"k0 :"<<k<<endl;
	       //((ndim-1)-1-i)*(ndim-1)+(ndim-1)-1-j-i+1
	       //int x  = (ndim -1)*(ndim -1) -x2 +1;
	       //for(int n1=0; n1 < tdim  ;n1++){
	       //   for(int n2=0; n2 < tdim   ;n2++){
//	       l =  tdim * tdim -j  -i*tdim;

	       //  if (k == -(ndim   -i)*n1 +x +n2  ){
//			 if (k == (ndim -1 )*(ndim -1)-1 ){
	

			 cout<<"draw"<<endl;
			 f0 -> Draw();	
			 c1 -> Update();
			 cout<<"l :"<<l<<endl
			     <<"///////////////////////////////"<<endl;
//			     <<"l :"<<l<<endl;

			 //		 }
			 // }
			 //}
	       
	       
	  }
     }

     c1->Print( "Error_Correlation.png" );

     //pdf.Close();

     
}

///////////////////////////////////////////////////////////////////////////
//draw pick up correlation
void pickup_draw2(int x1 ,int x2){     

     //TPDF pdf( "error_correlation.pdf" );
     


     int ndim = matrix.size() ,k = 0 ,l = 0;


     //((ndim-1)-1-i)*(ndim-1)+(ndim-1)-1-j-i+1
 

     //Set Canvas
     TCanvas *c1 = new TCanvas( "c1", "Error_Correlation" );
     int tdim =2;
     if(tdim > ndim){
	  cout << "table size is not proper" << endl;
	  //return 0;
     }
//     c1->Divide( tdim ,tdim );


     TF2 *f0 = new TF2("f0","[0]*x^2+[1]*x*y+[2]*y^2",-1,1,-1,1);
     f0->SetTitle( "" );
//     f0->SetTitle( "Correlation" );
     

//set Contour level
     double level[] = {0.5 ,1.0};
     f0 -> SetContour( 2, level);


//prepare to draw

	       if( matrix[ x1 ][ x1 ] == 0.0 ||
	          matrix[ x2 ][ x2 ] == 0.0 )
	       x1=1;
	       x2=0;


	       ind[ 0 ] =  x1;
	       ind[ 1 ] =  x2;

	       
	       cout<<"////////////////////////////"<<endl;	       
	       cout<<"ind0 :"<<ind[0]<<endl
		   <<"ind1 :"<<ind[1]<<endl;
	       for( int i0 = 0; i0 < 2; i0++ )
		    for( int i1 = 0; i1 < 2; i1++ )
			 m( i0, i1 ) = matrix[ ind[ i0 ] ][ ind[ i1 ] ];
	       
	       inv();

	       display();
	       
	       double offdiag = sqrt( m( 0, 0 ) * m( 1, 1 ) );
	       f0->SetParameters( minv( 0, 0 ) * m( 0, 0 ),
				  ( minv( 0, 1 ) +minv( 1, 0 ) ) * offdiag,
				  minv( 1, 1 ) * m( 1, 1 ) );
	       
	       f0->GetXaxis()->SetTitle( matrix.info()[ ind[ 0 ] ].c_str() );
	       f0->GetYaxis()->SetTitle( matrix.info()[ ind[ 1 ] ].c_str() );
	       


			    cout<<"draw"<<endl;
			    f0 -> Draw();	
			    c1 -> Update();


     
     c1->Print( "Error_Correlation.png" );

     //pdf.Close();

     
}
