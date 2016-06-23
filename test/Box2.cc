#include "gtest/gtest.h"
#include "constants.hpp"
#include "Box2.hpp"

#include <cmath>

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace Test;

TEST(Box2, constructor){
  Box2f a;
  EXPECT_EQ(posInf2, a.min);
  EXPECT_EQ(negInf2, a.max);

  a = Box2f(zero2, zero2);
  EXPECT_EQ( a.min, zero2 );
  EXPECT_EQ( a.max, zero2 );

  a = Box2f( zero2, one2 );
  EXPECT_EQ( a.min, zero2 );
  EXPECT_EQ( a.max, one2 );
}

TEST(Box2 , copy ){
  Box2f a( zero2, one2 );
  Box2f b = Box2f(a);
  EXPECT_EQ( b.min ,  zero2 );
  EXPECT_EQ( b.max ,  one2 );

  // ensure that it is a true copy
  a.min = zero2;
  a.max = one2;
  EXPECT_EQ( b.min ,  zero2 );
  EXPECT_EQ( b.max ,  one2 );
}

TEST(Box2 , set ){
  Box2f a = Box2f();

  a.set( zero2, one2 );
  EXPECT_EQ( a.min ,  zero2 );
  EXPECT_EQ( a.max ,  one2 );
}

TEST(Box2 , setFromPoints ){
  Box2f a;

  vector<Vector2<float> > temp = { zero2, one2, two2 };

  a.setFromPoints( temp );
  EXPECT_EQ( a.min ,  zero2 );
  EXPECT_EQ( a.max ,  two2 );

  vector<Vector2<float> > temp2 = { one2 };

  a.setFromPoints( temp2 );
  EXPECT_EQ( a.min ,  one2 );
  EXPECT_EQ( a.max ,  one2 );

  a.setFromPoints( {} );
  EXPECT_TRUE( a.empty());
}

TEST(Box2 , empty_makeEmpty ){
  Box2f a = Box2f();

  EXPECT_TRUE( a.empty());

  a = Box2f( zero2, one2 );
  EXPECT_TRUE( ! a.empty());

  a.makeEmpty();
  EXPECT_TRUE( a.empty());
}

TEST(Box2 , center ){
  Box2f a = Box2f( zero2, zero2 );

  EXPECT_EQ( a.center() ,  zero2 );

  a = Box2f( zero2, one2 );
  Vector2f midpoint = one2 * 0.5;
  EXPECT_EQ( a.center() ,  midpoint );
}

TEST(Box2 , size ){
  Box2f a = Box2f( zero2, zero2 );

  EXPECT_EQ( a.size() ,  zero2 );

  a = Box2f( zero2, one2 );
  EXPECT_EQ( a.size() ,  one2 );
}

TEST(Box2 , expandByPoint ){
  Box2f a = Box2f( zero2, zero2 );

  a.expandByPoint( zero2 );
  EXPECT_EQ( a.size() ,  zero2 );

  a.expandByPoint( one2 );
  EXPECT_EQ( a.size() ,  one2 );

  a.expandByPoint( -one2 );
  EXPECT_EQ( a.size() ,  one2 * 2 );
  EXPECT_EQ( a.center() ,  zero2 );
}

TEST(Box2 , expandByVector ){
  Box2f a = Box2f( zero2, zero2 );

  a.expandByVector( zero2 );
  EXPECT_EQ( a.size() ,  zero2 );

  a.expandByVector( one2 );
  EXPECT_EQ( a.size() ,  one2 * 2 );
  EXPECT_EQ( a.center() ,  zero2 );
}

TEST(Box2 , expandByScalar ){
  Box2f a = Box2f( zero2, zero2 );

  a.expandByScalar( 0 );
  EXPECT_EQ( a.size() ,  zero2 );

  a.expandByScalar( 1 );
  EXPECT_EQ( a.size() ,  one2 * 2 );
  EXPECT_EQ( a.center() ,  zero2 );
}

TEST(Box2 , containsPoint ){
  Box2f a = Box2f( zero2, zero2 );

  EXPECT_TRUE( a.containsPoint( zero2 ));
  EXPECT_TRUE( ! a.containsPoint( one2 ));

  a.expandByScalar( 1 );
  EXPECT_TRUE( a.containsPoint( zero2 ));
  EXPECT_TRUE( a.containsPoint( one2 ));
  EXPECT_TRUE( a.containsPoint( -one2 ));
}

TEST(Box2 , containsBox ){
  Box2f a = Box2f( zero2, zero2 );
  Box2f b = Box2f( zero2, one2 );
  Box2f c = Box2f( -one2, one2 );

  EXPECT_TRUE( a.containsBox( a ));
  EXPECT_TRUE( ! a.containsBox( b ));
  EXPECT_TRUE( ! a.containsBox( c ));

  EXPECT_TRUE( b.containsBox( a ));
  EXPECT_TRUE( c.containsBox( a ));
  EXPECT_TRUE( ! b.containsBox( c ));
}

TEST(Box2 , getParameter ){
  Box2f a = Box2f( zero2, one2 );
  Box2f b = Box2f( -one2, one2 );

  EXPECT_EQ( a.getParameter( Vector2f( 0, 0 ) ) ,  Vector2f( 0, 0 ) );
  EXPECT_EQ( a.getParameter( Vector2f( 1, 1 ) ) ,  Vector2f( 1, 1 ) );

  EXPECT_EQ( b.getParameter( Vector2f( -1, -1 ) ) , Vector2f( 0, 0 ) );
  EXPECT_EQ( b.getParameter( Vector2f( 0, 0 ) ) ,  Vector2f( 0.5, 0.5 ) );
  EXPECT_EQ( b.getParameter( Vector2f( 1, 1 ) ) ,  Vector2f( 1, 1 ) );
}

TEST(Box2 , clampPoint ){
  Box2f a = Box2f( zero2, zero2 );
  Box2f b = Box2f( -one2, one2 );

  EXPECT_EQ( a.clampPoint( Vector2f( 0, 0 ) ) ,  Vector2f( 0, 0 ) );
  EXPECT_EQ( a.clampPoint( Vector2f( 1, 1 ) ) ,  Vector2f( 0, 0 ) );
  EXPECT_EQ( a.clampPoint( Vector2f( -1, -1 ) ) , Vector2f( 0, 0 ) );

  EXPECT_EQ( b.clampPoint( Vector2f( 2, 2 ) ) ,  Vector2f( 1, 1 ) );
  EXPECT_EQ( b.clampPoint( Vector2f( 1, 1 ) ) ,  Vector2f( 1, 1 ) );
  EXPECT_EQ( b.clampPoint( Vector2f( 0, 0 ) ) ,  Vector2f( 0, 0 ) );
  EXPECT_EQ( b.clampPoint( Vector2f( -1, -1 ) ) , Vector2f( -1, -1 ) );
  EXPECT_EQ( b.clampPoint( Vector2f( -2, -2 ) ) , Vector2f( -1, -1 ) );
}

TEST(Box2 , distanceToPoint ){
  Box2f a = Box2f( zero2, zero2 );
  Box2f b = Box2f( -one2, one2 );

  EXPECT_EQ( a.distanceToPoint( Vector2f( 0, 0 ) ), 0 );
  EXPECT_EQ( a.distanceToPoint( Vector2f( 1, 1 ) ), sqrt( 2.0f ) );
  EXPECT_EQ( a.distanceToPoint( Vector2f( -1, -1 ) ), sqrt( 2.0f ) );

  EXPECT_EQ( b.distanceToPoint( Vector2f( 2, 2 ) ), sqrt( 2.0f ) );
  EXPECT_EQ( b.distanceToPoint( Vector2f( 1, 1 ) ), 0 );
  EXPECT_EQ( b.distanceToPoint( Vector2f( 0, 0 ) ), 0 );
  EXPECT_EQ( b.distanceToPoint( Vector2f( -1, -1 ) ), 0 );
  EXPECT_EQ( b.distanceToPoint( Vector2f( -2, -2 ) ), sqrt( 2.0f ) );
}

TEST(Box2 , isIntersectionBox ){
  Box2f a = Box2f( zero2, zero2 );
  Box2f b = Box2f( zero2, one2 );
  Box2f c = Box2f( -one2, one2 );

  EXPECT_TRUE( a.isIntersectionBox( a ) );
  EXPECT_TRUE( a.isIntersectionBox( b ) );
  EXPECT_TRUE( a.isIntersectionBox( c ) );

  EXPECT_TRUE( b.isIntersectionBox( a ) );
  EXPECT_TRUE( c.isIntersectionBox( a ) );
  EXPECT_TRUE( b.isIntersectionBox( c ) );

  b.translate( Vector2f( 2, 2 ) );
  EXPECT_TRUE( ! a.isIntersectionBox( b ) );
  EXPECT_TRUE( ! b.isIntersectionBox( a ) );
  EXPECT_TRUE( ! b.isIntersectionBox( c ) );
}

TEST(Box2 , intersect ){
  Box2f a = Box2f( zero2, zero2 );
  Box2f b = Box2f( zero2, one2 );
  Box2f c = Box2f( -one2, one2 );

  EXPECT_EQ( a.intersect( a ) ,  a );
  EXPECT_EQ( a.intersect( b ) ,  a );
  EXPECT_EQ( b.intersect( b ) ,  b );
  EXPECT_EQ( a.intersect( c ) ,  a );
  EXPECT_EQ( b.intersect( c ) ,  b );
  EXPECT_EQ( c.intersect( c ) ,  c );
}

TEST(Box2 , union ){
  Box2f a = Box2f( zero2, zero2 );
  Box2f b = Box2f( zero2, one2 );
  Box2f c = Box2f( -one2, one2 );

  EXPECT_EQ( a.boxUnion( a ) ,  a );
  EXPECT_EQ( a.boxUnion( b ) ,  b );
  EXPECT_EQ( a.boxUnion( c ) ,  c );
  EXPECT_EQ( b.boxUnion( c ) ,  c );
}

TEST(Box2 , translate ){
  Box2f a = Box2f( zero2, zero2 );
  Box2f b = Box2f( zero2, one2 );
  //Box2f c = Box2f( one2.negate(), one2 ); Unused
  Box2f d = Box2f( -one2, zero2 );

  EXPECT_EQ( Box2f(a).translate( one2 ) ,  Box2f( one2, one2 ) );
  EXPECT_EQ( Box2f(a).translate( one2 ).translate( -one2 ) ,  a );
  EXPECT_EQ( Box2f(d).translate( one2 ) ,  b );
  EXPECT_EQ( Box2f(b).translate( -one2 ) ,  d );
}
