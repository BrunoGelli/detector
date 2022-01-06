

	#include "TH1D.h"
	#include <time.h>
	#include "TVirtualFFT.h"
	#include "TRandom.h"
	#include "TF1.h"
	#include "TCanvas.h"
	#include "TStyle.h"
	#include "TMath.h"
	#include <math.h> 
	#include <stdlib.h>
	#include <stdio.h>


int iterecao = 25;


void plot()
{
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetPadGridX(1);
    gStyle->SetPadGridY(1);
	gStyle->SetStatY(0.9);
	gStyle->SetStatX(0.95);
	gStyle->SetStatW(0.3);
	gStyle->SetStatH(0.2);
	gStyle->SetStatBorderSize(3);

	FILE* ptr = fopen("FiltroOutPut.txt","r");

	if (ptr==NULL)
	{
		printf("no such file.");
		return;
	}

	double exp[5000], item[50000], eff[50000], effM[5000];
	double incerteza[5000];
	int i=0, k=0;
	double media=0;
	double expessura = iterecao;

	while (fscanf(ptr,"%lf\n",&eff[i])!= EOF)
	{
		i++;
		item[i]=i;
	}
	// cout << i << endl;

	for (int j = 0; j < i; ++j)
	{
		if(j%13 == 0 && j>0)
		{

			// cout << "entrada ->" << j << endl;

			for (int l = j-1; l >= j-12; --l)
			{
				media = media + eff[l];
				// cout << "        " << l << "   " << eff[l] << endl;
			}

			media = media/12;
			// cout << "        " << media << endl;
			effM[k] = media;
			exp[k] = expessura + iterecao*k;
			media = 0;
			k++;

		}
	}

	// for (int i = 0; i < k; ++i)
	// {
	// 	incerteza[i] = TMath::Sqrt(effM[i]);
	// }

	cout << k << endl;

	fclose(ptr);

	TGraph* Gconv1 = new TGraph(k,exp,effM);

	Gconv1->SetTitle("");
	Gconv1->GetXaxis()->SetTitle("Thickness (#mum)");
	Gconv1->GetYaxis()->SetTitle("Number of electrons + positrons (%)");
	// Gconv1->SetLineThickness(2);

	Gconv1->Draw("AL");

	// Gconv1->SetMarkerStyle(7);

	// auto mg2 = new TMultiGraph;

	// mg2->SetTitle("X-ARAPUCA Efficiency;Bar thickness (cm);Efficiency");
	// mg2->Add(Gconv1,"L");

	// mg2-> Draw("A");

	return;

} 
