
#include "Vector2.hpp"
#include "Math.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

  template <typename T>
  Vector2<T>& Vector2<T>::min(const Vector2<T>& v){

      if (x > v.x){
          x = v.x;
      }

      if (y > v.y){
          x = v.y;
      }

        return *this;

  }

  template <typename T>
  Vector2<T>& Vector2<T>::max(const Vector2<T>& v){

      if (x < v.x){
          x = v.x;
      }

      if (y < v.y){
          y = v.y;
      }

        return *this;

  }


template <typename T>
Vector2<T>& Vector2<T>::clamp(const Vector2<T>& min, const Vector2<T>& max){

      if (x < min.x) {
          x = min.x;
      } else if (x > max.x) {
          x = max.x;
      }

      if (y < min.y) {
          y = min.y;
      } else if (y > max.y) {
          y = max.y;
      }
     return *this;

  }


  template <typename T>
  Vector2<T>& Vector2<T>::clampScalar(T min, T max) {

      Vector2 low(min, min), high(max, max);

      return clamp(low, high);

  }



