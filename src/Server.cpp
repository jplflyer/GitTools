#include <iostream>

#include <showlib/CommonUsing.h>
#include <showlib/JSONSerializable.h>
#include <showlib/StringUtils.h>

#include "Server.h"

using namespace GitTools;

/**
 * Constructor.
 */
Server::Server() {
    hostname = ShowLib::getEnv("GIT_HOST", "api.github.com");
    username = ShowLib::getEnv("GIT_USER", "git");
    apiToken = ShowLib::getEnv("GIT_TOKEN");

    client.setHost( string{"https://"} + hostname);
    client.setStandardHeader("Accept", "application/vnd.github+json");
    client.setStandardHeader("User-Agent", "curl/7.54.1");
    client.setStandardHeader("X-GitHub-Api-Version", "2022-11-28");
}

void Server::ensureHeaders() {
    if (authHeader == nullptr && !username.empty() && !apiToken.empty()) {
        authHeader = client.makeAuthHeader(username, apiToken);
        client.pushStandardHeader(authHeader);
    }
}

/**
 * Retrieve repos for the authenticated user.
 */
Repository::Vector
Server::getRepositories() {
    Repository::Vector vec;
    ensureHeaders();

    string url = "/user/repos?per_page=100";
    int pageNum = 1;
    while (true) {
        cout << "Perform get on " << url << " and page " << pageNum << endl;
        JSON json = client.get(url + "&page=" + std::to_string(pageNum));
        ++pageNum;

        if (!json.is_array()) {
            break;
        }
        if (!json.size()) {
            break;
        }

        vec.fromJSON(json);
    }
    return vec;
}

/**
 * Retrieve teams for the named org.
 */
Team::Vector Server::getTeams( const OwnerName & orgName) {
    Team::Vector vec;
    ensureHeaders();

    string url = "/orgs/" + orgName.get() + "/teams?per_page=100";
    int pageNum = 1;
    while (true) {
        cout << "Perform get on " << url << " and page " << pageNum << endl;
        JSON json = client.get(url + "&page=" + std::to_string(pageNum));
        ++pageNum;

        if (!json.is_array()) {
            break;
        }
        if (!json.size()) {
            break;
        }

        vec.fromJSON(json);
    }

    return vec;
}

/**
 * Retrieve users for the named org.
 */
User::Vector Server::getUsers(const OwnerName & orgName) {
    User::Vector vec;
    ensureHeaders();

    string url = "/orgs/" + orgName.get() + "/members?per_page=100";
    int pageNum = 1;
    while (true) {
        cout << "Perform get on " << url << " and page " << pageNum << endl;
        JSON json = client.get(url + "&page=" + std::to_string(pageNum));
        ++pageNum;

        if (!json.is_array()) {
            break;
        }
        if (!json.size()) {
            break;
        }

        vec.fromJSON(json);
    }

    return vec;
}

/**
 * curl -X PUT -d '{"permission": "admin"}' -s -u "$GITHUB_USER:$GITHUB_TOKEN"
 * 	 "https://api.github.com/repos/verbit-ai/CT-Agents/collaborators/vitac-brentn"
 */
void Server::addUserToRepo(
        const OwnerName & orgName,
        const RepositoryName & repoName,
        const UserName & login,
        const PermissionName & permName )
{
    ensureHeaders();

    string url = "/repos/" + orgName.get() + "/" + repoName.get() + "/collaborators/" + login.get();

    JSON json = JSON::object();
    json["permission"] = permName.get();

    client.put(url, json);
}

/**
 * /repos/{owner}/{repo}/branches/{branch}/protection
 */
BranchProtection Server::getProtection(const OwnerName & orgName, const RepositoryName & repoName, const BranchName &branchName) {
    string url = "/repos/" + orgName.get() + "/" + repoName.get() + "/branches/" + branchName.get() + "/protection";
    BranchProtection bp;

    ensureHeaders();

    JSON json = client.get(url);
    bp.fromJSON(json);

    return bp;
}

void Server::deleteProtection(const OwnerName & orgName, const RepositoryName & repoName, const BranchName &branchName) {
    ensureHeaders();

    string url = "/repos/" + orgName.get() + "/" + repoName.get() + "/branches/" + branchName.get() + "/protection";

    JSON body = nullptr;
    client.del(url, body);
}

/**
 * Set branch protection.
 */
void Server::setProtection(const OwnerName & orgName, const RepositoryName & repoName, const BranchName &branchName, const UpdateBranchProtection &bp) {
    ensureHeaders();

    string url = "/repos/" + orgName.get() + "/" + repoName.get() + "/branches/" + branchName.get() + "/protection";

    JSON body = bp.toJSON();
    JSON reply = client.put(url, body);
    if ( ShowLib::JSONSerializable::hasKey(reply, "message") ) {
        string msg = ShowLib::JSONSerializable::stringValue(reply, "message");
        ShowLib::replaceAll(msg, "\\n", "\n");
        cout << "Reply message: " << msg << endl;
    }
}
