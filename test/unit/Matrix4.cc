#include "gtest/gtest.h"
#include "constants.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Euler.hpp"

#include <cmath>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace MyEngine;
using namespace Test;

template <typename T = float>
bool matrixEquals4(Matrix4<T> a, Matrix4<T> b, T tolerance = 0.0001) {
    for( unsigned i = 0, il = 16; i < il; ++i ) {
        T delta = a.elements[i] - b.elements[i];
        if( delta > tolerance ) {
            return false;
        }
    }
    return true;
}

TEST(Matrix4f, constructor) {
    Matrix4f a = Matrix4f();
    EXPECT_EQ(a.det(), 1);

    Matrix4f b = Matrix4f().set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    EXPECT_EQ(b.elements[0], 0 );
    EXPECT_EQ(b.elements[1], 4 );
    EXPECT_EQ(b.elements[2], 8 );
    EXPECT_EQ(b.elements[3], 12 );
    EXPECT_EQ(b.elements[4], 1 );
    EXPECT_EQ(b.elements[5], 5 );
    EXPECT_EQ(b.elements[6], 9 );
    EXPECT_EQ(b.elements[7], 13 );
    EXPECT_EQ(b.elements[8], 2 );
    EXPECT_EQ(b.elements[9], 6 );
    EXPECT_EQ(b.elements[10], 10 );
    EXPECT_EQ(b.elements[11], 14 );
    EXPECT_EQ(b.elements[12], 3 );
    EXPECT_EQ(b.elements[13], 7 );
    EXPECT_EQ(b.elements[14], 11 );
    EXPECT_EQ(b.elements[15], 15 );

    EXPECT_TRUE(! matrixEquals4( a, b ));
}

TEST(Matrix4f, copy) {
    Matrix4f a = Matrix4f().set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    Matrix4f b = Matrix4f().copy( a );

    EXPECT_TRUE(matrixEquals4( a, b ));

    // ensure that it is a true copy
    a.elements[0] = 2;
    EXPECT_TRUE(! matrixEquals4( a, b ));
}

TEST(Matrix4f, set) {
    Matrix4f b = Matrix4f();
    EXPECT_EQ(b.det(), 1);

    b.set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    EXPECT_EQ(b.elements[0], 0 );
    EXPECT_EQ(b.elements[1], 4 );
    EXPECT_EQ(b.elements[2], 8 );
    EXPECT_EQ(b.elements[3], 12 );
    EXPECT_EQ(b.elements[4], 1 );
    EXPECT_EQ(b.elements[5], 5 );
    EXPECT_EQ(b.elements[6], 9 );
    EXPECT_EQ(b.elements[7], 13 );
    EXPECT_EQ(b.elements[8], 2 );
    EXPECT_EQ(b.elements[9], 6 );
    EXPECT_EQ(b.elements[10], 10 );
    EXPECT_EQ(b.elements[11], 14 );
    EXPECT_EQ(b.elements[12], 3 );
    EXPECT_EQ(b.elements[13], 7 );
    EXPECT_EQ(b.elements[14], 11 );
    EXPECT_EQ(b.elements[15], 15 );
}

TEST(Matrix4f, identity) {
    Matrix4f b = Matrix4f().set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    EXPECT_EQ(b.elements[0], 0 );
    EXPECT_EQ(b.elements[1], 4 );
    EXPECT_EQ(b.elements[2], 8 );
    EXPECT_EQ(b.elements[3], 12 );
    EXPECT_EQ(b.elements[4], 1 );
    EXPECT_EQ(b.elements[5], 5 );
    EXPECT_EQ(b.elements[6], 9 );
    EXPECT_EQ(b.elements[7], 13 );
    EXPECT_EQ(b.elements[8], 2 );
    EXPECT_EQ(b.elements[9], 6 );
    EXPECT_EQ(b.elements[10], 10 );
    EXPECT_EQ(b.elements[11], 14 );
    EXPECT_EQ(b.elements[12], 3 );
    EXPECT_EQ(b.elements[13], 7 );
    EXPECT_EQ(b.elements[14], 11 );
    EXPECT_EQ(b.elements[15], 15 );

    Matrix4f a = Matrix4f();
    EXPECT_TRUE(! matrixEquals4( a, b ));

    b.identity();
    EXPECT_TRUE(matrixEquals4( a, b ));
}

TEST(Matrix4f, multiplyScalar) {
    Matrix4f b = Matrix4f().set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    EXPECT_EQ(b.elements[0], 0 );
    EXPECT_EQ(b.elements[1], 4 );
    EXPECT_EQ(b.elements[2], 8 );
    EXPECT_EQ(b.elements[3], 12 );
    EXPECT_EQ(b.elements[4], 1 );
    EXPECT_EQ(b.elements[5], 5 );
    EXPECT_EQ(b.elements[6], 9 );
    EXPECT_EQ(b.elements[7], 13 );
    EXPECT_EQ(b.elements[8], 2 );
    EXPECT_EQ(b.elements[9], 6 );
    EXPECT_EQ(b.elements[10], 10 );
    EXPECT_EQ(b.elements[11], 14 );
    EXPECT_EQ(b.elements[12], 3 );
    EXPECT_EQ(b.elements[13], 7 );
    EXPECT_EQ(b.elements[14], 11 );
    EXPECT_EQ(b.elements[15], 15 );

    b.multiplyScalar( 2 );
    EXPECT_EQ(b.elements[0], 0*2 );
    EXPECT_EQ(b.elements[1], 4*2 );
    EXPECT_EQ(b.elements[2], 8*2 );
    EXPECT_EQ(b.elements[3], 12*2 );
    EXPECT_EQ(b.elements[4], 1*2 );
    EXPECT_EQ(b.elements[5], 5*2 );
    EXPECT_EQ(b.elements[6], 9*2 );
    EXPECT_EQ(b.elements[7], 13*2 );
    EXPECT_EQ(b.elements[8], 2*2 );
    EXPECT_EQ(b.elements[9], 6*2 );
    EXPECT_EQ(b.elements[10], 10*2 );
    EXPECT_EQ(b.elements[11], 14*2 );
    EXPECT_EQ(b.elements[12], 3*2 );
    EXPECT_EQ(b.elements[13], 7*2 );
    EXPECT_EQ(b.elements[14], 11*2 );
    EXPECT_EQ(b.elements[15], 15*2 );
}

TEST(Matrix4f, determinant) {
    Matrix4f a = Matrix4f();
    EXPECT_EQ(a.det(), 1 );

    a.elements[0] = 2;
    EXPECT_EQ(a.det(), 2 );

    a.elements[0] = 0;
    EXPECT_EQ(a.det(), 0 );

    // calculated via http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
    a.set( 2, 3, 4, 5, -1, -21, -3, -4, 6, 7, 8, 10, -8, -9, -10, -12 );
    EXPECT_EQ(a.det(), 76 );
}

TEST(Matrix4f, getInverse) {
    Matrix4f identity = Matrix4f();

    Matrix4f a = Matrix4f();
    Matrix4f b = Matrix4f().set( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    Matrix4f c = Matrix4f().set( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    EXPECT_TRUE(! matrixEquals4( a, b ));
    b.getInverse( a, false );
    EXPECT_TRUE(matrixEquals4( b, Matrix4f() ));

    EXPECT_THROW(b.getInverse( c, true ), runtime_error);

    vector<Matrix4f> testMatrices = { {
        Matrix4f().makeRotationX( 0.3 ),
        Matrix4f().makeRotationX( -0.3 ),
        Matrix4f().makeRotationY( 0.3 ),
        Matrix4f().makeRotationY( -0.3 ),
        Matrix4f().makeRotationZ( 0.3 ),
        Matrix4f().makeRotationZ( -0.3 ),
        Matrix4f().makeScale( 1, 2, 3 ),
        Matrix4f().makeScale( 1.0/8, 1.0/2, 1.0/3 ),
        Matrix4f().makeFrustum( -1, 1, -1, 1, 1, 1000 ),
        Matrix4f().makeFrustum( -16, 16, -9, 9, 0.1, 10000 ),
        Matrix4f().makeTranslation( 1, 2, 3 )
    } };

    for( const Matrix4f& m : testMatrices ) {

        Matrix4f mInverse = Matrix4f().getInverse( m );
        Matrix4f mSelfInverse = Matrix4f(m);
        mSelfInverse.getInverse( mSelfInverse );

        // self-inverse should the same as inverse
        EXPECT_TRUE(matrixEquals4( mSelfInverse, mInverse ));

        // the determinant of the inverse should be the reciprocal
        EXPECT_LT(fabs( m.det() * mInverse.det() - 1 ), 0.0001);

        Matrix4f mProduct = Matrix4f().multiplyMatrices( m, mInverse );

        // the determinant of the identity matrix is 1
        EXPECT_LT(fabs( mProduct.det() - 1 ), 0.0001);
        EXPECT_TRUE(matrixEquals4( mProduct, identity ));
    }
}

TEST(Matrix4f, makeBasis_extractBasis) {
    array<Vector3f, 3> identityBasis = { { Vector3f( 1, 0, 0 ), Vector3f( 0, 1, 0 ), Vector3f( 0, 0, 1 ) } };
    Matrix4f a = Matrix4f().makeBasis( identityBasis[0], identityBasis[1], identityBasis[2] );
    Matrix4f identity = Matrix4f();
    EXPECT_TRUE(matrixEquals4( a, identity ));

    vector<array<Vector3f, 3> > testBases = { { { Vector3f( 0, 1, 0 ), Vector3f( -1, 0, 0 ), Vector3f( 0, 0, 1 ) } } };
    for(const array<Vector3f, 3>& testBasis : testBases) {
        Matrix4f b = Matrix4f().makeBasis( testBasis[0], testBasis[1], testBasis[2] );
        array<Vector3f, 3> outBasis = {{ Vector3f(), Vector3f(), Vector3f() }};
        b.extractBasis( outBasis[0], outBasis[1], outBasis[2] );
        // check what goes in, is what comes out.
        for(unsigned j = 0; j < 3; j ++ ) {
            //console.log( outBasis[j], testBasis[j] );
            EXPECT_TRUE(outBasis[j].equals( testBasis[j] ));
        }

        // get the basis out the hard war
        for(unsigned j = 0; j < 3; j ++ ) {
            outBasis[j].copy( identityBasis[j] );
            outBasis[j].applyMatrix4( b );
        }
        // did the multiply method of basis extraction work?
        for(unsigned j = 0; j < 3; j ++ ) {
            EXPECT_TRUE(outBasis[j].equals( testBasis[j] ));
        }
    }
}

TEST(Matrix4f, transpose) {
    Matrix4f a = Matrix4f();
    Matrix4f b = Matrix4f(a).transpose();
    EXPECT_TRUE(matrixEquals4( a, b ));

    b = Matrix4f().set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    Matrix4f c = Matrix4f(b).transpose();
    EXPECT_TRUE(! matrixEquals4( b, c ));
    c.transpose();
    EXPECT_TRUE(matrixEquals4( b, c ));
}

TEST(Matrix4f, clone) {
    Matrix4f a = Matrix4f().set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
    Matrix4f b = Matrix4f(a);

    EXPECT_TRUE(matrixEquals4( a, b ));

    // ensure that it is a true copy
    a.elements[0] = 2;
    EXPECT_TRUE(! matrixEquals4( a, b ));
}


TEST(Matrix4f, compose_decompose) {
    vector<Vector3f> tValues = { {
        Vector3f(),
        Vector3f( 3, 0, 0 ),
        Vector3f( 0, 4, 0 ),
        Vector3f( 0, 0, 5 ),
        Vector3f( -6, 0, 0 ),
        Vector3f( 0, -7, 0 ),
        Vector3f( 0, 0, -8 ),
        Vector3f( -2, 5, -9 ),
        Vector3f( -2, -5, -9 )
    } };

    vector<Vector3f> sValues = { {
        Vector3f( 1, 1, 1 ),
        Vector3f( 2, 2, 2 ),
        Vector3f( 1, -1, 1 ),
        Vector3f( -1, 1, 1 ),
        Vector3f( 1, 1, -1 ),
        Vector3f( 2, -2, 1 ),
        Vector3f( -1, 2, -2 ),
        Vector3f( -1, -1, -1 ),
        Vector3f( -2, -2, -2 )
    } };

    vector<Quaternionf> rValues = { {
        Quaternionf(),
        Quaternionf().setFromEuler( Eulerf( 1, 1, 0 ) ),
        Quaternionf().setFromEuler( Eulerf( 1, -1, 1 ) ),
        Quaternionf( 0, 0.9238795292366128, 0, 0.38268342717215614 )
    } };


    for(const Vector3f& t : tValues) {
        for(const Vector3f& s : sValues) {
            for(const Quaternionf& r : rValues) {
                Matrix4f m = Matrix4f().compose( t, r, s );
                Vector3f t2 = Vector3f();
                Quaternionf r2 = Quaternionf();
                Vector3f s2 = Vector3f();

                m.decompose( t2, r2, s2 );

                Matrix4f m2 = Matrix4f().compose( t2, r2, s2 );

                //bool matrixIsSame = matrixEquals4( m, m2 );
                /* debug code
                if( ! matrixIsSame ) {
                    console.log( t, s, r );
                    console.log( t2, s2, r2 );
                    console.log( m, m2 );
                }*/
                EXPECT_TRUE(matrixEquals4( m, m2 ));

            }
        }
    }
}
