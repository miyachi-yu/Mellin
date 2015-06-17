#include <string>

#include <TLegend.h>
#include <TColorWheel.h>

void hermesMulti( const int& t, const int& h, const int& color,
		  const double xmin = 0.02, const double xmax = 0.8 ){
  
  Manager *m = Manager::instance();

  vector< double > zbin( 6 );
  zbin[ 0 ] = 0.1;
  zbin[ 1 ] = 0.2;
  zbin[ 2 ] = 0.3;
  zbin[ 3 ] = 0.4;
  zbin[ 4 ] = 0.6;
  zbin[ 5 ] = 0.8;
  
  for( int i = 0; i < 5; i++ ){
    TGraph *pp1 = m->getMulti_x( t, h, // p, pi+
				 20, xmin, xmax, true,
				 56.2, // s
				 2.5,  // Q^2
				 0.5 * ( zbin[ i ] +  zbin[ i + 1 ] )
				 ); // x - z
    
    pp1->SetLineColor( color );
    pp1->SetLineStyle( 2 );
    pp1->Draw( "l" );
  }
}

TGraph* fragXsec_T( const double& s, const double& scale ){
  
  Manager *m = Manager::instance();
  
  TGraph *g = m->getFragXsec_Z( 1, // PIp only
				40, 0.02, 0.95, false,
				s * s, 
				scale ); // factor 2 should be applied 
  g->SetLineStyle( 2 );
  g->SetLineWidth( 2 );
  g->SetLineColor( kOrange + 5 );
  
  return g;
}  

int drawFragXsec(){

  bool sia_pipm     = true;
  
  Manager *m = Manager::instance();
  RExpData *sld = m->getDataSet( "SLD__inclusive_91.2_1999_PIpm", "zmin", "zmax" );
  RExpData *aleph = m->getDataSet( "ALEPH__91.2_1995_PIpm", "zmin", "zmax" );
  RExpData *opal = m->getDataSet( "OPAL_91.2_1994_PIpm", "zmin", "zmax" );
  RExpData *delphi = m->getDataSet( "DELPHI_inclusive_91.2_1998_PIpm", "zmin", "zmax" );
  RExpData *tasso44 = m->getDataSet( "TASSO_44_1989_PIpm", "zmin", "zmax" );
  RExpData *tasso34 = m->getDataSet( "TASSO_34_1989_PIpm", "zmin", "zmax" );
  RExpData *tpc = m->getDataSet( "TPC_29_1988_PIpm", "zmin", "zmax" );
  RExpData *belle = m->getDataSet( "Belle_2013_PIpm", "zmin", "zmax" );

  TCanvas *c = new TCanvas( "multiplicity", "", 800, 1200 );
  c->SetLogy();
  
  double xmin = -0.1;
  double xmax =  1.1;
  double ymin = 1E-7;
  double ymax = 1E+6;
  
  TH1 *frame = c->DrawFrame( xmin, ymin, xmax, ymax );
  frame->GetXaxis()->SetTitle( "z" );
  frame->GetYaxis()->SetTitle( "multiplicity" );

  TLine *line = new TLine();
  line->SetLineStyle( 2 );
  line->SetLineWidth( 2 );
  line->SetLineColor( kGray + 1 );

  line->DrawLine( 0.2, ymin, 0.2, ymax );
  line->DrawLine( 0.9, ymin, 0.9, ymax );
  
  TLegend *leg  = new TLegend( 0.55, 0.7, 0.89, 0.89 );
  leg->AddEntry( sld, "SLD 91.2 GeV (x1000)", "P" );
  leg->AddEntry( aleph, "ALEPH 91.2 GeV (x100)", "P" );
  leg->AddEntry( opal, "OPAL 91.2 GeV (x10)", "P" );
  leg->AddEntry( delphi, "OPAL 91.2 GeV", "P" );
  leg->Draw();

  TLegend *legb = new TLegend( 0.12, 0.12, 0.45, 0.3 );
  legb->AddEntry( tasso44, "TASSO 44 GeV (/10)", "P" );
  legb->AddEntry( tasso34, "TASSO 34 GeV (/100)", "P" );
  legb->AddEntry( tpc, "TPC 29 GeV (x10^{-3})", "P" );
  legb->AddEntry( belle, "Belle 10.52 GeV (x10^{-4})", "P" );
  legb->Draw();
  
  sld->rescale( 1000 );
  aleph->rescale( 100 );
  opal->rescale( 10 );
  tasso44->rescale( 0.1 );
  tasso34->rescale( 0.01 );
  tpc->rescale( 0.001 );
  belle->rescale( 0.0001 );

  sld->style(    21, 1, kBlue     );
  aleph->style(  21, 1, kBlue - 4 );
  opal->style(   21, 1, kBlue - 7 );
  delphi->style( 21, 1, kBlue - 9 );
  tasso44->style( 21, 1, kGreen + 3 );
  tasso34->style( 21, 1, kGreen - 2 );
  tpc->style(     21, 1, kGreen - 6 );
  belle->style(   21, 1, kRed   - 3 );

  belle->draw();
  tasso34->draw();
  tasso44->draw();
  tpc->draw();
  sld->draw();
  aleph->draw();
  opal->draw();
  delphi->draw();

  if( sia_pipm ){

    TGraph *g1 = fragXsec_T( 91.2, 2.0 * 1000 );
    TGraph *g2 = fragXsec_T( 91.2, 2.0 * 100 );
    TGraph *g3 = fragXsec_T( 91.2, 2.0 * 10);
    TGraph *g4 = fragXsec_T( 91.2, 2.0  );
    TGraph *g5 = fragXsec_T(   44, 2.0 / 10    );
    TGraph *g6 = fragXsec_T(   34, 2.0 / 100   );
    TGraph *g7 = fragXsec_T(   29, 2.0 / 1000  );
    TGraph *g8 = fragXsec_T(10.52, 2.0 / 10000 );
    
    g1->SetLineColor( kBlue );
    g2->SetLineColor( kBlue - 4 );
    g3->SetLineColor( kBlue - 7 );
    g4->SetLineColor( kBlue - 9 );
    g5->SetLineColor( kGreen + 3 );
    g6->SetLineColor( kGreen - 2 );
    g7->SetLineColor( kGreen - 6 );
    g8->SetLineColor( kRed   - 3 );
    
    g1->Draw( "l" );
    g2->Draw( "l" );
    g3->Draw( "l" );
    g4->Draw( "l" );
    g5->Draw( "l" );
    g6->Draw( "l" );
    g7->Draw( "l" );
    g8->Draw( "l" );
  }

  c->Print( "SIA_PI.png" );

}

void drawHermesMulti(){

  bool hermes_p_pip = true;
  bool hermes_p_pim = true;
  bool hermes_d_pip = false;
  bool hermes_d_pim = false;

  // get data from Manager
  Manager *m = Manager::instance();
  RExpData *hermesPp = m->getDataSet( "HERMES_Multi_PRO_PIp", "xmin", "xmax" );
  RExpData *hermesDp = m->getDataSet( "HERMES_Multi_DEU_PIp", "xmin", "xmax" );
  RExpData *hermesPm = m->getDataSet( "HERMES_Multi_PRO_PIm", "xmin", "xmax" );
  RExpData *hermesDm = m->getDataSet( "HERMES_Multi_DEU_PIm", "xmin", "xmax" );

  // set style
  hermesPp->style( 21, 1, kPink  + 1 );
  hermesDp->style( 21, 1, kPink  - 3 );
  hermesPm->style( 21, 1, kGreen + 1 );
  hermesDm->style( 21, 1, kGreen + 3 );

  // range of x to be shown
  double xmin = 0.02;
  double xmax = 0.7;
  
  // ------------------------------------------------------------- //
  TCanvas *c2 = new TCanvas( "SIDIS", "", 800, 1200 );
  c2->Divide( 1, 2 );
  
  TVirtualPad *pad1 = c2->cd( 1 );
  pad1->SetLogx();
  pad1->SetLogy();
  
  TH1 *frame2 = pad1->DrawFrame( xmin, 6E-3, xmax, 2E+1 );
  frame2->GetXaxis()->SetTitle( "x_{Bj}" );
  frame2->GetYaxis()->SetTitle( "multiplicity" );

  TLegend *leg2 = new TLegend( 0.6, 0.85, 0.99, 0.99 );
  leg2->AddEntry( hermesPp, "HERMES p-#pi^{+}", "P" );
  leg2->AddEntry( hermesDp, "HERMES d-pi^{+}", "P" );
  leg2->Draw();
  
  hermesPp->draw();
  hermesDp->draw();

  if( hermes_p_pip ) hermesMulti( 0, 1, kOrange + 1 );
  if( hermes_d_pip ) hermesMulti( 2, 1, kOrange - 1 );
  
  // ------------------------------------------------------------- //
  TVirtualPad *pad2 = c2->cd( 2 );
  
  pad2->SetLogx();
  pad2->SetLogy();

  TH1 *frame3 = pad2->DrawFrame( xmin, 6E-3, xmax, 2E+1 );
  frame3->GetXaxis()->SetTitle( "x_{Bj}" );
  frame3->GetYaxis()->SetTitle( "multiplicity" );

  TLegend *leg3 = new TLegend( 0.6, 0.85, 0.99, 0.99 );
  leg3->AddEntry( hermesPm, "HERMES p-#pi^{-}", "P" );
  leg3->AddEntry( hermesDm, "HERMES d-#pi^{-}", "P" );
  leg3->Draw();

  hermesPm->draw();
  hermesDm->draw();
  
  if( hermes_p_pim ) hermesMulti( 0, -1, kTeal + 1 );
  if( hermes_d_pim ) hermesMulti( 2, -1, kTeal - 3 );

  c2->Print( "SIDIS_PI.png" );

}


int pion()
{
  drawFragXsec();
  drawHermesMulti();
  return 0;
}
