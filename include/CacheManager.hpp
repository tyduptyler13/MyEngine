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
            virtual ~CachePiece(){}

            virtual void touch() = 0; //THIS MUST BE CALLED EVERY TIME THE INHERITING CLASS IS TOUCHED OR IT WILL BE DELETED
            virtual void load() = 0; //This is called when the data needs to be created.
            virtual void forget() = 0;
            virtual long int getSize() const = 0; //Returns the size in bytes of ALL the data in the object, including ptrs and their data.
            long int getTimestamp(){
                return timeStamp;
            }
        };

        template <typename T>
        class CacheHandle : ICacheHandle {
            private:

            std::shared_ptr<T> data;
            std::function<std::shared_ptr<T>()> loader;
            std::function<long int()> sizer;

            public:

            CacheHandle(std::function<shared_ptr<T>()> loader, std::function<long int()> sizer) : loader(loader), sizer(sizer) {
                timeStamp = 0;//TODO
                data = loader();
            }

            std::shared_ptr<T> get(){
                touch();
                if (data == NULL){
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

        }

        class CacheManager {
            private:

                std::forward_list<std::weak_ptr<ICachePiece>> cache;

                void clean(){
                    //TODO check if the cache is too full and for weak entries that need to be removed.
                }

            public:

                long int allocatedRam;

                template <typename T>
                shared_ptr<CachePiece<T>> getCacheHandle(std::function<std::shared_ptr<T>()> loader, std::function<long int()> sizer){
                    shared_ptr<CacheHandle<T>> ptr(new CacheHandle<T>(loader, sizer));
                    cache.emplace_front(ptr);
                    return ptr;
                }

        };

    }

}


#endif
