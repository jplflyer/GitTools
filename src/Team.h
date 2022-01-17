#pragma once

#include <string>

#include <showlib/JSONSerializable.h>

namespace GitTools {
    class Team;
}

class GitTools::Team: public ShowLib::JSONSerializable
{
public:
    typedef std::shared_ptr<Team> Pointer;
    typedef ShowLib::JSONSerializableVector<Team> Vector;

    void fromJSON(const JSON &);
    JSON &toJSON(JSON &) const;

    std::string node_id;
    std::string url;
    std::string html_url;
    std::string name;
    std::string slug;
    std::string description;
    std::string privacy;
    std::string permission;
    std::string members_url;
    std::string repositories_url;
    std::string parent;
    int id;

};

