#include <iostream>

#include <showlib/OptionHandler.h>
#include <showlib/Ranges.h>
#include <showlib/StringUtils.h>

#include "Server.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace GitTools;

enum class Action {
    Unknown,
    AddUser,
    GetRepos,
    GetTeams,
    GetUsers
};

class GitTool {
public:
    void processArgs(int, char **);
    void run();

    void getRepositories();
    void getTeams();
    void getUsers();
    void addUser();

    Action action = Action::Unknown;
    Server server;
    string orgName;
    ShowLib::StringVector repoNames;
    ShowLib::StringVector loginNames;

    std::string permName;
    bool checkForUsers = true;
};

/**
 * Entry point.
 */
int main(int argc, char **argv) {
    GitTool tool;

    tool.processArgs(argc, argv);
    tool.run();
}

void GitTool::processArgs(int argc, char ** argv) {
    ShowLib::OptionHandler::ArgumentVector args;

    args.addArg("host", [&](const char *value){ server.hostname = value; }, "github.com", "Specify a server");
    args.addArg("username", [&](const char *value){ server.username = value; }, "foofoo", "Specify your username");
    args.addArg("token", [&](const char *value){ server.apiToken = value; }, "12345", "Your API Token");

    args.addArg("login", [&](const char *value){ loginNames.add(value); }, "foo", "A user to add to a repo");
    args.addArg("repo", [&](const char *value){ repoNames.add(ShowLib::trim(value)); }, "Foo", "Repository name (without owner)");

    args.addNoArg("add-admin", [&](const char *){ action = Action::AddUser; permName = "admin"; }, "Add an admin to a repo");
    args.addNoArg("add-writer", [&](const char *){ action = Action::AddUser; permName = "push"; }, "Add a writer to a repo");
    args.addNoArg("repos", [&](const char *){ action = Action::GetRepos; }, "Retrieve repositories");
    args.addNoArg("teams", [&](const char *){ action = Action::GetTeams; }, "Retrieve teams");
    args.addNoArg("users", [&](const char *){ action = Action::GetUsers; }, "Retrieve users");

    args.addNoArg("no-usercheck", [&](const char *){ checkForUsers = false; }, "Don't validate the loginNames given.");

    args.addArg("org", [&](const char *value){ orgName = value; }, "foofoo", "Use this organization");

    if (!ShowLib::OptionHandler::handleOptions(argc, argv, args)) {
        exit(0);
    }

    if (server.username.empty() || server.apiToken.empty()) {
        cerr << "No authentication may be a problem." << endl;
    }
}

void GitTool::run() {
    switch (action) {
        case Action::Unknown: cerr << "Please specify one of [repos]" << endl; break;

        case Action::GetRepos: getRepositories(); break;
        case Action::GetTeams: getTeams(); break;
        case Action::GetUsers: getUsers(); break;
        case Action::AddUser: addUser(); break;

        default: cout << "Unknown action." << endl; break;
    }
}

void GitTool::getRepositories() {
    Repository::Vector repos;
    server.getRepositories(repos);
    cout << "Number of repos: " << repos.size() << endl;
    for (const Repository::Pointer & repo: repos) {
        cout << "Repo: " << repo->name << " -- " << repo->url << endl;
    }
}

void GitTool::getTeams() {
    Team::Vector teams;
    server.getTeams(teams, orgName);
    cout << "Number of teams: " << teams.size() << endl;
    for (const Team::Pointer & team: teams) {
        cout << "Team: " << team->name << " -- " << team->url << endl;
    }
}



void GitTool::getUsers() {
    User::Vector users;
    server.getUsers(users, orgName);
    cout << "Number of users: " << users.size() << endl;
    for (const User::Pointer & user: users) {
        cout << "User Login: " << user->login;
        if (user->name.size() > 0) {
            cout << " (" << user->name << ")";
        }
        if (user->email.size() > 0) {
            cout << " -- " << user->email;
        }
        cout << endl;
    }
}

/**
 * Give this user admin access to this repo.
 */
void GitTool::addUser() {
    cout << "Add User..." << endl;

    Repository::Vector repos;
    User::Vector users;
    server.getRepositories(repos);

    if (checkForUsers) {
        server.getUsers(users, orgName);
    }

    for (const std::shared_ptr<string> &nPtr: repoNames) {
        string repoName = ShowLib::toLower(*nPtr);
        Repository::Pointer repo = repos.findIf( [=](const Repository::Pointer & ptr) {
            return ShowLib::toLower(ptr->name) == repoName;
        } );

        if (repo == nullptr) {
            cout << "Repo " << repoName << " not found." << endl;
            continue;
        }

        for (const std::shared_ptr<string> & uPtr: loginNames) {
            string login = *uPtr;

            if (checkForUsers) {
                User::Pointer user = users.findIf( [=](const User::Pointer & ptr) { return ptr->login == login; } );
                if (user == nullptr) {
                    cout << "User " << login << " not found." << endl;
                    continue;
                }
            }

            server.addUserToRepo(orgName, repo->name, login, permName);
        }
    }
}
