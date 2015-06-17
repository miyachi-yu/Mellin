#include <fstream.h>

using namespace std; 

int testCharge_macro(){ 

//  double x1[10000],x2[401], y1[10000] , y2[10000], y3[10000] , y4[401],y5[401],y6[401],;
     const int idata1 = 401;
     const int idata2 = 401;
  double x1[ idata1 ], x2[ idata2 ], y1[ idata1 ], y2[ idata1 ], 
       y3[ idata1 ] , y4[ idata2 ], y5[ idata2 ], y6[ idata2 ];

  ifstream data1("testCharge.dat");
  int i=0;
  
  while(!data1.eof()) 
    {data1 >> x1[i] >> y1[i] >>  y2[i] >> y3[i] ; i++;}
  
  
  TCanvas *c = new TCanvas( "graph" );
  c->SetLogx();

  TH1 *frame = c->DrawFrame( 1.0, 0.0, 10000.0, 1.0 );
  frame->GetXaxis()->SetTitle("Q^{2} (GeV/c)^{2}");
  frame->GetYaxis()->SetTitle("charge");
  

  TGraph *gr1 = new TGraph(i,x1,y1);
  gr1->SetMarkerStyle( 7 );
  gr1->SetMarkerSize(1.2);
  gr1->SetMarkerColor(3);
  gr1->Draw("p"); 
  
  
  TGraph *gr2 = new TGraph(i,x1,y2);
  gr2->SetMarkerStyle( 7 );
  gr2->SetMarkerSize(1.2);
  gr2->SetMarkerColor(2);
  gr2->Draw("p"); 
  
  TGraph *gr3 = new TGraph(i,x1,y3);
  gr3->SetMarkerStyle( 7 );
  gr3->SetMarkerSize(1.2);
  gr3->SetMarkerColor(4);
  gr3->Draw("p"); 
  
  
  //graph = new TGraph(index, x, y2);
  //Tgraph->SetTitle(";q^2;charge");
  //graph->Draw("APl");
  
  data1.close();
   
  ifstream data2("charge.dat");
  int j=0; 
  while(!data2.eof()) 
    {data2 >> x2[j] >> y4[j] >>  y5[j] >> y6[j] ; j++;}
  
  TGraph *gr4 = new TGraph(j,x2,y4);
 
  gr4->SetMarkerStyle( 24 );
  gr4->SetMarkerSize(0.3);
  gr4->SetMarkerColor(1);
  gr4->Draw("p"); 
  
  TGraph *gr5 = new TGraph(j,x2,y5);
  
  gr5->SetMarkerStyle( 24 );
  gr5->SetMarkerSize(0.3);
  gr5->SetMarkerColor(1);
  gr5->Draw("p");
  
  
  TGraph *gr6 = new TGraph(j,x2,y6);
  
  gr6->SetMarkerStyle( 24 );
  gr6->SetMarkerSize(0.3);
  gr6->SetMarkerColor(1);
  gr6->Draw("p");
  
  c->Draw();


  data2.close();
  
  leg = new TLegend(0.7,0.65,0.85,0.8);
  leg->AddEntry(gr1,"gluon","p");
  leg->AddEntry(gr2,"u","p");
  leg->AddEntry(gr3,"d","p");
  //leg->SetTextSize(0.5);
  leg->Draw();


}
