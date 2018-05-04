
#include <vector>
#include <omp.h> 
using namespace std;
struct Detector
{
	int i;
	int j;
};
struct Cell
{
	double dz;
	double ez;
	double bz;
	double hz;
	double ga;
	double gb;
	double gc;
	double iz;
	double sz;
	double ihz;
	double shz;
	double gha;
	double ghb;
	double ghc;
	double nrj;
	double hx;
	double hy;
	double ex;
	double ey;
};
class fdtd2d
{
public:
	int nwdth; int nhght;
	vector<Detector> dats;
	//положение источников
	double * x1; double* x2; double* y1; double* y2;
	fdtd2d(double width,int nwidth, double height,int nheight, int npml, double *x1, double *x2, double *y1, double *y2 )
	{
		this->x1 = x1; this->x2 = x2; this->y1 = y1; this->y2 = y2;
		pi=3.14159;
		dats.resize(360);
		for(int i =0;i<360;i++)
		{
			dats[i].i=nwidth/2+100*cos(2*pi*(double)i/(360.));
			dats[i].j=nheight/2+100*sin(2*pi*(double)i/(360.));
		}
		ic=nwidth/2;
		jc=nheight/2;
		nwdth=nwidth;
		nhght=nheight;
		dt =ddx/6e10; /* Time steps */
		epsz = 8.8e-12;
		
		ddx=width/(double)nwidth;
		ddy=height/(double)nheight;
		yee.resize(nwidth);
		ihx.resize(nwidth);		ihy.resize(nwidth);
		gi2.resize(nwidth);		gi3.resize(nwidth);
		gj2.resize(nheight);	gj3.resize(nwidth);
		fi1.resize(nwidth);		fi2.resize(nwidth);		fi3.resize(nheight);
		fj1.resize(nheight);	fj2.resize(nheight);	fj3.resize(nheight);
		for(int i = 0;i<nwidth;i++)
		{
			yee[i].resize(nheight);
			ihx[i].resize(nheight);		
			ihy[i].resize(nheight);
		}
		for (int j = 0; j < nheight; j++ )
		{
			for (int i = 0; i < nwidth; i++ )
			{
				yee[i][j].dz=0.0;
				yee[i][j].hx=0.0;
				yee[i][j].hy=0.0;
				ihx[i][j]=0.0;
				ihy[i][j]=0.0;
				yee[i][j].ga=1.0;
			}
		}
		//Calculate the PML parameters 
		for (int i=0;i<nwidth; i++) 
		{
			gi2[i] = 1.0;
			gi3[i] = 1.0;
			fi1[i] = 0.0;
			fi2[i] = 1.0;
			fi3[i] = 1.0;
		}
		for (int j=0;j< nheight; j++)
		{
			gj2[j] = 1.0;
			gj3[j] = 1.0;
			fj1[j] = 0.0;
			fj2[j] = 1.0;
			fj3[j] = 1.0;
		}
		for (int i=0;i<= npml; i++) 
		{
			xnum = npml - i;
			xd = npml;
			xxn = xnum/xd;
			xn = 0.25*pow(xxn,3.0);
			gi2[i] = 1.0/(1.0+xn);
			gi2[nwidth-1-i] = 1.0/(1.0+xn);
			gi3[i] = (1.0 - xn)/(1.0 + xn);
			gi3[nwidth-i-1] = (1.0 - xn)/(1.0 + xn);
			xxn = (xnum-.5)/xd;
			xn = 0.25*pow(xxn,3.0);
			fi1[i] = xn;
			fi1[nwidth-2-i] = xn;
			fi2[i] = 1.0/(1.0+xn);
			fi2[nwidth-2-i] = 1.0/(1.0+xn);
			fi3[i] = (1.0 - xn)/(1.0 + xn);
			fi3[nwidth-2-i] = (1.0 - xn)/(1.0 + xn);
		}
		for (int j=0;j<= npml; j++) 
		{
			xnum = npml - j;
			xd = npml;
			xxn = xnum/xd;
			xn = 0.25*pow(xxn,3.0);

			gj2[j] = 1.0/(1.0+xn);
			gj2[nheight-1-j] = 1.0/(1.0+xn);
			gj3[j] = (1.0 - xn)/(1.0 + xn);
			gj3[nheight-j-1] = (1.0 - xn)/(1.0 + xn);
			xxn = (xnum-.5)/xd;
			xn = 0.25*pow(xxn,3.0);
			fj1[j] = xn;
			fj1[nheight-2-j] = xn;
			fj2[j] = 1.0/(1.0+xn);
			fj2[nheight-2-j] = 1.0/(1.0+xn);
			fj3[j] = (1.0 - xn)/(1.0 + xn);
			fj3[nheight-2-j] = (1.0 - xn)/(1.0 + xn);
		}
		istart = 0;
		jstart = 0;
		ifin = 0;
		jfin = 0;
		epsilon = 2;
		sigma = 0.01;
		chi1 = 1;
	
		tau = 0.001;
		del_exp = exp(-dt/tau); 
		tau = 1.e-6*tau;

		for (int j=jstart; j <= jfin; j++ ) 
		{
			for (int i=istart; i <= ifin; i++ ) 
			{
				yee[i][j].ga =1./(epsilon + sigma*dt/epsz + chi1*dt/tau);
				yee[i][j].gb = sigma*dt/epsz ;
				yee[i][j].gc = chi1*dt/tau ;
			}
		}
		t0 = 0.0;
		spread = 40.0;
		T = 0;
	};
	~fdtd2d()
	{
	};
	double ddx;
	double ddy;
	vector<vector<Cell>> yee;
	vector<vector<double>> ihx;	vector<vector<double>> ihy;
	vector<double> gi2;	vector<double> gi3;
	vector<double> gj2;	vector<double> gj3;
	vector<double> fi1;	vector<double> fi2;	vector<double> fi3;
	vector<double> fj1;	vector<double> fj2; vector<double> fj3;
	int l,ic,jc,nsteps,npml,istart,jstart,ifin,jfin;
	double dt,T,epsz,pi,epsilon,sigma,eaf;
	double xn,xxn,xnum,xd,curl_e;
	double t0,spread,pulse;
	double tau,chi1,del_exp;
	void GenNextStep(double ddt)
	{
		T = T + ddt;
		// ---- Start of the Main FDTD loop ---- 
		//Dz

		for(int j=1; j < nhght; j++ ) 
		{
			for(int i=1; i < nwdth; i++ ) 
			{
				yee[i][j].dz = gi3[i]*gj3[j]*yee[i][j].dz+gi2[i]*gj2[j]*ddt*(yee[i][j].hy-yee[i-1][j].hy-yee[i][j].hx+yee[i][j-1].hx);
			}
		}

		/*один маленький писк*/
		/*pulse=exp(-(T-50)*(T-50)/50);
		yee[ic][jc].dz = pulse;*/

		//// синусный источечничек
		//for(int i=0;i<101;i++)
		//{
		//	pulse = sin(2*pi*15500*T+i*pi/100);
		//	yee[ic-50+i][jc+1].dz = pulse;
		//	/*pulse = sin(2*pi*15500*T+pi+i*pi/100);
		//	yee[ic-50+i][jc].dz = pulse;*/
		//}
		/*pulse=1./(1.+exp(-0.1*(T-50)))*sin(2*pi*25000*T-3.*pi/4.);//exp(-(T-100)*(T-100)/50);
		yee[ic][jc].dz = pulse;*/

		/*точечный источнечичнек*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T);
		//yee[0.5*nwdth][0.5*nhght].dz += pulse;
		/*2 точечных источнечичнек*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T);
		pulse = 5*sin(2 * 0.03 *  pi * T);
		yee[*x1*nwdth][*y1*nhght].dz += pulse;
		//pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T);
		yee[*x2*nwdth][*y2*nhght].dz += pulse;
		/*4 точечных источнечичнек*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T);
		//yee[0.45*nwdth][0.45*nhght].dz += pulse;
		//pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T);
		//yee[0.45*nwdth][0.55*nhght].dz += pulse;
		//pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T );
		//yee[0.55*nwdth][0.55*nhght].dz += pulse;
		//pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T);
		//yee[0.55*nwdth][0.45*nhght].dz += pulse;		
		/*точечный источнечичнек*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*pulse = 1. / (1. + exp(-0.1*(T - 50)))*sin(2 * pi * 25000 * T);
		yee[0.5*nwdth][0.33*nhght].dz += pulse;
		for (int i = 0.33*nwdth; i < 0.66*nwdth; i++)
		{
			yee[i][0.66*nhght].dz = 0;
		}*/

		/*а вот так типа металлическая антенна отражатель походу задается**/
		/*for(int i = 30;i<150;i++)
		{
			yee[nhght*0.5 + 100 * cos(2 * pi*(double)i / (360.))][nwdth*0.5 + 100 * sin(2 * pi*(double)i / (360.))].dz = 0;
			//yee[ic-25][jc+i-100].dz = 0;
		}*/
		
		//Ez

		for (int j=1; j<nhght-1;j++) 
		{
			for (int i=1; i < nwdth-1; i++ ) 
			{
				yee[i][j].ez = yee[i][j].ga*yee[i][j].dz;
			} 
		}
		// Calculate the Hx field

		for (int j=0; j < nhght-1; j++ ) 
		{
			for (int i=0; i < nwdth; i++ ) 
			{
				curl_e=yee[i][j].ez-yee[i][j+1].ez;
				ihx[i][j]=ihx[i][j]+fi1[i]*curl_e;
				yee[i][j].hx = fj3[j]*yee[i][j].hx+fj2[j]*ddt*(curl_e+ihx[i][j]);
			} 
		}
		//Calculate the Hy field

		for (int j=0; j <= nhght-1; j++ ) 
		{
			for (int i=0; i < nwdth-1; i++ ) 
			{
				curl_e = yee[i+1][j].ez-yee[i][j].ez;
				ihy[i][j] = ihy[i][j] + fj1[j]*curl_e;
				yee[i][j].hy = fi3[i]*yee[i][j].hy+fi2[i]*ddt*(curl_e + ihy[i][j]);
			} 
		} 

	};
};
