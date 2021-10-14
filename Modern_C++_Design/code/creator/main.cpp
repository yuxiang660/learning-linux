#include <iostream>

template <class T>
struct OpNewCreator
{
   static T* Create()
   {
      std::cout << "OpNewCreator for type: " << typeid(T).name() << std::endl;
      return new T;
   }
};

template <class T>
struct MallocCreator
{
   static T* Create()
   {
      std::cout << "MallocCreator for type: " << typeid(T).name() << std::endl;
      void* buf = std::malloc(sizeof(T));
      if (!buf) return 0;
      return new(buf) T; // new replacement, construct an object for preallocated memory
   }
};

template <class T>
struct PrototypeCreator
{
   explicit PrototypeCreator(T* pObj = 0)
      : pPrototype_(pObj)
   {}

   T* Create()
   {
      std::cout << "PrototypeCreator for type: " << typeid(T).name() << std::endl;
      return pPrototype_ ? pPrototype_->Clone() : 0;
   }

   T* GetPrototype() { return pPrototype_;}

   void SetPrototype(T* pObj) { pPrototype_ = pObj; }

private:
   T* pPrototype_;
};

class Widget
{
public:
   Widget()
   {
      std::cout << "Widget Constructor" << std::endl;
   }
};

template <template <class> class CreationPolicy = OpNewCreator>
class WidgetManager : public CreationPolicy<Widget>
{
public:
   void DoSomething()
   {
      int* pInt = CreationPolicy<int>().Create();
   }
};
typedef WidgetManager<OpNewCreator> MyWidgetMgrNew;
typedef WidgetManager<MallocCreator> MyWidgetMgrMalloc;

int main()
{
   MyWidgetMgrNew mgr_new;
   auto obj_new = mgr_new.Create();
   mgr_new.DoSomething();

   MyWidgetMgrMalloc mgr_malloc;
   auto obj_malloc = mgr_malloc.Create();
   mgr_malloc.DoSomething();

   return 0;
}
