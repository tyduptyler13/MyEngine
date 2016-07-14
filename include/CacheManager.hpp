#ifndef MYUPLAY_MYENGINE_CACHEMANAGER
#define MYUPLAY_MYENGINE_CACHEMANAGER

#include <memory>
#include <string>
#include <queue>
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
            long int getTimestamp(){
                return timeStamp;
            }
        };

        template <typename T>
        class CacheHandle : ICacheHandle {
            private:

            std::shared_ptr<T> data;

            public:

            std::function<std::shared_ptr<T>()> loader;

            CacheHandle(std::shared_ptr<T>()> loader) : loader(loader) {
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

        }

        class CacheManager {
            private:

                std::priority_queue<std::shared_ptr<ICachePiece>> cache;

            public:

                long int allocatedRam;

                template <typename T>
                CachePiece<T> getCacheHandle(std::function<std::shared_ptr<T>()> loader){
                    return CacheHandle(loader);
                }


        };

    }

}


#endif
