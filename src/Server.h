#pragma once

#include <string>

#include <NamedType/named_type.hpp>

#include <showlib/CommonUsing.h>
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
    using OwnerName = fluent::NamedType<std::string, struct OwnerNameType, fluent::Callable, fluent::Printable>;
    using RepositoryName = fluent::NamedType<std::string, struct RepositoryNameType, fluent::Callable, fluent::Printable>;
    using BranchName = fluent::NamedType<std::string, struct BranchNameType, fluent::Callable, fluent::Printable>;
    using UserName = fluent::NamedType<std::string, struct BranchNameType, fluent::Callable, fluent::Printable>;
    using PermissionName = fluent::NamedType<std::string, struct BranchNameType, fluent::Callable, fluent::Printable>;

    Server();

    Repository::Vector getRepositories();
    Team::Vector getTeams(const OwnerName & orgName);
    User::Vector getUsers(const OwnerName & orgName);

    BranchProtection getProtection(const OwnerName & orgName, const RepositoryName & repoName, const BranchName & branchName);
    void setProtection(const OwnerName & orgName, const RepositoryName & repoName, const BranchName &branchName, const UpdateBranchProtection &);
    void deleteProtection(const OwnerName & orgName, const RepositoryName & repoName, const BranchName &branchName);
    void addUserToRepo(const OwnerName & orgName, const RepositoryName & repoName, const UserName & userName, const PermissionName &perm);

    std::string		hostname;
    std::string		username;
    std::string		apiToken;

protected:
    void ensureHeaders();

    ShowLib::RESTClient		client;
    ShowLib::RESTClient::HTTPHeader::Pointer	authHeader = nullptr;
};

