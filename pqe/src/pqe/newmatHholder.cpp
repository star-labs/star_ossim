//$$ hholder.cpp                                   QR decomposition

// Copyright (C) 1991,2,3,4: R B Davies

#define WANT_MATH
//#define WANT_STREAM

#include <cmath>
#include <pqe/newmatInclude.h>

#include <pqe/newmatap.h>

namespace pqe
{

#ifdef DO_REPORT
#define REPORT { static ExeCounter ExeCount(__LINE__,16); ++ExeCount; }
#else
#define REPORT {}
#endif


/*************************** QR decompositions ***************************/

inline Real square(Real x) { return x*x; }

void QRZT(Matrix& X, LowerTriangularMatrix& L)
{
   REPORT
	 Tracer et("QRZT(1)");
   int n = X.Ncols(); int s = X.Nrows(); L.ReSize(s);
   Real* xi = X.Store(); int k;
   for (int i=0; i<s; i++)
   {
      Real sum = 0.0;
      Real* xi0=xi; k=n; while(k--) { sum += square(*xi++); }
      sum = std::sqrt(sum);
      if (sum == 0.0)
      {
         REPORT
         k=n; while(k--) { *xi0++ = 0.0; }
         for (int j=i; j<s; j++) L.element(j,i) = 0.0;
      }
      else
      {
         L.element(i,i) = sum;
         Real* xj0=xi0; k=n; while(k--) { *xj0++ /= sum; }
         for (int j=i+1; j<s; j++)
         {
            sum=0.0;
            xi=xi0; Real* xj=xj0; k=n; while(k--) { sum += *xi++ * *xj++; }
            xi=xi0; k=n; while(k--) { *xj0++ -= sum * *xi++; }
            L.element(j,i) = sum;
         }
      }
   }
}

void QRZT(const Matrix& X, Matrix& Y, Matrix& M)
{
   REPORT
   Tracer et("QRZT(2)");
   int n = X.Ncols(); int s = X.Nrows(); int t = Y.Nrows();
   if (Y.Ncols() != n)
      { Throw(ProgramException("Unequal row lengths",X,Y)); }
   M.ReSize(t,s);
   Real* xi = X.Store(); int k;
   for (int i=0; i<s; i++)
   {
      Real* xj0 = Y.Store(); Real* xi0 = xi;
      for (int j=0; j<t; j++)
      {
         Real sum=0.0;
         xi=xi0; Real* xj=xj0; k=n; while(k--) { sum += *xi++ * *xj++; }
         xi=xi0; k=n; while(k--) { *xj0++ -= sum * *xi++; }
         M.element(j,i) = sum;
      }
   }
}

/*
void QRZ(Matrix& X, UpperTriangularMatrix& U)
{
	Tracer et("QRZ(1)");
	int n = X.Nrows(); int s = X.Ncols(); U.ReSize(s);
	Real* xi0 = X.Store(); int k;
	for (int i=0; i<s; i++)
	{
		Real sum = 0.0;
		Real* xi = xi0; k=n; while(k--) { sum += square(*xi); xi+=s; }
		sum = std::sqrt(sum);
		U.element(i,i) = sum;
		if (sum==0.0) Throw(SingularException(U));
		Real* xj0=xi0; k=n; while(k--) { *xj0 /= sum; xj0+=s; }
		xj0 = xi0;
		for (int j=i+1; j<s; j++)
		{
			sum=0.0;
			xi=xi0; k=n; xj0++; Real* xj=xj0;
			while(k--) { sum += *xi * *xj; xi+=s; xj+=s; }
			xi=xi0; k=n; xj=xj0;
			while(k--) { *xj -= sum * *xi; xj+=s; xi+=s; }
			U.element(i,j) = sum;
		}
		xi0++;
	}
}
*/

void QRZ(Matrix& X, UpperTriangularMatrix& U)
{
   REPORT
   Tracer et("QRZ(1)");
   int n = X.Nrows(); int s = X.Ncols(); U.ReSize(s); U = 0.0;
   Real* xi0 = X.Store(); Real* u0 = U.Store(); Real* u;
   int j, k; int J = s; int i = s;
   while (i--)
   {
      Real* xj0 = xi0; Real* xi = xi0; k = n;
      if (k) for (;;)
      {
         u = u0; Real Xi = *xi; Real* xj = xj0;
         j = J; while(j--) *u++ += Xi * *xj++;
         if (!(--k)) break;
         xi += s; xj0 += s;
      }

      Real sum = std::sqrt(*u0); *u0 = sum; u = u0+1;
      if (sum == 0.0)
      {
         REPORT
         j = J - 1; while(j--) *u++ = 0.0;

         xj0 = xi0++; k = n;
         if (k) for (;;)
         {
            *xj0 = 0.0;
            if (!(--k)) break;
	          xj0 += s;
         }
         u0 += J--;
      }
      else
      {
         int J1 = J-1; j = J1; while(j--) *u++ /= sum;

         xj0 = xi0; xi = xi0++; k = n;
         if (k) for (;;)
         {
            u = u0+1; Real Xi = *xi; Real* xj = xj0;
            Xi /= sum; *xj++ = Xi;
            j = J1; while(j--) *xj++ -= *u++ * Xi;
            if (!(--k)) break;
	          xi += s; xj0 += s;
         }
         u0 += J--;
      }
   }
}

void QRZ(const Matrix& X, Matrix& Y, Matrix& M)
{
   REPORT
   Tracer et("QRZ(2)");
   int n = X.Nrows(); int s = X.Ncols(); int t = Y.Ncols();
   if (Y.Nrows() != n)
      { Throw(ProgramException("Unequal column lengths",X,Y)); }
   M.ReSize(s,t); M = 0;Real* m0 = M.Store(); Real* m;
   Real* xi0 = X.Store();
   int j, k; int i = s;
   while (i--)
   {
      Real* xj0 = Y.Store(); Real* xi = xi0; k = n;
      if (k) for (;;)
      {
         m = m0; Real Xi = *xi; Real* xj = xj0;
         j = t; while(j--) *m++ += Xi * *xj++;
         if (!(--k)) break;
         xi += s; xj0 += t;
      }

      xj0 = Y.Store(); xi = xi0++; k = n;
      if (k) for (;;)
      {
         m = m0; Real Xi = *xi; Real* xj = xj0;
         j = t; while(j--) *xj++ -= *m++ * Xi;
         if (!(--k)) break;
         xi += s; xj0 += t;
      }
      m0 += t;
   }
}

/*

void QRZ(const Matrix& X, Matrix& Y, Matrix& M)
{
	Tracer et("QRZ(2)");
	int n = X.Nrows(); int s = X.Ncols(); int t = Y.Ncols();
	if (Y.Nrows() != n)
	{ Throw(ProgramException("Unequal column lengths",X,Y)); }
	M.ReSize(s,t);
	Real* xi0 = X.Store(); int k;
	for (int i=0; i<s; i++)
	{
		Real* xj0 = Y.Store();
		for (int j=0; j<t; j++)
		{
			Real sum=0.0;
			Real* xi=xi0; Real* xj=xj0; k=n;
			while(k--) { sum += *xi * *xj; xi+=s; xj+=t; }
			xi=xi0; k=n; xj=xj0++;
			while(k--) { *xj -= sum * *xi; xj+=t; xi+=s; }
			M.element(i,j) = sum;
		}
		xi0++;
	}
}
*/

void UpdateQRZT(Matrix& X, LowerTriangularMatrix& L)
{
   REPORT
	 Tracer et("UpdateQRZT");
   int n = X.Ncols(); int s = X.Nrows();
   if (s != L.Nrows())
      Throw(ProgramException("Incompatible dimensions",X,L)); 
   Real* xi = X.Store(); int k;
   for (int i=0; i<s; i++)
   {
      Real r = L.element(i,i); 
      Real sum = 0.0;
      Real* xi0=xi; k=n; while(k--) { sum += square(*xi++); }
      sum = std::sqrt(sum + square(r));
      if (sum == 0.0)
      {
         REPORT
         k=n; while(k--) { *xi0++ = 0.0; }
         for (int j=i; j<s; j++) L.element(j,i) = 0.0;
      }
      else
      {
         Real frs = std::fabs(r) + sum;
         Real a0 = std::sqrt(frs / sum); Real alpha = a0 / frs;
         if (r <= 0) { REPORT L.element(i,i) = sum; alpha = -alpha; }
         else { REPORT L.element(i,i) = -sum; }
         Real* xj0=xi0; k=n; while(k--) { *xj0++ *= alpha; }
         for (int j=i+1; j<s; j++)
         {
            sum = 0.0;
            xi=xi0; Real* xj=xj0; k=n; while(k--) { sum += *xi++ * *xj++; }
            sum += a0 * L.element(j,i);
            xi=xi0; k=n; while(k--) { *xj0++ -= sum * *xi++; }
            L.element(j,i) -= sum * a0;
         }
      }
   }
}

void UpdateQRZ(Matrix& X, UpperTriangularMatrix& U)
{
   REPORT
   Tracer et("UpdateQRZ");
   int n = X.Nrows(); int s = X.Ncols();
   if (s != U.Ncols())
      Throw(ProgramException("Incompatible dimensions",X,U)); 
   Real* xi0 = X.Store(); Real* u0 = U.Store(); Real* u;
   RowVector V(s); Real* v0 = V.Store(); Real* v; V = 0.0;
   int j, k; int J = s; int i = s;
   while (i--)
   {
      Real* xj0 = xi0; Real* xi = xi0; k = n;
      if (k) for (;;)
      {
         v = v0; Real Xi = *xi; Real* xj = xj0;
         j = J; while(j--) *v++ += Xi * *xj++;
         if (!(--k)) break;
         xi += s; xj0 += s;
      }

      Real r = *u0;
      Real sum = std::sqrt(*v0 + square(r));
      
      if (sum == 0.0)
      {
         REPORT
         u = u0; v = v0;
         j = J; while(j--) { *u++ = 0.0; *v++ = 0.0; }
         xj0 = xi0++; k = n;
         if (k) for (;;)
         {
            *xj0 = 0.0;
            if (!(--k)) break;
	          xj0 += s;
         }
         u0 += J--;
      }
      else
      {
         Real frs = std::fabs(r) + sum;
         Real a0 = std::sqrt(frs / sum); Real alpha = a0 / frs;
         if (r <= 0) { REPORT alpha = -alpha; *u0 = sum; }
         else { REPORT *u0 = -sum; }
      
         j = J - 1; v = v0 + 1; u = u0 + 1;     
         while (j--)
            { *v = a0 * *u + alpha * *v; *u -= a0 * *v; ++v; ++u; }

         xj0 = xi0; xi = xi0++; k = n;
         if (k) for (;;)
         {
            v = v0 + 1; Real Xi = *xi; Real* xj = xj0;
            Xi *= alpha; *xj++ = Xi;
            j = J - 1; while(j--) *xj++ -= *v++ * Xi;
            if (!(--k)) break;
	          xi += s; xj0 += s;
         }
         
         j = J; v = v0;
         while (j--) *v++ = 0.0;
         
         u0 += J--;
      }
   }
}

}
