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
    AddAdmin,
    GetRepos,
    GetTeams,
    GetUsers
};

void getRepositories(Server &server);
void getTeams(Server &server, const std::string &orgName);
void getUsers(Server &server, const std::string &orgName);

void addAdmin(Server &server, const std::string &orgName, const ShowLib::StringVector & repoNames, const std::string &login);


int main(int argc, char **argv) {
    ShowLib::OptionHandler::ArgumentVector args;
    Action action = Action::Unknown;
    Server server;
    string orgName;
    string loginName;
    ShowLib::StringVector repoNames;

    args.addArg("host", [&](const char *value){ server.hostname = value; }, "github.com", "Specify a server");
    args.addArg("username", [&](const char *value){ server.username = value; }, "foofoo", "Specify your username");
    args.addArg("token", [&](const char *value){ server.apiToken = value; }, "12345", "Your API Token");

    args.addArg("login", [&](const char *value){ loginName = value; }, "foo", "Another user");
    args.addArg("repo", [&](const char *value){ repoNames.add(ShowLib::trim(value)); }, "Foo", "Repository name (without owner)");

    args.addNoArg("add-admin", [&](const char *){ action = Action::AddAdmin; }, "Add an admin to a repo");
    args.addNoArg("repos", [&](const char *){ action = Action::GetRepos; }, "Retrieve repositories");
    args.addNoArg("teams", [&](const char *){ action = Action::GetTeams; }, "Retrieve teams");
    args.addNoArg("users", [&](const char *){ action = Action::GetUsers; }, "Retrieve users");

    args.addArg("org", [&](const char *value){ orgName = value; }, "foofoo", "Use this organization");

    if (!ShowLib::OptionHandler::handleOptions(argc, argv, args)) {
        return 0;
    }

    if (server.username.empty() || server.apiToken.empty()) {
        cerr << "No authentication may be a problem." << endl;
    }

    switch (action) {
        case Action::Unknown: cerr << "Please specify one of [repos]" << endl; return 0;

        case Action::GetRepos: getRepositories(server); break;
        case Action::GetTeams: getTeams(server, orgName); break;
        case Action::GetUsers: getUsers(server, orgName); break;
        case Action::AddAdmin: addAdmin(server, orgName, repoNames, loginName); break;
    }
}

void getRepositories(Server &server) {
    Repository::Vector repos;
    server.getRepositories(repos);
    cout << "Number of repos: " << repos.size() << endl;
    for (const Repository::Pointer & repo: repos) {
        cout << "Repo: " << repo->name << " -- " << repo->url << endl;
    }
}

void getTeams(Server &server, const string &orgName) {
    Team::Vector teams;
    server.getTeams(teams, orgName);
    cout << "Number of teams: " << teams.size() << endl;
    for (const Team::Pointer & team: teams) {
        cout << "Team: " << team->name << " -- " << team->url << endl;
    }
}



void getUsers(Server &server, const string &orgName) {
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
void addAdmin(Server &server, const std::string &orgName, const ShowLib::StringVector & repoNames, const std::string &login) {
    Repository::Vector repos;
    User::Vector users;
    server.getRepositories(repos);
    server.getUsers(users, orgName);

    for (const std::shared_ptr<string> &nPtr: repoNames) {
        string repoName = ShowLib::toLower(*nPtr);
        Repository::Pointer repo = repos.findIf( [=](const Repository::Pointer & ptr) {
            return ShowLib::toLower(ptr->name) == repoName;
        } );
        User::Pointer user = users.findIf( [=](const User::Pointer & ptr) { return ptr->login == login; } );

        if (repo == nullptr) {
            cout << "Repo " << repoName << " not found." << endl;
            continue;
        }
        if (user == nullptr) {
            cout << "User " << login << " not found." << endl;
            continue;
        }

        server.addAdmin(orgName, repo->name, login);
    }
}
