// Example File Structure
// Covers polymorphism.

#include <iostream>
#include <stdexcept>

#include <string>
#include <vector>
#include <memory>

using namespace std;

// Characters always allowed within our directory names and file names.
std::string const ALLOWED_CHARACTERS =
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"0123456789_-";


// -- Class declarations --

class StructuredObject;

class Directory;

class File;


// Base class for directories and files.
class StructuredObject
{
	public:
		StructuredObject(Directory    * parent,
		                 string         name,
		                 string const & allowedCharacters);

		virtual
		~StructuredObject() {}

		virtual
		string getAllowedCharacters() const { return ALLOWED_CHARACTERS; }

		string concatPath() const;

		string concatPathFull() const { return concatPath() + pathEnding(); }

		string concatFullName() const { return getName()    + pathEnding(); }

		virtual
		string pathEnding() const { return ""; }


		string const & getNameRef() const { return name_; }

		string getName() const { return name_; }

		Directory * getParent() const { return parent_; }

	private:
		string verifyAllowedName(string         name,
		                         string const & allowedCharacters) const;

		string concatPathRecursive() const;

		Directory * parent_;
		string      name_;
};


class Directory : public StructuredObject
{
	public:
		typedef vector<unique_ptr<StructuredObject> > ChildList;

		Directory(Directory * parent,
		          string      name);

		StructuredObject * findChild(string name);

		ChildList const & getChildrenRef() const { return children_; }

		string pathEnding() const { return "/"; }

		void executeMkdir(string name);

		void executeTouch(string name);

		void executeLs();

	private:
		StructuredObject * findChildClean(string const & name);

		ChildList children_;
};


class File : public StructuredObject {
	public:
		File(Directory * parent,
		     string      name);

		string getAllowedCharacters() const { return ALLOWED_CHARACTERS + "."; }

};



// -- Global function definitions --

vector<string>
extractTokens(string const & str)
{
	vector<string> toReturn;
	bool inItem {false};

	for (auto const &i : str) {
		if (isspace(i)) {
			if (inItem) {
				inItem = false;
			}
		} else {
			if (inItem) {
				toReturn.back() += i;
			} else {
				inItem = true;
				toReturn.push_back({i});
			}
		}
	}

	return toReturn;
}


bool
checkCorrectTokens(vector<string> & tokens,
                   string   const & name,
                   size_t           paramCount)
{
	if (tokens.at(0) != name)
		return false;

	if (tokens.size() != paramCount + 1) {
		throw runtime_error("Incorrect arguments");
	}

	return true;
}


void eraseIfEquals(string &str,
                   size_t pos,
                   char desired)
{
	if (str.at(pos) == desired)
		str.erase(pos);
}



// -- Member function definitions --

// StructuredObject

StructuredObject::StructuredObject(
	Directory    * parent,
	string         name,
	string const & allowedCharacters) :

	parent_(parent),
	name_(verifyAllowedName(move(name), allowedCharacters))
{

}


string
StructuredObject::verifyAllowedName(
	string         name,
	string const & allowedCharacters) const
{
	if (name.find_first_not_of(allowedCharacters) != string::npos)
		throw runtime_error("Name not allowed");

	else return move(name);
}


string
StructuredObject::concatPath() const
{
	if (parent_ == nullptr)
		return name_;
	else
		return parent_->concatPathRecursive() + name_;
}


string
StructuredObject::concatPathRecursive() const
{
	if (parent_ == nullptr)
		return concatFullName();
	else
		return parent_->concatPathRecursive() + concatFullName();
}





// Directory

Directory::Directory(Directory * parent,
                     string      name) :
	StructuredObject(parent,
	                 name,
	                 getAllowedCharacters())
{

}


StructuredObject *
Directory::findChild(string name) {
	eraseIfEquals(name, name.size() - 1, '/');

	return findChildClean(name);
}


StructuredObject *
Directory::findChildClean(string const & name)
{
	for (auto const &i : children_) {
		if (i->getNameRef() == name) {
			return i.get();
		}
	}

	return nullptr;
}


void
Directory::executeMkdir(string name)
{
	eraseIfEquals(name, name.size() - 1, '/');

	if (findChildClean(name) == nullptr)
		children_.emplace_back(new Directory(this, move(name)));

	else
		throw runtime_error("Already exists");
}

void
Directory::executeTouch(string name)
{
	if (findChildClean(name) == nullptr)
		children_.emplace_back(new File(this, move(name)));

	else
		throw runtime_error("Already exists");
}

void
Directory::executeLs()
{
	for (auto const &i : children_) {
		cout << "  " << i->concatFullName() << '\n';
	}
}



// File

File::File(Directory * parent,
           string      name) :
	StructuredObject(parent,
	                 name,
	                 getAllowedCharacters())
{
	if (getNameRef() == ".." || getNameRef() == ".")
		throw runtime_error("Name not allowed");
}



// -- Main function --

int main()
{
	string input;
	Directory *currentDirectory;
	unique_ptr<Directory> rootDirectory;

	cout << "Please enter username: \n";

	while (true) {
		try {
			getline(cin, input);

			rootDirectory.reset(new Directory(nullptr, input));

			currentDirectory = rootDirectory.get();

			break;
		}

		catch (exception const &e) {
			cout << e.what() << '\n';
		}
	}


	while (true) {
		try {
			cout << currentDirectory->concatPath() << "> ";

			getline(cin, input);

			auto in = extractTokens(input);

			if (in.empty()) continue;


			if (checkCorrectTokens(in, "quit", 0)) {
				break;
			} else if (checkCorrectTokens(in, "mkdir", 1)) {
				currentDirectory->executeMkdir(in[1]);
			} else if (checkCorrectTokens(in, "touch", 1)) {
				currentDirectory->executeTouch(in[1]);
			} else if (checkCorrectTokens(in, "ls", 0)) {
				currentDirectory->executeLs();
			} else if (checkCorrectTokens(in, "cd", 1)) {
				if (in[1] == "..") {
					if (currentDirectory->getParent() != nullptr)
						currentDirectory = currentDirectory->getParent();
				} else if (in[1] == "/") {
					currentDirectory = rootDirectory.get();
				} else {

					StructuredObject *obj{currentDirectory->findChild(in[1])};

					if (obj == nullptr)
						throw runtime_error("Not a directory");

					else {
						Directory *toGo{dynamic_cast<Directory *>(obj)};
						if (toGo == nullptr)
							throw runtime_error("Must be a directory, not a file");
						else
							currentDirectory = toGo;
					}
				}
			}
		}

		catch (exception const &e) {
			cout << e.what() << '\n';
		}
	}
}