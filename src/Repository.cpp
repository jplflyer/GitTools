#include "Repository.h"

using namespace GitTools;

Repository::Repository()
{
}

void Repository::fromJSON(const JSON &json) {
    JSON ownerJSON = jsonValue(json, "owner");
    JSON permJSON = jsonValue(json, "permissions");
    JSON topicsArray = jsonArray(json, "topics");

    owner.fromJSON(ownerJSON);
    permissions.fromJSON(permJSON);
    topics.fromJSON(topicsArray);

    nodeId = stringValue(json, "node_id");
    name = stringValue(json, "name");
    fullName = stringValue(json, "full_name");
    html_url = stringValue(json, "html_url");
    description = stringValue(json, "description");
    url = stringValue(json, "url");
    archive_url = stringValue(json, "archive_url");
    assignees_url = stringValue(json, "assignees_url");
    blobs_url = stringValue(json, "blobs_url");
    branches_url = stringValue(json, "branches_url");
    collaborators_url = stringValue(json, "collaborators_url");
    comments_url = stringValue(json, "comments_url");
    commits_url = stringValue(json, "commits_url");
    compare_url = stringValue(json, "compare_url");
    contents_url = stringValue(json, "contents_url");
    contributors_url = stringValue(json, "contributors_url");
    deployments_url = stringValue(json, "deployments_url");
    downloads_url = stringValue(json, "downloads_url");
    events_url = stringValue(json, "events_url");
    forks_url = stringValue(json, "forks_ur");
    git_commits_url = stringValue(json, "git_commits_url");
    git_refs_url = stringValue(json, "git_refs_url");
    git_tags_url = stringValue(json, "git_tags_url");
    git_url = stringValue(json, "git_url");
    issue_comment_url = stringValue(json, "issue_comment_url");
    issue_events_url = stringValue(json, "issue_events_url");
    issues_url = stringValue(json, "issues_url");
    keys_url = stringValue(json, "keys_url");
    labels_url = stringValue(json, "labels_url");
    languages_url = stringValue(json, "languages_url");
    merges_url = stringValue(json, "merges_url");
    milestones_url = stringValue(json, "milestones_url");
    notifications_url = stringValue(json, "notifications_url");
    pulls_url = stringValue(json, "pulls_url");
    releases_url = stringValue(json, "releases_url");
    ssh_url = stringValue(json, "ssh_url");
    stargazers_url = stringValue(json, "stargazers_url");
    statuses_url = stringValue(json, "statuses_url");
    subscribers_url = stringValue(json, "subscribers_url");
    subscription_url = stringValue(json, "subscription_url");
    tags_url = stringValue(json, "tags_url");
    teams_url = stringValue(json, "teams_url");
    trees_url = stringValue(json, "trees_url");
    clone_url = stringValue(json, "clone_url");
    mirror_url = stringValue(json, "mirror_url");
    hooks_url = stringValue(json, "hooks_url");
    svn_url = stringValue(json, "svn_url");
    homepage = stringValue(json, "homepage");
    language = stringValue(json, "language");
    default_branch = stringValue(json, "default_branch");
    visibility = stringValue(json, "visibility");
    pushed_at = stringValue(json, "pushed_at");
    created_at = stringValue(json, "updated_at");
    template_repository = stringValue(json, "template_repository");

    forks_count = intValue(json, "forks_count");
    stargazers_count = intValue(json, "stargazers_count");
    watchers_count = intValue(json, "watchers_count");
    size = intValue(json, "size");
    id = intValue(json, "id");
    open_issues_count = intValue(json, "open_issues_count");
    is_template = intValue(json, "is_template");

    isPrivate = boolValue(json, "private");
    fork = boolValue(json, "fork");
    has_issues = boolValue(json, "has_issues");
    has_projects = boolValue(json, "has_projects");
    has_wiki = boolValue(json, "has_wiki");
    has_pages = boolValue(json, "has_pages");
    has_downloads = boolValue(json, "has_downloads");
    archived = boolValue(json, "archived");
    disabled = boolValue(json, "disabled");
}

JSON Repository::toJSON() const {
    JSON json = JSON::object();

    json["owner"] =  owner.toJSON();
    json["permissions"] =  permissions.toJSON();
    json["topics"] =  topics.toJSON();

    json[ "node_id" ] = nodeId;
    json[ "name" ] = name;
    json[ "full_name" ] = fullName;
    json[ "html_url" ] = html_url;
    json[ "description" ] = description;
    json[ "url" ] = url;
    json[ "archive_url" ] = archive_url;
    json[ "assignees_url" ] = assignees_url;
    json[ "blobs_url" ] = blobs_url;
    json[ "branches_url" ] = branches_url;
    json[ "collaborators_url" ] = collaborators_url;
    json[ "comments_url" ] = comments_url;
    json[ "commits_url" ] = commits_url;
    json[ "compare_url" ] = compare_url;
    json[ "contents_url" ] = contents_url;
    json[ "contributors_url" ] = contributors_url;
    json[ "deployments_url" ] = deployments_url;
    json[ "downloads_url" ] = downloads_url;
    json[ "events_url" ] = events_url;
    json[ "forks_url" ] = forks_url;
    json[ "git_commits_url" ] = git_commits_url;
    json[ "git_refs_url" ] = git_refs_url;
    json[ "git_tags_url" ] = git_tags_url;
    json[ "git_url" ] = git_url;
    json[ "issue_comment_url" ] = issue_comment_url;
    json[ "issue_events_url" ] = issue_events_url;
    json[ "issues_url" ] = issues_url;
    json[ "keys_url" ] = keys_url;
    json[ "labels_url" ] = labels_url;
    json[ "languages_url" ] = languages_url;
    json[ "merges_url" ] = merges_url;
    json[ "milestones_url" ] = milestones_url;
    json[ "notifications_url" ] = notifications_url;
    json[ "pulls_url" ] = pulls_url;
    json[ "releases_url" ] = releases_url;
    json[ "ssh_url" ] = ssh_url;
    json[ "stargazers_url" ] = stargazers_url;
    json[ "statuses_url" ] = statuses_url;
    json[ "subscribers_url" ] = subscribers_url;
    json[ "subscription_url" ] = subscription_url;
    json[ "tags_url" ] = tags_url;
    json[ "teams_url" ] = teams_url;
    json[ "trees_url" ] = trees_url;
    json[ "clone_url" ] = clone_url;
    json[ "mirror_url" ] = mirror_url;
    json[ "hooks_url" ] = hooks_url;
    json[ "svn_url" ] = svn_url;
    json[ "homepage" ] = homepage;
    json[ "language" ] = language;
    json[ "default_branch" ] = default_branch;
    json[ "visibility" ] = visibility;
    json[ "pushed_at" ] = pushed_at;
    json[ "updated_at" ] = updated_at;
    json[ "template_repository" ] = template_repository;

    json[ "forks_count" ] = forks_count;
    json[ "stargazers_count" ] = stargazers_count;
    json[ "watchers_count" ] = watchers_count;
    json[ "size" ] = size;
    json[ "id" ] = id;
    json[ "open_issues_count" ] = open_issues_count;
    json[ "is_template" ] = is_template;

    json[ "private" ] = isPrivate;
    json[ "fork" ] = fork;
    json[ "has_issues" ] = has_issues;
    json[ "has_projects" ] = has_projects;
    json[ "has_wiki" ] = has_wiki;
    json[ "has_pages" ] = has_pages;
    json[ "has_downloads" ] = has_pages;
    json[ "archived" ] = archived;
    json[ "disabled" ] = disabled;

    return json;
}

void Repository::Owner::fromJSON(const JSON &json) {
    login = stringValue(json, "login");
    nodeId = stringValue(json, "node_id");
    avatarURL = stringValue(json, "avatar_url");
    gravatarID = stringValue(json, "gravatar_id");
    url = stringValue(json, "url");
    htmlURL = stringValue(json, "html_url");
    followersURL = stringValue(json, "followers_url");
    followingURL = stringValue(json, "following_url");
    gistsURL = stringValue(json, "gists_url");
    starredURL = stringValue(json, "started_url");
    subscriptionsURL = stringValue(json, "subscriptions_url");
    organizationsURL = stringValue(json, "organizations_url");
    reposURL = stringValue(json, "repos_url");
    eventsURL = stringValue(json, "events_url");
    receivedEventsURL = stringValue(json, "received_events_url");
    type = stringValue(json, "type");
    id = intValue(json, "id");
    siteAdmin = boolValue(json, "site_admin");
}

JSON Repository::Owner::toJSON() const {
    JSON json = JSON::object();

    json[ "login" ] = login;
    json[ "node_id" ] = nodeId;
    json[ "avatar_url" ] = avatarURL;
    json[ "gravatar_id" ] = gravatarID;
    json[ "url" ] = url;
    json[ "html_url" ] = htmlURL;
    json[ "followers_url" ] = followersURL;
    json[ "following_url" ] = followingURL;
    json[ "gists_url" ] = gistsURL;
    json[ "started_url" ] = starredURL;
    json[ "subscriptions_url" ] = subscriptionsURL;
    json[ "organizations_url" ] = organizationsURL;
    json[ "repos_url" ] = reposURL;
    json[ "events_url" ] = eventsURL;
    json[ "received_events_url" ] = receivedEventsURL;
    json[ "type" ] = type;
    json[ "id" ] = id;
    json[ "site_admin" ] = siteAdmin;

    return json;
}

void Repository::Permissions::fromJSON(const JSON &json) {
    admin = boolValue(json, "admin");
    push = boolValue(json, "push");
    pull = boolValue(json, "pull");
}

JSON Repository::Permissions::toJSON() const {
    JSON json = JSON::object();

    json["admin"] = admin;
    json["push"] = push;
    json["pull"] = pull;

    return json;
}
