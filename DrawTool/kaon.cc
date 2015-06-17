#include <string>

#include <TLegend.h>
#include <TColorWheel.h>

TGraph* fragXsec_T( const double& s, const double& scale ){
  
  Manager *m = Manager::instance();
  
  TGraph *g = m->getFragXsec_Z( 2, // KAp only
				40, 0.02, 0.95, false,
				s * s, 
				scale ); // factor 2 should be applied 
  g->SetLineStyle( 2 );
  g->SetLineWidth( 2 );
  g->SetLineColor( kOrange + 5 );
  
  return g;
}  


int kaon()
{

  bool sia_kapm = true;

  bool hermes_p_kap = true;
  bool hermes_p_kam = true;
  bool hermes_d_kap = false;
  bool hermes_d_kam = false;

  Manager *m = Manager::instance();
  
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
  TLegend *legb = new TLegend( 0.12, 0.12, 0.45, 0.3 );

  RExpData *sld = m->getDataSet( "SLD__inclusive_91.2_1999_KApm", "zmin", "zmax" );
  sld->rescale( 1000 );
  leg->AddEntry( sld, "SLD 91.2 GeV (x1000)", "P" );
  
  RExpData *aleph = m->getDataSet( "ALEPH__91.2_1995_KApm", "zmin", "zmax" );
  aleph->rescale( 100 );
  leg->AddEntry( aleph, "ALEPH 91.2 GeV (x100)", "P" );

  RExpData *opal = m->getDataSet( "OPAL_91.2_1994_KApm", "zmin", "zmax" );
  opal->rescale( 10 );
  leg->AddEntry( opal, "OPAL 91.2 GeV (x10)", "P" );

  RExpData *delphi = m->getDataSet( "DELPHI_inclusive_91.2_1998_KApm", "zmin", "zmax" );
  leg->AddEntry( delphi, "OPAL 91.2 GeV", "P" );

  RExpData *tasso44 = m->getDataSet( "TASSO_44_1989_KApm", "zmin", "zmax" );
  tasso44->rescale( 0.1 );
  legb->AddEntry( tasso44, "TASSO 44 GeV (/10)", "P" );

  RExpData *tasso34 = m->getDataSet( "TASSO_34_1989_KApm", "zmin", "zmax" );
  tasso34->rescale( 0.01 );
  legb->AddEntry( tasso34, "TASSO 34 GeV (/100)", "P" );
  
  RExpData *tpc = m->getDataSet( "TPC_29_1988_KApm", "zmin", "zmax" );
  tpc->rescale( 0.001 );
  legb->AddEntry( tpc, "TPC 29 GeV (x10^{-3})", "P" );
  
  RExpData *belle = m->getDataSet( "Belle_2013_KApm", "zmin", "zmax" );
  belle->rescale( 0.0001 );
  legb->AddEntry( belle, "Belle 10.52 GeV (x10^{-4})", "P" );

  sld->style(    21, 1, kBlue     );
  aleph->style(  21, 1, kBlue - 4 );
  opal->style(   21, 1, kBlue - 7 );
  delphi->style( 21, 1, kBlue - 9 );

  tasso44->style( 21, 1, kGreen + 3 );
  tasso34->style( 21, 1, kGreen - 2 );
  tpc->style(     21, 1, kGreen - 6 );

  belle->style(   21, 1, kRed   - 3 );
  
  leg->Draw();
  legb->Draw();

  belle->Draw( "P" );
  belle->fit_->Draw( "*" );

  tasso34->Draw( "P" );
  tasso34->fit_->Draw( "*" );

  tasso44->Draw( "P" );
  tasso44->fit_->Draw( "*" );

  tpc->Draw( "P" );
  tpc->fit_->Draw( "*" );

  sld->Draw( "P" );
  sld->fit_->Draw( "*" );
  
  aleph->Draw( "P" );
  aleph->fit_->Draw( "*" );
 
  opal->Draw( "P" );
  opal->fit_->Draw( "*" );

  delphi->Draw( "P" );
  delphi->fit_->Draw( "*" );

  if( sia_kapm ){
    
    TGraph *g1 = fragXsec_T( 91.2, 2.0 * 1000 );
    if( g1 != NULL ){
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
  }
  c->Print( "SIA_KA.png" );

  // ------------------------------------------------------------- //
  // ------------------------------------------------------------- //
  // ------------------------------------------------------------- //

  
  TCanvas *c2 = new TCanvas( "SIDIS", "", 800, 1200 );
  c2->Divide( 1, 2, 0.0 );
  
  TVirtualPad *pad = c2->cd( 1 );
  
  pad->SetLogx();
  pad->SetLogy();
  TH1 *frame2 = pad->DrawFrame( 0.8, 8E-3, 20.0, 1.5E+0 );
  frame2->GetXaxis()->SetTitle( "(GeV/c)^2" );
  frame2->GetYaxis()->SetTitle( "multiplicity" );
  
  TLegend *leg2 = new TLegend( 0.6, 0.85, 0.99, 0.99 );

  RExpData *hermesPp = m->getDataSet( "HERMES_Multi_PRO_KAp",
				     "Q^2 min", "Q^2 max" );
  leg2->AddEntry( hermesPp, "HERMES e p-K^{+}", "P" );
  
  RExpData *hermesDp = m->getDataSet( "HERMES_Multi_DEU_KAp",
				     "Q^2 min", "Q^2 max" );
  
  leg2->AddEntry( hermesDp, "HERMES d-K^{+}", "P" );

  leg2->Draw();

  hermesPp->Draw("P");
  hermesPp->fit_->Draw("*");

  hermesDp->Draw("P");
  hermesDp->fit_->Draw("*");
  
  vector< double > zbin( 6 );
  zbin[ 0 ] = 0.1;
  zbin[ 1 ] = 0.2;
  zbin[ 2 ] = 0.3;
  zbin[ 3 ] = 0.4;
  zbin[ 4 ] = 0.6;
  zbin[ 5 ] = 0.8;
  
  if( hermes_p_kap ){
    
    for( int i = 0; i < 5; i++ ){
      TGraph *pp1 = m->getMulti_Q2( 0, 2, // p, ka+
				    10, 1.0, 20.0, true,
				    56.2, 0.023, 0.6, 
				    zbin[ i ], zbin[ i + 1 ] ); // x - z
      if( pp1 == NULL ) break;
      
      pp1->SetLineColor( kOrange + 1 );
      pp1->SetLineStyle( 2 );
      pp1->Draw( "l" );
    }

  }

  if( hermes_d_kap ){

    for( int i = 0; i < 5; i++ ){
      TGraph *pp1 = m->getMulti_Q2( 2, 2, // p, ka+
				    10, 1.0, 20.0, true,
				    56.2, 0.023, 0.6, 
				    zbin[ i ], zbin[ i + 1 ] ); // x - z
      
      if( pp1 == NULL ) break;

      pp1->SetLineColor( kOrange - 3 );
      pp1->SetLineStyle( 2 );
      pp1->Draw( "l" );
    }

  }


  // ------------------------------------------------------------- //
  TVirtualPad *pad2 = c2->cd( 2 );
  
  pad2->SetLogx();
  pad2->SetLogy();
  TH1 *frame3 = pad2->DrawFrame( 0.8, 3E-4, 20.0, 3E+0 );
  frame3->GetXaxis()->SetTitle( "(GeV/c)^2" );
  frame3->GetYaxis()->SetTitle( "multiplicity" );
  
  TLegend *leg3 = new TLegend( 0.6, 0.85, 0.99, 0.99 );

  RExpData *hermesPm = m->getDataSet( "HERMES_Multi_PRO_KAm",
				     "Q^2 min", "Q^2 max" );
  leg3->AddEntry( hermesPm, "HERMES p-K^{-}", "P" );
  
  RExpData *hermesDm = m->getDataSet( "HERMES_Multi_DEU_KAm",
				     "Q^2 min", "Q^2 max" );
  
  leg3->AddEntry( hermesDm, "HERMES d-K^{-}", "P" );

  leg3->Draw();
  
  hermesPp->style( 21, 1, kPink  + 1 );
  hermesDp->style( 21, 1, kPink  - 3 );

  hermesPm->style( 21, 1, kGreen + 1 );
  hermesDm->style( 21, 1, kGreen + 3 );


  hermesPm->Draw("P");
  hermesPm->fit_->Draw("*");

  hermesDm->Draw("P");
  hermesDm->fit_->Draw("*");
  
  if( hermes_p_kam ){
    
    for( int i = 0; i < 5; i++ ){
      TGraph *pp1 = m->getMulti_Q2( 0, -2, // p, ka-
				    10, 1.0, 20.0, true,
				    56.2, 0.023, 0.6, 
				    zbin[ i ], zbin[ i + 1 ] ); // x - z
      
      if( pp1 == NULL ) break;
      
      pp1->SetLineColor( kTeal + 1 );
      pp1->SetLineStyle( 2 );
      pp1->Draw( "l" );
    }
    
  }
  
  if( hermes_d_kam ){
    
    for( int i = 0; i < 5; i++ ){
      TGraph *pp1 = m->getMulti_Q2( 2, -2, // p, ka-
				    10, 1.0, 20.0, true,
				    56.2, 0.023, 0.6, 
				    zbin[ i ], zbin[ i + 1 ] ); // x - z
      
      if( pp1 == NULL ) break;

      pp1->SetLineColor( kTeal + 3 );
      pp1->SetLineStyle( 2 );
      pp1->Draw( "l" );
    }

  }

  c2->Print( "SIDIS_KA.png" );
  

  return 0;
}
