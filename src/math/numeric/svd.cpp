/**
 * @file svd.cpp Implementation of singular value decomposition
 */

#include "svd.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <limits>

namespace math {
namespace numeric {

void svbksb(long double** U, long double W[], long double** V, int m, int n,
            long double B[], long double X[]) {
  int jj;
  int j;
  int i;
  long double s;
  long double* tmp = new long double[n];
  // Calculate U^T B
  for (j = 0; j < n; j++) {
    s = 0;
    if (W[j]) {
      for (i = 0; i < m; i++) {
        s += U[i][j] * B[i];
      }
      s /= W[j];
    }
    tmp[j] = s;
  }
  // Matrix multiply by V to get an answer
  for (j = 0; j < n; j++) {
    s = 0;
    for (jj = 0; jj < n; jj++) {
      s += V[j][jj] * tmp[jj];
    }
    X[j] = s;
  }
  delete[] tmp;
}

void solveViaSVD(long double** A, long double X[], long double B[], int m,
                 int n) {
  long double** U = new long double*[m];
  int i;
  for (i = 0; i < m; i++) {
    U[i] = new long double[n];
    for (int j = 0; j < n; j++) {
      U[i][j] = A[i][j];
    }
  }
  long double* W = new long double[n];
  long double** V = new long double*[n];
  for (i = 0; i < n; i++) {
    V[i] = new long double[n];
  }
  svdcmp(U, m, n, W, V);
  long double wmax = 0.0;  // Will be the maximum singular value obtained.
  for (i = 0; i < n; i++) {
    if (W[i] > wmax) {
      wmax = W[i];
    }
  }
  // This is where we set the threshold for singular values
  // allowed to be nonzero. The constant is typical, but not universal.
  // You have to experiment with your own application.
  long double wmin =
      wmax * std::numeric_limits<long double>::epsilon();  // 1.0e-6;
  for (i = 0; i < n; i++) {
    if (W[i] < wmin) {
      W[i] = 0.0;
    }
  }
  svbksb(U, W, V, m, n, B, X);  // Back substitute
  for (i = 0; i < m; i++) {
    delete[] U[i];
  }
  delete[] U;
  for (i = 0; i < n; i++) {
    delete[] V[i];
  }
  delete[] V;
  delete[] W;
}

//#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
inline long double sign(long double a, long double b) {
  return (b) >= 0.0 ? std::abs(a) : -std::abs(a);
}
inline long double pythag(long double a, long double b) {
  long double absa, absb;
  absa = std::abs(a);
  absb = std::abs(b);
  if (absa > absb) {
    long double val = absb / absa;
    val *= val;
    return absa * std::sqrt(1.0 + val);
  } else {
    if (absb == 0.0) {
      return 0.0;
    } else {
      long double val = absa / absb;
      val *= val;
      return absb * std::sqrt(1.0 + val);
    }
  }
}

// Given a matrix a[0..m-1][0..n-1], this routine computes its singular value
// decomposition, A = UxWxV^T. The matrix U replaces a on output. The
// diagonal matrix of singular values W is output as a vector w[0..n-1].
// The matrix V (not the transpose V^T ) is output as nxn v[0..n-1][0..n-1].

bool svdcmp(long double** a, int m, int n, long double w[], long double** v) {
  const long max_iter = 30;
  const long double eps = std::numeric_limits<long double>::epsilon();
  long nm = 0, l = 0;
  bool flag;
  long double c, f, h, s, x, y, z;
  long double g = 0.0;
  long double scale = 0.0;
  long double anorm = 0.0;
  long double* rv1 = new long double[n];

  for (long i = 0; i < n; ++i) {
    l = i + 1;
    rv1[i] = scale * g;
    g = s = scale = 0.0;
    if (i < m) {
      for (long k = i; k < m; ++k) {
        scale += std::abs(a[k][i]);
      }
      if (scale) {
        for (long k = i; k < m; ++k) {
          a[k][i] /= scale;
          s += a[k][i] * a[k][i];
        }
        f = a[i][i];
        g = -sign(std::sqrt(s), f);
        h = f * g - s;
        a[i][i] = f - g;
        for (long j = l; j < n; ++j) {
          s = 0.0;
          for (long k = i; k < m; ++k) {
            s += a[k][i] * a[k][j];
          }
          f = s / h;

          for (long k = i; k < m; ++k) a[k][j] += f * a[k][i];
        }
        for (long k = i; k < m; ++k) a[k][i] *= scale;
      }
    }

    w[i] = scale * g;

    g = s = scale = 0.0;

    if (i < m && i < n - 1) {
      for (long k = l; k < n; ++k) scale += std::abs(a[i][k]);

      if (scale) {
        for (long k = l; k < n; ++k) {
          a[i][k] /= scale;
          s += a[i][k] * a[i][k];
        }
        f = a[i][l];
        g = -sign(std::sqrt(s), f);
        h = f * g - s;
        a[i][l] = f - g;

        for (long k = l; k < n; ++k) rv1[k] = a[i][k] / h;

        for (long j = l; j < m; ++j) {
          s = 0.0;
          for (long k = l; k < n; ++k) s += a[j][k] * a[i][k];

          for (long k = l; k < n; ++k) a[j][k] += s * rv1[k];
        }
        for (long k = l; k < n; ++k) a[i][k] *= scale;
      }
    }
    anorm = std::max(anorm, (std::abs(w[i]) + std::abs(rv1[i])));
  }
  for (long i = n - 1; i >= 0; --i) {
    if (i < n - 1) {
      if (g != 0) {
        for (long j = l; j < n; ++j) v[j][i] = (a[i][j] / a[i][l]) / g;

        for (long j = l; j < n; ++j) {
          s = 0.0;
          for (long k = l; k < n; ++k) s += a[i][k] * v[k][j];

          for (long k = l; k < n; ++k) v[k][j] += s * v[k][i];
        }
      }

      for (long j = l; j < n; ++j) v[i][j] = v[j][i] = 0.0;
    }

    v[i][i] = 1.0;
    g = rv1[i];
    l = i;
  }

  for (long i = std::min(m, n) - 1; i >= 0; --i) {
    l = i + 1;
    g = w[i];

    for (long j = l; j < n; ++j) a[i][j] = 0.0;

    if (g != 0) {
      g = 1.0 / g;

      for (long j = l; j < n; ++j) {
        s = 0.0;
        for (long k = l; k < m; ++k) s += a[k][i] * a[k][j];

        f = (s / a[i][i]) * g;

        for (long k = i; k < m; ++k) a[k][j] += f * a[k][i];
      }
      for (long j = i; j < m; ++j) a[j][i] *= g;
    } else {
      for (long j = i; j < m; ++j) a[j][i] = 0.0;
    }

    ++a[i][i];
  }

  for (long k = n - 1; k >= 0; --k) {
    for (long its = 1; its <= max_iter; ++its) {
      flag = true;
      for (l = k; l >= 1; --l) {
        nm = l - 1;
        if (std::abs(rv1[l]) <= eps * anorm) {
          flag = false;
          break;
        }
        if (std::abs(w[nm]) <= eps * anorm) {
          break;
        }
      }

      if (flag) {
        c = 0.0;
        s = 1.0;
        for (long i = l; i <= k; ++i) {
          f = s * rv1[i];
          rv1[i] = c * rv1[i];
          if (std::abs(f) <= eps * anorm) break;

          g = w[i];
          h = pythag(f, g);
          w[i] = h;
          h = 1.0 / h;
          c = g * h;
          s = -f * h;
          for (long j = 0; j < m; ++j) {
            y = a[j][nm];
            z = a[j][i];
            a[j][nm] = y * c + z * s;
            a[j][i] = z * c - y * s;
          }
        }
      }

      z = w[k];
      if (l == k) {
        if (z < 0.0) {
          w[k] = -z;
          for (long j = 0; j < n; ++j) v[j][k] = -v[j][k];
        }
        break;
      }

      if (its == max_iter) return false;

      x = w[l];
      nm = k - 1;
      y = w[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
      g = pythag(f, 1.0);
      f = ((x - z) * (x + z) + h * ((y / (f + sign(g, f))) - h)) / x;
      c = s = 1.0;
      for (long j = l; j <= nm; ++j) {
        long i = j + 1;
        g = rv1[i];
        y = w[i];
        h = s * g;
        g = c * g;
        z = pythag(f, h);
        rv1[j] = z;
        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = g * c - x * s;
        h = y * s;
        y *= c;
        for (long jj = 0; jj < n; ++jj) {
          x = v[jj][j];
          z = v[jj][i];
          v[jj][j] = x * c + z * s;
          v[jj][i] = z * c - x * s;
        }
        z = pythag(f, h);
        w[j] = z;
        if (z != 0) {
          z = 1.0 / z;
          c = f * z;
          s = h * z;
        }
        f = c * g + s * y;
        x = c * y - s * g;
        for (long jj = 0; jj < m; ++jj) {
          y = a[jj][j];
          z = a[jj][i];
          a[jj][j] = y * c + z * s;
          a[jj][i] = z * c - y * s;
        }
      }
      rv1[l] = 0.0;
      rv1[k] = f;
      w[k] = x;
    }
  }
  delete[] rv1;
  return true;
}

/*
 void svdcmp(double **a, int m, int n, double w[], double **v) {
 //int flag;
 //int i,its,j,jj,k,l,nm;
 //float anorm,c,f,g,h,s,scale,x,y,z,*rv1;
 double* rv1 = new double[n];
 double g = 0.0;
 double scale = 0.0;
 double anorm = 0.0;
 // Householder reduction to bidiagonal form.
 int i, j, k; // Loop indices
 for (i = 0; i < n; i++) {
 int l = i + 1;
 rv1[i] = scale * g;
 double s = 0.0;
 g = 0.0;
 scale = 0.0;
 if (i < m) {
 for (k = i; k < m; k++) {
 scale += fabs(a[k][i]);
 }
 if (scale) {
 for (k = i; k < m; k++) {
 a[k][i] /= scale;
 s += a[k][i] * a[k][i];
 }
 double f = a[i][i];
 g = -sign(sqrt(s), f);
 double h = f * g - s;
 a[i][i] = f - g;
 for (j = l; j < n; j++) {
 s = 0.0;
 for (k = i; k < m; k++) {
 s += a[k][i] * a[k][j];
 }
 f = s / h;
 for (k = i; k < m; k++) {
 a[k][j] += f * a[k][i];
 }
 }
 for (k = i; k < m; k++) {
 a[k][i] *= scale;
 }
 }
 }
 w[i] = scale * g;
 g = s = scale = 0.0;
 if (i < m && i != (n - 1)) {
 for (k = l; k < n; k++) {
 scale += fabs(a[i][k]);
 }
 if (scale) {
 for (k = l; k < n; k++) {
 a[i][k] /= scale;
 s += a[i][k] * a[i][k];
 }
 double f = a[i][l];
 g = -sign(sqrt(s), f);
 double h = f * g - s;
 a[i][l] = f - g;
 for (k = l; k < n; k++)
 rv1[k] = a[i][k] / h;
 for (j = l; j < m; j++) {
 s = 0.0;
 for (k = l; k < n; k++) {
 s += a[j][k] * a[i][k];
 }
 for (k = l; k < n; k++) {
 a[j][k] += s * rv1[k];
 }
 }
 for (k = l; k < n; k++) {
 a[i][k] *= scale;
 }
 }
 }
 anorm = max(anorm, (fabs(w[i]) + fabs(rv1[i])));
 }
 // Accumulation of right-hand transformations.
 for (i = n - 1; i >= 0; i--) {
 if (i < (n - 1)) {
 if (g) {
 for (j = l; j < n; j++) {
 // Double division to avoid possible underflow.
 v[j][i] = (a[i][j] / a[i][l]) / g;
 }
 for (j = l; j < n; j++) {
 for (s = 0.0, k = l; k < n; k++) {
 s += a[i][k] * v[k][j];
 }
 for (k = l; k < n; k++) {
 v[k][j] += s * v[k][i];
 }
 }
 }
 for (j = l; j < n; j++) {
 v[i][j] = v[j][i] = 0.0;
 }
 }
 v[i][i] = 1.0;
 g = rv1[i];
 l = i;
 }
 // Accumulation of left-hand transformations.
 for (i = IMIN(m, n) - 1; i >= 0; i--) {
 l = i + 1;
 g = w[i];
 for (j = l; j < n; j++) {
 a[i][j] = 0.0;
 }
 if (g) {
 g = 1.0 / g;
 for (j = l; j < n; j++) {
 for (s = 0.0, k = l; k < m; k++)
 s += a[k][i] * a[k][j];
 f = (s / a[i][i]) * g;
 for (k = i; k < m; k++)
 a[k][j] += f * a[k][i];
 }
 for (j = i; j < m; j++)
 a[j][i] *= g;
 } else
 for (j = i; j < m; j++)
 a[j][i] = 0.0;
 ++a[i][i];
 }
 for (k = n - 1; k >= 0; k--) {
 // Diagonalization of the bidiagonal form: Loop over
 // singular values, and over allowed iterations.
 for (its = 1; its <= 30; its++) {
 flag = 1;
 for (l = k; l >= 0; l--) {
 //Test for splitting.
 nm = l - 1;
 //Note that rv1[1] is always zero.
 if ((fabs(rv1[l]) + anorm) == anorm) {
 flag = 0;
 break;
 }
 if ((fabs(w[nm]) + anorm) == anorm)
 break;
 }
 if (flag) {
 c = 0.0;
 //Cancellation of rv1[l], if l> 1.
 s = 1.0;
 for (i = l; i < k; i++) {

 f = s * rv1[i];
 rv1[i] = c * rv1[i];
 if ((fabs(f) + anorm) == anorm)
 break;
 g = w[i];
 h = pythag(f, g);
 w[i] = h;
 h = 1.0 / h;
 c = g * h;
 s = -f * h;
 for (j = 0; j < m; j++) {
 y = a[j][nm];
 z = a[j][i];
 a[j][nm] = y * c + z * s;
 a[j][i] = z * c - y * s;
 }
 }
 }
 z = w[k];
 if (l == k) { // Convergence.
 if (z < 0.0) { // Singular value is made nonnegative.
 w[k] = -z;
 for (j = 0; j < n; j++)
 v[j][k] = -v[j][k];
 }
 break;
 }
 if (its == 30) {
 nrerror("no convergence in 30 svdcmp iterations");
 }
 x = w[l];
 // Shift from bottom 2-by-2 minor.
 nm = k - 1;
 y = w[nm];
 g = rv1[nm];
 h = rv1[k];
 f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
 g = pythag(f, 1.0);
 f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
 c = s = 1.0;
 // Next QR transformation:
 for (j = l; j < nm; j++) {
 i = j + 1;
 g = rv1[i];
 y = w[i];
 h = s * g;
 g = c * g;
 z = pythag(f, h);
 rv1[j] = z;
 c = f / z;
 s = h / z;
 f = x * c + g * s;
 g = g * c - x * s;
 h = y * s;
 y *= c;
 for (jj = 0; jj < n; jj++) {
 x = v[jj][j];
 z = v[jj][i];
 v[jj][j] = x * c + z * s;
 v[jj][i] = z * c - x * s;
 }
 z = pythag(f, h);
 w[j] = z;
 // Rotation can be arbitrary if z = 0.
 if (z) {
 z = 1.0 / z;
 c = f * z;
 s = h * z;
 }
 f = c * g + s * y;
 x = c * y - s * g;
 for (jj = 0; jj < m; jj++) {
 y = a[jj][j];
 z = a[jj][i];
 a[jj][j] = y * c + z * s;
 a[jj][i] = z * c - y * s;
 }
 }
 rv1[l] = 0.0;
 rv1[k] = f;
 w[k] = x;
 }
 }
 delete[] rv1;
 }
 */

/**
 * @brief Computes sqrt(a^2 + b^2) without destructive underflow or overflow.
 */
/*
double pythag(double a, double b)

{
        double absa, absb;
        absa = fabs(a);
        absb = fabs(b);
        if (absa > absb)
                return absa * sqrt(1.0 + SQR(absb / absa));
        else
                return (absb == 0.0 ? 0.0 : absb * sqrt(1.0 + SQR(absa /
absb)));
}
*/

}  // namespace numeric
}  // namespace math
