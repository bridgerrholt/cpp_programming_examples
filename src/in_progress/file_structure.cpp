#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <iterator>

#include <string>
#include <vector>
#include <memory>

using namespace std;

std::string const ALLOWED_CHARACTERS =
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "0123456789_-";


// Class declarations

class Directory;

class StructuredObject
{
  public:
    StructuredObject(Directory * parent,
                     string      name);

    virtual
    ~StructuredObject() {}


    virtual
    string getAllowedCharacters() const { return ALLOWED_CHARACTERS; }

    string verifyAllowedName(string name) const;


    string concatFullPath() const;
    string concatFullName() const { return getName() + pathEnding(); }

    virtual
    string pathEnding() const { return ""; }


    string const & getNameRef() const { return name_; }
    string         getName()    const { return name_; }

    Directory * getParent() const { return parent_; }

  private:
    Directory * parent_;
    string      name_;
};

class File;

class Directory : public StructuredObject
{
  public:
    typedef vector<unique_ptr<StructuredObject> > ChildList;

    Directory(Directory * parent,
              string      name) : StructuredObject(parent, name) {}

    void addDirectory(string name);
    void addFile     (string name);

    StructuredObject * findChild(string name);

    ChildList const & getChildrenRef() const { return children_; }

    string pathEnding() const { return "/"; }

  private:
    ChildList children_;
};


class File : public StructuredObject
{
  public:
    File(Directory * parent,
         string      name) : StructuredObject(parent, name) {}

    string getAllowedCharacters() const { return ALLOWED_CHARACTERS + '.'; }

};


// Class definitions

// StructuredObject

StructuredObject::StructuredObject(
  Directory * parent,
  string      name) :

  parent_(parent),
  name_(verifyAllowedName(move(name)))
{

}


string
StructuredObject::verifyAllowedName(string name) const
{
  if (name.find_first_not_of(getAllowedCharacters()) != string::npos)
    throw runtime_error("Name not allowed");

  else return move(name);
}


string
StructuredObject::concatFullPath() const
{
  if (parent_ == nullptr)
    return name_ + pathEnding();
  else
    return parent_->concatFullPath() + name_ + pathEnding();
}



// Directory

void
Directory::addDirectory(string name)
{
  children_.emplace_back(new Directory(this, move(name)));
}

void
Directory::addFile(string name)
{
  children_.emplace_back(new File(this, move(name)));
}


StructuredObject *
Directory::findChild(string name)
{
  for (auto const & i : children_) {
    if (name.back() == '/')
      name.erase(name.size()-1);

    if (i->getNameRef() == name) {
      return i.get();
    }
  }

  return nullptr;
}


// Global functions

vector<string>
extractTokens(string const & str)
{
  vector<string> toReturn;
  bool inItem { false };

  for (auto const & i : str) {
    if (isspace(i)) {
      if (inItem) {
        inItem = false;
      }
    }

    else {
      if (inItem) {
        toReturn.back() += i;
      }
      else {
        inItem = true;
        toReturn.push_back({i});
      }
    }
  }

  return toReturn;
}



bool
checkCorrectTokens(vector<string> & tokens,
                   string const   & name,
                   size_t           paramCount)
{
  if (tokens.at(0) != name)
    return false;

  if (tokens.size() != paramCount + 1) {
    throw runtime_error("Incorrect arguments");
  }

  return true;
}



int main()
{
  string      input;
  Directory * currentDirectory;
  unique_ptr<Directory> rootDirectory;

  cout << "Please enter username: \n";

  while (true) {
    try {
      getline(cin, input);

      rootDirectory.reset(new Directory(nullptr, input));

      currentDirectory = rootDirectory.get();

      break;
    }

    catch (exception const & e) {
      cout << e.what() << '\n';
    }
  }


  while (true) {
    try {
      cout << currentDirectory->concatFullPath() << "> ";

      getline(cin, input);

      auto in = extractTokens(input);

      if (in.empty()) continue;


      if (checkCorrectTokens(in, "quit", 0)) {
        break;
      }

      else if (checkCorrectTokens(in, "mkdir", 1)) {
        currentDirectory->addDirectory(in[1]);
      }

      else if (checkCorrectTokens(in, "touch", 1)) {
        currentDirectory->addFile(in[1]);
      }

      else if (checkCorrectTokens(in, "ls", 0)) {
        for (auto const & i : currentDirectory->getChildrenRef()) {
          cout << i->concatFullName() << '\n';
        }
      }

      else if (checkCorrectTokens(in, "cd", 1)) {
        if (in[1] == "..") {
          if (currentDirectory->getParent() != nullptr)
            currentDirectory = currentDirectory->getParent();
        }

        else {

          StructuredObject * obj { currentDirectory->findChild(in[1]) };

          if (obj == nullptr)
            throw runtime_error("Not a directory");

          else {
            Directory * toGo { dynamic_cast<Directory*>(obj) };
            if (toGo == nullptr)
              throw runtime_error("Must be a directory, not a file");
            else
              currentDirectory = toGo;
          }

        }
      }


    }

    catch (exception const & e) {
      cout << e.what() << '\n';
    }
  }
}