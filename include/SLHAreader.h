#ifndef SLHAREADER_H
#define SLHAREADER_H

#include <map>
#include <string>
#include <vector>

class Item
{
 public:
 Item() :
  comment("NOTFOUND"), value(0), svalue("")
    {}
  ~Item() {}

  std::string comment;
  double value;
  std::string svalue;
};

class SLHAreader
{
 public:
  SLHAreader();
  SLHAreader(std::string _slhafilename);
  ~SLHAreader();

  std::vector<std::string> keys(std::string blockname);
  Item block(std::string key);
  Item decay(std::string key);
  Item operator[](std::string key);
  void writeKeys(std::string filename="slhakeys.txt");
  
 private:
  std::string slhafilename;
  std::map<std::string, Item> blockmap;
  std::map<std::string, Item> decaymap;
  std::map<std::string, std::vector<std::string> > keymap;
  std::vector<std::string> keylist;
};

#endif
