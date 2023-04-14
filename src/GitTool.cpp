//======================================================================
// When using this program, you can use these environment variables:
//
//		GIT_HOST	(default is probably fine)
//		GIT_USER	(default of "git" is probably fine)
//		GIT_TOKEN	This is your personal API token.
//======================================================================
#include <iomanip>
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
    GetUsers,
    CheckBranchProtection,
    AddBranchProtection,
    DeleteBranchProtection
};

enum class Option {
    EnforceAdmins_Set,
    EnforceAdmins_Clear,
    Allow_Delete_Set,
    Allow_Delete_Clear,
    Require_PullRequests_Set,
    Require_PullRequests_Clear
};

class GitTool {
public:
    void processArgs(int, char **);
    void run();

    void getRepositories();
    void getTeams();
    void getUsers();
    void addUser();

    void checkBranchProtection();
    void addBranchProtection();
    void deleteBranchProtection();

    Action action = Action::Unknown;
    Server server;
    Server::OwnerName orgName;
    Server::RepositoryName repoName;
    Server::BranchName branchName = Server::BranchName("main");
    ShowLib::StringVector repoNames;
    ShowLib::StringVector loginNames;

    std::vector<Option> options;

    Server::PermissionName permName;
    bool checkForUsers = true;
};

/**
 * Entry point.
 */
int main(int argc, char **argv) {
    cout << std::boolalpha;
    GitTool tool;

    tool.processArgs(argc, argv);
    tool.run();
}

void GitTool::processArgs(int argc, char ** argv) {
    ShowLib::OptionHandler::ArgumentVector args;

    args.addArg("host", [&](const char *value){ server.hostname = value; }, "github.com", "Specify a server");
    args.addArg("username", [&](const char *value){ server.username = value; }, "foofoo", "Specify your username");
    args.addArg("token", [&](const char *value){ server.apiToken = value; }, "12345", "Your API Token");

    args.addArg("login",  [&](const char *value){ loginNames.add(value); },                  "foo",  "A user to add to a repo");
    args.addArg("repo",   [&](const char *value){ repoNames.add(ShowLib::trim(value)); },    "Foo",  "A repository name (without owner)");
    args.addArg("branch", [&](const char *value){ branchName = Server::BranchName(value); }, "main", "The branch name");

    args.addNoArg("add-admin", [&](const char *){ action = Action::AddUser; permName = Server::PermissionName("admin"); }, "Add an admin to a repo");
    args.addNoArg("add-writer", [&](const char *){ action = Action::AddUser; permName = Server::PermissionName("push"); }, "Add a writer to a repo");
    args.addNoArg("repos", [&](const char *){ action = Action::GetRepos; }, "Retrieve repositories");
    args.addNoArg("teams", [&](const char *){ action = Action::GetTeams; }, "Retrieve teams");
    args.addNoArg("users", [&](const char *){ action = Action::GetUsers; }, "Retrieve users");

    args.addNoArg("no-usercheck", [&](const char *){ checkForUsers = false; }, "Don't validate the loginNames given.");

    args.addArg("org", [&](const char *value){ orgName = Server::OwnerName(value); }, "foofoo", "Use this organization (used by users/teams calls)");

    args.addArg("check-branch-protection", [&](const char *value){ action = Action::CheckBranchProtection; repoName = Server::RepositoryName(value); }, "repo", "Display branch protection. See --branch");
    args.addArg("delete-branch-protection", [&](const char *value){ action = Action::DeleteBranchProtection; repoName = Server::RepositoryName(value); }, "repo", "Delete branch protection. See --branch");
    args.addArg("add-branch-protection", [&](const char *value){ action = Action::AddBranchProtection; repoName = Server::RepositoryName(value); }, "repo", "Add branch protection. See --branch and other options below.");

    // These flags are for add-branch-protection
    args.addNoArg("enforce-admins",    [&](const char *) { options.push_back(Option::EnforceAdmins_Set); },   "For add-branch-protection: admins cannot bypass the other flags.");
    args.addNoArg("no-enforce-admins", [&](const char *) { options.push_back(Option::EnforceAdmins_Clear); }, "For add-branch-protection: admins can bypass the other flags.");

    args.addNoArg("allow-delete",      [&](const char *) { options.push_back(Option::Allow_Delete_Clear); }, "For add-branch-protection: allow_deletions == true.");
    args.addNoArg("no-allow-delete",   [&](const char *) { options.push_back(Option::Allow_Delete_Set); },   "For add-branch-protection: allow_deletions == false.");

    args.addNoArg("pull-requests",     [&](const char *) { options.push_back(Option::Require_PullRequests_Set); },   "For add-branch-protection: Require pull requests before merging");
    args.addNoArg("no-pull-requests",  [&](const char *) { options.push_back(Option::Require_PullRequests_Clear); }, "For add-branch-protection: Do not require pull requests before merging");

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

        case Action::CheckBranchProtection: checkBranchProtection(); break;
        case Action::AddBranchProtection: addBranchProtection(); break;
        case Action::DeleteBranchProtection: deleteBranchProtection(); break;

        default: cout << "Unknown action." << endl; break;
    }
}

void GitTool::getRepositories() {
    Repository::Vector repos = server.getRepositories();
    cout << "Number of repos: " << repos.size() << endl;
    for (const Repository::Pointer & repo: repos) {
        cout << "Repo: " << repo->name << " -- " << repo->url << endl;
    }
}

void GitTool::getTeams() {
    Team::Vector teams = server.getTeams(orgName);
    cout << "Number of teams: " << teams.size() << endl;
    for (const Team::Pointer & team: teams) {
        cout << "Team: " << team->name << " -- " << team->url << endl;
    }
}



void GitTool::getUsers() {
    User::Vector users = server.getUsers(orgName);
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

    Repository::Vector repos = server.getRepositories();
    User::Vector users;

    if (checkForUsers) {
        users = server.getUsers(orgName);
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

            server.addUserToRepo(orgName, Server::RepositoryName(repo->name), Server::UserName(login), permName);
        }
    }
}

/**
 * We're going to retrieve the branch protection information for this repo.
 */
void GitTool::checkBranchProtection() {
    BranchProtection bp = server.getProtection(orgName, repoName, branchName);
    if ( !bp.getEnabled() ) {
        cout << "Protection not enabled.\n";
    }
    else {
        cout << "Protection:\n" << bp.toJSON().dump(2) << endl;
    }
}

/**
 * Apply the changes they've been specifying.
 */
void GitTool::addBranchProtection() {
    if (options.empty()) {
        cerr << "You specified no options.\n";
        return;
    }

    BranchProtection bp = server.getProtection(orgName, repoName, branchName);
    UpdateBranchProtection ubp ( bp );
    for (const Option &option: options) {
        switch (option) {
            case Option::EnforceAdmins_Set:   ubp.setEnforceAdmins(true); break;
            case Option::EnforceAdmins_Clear: ubp.setEnforceAdmins(false); break;

            case Option::Allow_Delete_Set:    ubp.setAllowDeletions(true); break;
            case Option::Allow_Delete_Clear:  ubp.setAllowDeletions(false); break;

            case Option::Require_PullRequests_Set: ubp.getRequiredPullRequestReviews().setApprovingReviewCount(1); break;
            case Option::Require_PullRequests_Clear: ubp.getRequiredPullRequestReviews().clearAll(); break;
        }
    }
    cout << "Protections should become:\n" << ubp.toJSON().dump(2) << endl;

    server.setProtection(orgName, repoName, branchName, ubp);
}

/**
 * If present, delete this branch's protection.
 */
void GitTool::deleteBranchProtection() {
    server.deleteProtection(orgName, repoName, branchName);
}
