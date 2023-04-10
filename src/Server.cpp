#include <iostream>

#include <showlib/JSONSerializable.h>
#include <showlib/StringUtils.h>

#include "Server.h"

using std::cout;
using std::endl;
using std::string;
using namespace GitTools;

/**
 * Constructor.
 */
Server::Server() {
    hostname = ShowLib::getEnv("GIT_HOST", "api.github.com");
    username = ShowLib::getEnv("GIT_USER");
    apiToken = ShowLib::getEnv("GIT_TOKEN");

    client.setHost( string{"https://"} + hostname);
    client.setStandardHeader("Accept", "application/vnd.github+json");
    client.setStandardHeader("User-Agent", "curl/7.54.1");
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
Team::Vector
Server::getTeams( const std::string &orgName) {
    Team::Vector vec;
    ensureHeaders();

    string url = "/orgs/" + orgName + "/teams?per_page=100";
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
User::Vector
Server::getUsers(const std::string &orgName) {
    User::Vector vec;
    ensureHeaders();

    string url = "/orgs/" + orgName + "/members?per_page=100";
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
        const std::string &orgName,
        const std::string & repoName,
        const std::string & login,
        const std::string & permName )
{
    ensureHeaders();

    string url = "/repos/" + orgName + "/" + repoName + "/collaborators/" + login;

    JSON json = JSON::object();
    json["permission"] = permName;

    client.put(url, json);
}

