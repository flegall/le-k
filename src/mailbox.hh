#ifndef mailbox_hh
#define mailbox_hh

#include <SDL.h>
#include <SDL_thread.h>

#include <log.hh>

#include <algorithm>
#include <vector>
using namespace std;

template<class T>
class MailBox
{
   public:
      MailBox() : mutex(SDL_CreateMutex()), producer_id(NULL), consumer_id(NULL)
      {
	 info("");
      };
      ~MailBox()
      {
	 info("");
	 if(NULL != producer_id)
	 {
	    delete producer_id;
	 }
	 if(NULL != consumer_id)
	 {
	    delete consumer_id;
	 }
	 SDL_DestroyMutex(mutex);
      };
      
      void post(const T message)
      {
	 SDL_LockMutex(mutex);
	 const Uint32 id = SDL_ThreadID();
	 bool allow = false;
	 if(NULL == producer_id)
	 {
	    producer_id = new Uint32(id);
	    allow = true;
	 }
	 else
	 {
	    allow = (id == *producer_id);
	 }
	 
	 if(allow)
	 {
	    values.push_back(message);
	 }
	 else
	 {
	    SDL_UnlockMutex(mutex);
	    throw string("Current thread %d, tried to post on mailbox registered by thread %d!!", id, *producer_id);
	 }
	 SDL_UnlockMutex(mutex);
      };
      
      const vector<T> fetchMail()
      {
         SDL_LockMutex(mutex);
	 const Uint32 id = SDL_ThreadID();
	 bool allow = false;
	 if(NULL == consumer_id)
	 {
	    consumer_id = new Uint32(id);
	    allow = true;
	 }
	 else
	 {
	    allow = (id == *consumer_id);
	 }

	 if(allow)
	 {
	    vector<T> result;
	    for(unsigned int i = 0; i < values.size(); i++)
	    {
	       result.push_back(values[i]);
	    }
	    values.clear();
	    SDL_UnlockMutex(mutex);
	    return result;
	 }
	 else
	 {
	    SDL_UnlockMutex(mutex);
	    throw string("Current thread %d, tried to read mailbox registered by thread %d!!", id, *consumer_id);
	 }
      };
      
   private:
      SDL_mutex *mutex;
      vector<T> values;
      Uint32* producer_id;
      Uint32* consumer_id;
};

class Notifier
{
   public:
      virtual void notify() = 0;
      virtual ~Notifier();
};

template <class EffectClass, class T>
class NotifierImpl : public Notifier
{
   public:
      NotifierImpl(shared_ptr<EffectClass> instance, shared_ptr<MailBox<T> > mailbox) :
	 effectInstance(instance),
	 mailBox(mailbox)
      {
	 info("");
      };
      
      void notify()
      {
	 vector<T> mail(mailBox->fetchMail());
	 effectInstance->notify(mail);
      };

      ~NotifierImpl()
      {
	 info("");	 
      };
      
   private:
      shared_ptr<EffectClass> effectInstance;
      shared_ptr<MailBox<T> > mailBox;
};

#endif
