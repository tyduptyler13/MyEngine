#include "gtest/gtest.h"
#include "constants.hpp"
#include "Vector3.hpp"

#include <cmath>

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace Test;

TEST(Vector3, constructor){
    Vector3f a = Vector3f();
    EXPECT_EQ( a.x, 0);
    EXPECT_EQ( a.y, 0);
    EXPECT_EQ( a.z, 0);

    a = Vector3f( x, y, z );
    EXPECT_EQ( a.x, x);
    EXPECT_EQ( a.y, y);
    EXPECT_EQ( a.z, z);
}

TEST(Vector3, copy){
    Vector3f a = Vector3f( x, y, z );
    Vector3f b = Vector3f( a );
    EXPECT_EQ( b.x, x);
    EXPECT_EQ( b.y, y);
    EXPECT_EQ( b.z, z);

    // ensure that it is a true copy
    a.x = 0;
    a.y = -1;
    a.z = -2;
    EXPECT_EQ( b.x, x);
    EXPECT_EQ( b.y, y);
    EXPECT_EQ( b.z, z);
}

TEST(Vector3, set){
    Vector3f a = Vector3f();
    EXPECT_EQ( a.x, 0);
    EXPECT_EQ( a.y, 0);
    EXPECT_EQ( a.z, 0);

    a.set( x, y, z );
    EXPECT_EQ( a.x, x);
    EXPECT_EQ( a.y, y);
    EXPECT_EQ( a.z, z);
}

TEST(Vector3, setX_setY_setZ){
    Vector3f a = Vector3f();
    EXPECT_EQ( a.x, 0);
    EXPECT_EQ( a.y, 0);
    EXPECT_EQ( a.z, 0);

    a.setX( x );
    a.setY( y );
    a.setZ( z );

    EXPECT_EQ( a.x, x);
    EXPECT_EQ( a.y, y);
    EXPECT_EQ( a.z, z);
}

// Removed...
// TEST(Vector3, setComponent_getComponent){
//     Vector3f a = Vector3f();
//     EXPECT_EQ( a.x, 0);
//     EXPECT_EQ( a.y, 0);
//     EXPECT_EQ( a.z, 0);

//     a.setComponent( 0, 1 );
//     a.setComponent( 1, 2 );
//     a.setComponent( 2, 3 );
//     EXPECT_EQ( a.getComponent( 0 ), 1);
//     EXPECT_EQ( a.getComponent( 1 ), 2);
//     EXPECT_EQ( a.getComponent( 2 ), 3);
// }

TEST(Vector3, add){
    Vector3f a = Vector3f( x, y, z );
    Vector3f b = Vector3f( -x, -y, -z );

    a.add( b );
    EXPECT_EQ( a.x, 0);
    EXPECT_EQ( a.y, 0);
    EXPECT_EQ( a.z, 0);

    Vector3f c = b + b;
    EXPECT_EQ( c.x, -2*x);
    EXPECT_EQ( c.y, -2*y);
    EXPECT_EQ( c.z, -2*z);
}

TEST(Vector3, sub){
    Vector3f a = Vector3f( x, y, z );
    Vector3f b = Vector3f( -x, -y, -z );

    a.sub( b );
    EXPECT_EQ( a.x, 2*x);
    EXPECT_EQ( a.y, 2*y);
    EXPECT_EQ( a.z, 2*z);

    Vector3f c = a - a;
    EXPECT_EQ( c.x, 0);
    EXPECT_EQ( c.y, 0);
    EXPECT_EQ( c.z, 0);
}

TEST(Vector3, multiply_divide){
    Vector3f a = Vector3f( x, y, z );
    Vector3f b = Vector3f( -x, -y, -z );

    a.multiply( -2 );
    EXPECT_EQ( a.x, x*-2);
    EXPECT_EQ( a.y, y*-2);
    EXPECT_EQ( a.z, z*-2);

    b.multiply( -2 );
    EXPECT_EQ( b.x, 2*x);
    EXPECT_EQ( b.y, 2*y);
    EXPECT_EQ( b.z, 2*z);

    a.divide( -2 );
    EXPECT_EQ( a.x, x);
    EXPECT_EQ( a.y, y);
    EXPECT_EQ( a.z, z);

    b.divide( -2 );
    EXPECT_EQ( b.x, -x);
    EXPECT_EQ( b.y, -y);
    EXPECT_EQ( b.z, -z);
}

TEST(Vector3, min_max_clamp){
    Vector3f a = Vector3f( x, y, z );
    Vector3f b = Vector3f( -x, -y, -z );
    Vector3f c = Vector3f();

    c.copy( a ).min( b );
    EXPECT_EQ( c.x, -x);
    EXPECT_EQ( c.y, -y);
    EXPECT_EQ( c.z, -z);

    c.copy( a ).max( b );
    EXPECT_EQ( c.x, x);
    EXPECT_EQ( c.y, y);
    EXPECT_EQ( c.z, z);

    c.set( -2*x, 2*y, -2*z );
    c.clamp( b, a );
    EXPECT_EQ( c.x, -x);
    EXPECT_EQ( c.y, y);
    EXPECT_EQ( c.z, -z);
}

TEST(Vector3, negate){
    Vector3f a = Vector3f( x, y, z );

    a.negate();
    EXPECT_EQ( a.x, -x);
    EXPECT_EQ( a.y, -y);
    EXPECT_EQ( a.z, -z);
}

TEST(Vector3, dot){
    Vector3f a = Vector3f( x, y, z );
    Vector3f b = Vector3f( -x, -y, -z );
    Vector3f c = Vector3f();

    Vector3f result = a.dot( b );
    EXPECT_EQ( result, (-x*x-y*y-z*z));

    result = a.dot( c );
    EXPECT_EQ( result, 0);
}

TEST(Vector3, length_lengthSq){
    Vector3f a = Vector3f( x, 0, 0 );
    Vector3f b = Vector3f( 0, -y, 0 );
    Vector3f c = Vector3f( 0, 0, z );
    Vector3f d = Vector3f();

    EXPECT_EQ( a.length(), x);
    EXPECT_EQ( a.lengthSq(), x*x);
    EXPECT_EQ( b.length(), y);
    EXPECT_EQ( b.lengthSq(), y*y);
    EXPECT_EQ( c.length(), z);
    EXPECT_EQ( c.lengthSq(), z*z);
    EXPECT_EQ( d.length(), 0);
    EXPECT_EQ( d.lengthSq(), 0);

    a.set( x, y, z );
    EXPECT_EQ( a.length(), sqrt( x*x + y*y + z*z ));
    EXPECT_EQ( a.lengthSq(), ( x*x + y*y + z*z ));
}

TEST(Vector3, normalize){
    Vector3f a = Vector3f( x, 0, 0 );
    Vector3f b = Vector3f( 0, -y, 0 );
    Vector3f c = Vector3f( 0, 0, z );

    a.normalize();
    EXPECT_EQ( a.length(), 1);
    EXPECT_EQ( a.x, 1);

    b.normalize();
    EXPECT_EQ( b.length(), 1);
    EXPECT_EQ( b.y, -1);

    c.normalize();
    EXPECT_EQ( c.length(), 1);
    EXPECT_EQ( c.z, 1);
}

TEST(Vector3, distanceTo_distanceToSquared){
    Vector3f a = Vector3f( x, 0, 0 );
    Vector3f b = Vector3f( 0, -y, 0 );
    Vector3f c = Vector3f( 0, 0, z );
    Vector3f d = Vector3f();

    EXPECT_EQ( a.distanceTo( d ), x);
    EXPECT_EQ( a.distanceToSquared( d ), x*x);

    EXPECT_EQ( b.distanceTo( d ), y);
    EXPECT_EQ( b.distanceToSquared( d ), y*y);

    EXPECT_EQ( c.distanceTo( d ), z);
    EXPECT_EQ( c.distanceToSquared( d ), z*z);
}

TEST(Vector3, setLength){
    Vector3f a = Vector3f( x, 0, 0 );

    EXPECT_EQ( a.length(), x);
    a.setLength( y );
    EXPECT_EQ( a.length(), y);

    a = Vector3f( 0, 0, 0 );
    EXPECT_EQ( a.length(), 0);
    a.setLength( y );
    EXPECT_EQ( a.length(), 0);

}

TEST(Vector3, projectOnVector){
    Vector3f a = Vector3f( 1, 0, 0 );
    Vector3f b = Vector3f();
    Vector3f normal = Vector3f( 10, 0, 0 );

    EXPECT_EQ( b.copy( a ).projectOnVector( normal ),  Vector3f( 1, 0, 0 ) );

    a.set( 0, 1, 0 );
    EXPECT_EQ( b.copy( a ).projectOnVector( normal ),  Vector3f( 0, 0, 0 ) );

    a.set( 0, 0, -1 );
    EXPECT_EQ( b.copy( a ).projectOnVector( normal ),  Vector3f( 0, 0, 0 ) );

    a.set( -1, 0, 0 );
    EXPECT_EQ( b.copy( a ).projectOnVector( normal ),  Vector3f( -1, 0, 0 ) );

}

TEST(Vector3, projectOnPlane){
    Vector3f a = Vector3f( 1, 0, 0 );
    Vector3f b = Vector3f();
    Vector3f normal = Vector3f( 1, 0, 0 );

    EXPECT_EQ( b.copy( a ).projectOnPlane( normal ),  Vector3f( 0, 0, 0 ) );

    a.set( 0, 1, 0 );
    EXPECT_EQ( b.copy( a ).projectOnPlane( normal ),  Vector3f( 0, 1, 0 ) );

    a.set( 0, 0, -1 );
    EXPECT_EQ( b.copy( a ).projectOnPlane( normal ),  Vector3f( 0, 0, -1 ) );

    a.set( -1, 0, 0 );
    EXPECT_EQ( b.copy( a ).projectOnPlane( normal ),  Vector3f( 0, 0, 0 ) );

}

TEST(Vector3, reflect){
    Vector3f a = Vector3f();
    Vector3f normal = Vector3f( 0, 1, 0 );
    Vector3f b = Vector3f();

    a.set( 0, -1, 0 );
    EXPECT_EQ( b.copy( a ).reflect( normal ),  Vector3f( 0, 1, 0 ) );

    a.set( 1, -1, 0 );
    EXPECT_EQ( b.copy( a ).reflect( normal ),  Vector3f( 1, 1, 0 ) );

    a.set( 1, -1, 0 );
    normal.set( 0, -1, 0 );
    EXPECT_EQ( b.copy( a ).reflect( normal ),  Vector3f( 1, 1, 0 ) );
}

TEST(Vector3, angleTo){
    Vector3f a = Vector3f( 0, -0.18851655680720186, 0.9820700116639124 );
    Vector3f b = Vector3f( 0, 0.18851655680720186, -0.9820700116639124 );

    EXPECT_EQ( a.angleTo( a ), 0 );
    EXPECT_EQ( a.angleTo( b ), (float) Math::PI );

    Vector3f x = Vector3f( 1, 0, 0 );
    Vector3f y = Vector3f( 0, 1, 0 );
    Vector3f z = Vector3f( 0, 0, 1 );

    EXPECT_EQ( x.angleTo( y ), (float) Math::PI / 2 );
    EXPECT_EQ( x.angleTo( z ), (float) Math::PI / 2 );
    EXPECT_EQ( z.angleTo( x ), (float) Math::PI / 2 );

    EXPECT_TRUE( abs( x.angleTo( Vector3f( 1, 1, 0 ) ) - ( Math::PI / 4 ) ) < 0.0000001 );
}

TEST(Vector3, lerp_clone){
    Vector3f a = Vector3f( x, 0, z );
    Vector3f b = Vector3f( 0, -y, 0 );

    EXPECT_EQ( a.lerp( a, 0 ),  a.lerp( a, 0.5 ) );
    EXPECT_EQ( a.lerp( a, 0 ),  a.lerp( a, 1 ) );

    EXPECT_EQ( Vector3f(a).lerp( b, 0 ),  a );

    EXPECT_EQ( Vector3f(a).lerp( b, 0.5 ).x, x*0.5);
    EXPECT_EQ( Vector3f(a).lerp( b, 0.5 ).y, -y*0.5);
    EXPECT_EQ( Vector3f(a).lerp( b, 0.5 ).z, z*0.5);

    EXPECT_EQ( Vector3f(a).lerp( b, 1 ),  b );
}

TEST(Vector3, equals){
    Vector3f a = Vector3f( x, 0, z );
    Vector3f b = Vector3f( 0, -y, 0 );

    EXPECT_NE(a.x, b.x);
    EXPECT_NE(a.y, b.y);
    EXPECT_NE(a.z, b.z);

    EXPECT_NE( a,  b );
    EXPECT_NE( b,  a );

    a.copy( b );
    EXPECT_EQ( a.x, b.x);
    EXPECT_EQ( a.y, b.y);
    EXPECT_EQ( a.z, b.z);

    EXPECT_EQ( a,  b );
    EXPECT_EQ( b,  a );
}
