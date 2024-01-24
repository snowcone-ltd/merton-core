#pragma once

#include "OSystem.hxx"
#include "repository/CompositeKeyValueRepositoryNoop.hxx"

#ifdef _WIN32
	const char *BAD_PATH = "BAD_DRIVE:\\";
#else
	const char *BAD_PATH = "/dev/null/";
#endif

class OSystemCore : public OSystem {
	public:
		OSystemCore() = default;
		~OSystemCore() override = default;

		void getBaseDirectories(string& basedir, string& homedir, bool useappdir, string_view usedir) override
		{
			basedir = homedir = BAD_PATH;
		}

		shared_ptr<KeyValueRepository> getSettingsRepository() override
		{
			return make_shared<KeyValueRepositoryNoop>();
		}

		shared_ptr<CompositeKeyValueRepository> getPropertyRepository() override
		{
			return make_shared<CompositeKeyValueRepositoryNoop>();
		}

		shared_ptr<CompositeKeyValueRepositoryAtomic> getHighscoreRepository() override
		{
			return make_shared<CompositeKeyValueRepositoryNoop>();
		}

	protected:
		void initPersistence(FSNode& basedir) override {}
		string describePresistence() override {return "none";}
};
