#pragma once

#include <string>

#include <showlib/RESTClient.h>

#include "BranchProtection.h"
#include "Repository.h"
#include "Team.h"
#include "User.h"

namespace GitTools {
    class Server;
}

/**
 * Information on the server we're using.
 */
class GitTools::Server
{
public:
    Server();

    Repository::Vector getRepositories();
    Team::Vector getTeams(const std::string &orgName);
    User::Vector getUsers(const std::string &orgName);

    BranchProtection getProtection(const std::string &orgName, const std::string repoName, const std::string &branchName);

    void addUserToRepo(const std::string &orgName, const std::string & repoName, const std::string & userName, const std::string &perm);

    std::string		hostname;
    std::string		username;
    std::string		apiToken;

protected:
    void ensureHeaders();

    ShowLib::RESTClient		client;
    ShowLib::RESTClient::HTTPHeader::Pointer	authHeader = nullptr;
};

