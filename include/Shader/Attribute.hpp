/*
 * ShaderAttribute.hpp
 *
 *  Created on: Jul 13, 2016
 *      Author: tyler
 */

#ifndef MYUPLAY_MYENGINE_ATTRIBUTE
#define MYUPLAY_MYENGINE_ATTRIBUTE

#include <array>
#include <functional>
#include <exception>

#include "Shader/ShaderNode.hpp"

namespace MyUPlay{
	namespace MyEngine {

		class GLES2Renderer;

		namespace Shader {

			enum ShaderScope {
				PerPrimitive, //Accessible in vertex and frag
				PerVertex //Accessible only in vertex, must be interpolated for frag
			};

			struct IAttribute : public IShaderNode {
				ShaderScope scope;

				virtual ~IAttribute(){}

				//This will be called before every shader process.
				virtual void bind() = 0;

				/**
				 * Attributes don't have instances, instead you use its name.
				 */
				std::string getInstance() const override {
					return "";
				}

			};

			/**
			 * MappedArrayAttribute
			 *
			 * The value in this array exactly matches that in gpu memory. The memory is
			 * mapped between system and gpu memory for easy modification. In order to resize,
			 * additional operations may need to first happen. This class is specialized for every
			 * renderer through template specialization.
			 *
			 * @param name - A required name for the attribute. Randomly generated names are not provided automatically.
			 * @param size - An initial size to create as shared memory.
			 */
			template <class R, typename T>
			class MappedArrayAttribute : public IAttribute {
			private:
				T* value;

			public:

				Output<T> out;
				unsigned size;

				MappedArrayAttribute(std::string name, unsigned size)
				: out(name), size(size) {
					value = new T[size];
				}

				~MappedArrayAttribute(){
					delete[] value;
				}

				T& operator[](unsigned i){
					if (i >= size) throw std::range_error("Attempt to access data out of range!");
					return value[i];
				}

				virtual void resize(unsigned size) = 0;

			};

			/**
			 * This is the standard method to create array attributes. In openGL this translates to an ArrayBuffer
			 *
			 * It is the responsibility of the designer to implement the isDirty and update methods.
			 * It is recommended to either extend this class or create an unnamed in place class.
			 *
			 * isDirty will be called at every frame and if it returns true, then update will also
			 * be called to collect a new array of data and push it to the gpu. Keep in mind that this
			 * is usually the most expensive operation in a game engine as bandwidth is slow.
			 *
			 * isDirty also has a start and length parameter passed into the call, should you choose to update
			 * these, the program will do its best to cut down on bandwidth by only updating part of the data
			 * that needs to be udpated. Should this be used, update will also pass a non 0 value into the
			 * start parameter to let you know where the array is offset to begin its update.
			 * Note:Some renderers may still ignore start and len and not set start as it might not be supported.
			 *
			 * Finally, the type on this class is recommended to be a basic data type and then use
			 */
			template <class R, typename T>
			struct ArrayAttribute : public IAttribute {
				Output<T> out;
				unsigned size;

				ArrayAttribute(std::string name, unsigned size) : out(name), size(size) {}

				virtual bool isDirty(unsigned& start, unsigned& len) = 0;
				virtual void update(T*, unsigned len, unsigned start = 0) = 0; //As soon as this returns, the data at T* will be pushed.
				virtual void resize(unsigned size) = 0; //Calling this will throw away old data and call update for new data.

			};

			/**
			 * This is a single value Attribute. Its value is pushed to the gpu at every frame. If you want to
			 * keep data on a gpu, it is recommended to use an ArrayAttribute to store it and all similar values
			 * and then use a normal attribute to mark the index of that object you are referring to.
			 *
			 * Expect update to be called every time the renderer needs its value.
			 */
			template <class R, typename T>
			struct Attribute : public IAttribute {
				bool dirty = true;
				Output<T> out;

				Attribute(std::string name) : out(name) {}

				virtual T update() = 0;

			};

		}
	}
}

#endif /* INCLUDE_SHADER_SHADERATTRIBUTE_HPP_ */
