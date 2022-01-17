#include <iostream>

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
    client.setStandardHeader("Accept", "application/json");
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
void
Server::getRepositories(Repository::Vector &vec) {
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
}

/**
 * Retrieve teams for the named org.
 */
void
Server::getTeams(Team::Vector &vec, const std::string &orgName) {
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
}

/**
 * Retrieve users for the named org.
 */
void
Server::getUsers(User::Vector &vec, const std::string &orgName) {
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
}

/**
 * curl -X PUT -d '{"permission": "admin"}' -s -u "$GITHUB_USER:$GITHUB_TOKEN"
 * 	 "https://api.github.com/repos/verbit-ai/CT-Agents/collaborators/vitac-brentn"
 */
void Server::addAdmin(const std::string &orgName, const std::string & repoName, const std::string & login) {
    ensureHeaders();

    string url = "/repos/" + orgName + "/" + repoName + "/collaborators/" + login;

    JSON json = JSON::object();
    json["permission"] = "admin";

    client.put(url, json);
}

