// ---------------------------------------------------------------------------
// Extract blocks from an SLHA file
// Created: A long time ago in a galaxy far far away HBP
// ---------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "TString.h"
#include "SLHAreader.h"
using namespace std;
// ---------------------------------------------------------------------------
namespace {
  bool DEBUG = false;
  void error(std::string message)
  {
    std::cout << "** error ** " << message << std::endl;
    exit(0);
  }

  std::vector<std::string> split(std::string str)
  {
    vector<string> vstr;
    std::istringstream stream(str);
    while ( stream )
      {
	std::string astr;
	stream >> astr;
	if ( stream ) vstr.push_back(astr);
      }
    return vstr;
  }
  
  std::string joinfields(vector<std::string> items, string delim=" ",
			 int start=0, int end=-1)
  {
    if ( start < 0 ) start = 0;
    if ( end < 0 )   end   = static_cast<int>(items.size()-1);
    string record("");
    for(int i=start; i <= end; i++)
      {
	if ( i < end && items[i+1] != ")" )
	  record += items[i] + delim;
	else
	  record += items[i];
      }
    return record;
  }
};
 
SLHAreader::SLHAreader()
  : slhafilename(""),
    blockmap(std::map<std::string, Item>()),
    decaymap(std::map<std::string, Item>()),
    keymap(std::map<std::string, std::vector<std::string> >()),
    keylist(std::vector<std::string>())
{
}

SLHAreader::SLHAreader(std::string _slhafilename)
  : slhafilename(_slhafilename),
    blockmap(std::map<std::string, Item>()),
    decaymap(std::map<std::string, Item>()),
    keymap(std::map<std::string, std::vector<std::string> >()),
    keylist(std::vector<std::string>())
{
  ifstream fin(slhafilename.c_str());
  if ( ! fin.good() ) error(string("can't open file ") + slhafilename);

  vector<string> records;
  string line;
  while ( getline(fin, line) )
    {
      if (line.substr(0, 1) == "#") continue;
      records.push_back(line);
    }
  fin.close();

  // now decode list of strings
  string name("");
  bool InBlock = false;
  int c = -1;
  while ( c < (int)records.size()-1 )
    {
      c++;
      line = records[c];
      size_t ii = line.find("#");
      string comment("");
      if ( ii != string::npos )
	{
	  comment = joinfields(split(line.substr(ii+1)));
	  line = line.substr(0, ii);
	}
      vector<string> vstr = split(line);

      string token = vstr[0];
      if ( token == "BLOCK" ||
	   token == "block" )
	{
	  InBlock = true;
	  name  = vstr[1];
	  keymap[name] = vector<string>();
	  
	  string key = name;
	  Item item;
	  item.comment = comment;
	  if ( vstr.size() > 3 )
	    {
	      item.value = atof(vstr.back().c_str()); // Q
	      blockmap[key] = item;
	      keylist.push_back(key);
	      if ( DEBUG ) printf("BLOCK[%s]: value = %10.4e\n",
				  key.c_str(), item.value);
	    }

	  continue;
	}
      else if ( token == "DECAY" ||
		token == "decay" )
	{
	  InBlock = false;
	  name = vstr[1];
	  keymap[name] = vector<string>();
	  
	  Item item;
	  item.comment = comment;
	  item.value = atof(vstr.back().c_str()); // width
	  string key = name;
	  decaymap[key] = item;
	  keylist.push_back(key);
	  if ( DEBUG ) printf("DECAY[%s]: value = %10.4e\n",
			      key.c_str(), item.value);
	  continue;
	}

      if ( InBlock )
	{
	  // we're in block
	  Item item;
	  item.comment = comment;
	  string key = name;
	  if ( name == "DCINFO" ||
	       name == "dcinfo" ||
	       name == "SPINFO" ||
	       name == "spinfo" )
	    {
	      item.svalue = joinfields(vstr, "", 1);
	      key += string(" ") + vstr[0];
	    }
	  else
	    {
	      item.value = atof(vstr.back().c_str()); // width
	      if ( vstr.size() == 1 )
		key += " 1";
	      else if ( vstr.size() == 2 )
		key += string(" ") + vstr[0];
	      else
		key += string(" ") + vstr[0] + string(" ") + vstr[1];
	    }
	  blockmap[key] = item;
	  blockmap[comment] = item;
	  keymap[name].push_back(key);
	  keylist.push_back(key + string("|") + comment);

	  if ( DEBUG )
	    {
	      if ( name == "DCINFO" ||
		   name == "dcinfo" ||
		   name == "SPINFO" ||
		   name == "spinfo" )
		printf(" item[%s|%s]\t: %s\n",
		       key.c_str(),
		       comment.c_str(),
		       item.svalue.c_str());	    
	      else
		printf(" item[%s|%s]\t: %10.4e\n",
		       key.c_str(),
		       comment.c_str(),
		       item.value);
	    }
	}
      else
	{
	  // we're in a decay
	  Item item;
	  item.comment = comment;
	  item.value = atof(vstr[0].c_str()); // width

	  string key = name + string(" ") +
	    vstr[2] + string(" ") + vstr[3];
	  decaymap[key] = item;
	  decaymap[comment] = item;
	  keymap[name].push_back(key);
	  keylist.push_back(key + string("|") + comment);

	  if ( DEBUG )
	    printf(" item[%s|%s]\t: %8.3e\n",
		   key.c_str(),
		   comment.c_str(),
		   item.value);
	}
    }
}

SLHAreader::~SLHAreader()
{
}

vector<string>
SLHAreader::keys(string blockname)
{
  vector<string> vnull;
  if ( keymap.find(blockname) != keymap.end() )
    return keymap[blockname];
  else
    return vnull;
}

void
SLHAreader::writeKeys(string keyfile)
{
  ofstream fout(keyfile);
  for(size_t c=0; c < keylist.size(); c++)
    fout << keylist[c] << endl;
  fout.close();
}

Item
SLHAreader::block(string key)
{
  string stdkey = joinfields(split(key));
  if ( blockmap.find(stdkey) != blockmap.end() )
    return blockmap[stdkey];
  else
    return Item(); 
}

Item
SLHAreader::decay(string key)
{
  Item item;
  string stdkey = joinfields(split(key));
  if ( decaymap.find(stdkey) != decaymap.end() )
    item = decaymap[stdkey];
  return item;
}

Item SLHAreader::operator[](string key)
{
  Item item = block(key);
  if ( item.comment == "NOTFOUND" )
    item = decay(key);
  return item;
}
