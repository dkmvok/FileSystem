#include <numeric>
#include <string>
#include <set>
#include <iostream>

class Folder;
class File;

class FileSystem
{
    std::string name;
    Folder* parent;

protected:
    FileSystem(std::string name, Folder* parent);

    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;

    virtual ~FileSystemObject();

public:
    void changeName(std::string& name){ name = name; }

    std::string name() const{ return name; }

    Folder* parent() const{ return parent; }

    virtual std::size_t size() const = 0;

    void setParent(Folder* parent);
	Folder* getParent(){ return parent;} 
};
//--------------------------------------------------------

class Folder : public FileSystem
{
    std::set<FileSystem*> children = {};
public:
    Folder(std::string name, Folder* parent = nullptr)
        : FileSystem{std::move(name), parent}
    {
    }

    ~Folder(){
        for (auto c: children) {
            c->setParent(nullptr);
        }
    }

    std::size_t size() const override{
        return std::accumulate(children.begin(), children.end(), 0,
                               [](auto sum, auto child){ return sum + child->size(); });
    }

	int numOfChildren() const {
        return children.size();
    }

    void addElement(FileSystem* b){
        children.insert(b);
    }

    void deleteElement(FileSystem* b){
        children.erase(b);
    }
};

//------------------------------------------------------
class File : public FileSystem
{
    std::string contents;
public:
    File(std::string contents, std::string name, Folder* parent)
        : FileSystem{name, parent},
          contents{contents}
    {
    }

    std::size_t size() const override{ return contents.size(); }

    void write(std::string content) { contents = std::move(content); }

    std::string read() const { return contents; }

};

FileSystem::FileSystem(std::string name, Folder* parent)
    : name{std::move(name)},
      parent{parent}
{
    if (parent) {
        parent->addElement(this);
    }
}


FileSystem::~FileSystem()
{
    if (parent) {
        parent->deleteElement(this);
    }
}
void FileSystem::getParent(Folder* parent)
{
	
    if (!parent) {  
        parent->deleteElement(this);
    }
}
void FileSystem::setParent(Folder* parent)
{
	
    if (parent) {  
        parent->addElement(this);
    }
}




int main()
{
    Folder rootDirectory{"/"};
    Folder home{"home", &rootDirectory};
 
	
    std::cout << "Elements in root: " << rootDirectory.numOfChildren() << "\n";
    std::cout << "Elements in home: " << home.numOfChildren() << "\n";
    Folder wrkSpace{"wrkSpace", &home};
   
  
    std::cout << "Elements in root: " << rootDirectory.numOfChildren() << "\n";
    std::cout << "Elements in home: " << home.numOfChildren() << '\n';
    std::cout << "Elements in in wrkSpace: " << wrkSpace.numOfChildren() << "\n";
	
   
    std::cout << "Elements in root: " << rootDirectory.numOfChildren() << "\n";
    std::cout << "Size of root: " << rootDirectory.size() << "\n";
}