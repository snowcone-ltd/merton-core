#pragma once

#include "FSNode.hxx"

class FSNodeCore : public AbstractFSNode {
	public:
		FSNodeCore();

		explicit FSNodeCore(string_view path);

		bool exists() const override;
		const string& getName() const override	{return _name;}
		void setName(string_view name) override {_name = name;}
		const string& getPath() const override {return _path;}
		string getShortPath() const override;
		bool hasParent() const override {return false;}
		bool isDirectory() const override {return _isDirectory;}
		bool isFile() const override {return _isFile;}
		bool isReadable() const override;
		bool isWritable() const override;
		bool makeDir() override;
		bool rename(string_view newfile) override;

		bool getChildren(AbstractFSList& list, ListMode mode) const override;
		AbstractFSNodePtr getParent() const override;

		size_t read(ByteBuffer& image, size_t) const override;

	protected:
		string _name;
		string _path;
		bool _isDirectory{false};
		bool _isFile{true};
		bool _isValid{true};
};
