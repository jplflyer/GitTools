#pragma once

#include <string>
#include <showlib/JSONSerializable.h>
#include <showlib/StringVector.h>

#include "User.h"
#include "Team.h"

namespace GitTools {
    class RequiredStatusChecks;
    class EnforceAdmins;
    class UsersTeamsApps;
    class DismissalRestrictions;
    class RequiredPullRequestReviews;
    class Restrictions;
    class EnabledFlag;
    class RequiredSignatures;
    class BranchProtection;
}

class GitTools::EnabledFlag: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

    void setJSON_IfSet(JSON &json, const std::string &key) const;

protected:
    bool isSet = false;
    bool enabled = false;
};

class GitTools::RequiredStatusChecks: public ShowLib::JSONSerializable {
public:
    typedef std::shared_ptr<RequiredStatusChecks> Pointer;
    typedef ShowLib::JSONSerializableVector<RequiredStatusChecks> Vector;

    class Check: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Check> Pointer;
        typedef ShowLib::JSONSerializableVector<Check> Vector;

    protected:
        std::string context;
        int appId;
    };

    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    std::string url;
    std::string contextsURL;
    std::string enforcementLevel;
    ShowLib::StringVector contexts;
    Check::Vector checks;
    bool strict;
};


class GitTools::EnforceAdmins: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    std::string url;
    bool enabled;
};

class GitTools::UsersTeamsApps: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    /** This is the list of users with review dismissal access. */
    User::Vector users;

    /** This is the list of teams with review dismissal access. */
    Team::Vector teams;

    // Apps currently ignored
};

class GitTools::DismissalRestrictions: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    std::string url;
    std::string usersURL;
    std::string teamsURL;

    UsersTeamsApps usersTeamsApps;
};

/**
 * This appears to be the list associated with who can do what with pull requests.
 */
class GitTools::RequiredPullRequestReviews: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    std::string url;
    DismissalRestrictions dismissalRestrictions;

    /** This controls who can bypass pull request requirements. */
    UsersTeamsApps bypassPullRequestAllowances;

    bool dismissStaleReviews;
    bool requireCodeOwnerReviews;
    int requiredApprovingReviewCount;
    bool requireLastPushApproval;
};

/**
 * This controls who does and does not have access. I might not have this structure
 * correct. It looked like a bunch of users/teams/apps.
 */
class GitTools::Restrictions: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    std::string url;
    std::string usersURL;
    std::string teamsURL;
    std::string appsURL;

    UsersTeamsApps usersTeamsApps;
};

/**
 * Are signed commits required? I'm not clear what the difference is between signed
 * and unsigned commits.
 */
class GitTools::RequiredSignatures: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    std::string url;
    bool enabled;
};

/**
 * The return from /repos/{owner}/{repo}/branches/{branch}/protection.
 */
class GitTools::BranchProtection: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

protected:
    std::string url;
    std::string contextsURL;
    std::string protectionURL;
    std::string name;
    bool enabled;

    RequiredStatusChecks requiredStatusChecks;
    EnforceAdmins enforceAdmins;
    RequiredPullRequestReviews requiredPullRequestReviews;
    Restrictions restrictions;
    RequiredSignatures requiredSignatures;

    EnabledFlag requiredLinearHistory;
    EnabledFlag allowForcePushes;
    EnabledFlag allowDeletions;
    EnabledFlag blockCreations;
    EnabledFlag requiredConversationResolution;

    /** Is read-only? Default == false. */
    EnabledFlag lockBranch;

    /** Whether users can pull changes from upstream when the branch is locked. Set to `true` to allow fork syncing. Set to `false` (default) to prevent fork syncing. */
    EnabledFlag allowForkSyncing;
};
