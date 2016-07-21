#ifndef MYUPLAY_MYENGINE_CACHEMANAGER
#define MYUPLAY_MYENGINE_CACHEMANAGER

#include <memory>
#include <string>
#include <forward_list>
#include <functional>

namespace MyUPlay {

	namespace MyEngine {

		class ICacheHandle {
		protected:
			long int timeStamp;
		public:
			virtual ~ICacheHandle(){}

			virtual void touch() = 0; //THIS MUST BE CALLED EVERY TIME THE INHERITING CLASS IS TOUCHED OR IT WILL BE DELETED
			virtual void load() = 0; //This is called when the data needs to be created.
			virtual void forget() = 0;
			virtual long int getSize() const = 0; //Returns the size in bytes of ALL the data in the object, including ptrs and their data.
			long int getTimestamp(){
				return timeStamp;
			}
		};

		template <typename T = unsigned char, class T2 = std::allocator<T>>
		class CacheHandle : ICacheHandle {
		private:

			std::shared_ptr<const std::vector<T, T2>> data;
			std::function<std::shared_ptr<const std::vector<T, T2>>()> loader;
			std::function<long int()> sizer;

		public:

			CacheHandle(std::function<std::shared_ptr<const std::vector<T, T2>>()> loader, std::function<long int()> sizer)
				: loader(loader), sizer(sizer) {
				timeStamp = 0;//TODO
				data = loader();
			}

			std::shared_ptr<const std::vector<T, T2>> get(){
				touch();
				if (data){
					load();
				}
				return data;
			}

			void touch() override {
				//TODO
			}

			void load() override {
				data = loader();
			}

			void forget() override {
				data = NULL; //Removes data from ram.
			}

			long int getSize() const override {
				return sizer();
			}

		};

		class CacheManager {
		private:

			std::forward_list<std::weak_ptr<ICacheHandle>> cache;

			void clean(){
				//TODO check if the cache is too full and for weak entries that need to be removed.
			}

			unsigned long maxAllocation = 2e+9; //2GB

		public:

			CacheManager(){}
			CacheManager(unsigned long maxAllocation) : maxAllocation(maxAllocation){}

			template <typename T, class T2 = std::allocator<T> >
			std::shared_ptr<CacheHandle<T, T2>> getCacheHandle(
					std::function<std::shared_ptr<const std::vector<T, T2>>()> loader, std::function<long int()> sizer){
				std::shared_ptr<CacheHandle<T, T2>> ptr = new CacheHandle<T, T2>(loader, sizer);
				cache.push_front(std::weak_ptr<ICacheHandle>(ptr));
				return ptr;
			}

		};

	}

}


#endif
