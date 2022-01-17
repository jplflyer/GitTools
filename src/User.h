#pragma once

#include <showlib/JSONSerializable.h>

namespace GitTools {
    class User;
}

class GitTools::User: public ShowLib::JSONSerializable
{
public:
    typedef std::shared_ptr<User> Pointer;
    typedef ShowLib::JSONSerializableVector<User> Vector;

    void fromJSON(const JSON &);
    JSON &toJSON(JSON &) const;

    std::string avatar_url;
    std::string bio;
    std::string blog;
    std::string company;
    std::string email;
    std::string events_url;
    std::string followers_url;
    std::string following_url;
    std::string gists_url;
    std::string gravatar_id;
    std::string html_url;
    std::string location;
    std::string login;
    std::string name;
    std::string node_id;
    std::string organizations_url;
    std::string received_events_url;
    std::string repos_url;
    std::string starred_url;
    std::string subscriptions_url;
    std::string type;
    std::string twitter_username;
    std::string url;
    std::string created_at;
    std::string updated_at;

    int collaborators;
    int disk_usage;
    int followers;
    int following;
    int id;
    int owned_private_repos;
    int public_repos;
    int public_gists;
    int private_gists;
    int total_private_repos;

    bool hireable;
    bool site_admin;
    bool two_factor_authentication;

    // These are in field "plan".
    std::string plan_name;
    int plan_space;
    int plan_private_repos;
    int plan_collaborators;
};

