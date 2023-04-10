#pragma once

#include <string>
#include <showlib/JSONSerializable.h>
#include <showlib/StringVector.h>

namespace GitTools {
    class Repository;
}

class GitTools::Repository: public ShowLib::JSONSerializable
{
public:
    typedef std::shared_ptr<Repository> Pointer;
    typedef ShowLib::JSONSerializableVector<Repository> Vector;

    class Owner: public ShowLib::JSONSerializable
    {
    public:
        std::string login;
        std::string nodeId;
        std::string avatarURL;
        std::string gravatarID;
        std::string url;
        std::string htmlURL;
        std::string followersURL;
        std::string followingURL;
        std::string gistsURL;
        std::string starredURL;
        std::string subscriptionsURL;
        std::string organizationsURL;
        std::string reposURL;
        std::string eventsURL;
        std::string receivedEventsURL;
        std::string type;
        int id;
        bool siteAdmin;

        void fromJSON(const JSON &) override;
        JSON toJSON() const override;
    };

    class Permissions: public ShowLib::JSONSerializable
    {
    public:
        bool admin;
        bool push;
        bool pull;

        void fromJSON(const JSON &) override;
        JSON toJSON() const override;
    };

    //======================================================================
    // Methods.
    //======================================================================
    Repository();

    void fromJSON(const JSON &) override;
    JSON toJSON() const override;

    //======================================================================
    // Fields.
    //======================================================================
    Owner owner;
    Permissions permissions;
    ShowLib::StringVector topics;

    std::string nodeId;
    std::string name;
    std::string fullName;
    std::string html_url; // https://github.com/octocat/Hello-World
    std::string description;
    std::string url; // https://api.github.com/repos/octocat/Hello-World
    std::string archive_url; // https://api.github.com/repos/octocat/Hello-World/{archive_format}{/ref}
    std::string assignees_url; // https://api.github.com/repos/octocat/Hello-World/assignees{/user}
    std::string blobs_url; // https://api.github.com/repos/octocat/Hello-World/git/blobs{/sha}
    std::string branches_url; // https://api.github.com/repos/octocat/Hello-World/branches{/branch}
    std::string collaborators_url; // https://api.github.com/repos/octocat/Hello-World/collaborators{/collaborator}
    std::string comments_url; // https://api.github.com/repos/octocat/Hello-World/comments{/number}
    std::string commits_url; // https://api.github.com/repos/octocat/Hello-World/commits{/sha}
    std::string compare_url; // https://api.github.com/repos/octocat/Hello-World/compare/{base}...{head}
    std::string contents_url; // https://api.github.com/repos/octocat/Hello-World/contents/{+path}
    std::string contributors_url; // https://api.github.com/repos/octocat/Hello-World/contributors
    std::string deployments_url; // https://api.github.com/repos/octocat/Hello-World/deployments
    std::string downloads_url; // https://api.github.com/repos/octocat/Hello-World/downloads
    std::string events_url; // https://api.github.com/repos/octocat/Hello-World/events
    std::string forks_url; // https://api.github.com/repos/octocat/Hello-World/forks
    std::string git_commits_url; // https://api.github.com/repos/octocat/Hello-World/git/commits{/sha}
    std::string git_refs_url; // https://api.github.com/repos/octocat/Hello-World/git/refs{/sha}
    std::string git_tags_url; // https://api.github.com/repos/octocat/Hello-World/git/tags{/sha}
    std::string git_url; // git:github.com/octocat/Hello-World.git
    std::string issue_comment_url; // https://api.github.com/repos/octocat/Hello-World/issues/comments{/number}
    std::string issue_events_url; // https://api.github.com/repos/octocat/Hello-World/issues/events{/number}
    std::string issues_url; // https://api.github.com/repos/octocat/Hello-World/issues{/number}
    std::string keys_url; // https://api.github.com/repos/octocat/Hello-World/keys{/key_id}
    std::string labels_url; // https://api.github.com/repos/octocat/Hello-World/labels{/name}
    std::string languages_url; // https://api.github.com/repos/octocat/Hello-World/languages
    std::string merges_url; // https://api.github.com/repos/octocat/Hello-World/merges
    std::string milestones_url; // https://api.github.com/repos/octocat/Hello-World/milestones{/number}
    std::string notifications_url; // https://api.github.com/repos/octocat/Hello-World/notifications{?since,all,participating}
    std::string pulls_url; // https://api.github.com/repos/octocat/Hello-World/pulls{/number}
    std::string releases_url; // https://api.github.com/repos/octocat/Hello-World/releases{/id}
    std::string ssh_url; // git@github.com:octocat/Hello-World.git
    std::string stargazers_url; // https://api.github.com/repos/octocat/Hello-World/stargazers
    std::string statuses_url; // https://api.github.com/repos/octocat/Hello-World/statuses/{sha}
    std::string subscribers_url; // https://api.github.com/repos/octocat/Hello-World/subscribers
    std::string subscription_url; // https://api.github.com/repos/octocat/Hello-World/subscription
    std::string tags_url; // https://api.github.com/repos/octocat/Hello-World/tags
    std::string teams_url; // https://api.github.com/repos/octocat/Hello-World/teams
    std::string trees_url; // https://api.github.com/repos/octocat/Hello-World/git/trees{/sha}
    std::string clone_url; // https://github.com/octocat/Hello-World.git
    std::string mirror_url; // git:git.example.com/octocat/Hello-World
    std::string hooks_url; // https://api.github.com/repos/octocat/Hello-World/hooks
    std::string svn_url; // https://svn.github.com/octocat/Hello-World
    std::string homepage; // https://github.com
    std::string language;
    std::string default_branch;
    std::string visibility;
    std::string pushed_at;
    std::string created_at;
    std::string updated_at;
    std::string template_repository;

    int forks_count;
    int stargazers_count;
    int watchers_count;
    int size;
    int id;
    int open_issues_count;
    int is_template;

    bool isPrivate;
    bool fork;
    bool has_issues;
    bool has_projects;
    bool has_wiki;
    bool has_pages;
    bool has_downloads;
    bool archived;
    bool disabled;
};
