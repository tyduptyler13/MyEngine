#include "gtest/gtest.h"
#include "constants.hpp"
#include "Vector2.hpp"

#include <cmath>

using namespace std;
using namespace MyEngine;
using namespace Test;

TEST(Vector2, constructor){
    Vector2f a = Vector2f();
    EXPECT_EQ(a.x, 0);
    EXPECT_EQ(a.y, 0);

    a = Vector2f( x, y );
    EXPECT_EQ(a.x, x);
    EXPECT_EQ(a.y, y);
}

TEST(Vector2, copy){
    Vector2f a = Vector2f( x, y );
    Vector2f b = Vector2f( a );
    EXPECT_EQ(b.x, x);
    EXPECT_EQ(b.y, y);

    // ensure that it is a true copy
    a.x = 0;
    a.y = -1;
    EXPECT_EQ(b.x, x);
    EXPECT_EQ(b.y, y);
}

TEST(Vector2, set){
    Vector2f a = Vector2f();
    EXPECT_EQ(a.x, 0);
    EXPECT_EQ(a.y, 0);

    a.set( x, y );
    EXPECT_EQ(a.x, x);
    EXPECT_EQ(a.y, y);
}

TEST(Vector2, setX_setY){
    Vector2f a = Vector2f();
    EXPECT_EQ(a.x, 0);
    EXPECT_EQ(a.y, 0);

    a.setX( x );
    a.setY( y );
    EXPECT_EQ(a.x, x);
    EXPECT_EQ(a.y, y);
}

// TEST(Vector2, setComponent_getComponent){
//     Vector2f a = Vector2f();
//     EXPECT_EQ(a.x, 0);
//     EXPECT_EQ(a.y, 0);

//     a.setComponent( 0, 1 );
//     a.setComponent( 1, 2 );
//     EXPECT_EQ(a.getComponent( 0 ), 1);
//     EXPECT_EQ(a.getComponent( 1 ), 2);
// }

TEST(Vector2, add){
    Vector2f a = Vector2f( x, y );
    Vector2f b = Vector2f( -x, -y );

    a.add( b );
    EXPECT_EQ(a.x, 0);
    EXPECT_EQ(a.y, 0);

    Vector2f c =  b + b;
    EXPECT_EQ(c.x, -2*x);
    EXPECT_EQ(c.y, -2*y);
}

TEST(Vector2, sub){
    Vector2f a = Vector2f( x, y );
    Vector2f b = Vector2f( -x, -y );

    a.sub( b );
    EXPECT_EQ(a.x, 2*x);
    EXPECT_EQ(a.y, 2*y);

    Vector2f c = a - a;
    EXPECT_EQ(c.x, 0);
    EXPECT_EQ(c.y, 0);
}

TEST(Vector2, multiply_divide){
    Vector2f a = Vector2f( x, y );
    Vector2f b = Vector2f( -x, -y );

    a.multiplyScalar( -2 );
    EXPECT_EQ(a.x, x*-2);
    EXPECT_EQ(a.y, y*-2);

    b.multiplyScalar( -2 );
    EXPECT_EQ(b.x, 2*x);
    EXPECT_EQ(b.y, 2*y);

    a.divideScalar( -2 );
    EXPECT_EQ(a.x, x);
    EXPECT_EQ(a.y, y);

    b.divideScalar( -2 );
    EXPECT_EQ(b.x, -x);
    EXPECT_EQ(b.y, -y);
}


TEST(Vector2, min_max_clamp){
    Vector2f a = Vector2f( x, y );
    Vector2f b = Vector2f( -x, -y );
    Vector2f c = Vector2f();

    c.copy( a ).min( b );
    EXPECT_EQ(c.x, -x);
    EXPECT_EQ(c.y, -y);

    c.copy( a ).max( b );
    EXPECT_EQ(c.x, x);
    EXPECT_EQ(c.y, y);

    c.set( -2*x, 2*y );
    c.clamp( b, a );
    EXPECT_EQ(c.x, -x);
    EXPECT_EQ(c.y, y);

    c.set(-2*x, 2*x);
    c.clampScalar( -x, x );
    EXPECT_EQ( c.x, -x );
    EXPECT_EQ( c.y,  x );
}

TEST(Vector2, rounding){
    EXPECT_EQ( Vector2f( -0.1, 0.1 ).floor(), Vector2f( -1,  0 ));
    EXPECT_EQ( Vector2f( -0.5, 0.5 ).floor(), Vector2f( -1,  0 ));
    EXPECT_EQ( Vector2f( -0.9, 0.9 ).floor(), Vector2f( -1,  0 ));

    EXPECT_EQ( Vector2f( -0.1, 0.1 ).ceil(), Vector2f( 0,  1 ));
    EXPECT_EQ( Vector2f( -0.5, 0.5 ).ceil(), Vector2f( 0,  1 ));
    EXPECT_EQ( Vector2f( -0.9, 0.9 ).ceil(), Vector2f( 0,  1 ));

    EXPECT_EQ( Vector2f( -0.1, 0.1 ).round(), Vector2f( 0,  0 ));
    //The following was altered because c++ round goes away from zero, not always higher like javascript.
    EXPECT_EQ( Vector2f( -0.5, 0.5 ).round(), Vector2f( -1,  1 ));
    EXPECT_EQ( Vector2f( -0.9, 0.9 ).round(), Vector2f( -1,  1 ));

    EXPECT_EQ( Vector2f( -0.1, 0.1 ).roundToZero(), Vector2f( 0,  0 ));
    EXPECT_EQ( Vector2f( -0.5, 0.5 ).roundToZero(), Vector2f( 0,  0 ));
    EXPECT_EQ( Vector2f( -0.9, 0.9 ).roundToZero(), Vector2f( 0,  0 ));
    EXPECT_EQ( Vector2f( -1.1, 1.1 ).roundToZero(), Vector2f( -1,  1 ));
    EXPECT_EQ( Vector2f( -1.5, 1.5 ).roundToZero(), Vector2f( -1,  1 ));
    EXPECT_EQ( Vector2f( -1.9, 1.9 ).roundToZero(), Vector2f( -1,  1 ));
}

TEST(Vector2, negate){
    Vector2f a = Vector2f( x, y );

    a.negate();
    EXPECT_EQ(a.x, -x);
    EXPECT_EQ(a.y, -y);
}

TEST(Vector2, dot){
    Vector2f a = Vector2f( x, y );
    Vector2f b = Vector2f( -x, -y );
    Vector2f c = Vector2f();

    float result = a.dot( b );
    EXPECT_EQ(result, (-x*x-y*y));

    result = a.dot( c );
    EXPECT_EQ(result, 0);
}

TEST(Vector2, length_lengthSq){
    Vector2f a = Vector2f( x, 0 );
    Vector2f b = Vector2f( 0, -y );
    Vector2f c = Vector2f();

    EXPECT_EQ(a.length(), x);
    EXPECT_EQ(a.lengthSq(), x*x);
    EXPECT_EQ(b.length(), y);
    EXPECT_EQ(b.lengthSq(), y*y);
    EXPECT_EQ(c.length(), 0);
    EXPECT_EQ(c.lengthSq(), 0);

    a.set( x, y );
    EXPECT_EQ(a.length(), sqrt( x*x + y*y ));
    EXPECT_EQ(a.lengthSq(), ( x*x + y*y ));
}

TEST(Vector2, normalize){
    Vector2f a = Vector2f( x, 0 );
    Vector2f b = Vector2f( 0, -y );
    //Vector2f c = Vector2f(); //Unused

    a.normalize();
    EXPECT_EQ(a.length(), 1);
    EXPECT_EQ(a.x, 1);

    b.normalize();
    EXPECT_EQ(b.length(), 1);
    EXPECT_EQ(b.y, -1);
}

TEST(Vector2, distanceTo_distanceToSquared){
    Vector2f a = Vector2f( x, 0 );
    Vector2f b = Vector2f( 0, -y );
    Vector2f c = Vector2f();

    EXPECT_EQ(a.distanceTo( c ), x);
    EXPECT_EQ(a.distanceToSquared( c ), x*x);

    EXPECT_EQ(b.distanceTo( c ), y);
    EXPECT_EQ(b.distanceToSquared( c ), y*y);
}

TEST(Vector2, setLength){
    Vector2f a = Vector2f( x, 0 );

    EXPECT_EQ(a.length(), x);
    a.setLength( y );
    EXPECT_EQ(a.length(), y);

    a = Vector2f( 0, 0 );
    EXPECT_EQ(a.length(), 0);
    // Invalid in c, dividing by 0 in c results in nan
    // a.setLength( y );
    // EXPECT_EQ(a.length(), 0);
}

TEST(Vector2, lerp_clone){
    Vector2f a = Vector2f( x, 0 );
    Vector2f b = Vector2f( 0, -y );

    EXPECT_EQ(a.lerp( a, 0 ),  a.lerp( a, 0.5 ) );
    EXPECT_EQ(a.lerp( a, 0 ),  a.lerp( a, 1 ) );

    EXPECT_EQ(Vector2f(a).lerp( b, 0 ),  a );

    EXPECT_EQ(Vector2f(a).lerp( b, 0.5 ).x, x*0.5);
    EXPECT_EQ(Vector2f(a).lerp( b, 0.5 ).y, -y*0.5);

    EXPECT_EQ(Vector2f(a).lerp( b, 1 ),  b );
}

TEST(Vector2, equals){
    Vector2f a = Vector2f( x, 0 );
    Vector2f b = Vector2f( 0, -y );

    EXPECT_NE(a.x, b.x);
    EXPECT_NE(a.y, b.y);

    EXPECT_NE( a,  b );
    EXPECT_NE( b,  a );

    a.copy( b );
    EXPECT_EQ(a.x, b.x);
    EXPECT_EQ(a.y, b.y);

    EXPECT_EQ(a,  b );
    EXPECT_EQ(b,  a );
}
