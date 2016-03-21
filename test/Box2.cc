#include "gtest/gtest.h"
#include "unit_test_constants.hpp"
#include "Box2.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;


TEST(Box2, constructor){
   Box2f a;
   EXPECT_EQ(posInf2, a.min);
   EXPECT_EQ(negInf2, a.max);

   a = Box2f(zero2, zero2);
   EXPECT_TRUE( a.min == zero2 );
   EXPECT_TRUE( a.max == zero2 );

   a = Box2f( zero2, one2 );
   EXPECT_TRUE( a.min == zero2 );
   EXPECT_TRUE( a.max == one2 );
};

TEST(Box2 , copy ){
   Box2f a( zero2, one2 );
   Box2f b = Box2f(a);
   EXPECT_EQ( b.min ,  zero2 ) ;
   EXPECT_EQ( b.max ,  one2 ) ;

   // ensure that it is a true copy
   a.min = zero2;
   a.max = one2;
   EXPECT_EQ( b.min ,  zero2 ) ;
   EXPECT_EQ( b.max ,  one2 ) ;
};

TEST(Box2 , set ){
   Box2f a = Box2f();

   a.set( zero2, one2 );
   EXPECT_EQ( a.min ,  zero2 ) ;
   EXPECT_EQ( a.max ,  one2 ) ;
};

TEST(Box2 , setFromPoints ){
   Box2f a;

   vector<const Vector2<float> > temp[3] = { zero2, one2, two2 };

   a.setFromPoints( temp );
   EXPECT_EQ( a.min ,  zero2 ) ;
   EXPECT_EQ( a.max ,  two2 ) ;

   vector<const Vector2<float> > temp2[1] = { one2 };

   a.setFromPoints( temp2 );
   EXPECT_EQ( a.min ,  one2 ) ;
   EXPECT_EQ( a.max ,  one2 ) ;

   a.setFromPoints( [] );
   EXPECT_TRUE( a.empty()) ;
};

TEST(Box2 , empty_makeEmpty ){
   Box2f a = Box2f();

   EXPECT_TRUE( a.empty()) ;

   a = Box2f( zero2, one2 );
   EXPECT_TRUE( ! a.empty()) ;

   a.makeEmpty();
   EXPECT_TRUE( a.empty()) ;
};

TEST(Box2 , center ){
   Box2f a = Box2f( zero2, zero2 );

   EXPECT_EQ( a.center() ,  zero2 ) ;

   a = Box2f( zero2, one2 );
   Box2f midpoint = one2.multiplyScalar( 0.5 );
   EXPECT_EQ( a.center() ,  midpoint ) ;
};

TEST(Box2 , size ){
   Box2f a = Box2f( zero2, zero2 );

   EXPECT_EQ( a.size() ,  zero2 ) ;

   a = Box2f( zero2, one2 );
   EXPECT_EQ( a.size() ,  one2 ) ;
};

TEST(Box2 , expandByPoint ){
   Box2f a = Box2f( zero2, zero2 );

   a.expandByPoint( zero2 );
   EXPECT_EQ( a.size() ,  zero2 ) ;

   a.expandByPoint( one2 );
   EXPECT_EQ( a.size() ,  one2 ) ;

   a.expandByPoint( one2.negate() );
   EXPECT_EQ( a.size() ,  one2.multiplyScalar( 2 ) ) ;
   EXPECT_EQ( a.center() ,  zero2 ) ;
};

TEST(Box2 , expandByVector ){
   Box2f a = Box2f( zero2, zero2 );

   a.expandByVector( zero2 );
   EXPECT_EQ( a.size() ,  zero2 ) ;

   a.expandByVector( one2 );
   EXPECT_EQ( a.size() ,  one2.multiplyScalar( 2 ) ) ;
   EXPECT_EQ( a.center() ,  zero2 ) ;
};

TEST(Box2 , expandByScalar ){
   Box2f a = Box2f( zero2, zero2 );

   a.expandByScalar( 0 );
   EXPECT_EQ( a.size() ,  zero2 ) ;

   a.expandByScalar( 1 );
   EXPECT_EQ( a.size() ,  one2.multiplyScalar( 2 ) ) ;
   EXPECT_EQ( a.center() ,  zero2 ) ;
};

TEST(Box2 , containsPoint ){
   Box2f a = Box2f( zero2, zero2 );

   EXPECT_TRUE( a.containsPoint( zero2 ) ;
   EXPECT_TRUE( ! a.containsPoint( one2 ) ;

   a.expandByScalar( 1 );
   EXPECT_TRUE( a.containsPoint( zero2 ) ;
   EXPECT_TRUE( a.containsPoint( one2 ) ;
   EXPECT_TRUE( a.containsPoint( one2.negate() ) ;
};

TEST(Box2 , containsBox ){
   Box2f a = Box2f( zero2, zero2 );
   Box2f b = Box2f( zero2, one2 );
   Box2f c = Box2f( one2.negate(), one2 );

   EXPECT_TRUE( a.containsBox( a ) ;
   EXPECT_TRUE( ! a.containsBox( b ) ;
   EXPECT_TRUE( ! a.containsBox( c ) ;

   EXPECT_TRUE( b.containsBox( a ) ;
   EXPECT_TRUE( c.containsBox( a ) ;
   EXPECT_TRUE( ! b.containsBox( c ) ;
};

TEST(Box2 , getParameter ){
   Box2f a = Box2f( zero2, one2 );
   Box2f b = Box2f( one2.negate(), one2 );

   EXPECT_EQ( a.getParameter( Vector2( 0, 0 ) ) ,  Vector2( 0, 0 ) ) ;
   EXPECT_EQ( a.getParameter( Vector2( 1, 1 ) ) ,  Vector2( 1, 1 ) ) ;

   EXPECT_EQ( b.getParameter( Vector2( -1, -1 ) ) , Vector2( 0, 0 ) ) ;
   EXPECT_EQ( b.getParameter( Vector2( 0, 0 ) ) ,  Vector2( 0.5, 0.5 ) ) ;
   EXPECT_EQ( b.getParameter( Vector2( 1, 1 ) ) ,  Vector2( 1, 1 ) ) ;
};

TEST(Box2 , clampPoint ){
   Box2f a = Box2f( zero2, zero2 );
   Box2f b = Box2f( one2.negate(), one2 );

   EXPECT_EQ( a.clampPoint( Vector2( 0, 0 ) ) ,  Vector2( 0, 0 ) ) ;
   EXPECT_EQ( a.clampPoint( Vector2( 1, 1 ) ) ,  Vector2( 0, 0 ) ) ;
   EXPECT_EQ( a.clampPoint( Vector2( -1, -1 ) ) , Vector2( 0, 0 ) ) ;

   EXPECT_EQ( b.clampPoint( Vector2( 2, 2 ) ) ,  Vector2( 1, 1 ) ) ;
   EXPECT_EQ( b.clampPoint( Vector2( 1, 1 ) ) ,  Vector2( 1, 1 ) ) ;
   EXPECT_EQ( b.clampPoint( Vector2( 0, 0 ) ) ,  Vector2( 0, 0 ) ) ;
   EXPECT_EQ( b.clampPoint( Vector2( -1, -1 ) ) , Vector2( -1, -1 ) ) ;
   EXPECT_EQ( b.clampPoint( Vector2( -2, -2 ) ) , Vector2( -1, -1 ) ) ;
};

TEST(Box2 , distanceToPoint ){
   Box2f a = Box2f( zero2, zero2 );
   Box2f b = Box2f( one2.negate(), one2 );

   EXPECT_TRUE( a.distanceToPoint( Vector2( 0, 0 ) ) == 0 ;
   EXPECT_TRUE( a.distanceToPoint( Vector2( 1, 1 ) ) == Math.sqrt( 2 ) ;
   EXPECT_TRUE( a.distanceToPoint( Vector2( -1, -1 ) ) == Math.sqrt( 2 ) ;

   EXPECT_TRUE( b.distanceToPoint( Vector2( 2, 2 ) ) == Math.sqrt( 2 ) ;
   EXPECT_TRUE( b.distanceToPoint( Vector2( 1, 1 ) ) == 0 ;
   EXPECT_TRUE( b.distanceToPoint( Vector2( 0, 0 ) ) == 0 ;
   EXPECT_TRUE( b.distanceToPoint( Vector2( -1, -1 ) ) == 0 ;
   EXPECT_TRUE( b.distanceToPoint( Vector2( -2, -2 ) ) == Math.sqrt( 2 ) ;
};

TEST(Box2 , isIntersectionBox ){
   Box2f a = Box2f( zero2, zero2 );
   Box2f b = Box2f( zero2, one2 );
   Box2f c = Box2f( one2.negate(), one2 );

   EXPECT_TRUE( a.isIntersectionBox( a ) ;
   EXPECT_TRUE( a.isIntersectionBox( b ) ;
   EXPECT_TRUE( a.isIntersectionBox( c ) ;

   EXPECT_TRUE( b.isIntersectionBox( a ) ;
   EXPECT_TRUE( c.isIntersectionBox( a ) ;
   EXPECT_TRUE( b.isIntersectionBox( c ) ;

   b.translate( Vector2( 2, 2 ) );
   EXPECT_TRUE( ! a.isIntersectionBox( b ) ;
   EXPECT_TRUE( ! b.isIntersectionBox( a ) ;
   EXPECT_TRUE( ! b.isIntersectionBox( c ) ;
});

TEST(Box2 , intersect ){
   Box2f a = Box2f( zero2, zero2 );
   Box2f b = Box2f( zero2, one2 );
   Box2f c = Box2f( one2.negate(), one2 );

   EXPECT_EQ( a.intersect( a ) ,  a ) ;
   EXPECT_EQ( a.intersect( b ) ,  a ) ;
   EXPECT_EQ( b.intersect( b ) ,  b ) ;
   EXPECT_EQ( a.intersect( c ) ,  a ) ;
   EXPECT_EQ( b.intersect( c ) ,  b ) ;
   EXPECT_EQ( c.intersect( c ) ,  c ) ;
});

TEST(Box2 , union ){
   Box2f a = Box2f( zero2, zero2 );
   Box2f b = Box2f( zero2, one2 );
   Box2f c = Box2f( one2.negate(), one2 );

   EXPECT_EQ( a.union( a ) ,  a ) ;
   EXPECT_EQ( a.union( b ) ,  b ) ;
   EXPECT_EQ( a.union( c ) ,  c ) ;
   EXPECT_EQ( b.union( c ) ,  c ) ;
});

TEST(Box2 , translate ){
   Box2f a = Box2f( zero2, zero2 );
   Box2f b = Box2f( zero2, one2 );
   Box2f c = Box2f( one2.negate(), one2 );
   Box2f d = Box2f( one2.negate(), zero2 );

   EXPECT_EQ( a.translate( one2 ) ,  Box2f( one2, one2 ) ) ;
   EXPECT_EQ( a.translate( one2 ).translate( one2.negate() ) ,  a ) ;
   EXPECT_EQ( d.translate( one2 ) ,  b ) ;
   EXPECT_EQ( b.translate( one2.negate() ) ,  d ) ;
});
