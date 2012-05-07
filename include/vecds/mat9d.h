
// -------------------------------------------------------------------
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------


#ifndef MAT9D_H
#define MAT9D_H

#include <qglobal.h>
#include <QVector3D>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

/**
 * Structure and algebraic operations for the \f$3\times 3\f$ matrix
 * <code>Mat9d</code>. The matrix elements of a <code>Mat9d</code>
 * object \f$M\f$ are written in "vector" form: 
 *
 * \f[ M=\left[\begin{array}{ccc} m_1 & m_2 & m_3 \\ m_4 & m_5 & m_6
 \\ m_7 & m_8 & m_9 \end{array}\right]\quad.\f]
 * 
 * @note Some of these functions are marked as inline. 
 *
 * @author Jan Cholewinski, Toby D. Young 2010
 */

inline glm::dvec3 to_dvec3(QVector3D x)
{ return glm::dvec3(x.x(), x.y(), x.z()); }


inline QVector3D to_QV(glm::dvec3 x)
{ return QVector3D(x.x, x.y, x.z); }


struct Mat9d
{
  /**
   * Actual values of the matrix elements of <code>Mat9d</code>. See
   * cleas documentation for more details.
   */
  double m1, m2, m3, m4, m5, m6, m7, m8, m9;
  
  /**
   * Constructor. Allocates memory for a \f$3\times3\f$ matrix
   * <code>Mat9d</code> initialized with all elements equal to zero.
   */
  Mat9d () 
  : m1(0), m2(0), m3(0), 
    m4(0), m5(0), m6(0), 
    m7(0), m8(0), m9(0)
  {}
  
  /**
   * Constructor. Allocates memory for a \f$3\times3\f$ matrix
   * <code>Mat9d</code> initialized with user defined elements
   * \f$m_i\,\forall m\in\{1,...,9\}\f$.
   */
  Mat9d (const double m_1, const double m_2, const double m_3, 
	 const double m_4, const double m_5, const double m_6, 
	 const double m_7, const double m_8, const double m_9) 
  : m1(m_1), m2(m_2), m3(m_3), 
    m4(m_4), m5(m_5), m6(m_6), 
    m7(m_7), m8(m_8), m9(m_9)
  {}
  
  /**
   * Constructor. Allocates memory for a \f$3\times3\f$ matrix
   * <code>Mat9d</code> initialized with user defined glm::dmat3
   */
  Mat9d (const glm::dmat3 gm)
  : m1(gm[0][0]), m2(gm[0][1]), m3(gm[0][2]), 
    m4(gm[1][0]), m5(gm[1][1]), m6(gm[1][2]), 
    m7(gm[2][0]), m8(gm[2][1]), m9(gm[2][2])
  {}
  

  /**
   * Matrix-matrix addition. Add matrix \f$p\f$ to <code>this</code>
   * matrix.
   */
  Mat9d operator+ (const Mat9d &p) const
  {
    return Mat9d (*this) += p;
  }
  
  /**
   * Matrix-matrix subtraction. Subtract matrix \f$p\f$ from
   * <code>this</code> matrix.
   */
  Mat9d operator- (const Mat9d &p) const
  {
    return Mat9d (*this) -= p;
  }
  
  /**
   * Scalar-matrix subtraction \f$M\cdot a\f$. Multiply
   * <code>this</code> matrix by the scalar quantity \f$a\f$.
   */
  Mat9d operator* (const double a) const
  {
    return Mat9d (*this) *= a;
  }

  /**
   * Return the trace of <code>this</code> matrix by summing over the
   * diagonal elements of <code>this</code> matrix, ie. \f${\rm
   * Tr}M\f$.
   */
  double trace () const
  {
    return 
      m1 + m5 + m9;
  }

  /**
   * Inline matrix addition.
   */
  Mat9d &operator+= (const Mat9d &p)
  {
    m1 += p.m1;
    m2 += p.m2;
    m3 += p.m3;
    m4 += p.m4;
    m5 += p.m5;
    m6 += p.m6;
    m7 += p.m7;
    m8 += p.m8;
    m9 += p.m9;

    return *this;
  }
  
  /**
   * Inline matrix subtraction.
   */
  Mat9d &operator-= (const Mat9d &p)
  {
    m1 -= p.m1;
    m2 -= p.m2;
    m3 -= p.m3;
    m4 -= p.m4;
    m5 -= p.m5;
    m6 -= p.m6;
    m7 -= p.m7;
    m8 -= p.m8;
    m9 -= p.m9;

    return *this;
  }
  
  /**
   * Matrix multiplication operator. Multiply <code>this</code> matrix
   * by the constant \f$a\f$.
   */
  Mat9d &operator*= (const double a)
  {
    m1 *= a;
    m2 *= a;
    m3 *= a;
    m4 *= a;
    m5 *= a;
    m6 *= a;
    m7 *= a;
    m8 *= a;
    m9 *= a;

    return *this;
  }
  
  /** 
   * Read/write access to the <code>index</code>th-component of
   * <code>this</code> matrix.
   */
  double &operator[](const unsigned int index) 
  {
    Q_ASSERT(index < 9);
    return (&m1)[index];
  }

  /** 
   * Read only access to the <code>index</code>th-component of
   * <code>this</code> matrix.
   */  
  const double &operator[](const unsigned int index) const 
  {
    Q_ASSERT(index < 9);
    return (&m1)[index];
  }
  
};

/**
 * Return the determinant \f${\rm det}\,M\f$ of <code>this</code>
 * matrix.
 */
inline double determinant (const Mat9d &m)
{
  return 
    m.m1*m.m5*m.m9 +
    m.m3*m.m4*m.m8 +
    m.m7*m.m2*m.m6 -
    m.m3*m.m5*m.m7 -
    m.m6*m.m8*m.m1 -
    m.m2*m.m4*m.m9;
}

/**
 * Return the inverse of the deteminant \f$({\rm det}\,M)^{-1}\f$ of
 * <code>this</code> matrix.
 */
inline const double inverse_determinant (const Mat9d &m)
{
  return 1./determinant (m);
}

/**
 * Return the inverse \f${\rm inv}\,M=M^{-1}\f$ of <code>this</code>
 * matrix.
 */
inline Mat9d invert (const Mat9d &m)
{
  double invdet = inverse_determinant (m);

  double m1 = invdet*(m.m5*m.m9 - m.m6*m.m8);
  double m2 = invdet*(m.m3*m.m8 - m.m2*m.m9);
  double m3 = invdet*(m.m2*m.m6 - m.m3*m.m5);
  double m4 = invdet*(m.m6*m.m7 - m.m4*m.m9);
  double m5 = invdet*(m.m1*m.m9 - m.m3*m.m7);
  double m6 = invdet*(m.m3*m.m4 - m.m1*m.m6);
  double m7 = invdet*(m.m4*m.m8 - m.m5*m.m7);
  double m8 = invdet*(m.m2*m.m7 - m.m1*m.m8);
  double m9 = invdet*(m.m1*m.m5 - m.m2*m.m4);

  return Mat9d (m1, m2, m3, m4, m5, m6, m7, m8, m9);
}

/**
 * Return the transpose of an <code>Mat9d</code> object.
 */
inline Mat9d transpose (const Mat9d &m)
{
  return Mat9d (m.m1, m.m4, m.m7, 
		m.m2, m.m5, m.m8, 
		m.m3, m.m6, m.m9);
}

/**
 * Return the matrix multiplication of matrice <code>m1</code> and
 * <code>m2</code>.
 */
inline Mat9d matmult(const Mat9d &m1, 
		     const Mat9d &m2)
{
  Mat9d mat;
  
  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      mat[3*i+j] 
	= m1[3*i]*m2[j] 
	+ m1[3*i+1]*m2[3+j] 
	+ m1[3*i+2]*m2[6+j];
  
  return mat;
}

/**
 * Return the vector multiplication of matrice <code>rotmat</code> and
 * vector<code>point</code>.
 */
inline QVector3D matvecmult(const Mat9d &mat, const QVector3D &v)
{
   return QVector3D(v.x()*mat.m1 + v.y()*mat.m2 + v.z()*mat.m3, 
                    v.x()*mat.m4 + v.y()*mat.m5 + v.z()*mat.m6,
                    v.x()*mat.m7 + v.y()*mat.m8 + v.z()*mat.m9);
}
#endif
