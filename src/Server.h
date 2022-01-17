#pragma once

#include <string>

#include <showlib/RESTClient.h>

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

    void getRepositories(Repository::Vector &vec);
    void getTeams(Team::Vector &vec, const std::string &orgName);
    void getUsers(User::Vector &vec, const std::string &orgName);

    void addAdmin(const std::string &orgName, const std::string & repoName, const std::string & userName);

    std::string		hostname;
    std::string		username;
    std::string		apiToken;

protected:
    void ensureHeaders();

    ShowLib::RESTClient		client;
    ShowLib::RESTClient::HTTPHeader::Pointer	authHeader = nullptr;
};

