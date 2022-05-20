#include <thread>
#include <string>
#include <shared_mutex>
#include <mutex>
#include <map>
#include <iostream>

std::map<std::string, int> teleBook{
    {"Dijkstra", 1972},
    {"Scott", 1976},
    {"Ritchie", 1983}};

std::shared_mutex teleBookMutex;

void addToTeleBook(const std::string &na, int tele)
{
   std::lock_guard<std::shared_mutex> writeLock(teleBookMutex);
   std::cout << "\nSTARTING UPDATA " << na;
   std::this_thread::sleep_for(std::chrono::milliseconds(500));
   teleBook[na] = tele;
   std::cout << "... ENDING UPDATA " << na << std::endl;
}

void printNumber(const std::string &na)
{
   std::shared_lock<std::shared_mutex> readLock(teleBookMutex);
   if (teleBook.find(na) != teleBook.end())
   {
      printf("%s : %d\n", na.c_str(), teleBook[na]);
   }
   else
   {
      printf("%s is not in the tele book\n", na.c_str());
   }
}

int main()
{
   std::thread reader1([]() { printNumber("Scott"); });
   std::thread reader2([]() { printNumber("Ritchie"); });
   std::thread writer1([]() { addToTeleBook("Scott", 1968); });
   std::thread reader3([]() { printNumber("Dijkstra"); });
   std::thread reader4([]() { printNumber("Scott"); });
   std::thread writer2([]() { addToTeleBook("Bjarne", 1965); });
   std::thread reader5([]() { printNumber("Scott"); });
   std::thread reader6([]() { printNumber("Ritchie"); });
   std::thread reader7([]() { printNumber("Scott"); });
   std::thread reader8([]() { printNumber("Bjarne"); });

   reader1.join();
   reader2.join();
   reader3.join();
   reader4.join();
   reader5.join();
   reader6.join();
   reader7.join();
   reader8.join();
   writer1.join();
   writer2.join();

   std::cout << "\nThe new telephone book:\n";
   for (auto teleIt : teleBook)
   {
      std::cout << teleIt.first << ": " << teleIt.second << std::endl;
   }

   return 0;
}
