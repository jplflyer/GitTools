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

/**
 * This is a generic flag like "foo": { "enabled": true; }
 * This appears in a few places.
 */
class GitTools::EnabledFlag: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

    void setJSON_IfSet(JSON &json, const std::string &key) const;

    // Getters/setters
    bool getIsSet() const { return isSet; }
    bool getEnabled() const { return enabled; }

    void setEnabled(bool value) { enabled = value; isSet = true; }
    void clear() { enabled = false; isSet = false; }

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

        JSON toJSON() const override;
        void fromJSON(const JSON &) override;

    protected:
        std::string context;
        int appId;
    };

    JSON toJSON() const override;
    void fromJSON(const JSON &) override;
    void setJSON_IfSet(JSON &json, const std::string &key) const;

protected:
    bool enabled = false;
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
    void setJSON_IfSet(JSON &json, const std::string &key) const;

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
    void setJSON_IfSet(JSON &json, const std::string &key) const;

protected:
    bool enabled = false;
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
    void setJSON_IfSet(JSON &json, const std::string &key) const;

protected:
    bool enabled = false;
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
 * This controls who does and does not have access. I might not have this structure
 * correct. It looked like a bunch of users/teams/apps.
 */
class GitTools::Restrictions: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;
    void setJSON_IfSet(JSON &json, const std::string &key) const;

protected:
    bool enabled = false;
    std::string url;
    std::string usersURL;
    std::string teamsURL;
    std::string appsURL;

    UsersTeamsApps usersTeamsApps;
};

/**
 * The return from /repos/{owner}/{repo}/branches/{branch}/protection.
 *
 * Note that the various sections may only appear when they're enabled,
 * and that they're off if they don't appear. For instance, if in the web
 * GUI you click  "Require a Pull Request before Merging", there's no boolean
 * flag that corresponds to that checkbox. However, when retrieving protections,
 * if that checkbox doesn't appear, then "required_pull_request_reviews" isn't
 * included. So in a way, there's an implied boolean.
 *
 * This seems to be true for all of the sections.
 */
class GitTools::BranchProtection: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

    // Getters and setters.
    bool getEnabled() const { return enabled; }
    const std::string &getUrl() const { return url; }
    const std::string &getContextsURL() const { return contextsURL; }
    const std::string &getProtectionURL() const { return protectionURL; }
    const std::string &getName() const { return name; }
    const EnforceAdmins &getEnforceAdmins() const { return enforceAdmins; }
    const RequiredStatusChecks &getRequiredStatusChecks() const { return requiredStatusChecks; }
    const RequiredPullRequestReviews &getRequiredPullRequestReviews() const { return requiredPullRequestReviews; }
    const RequiredSignatures &getRequiredSignatures() const { return requiredSignatures; }
    const Restrictions &getRestrictions() const { return restrictions; }
    const EnabledFlag &getAllowDeletions() const { return allowDeletions; }
    const EnabledFlag &getAllowForcePushes() const { return allowForcePushes; }
    const EnabledFlag &getAllowForkSyncing() const { return allowForkSyncing; }
    const EnabledFlag &getRequiredLinearHistory() const { return requiredLinearHistory; }
    const EnabledFlag &getBlockCreations() const { return blockCreations; }
    const EnabledFlag &getRequiredConversationResolution() const { return requiredConversationResolution; }
    const EnabledFlag &getLockBranch() const { return lockBranch; }

    BranchProtection & setEnabled(bool value) { enabled = value; return *this; }
    BranchProtection & setUrl(const std::string &value) { url = value; return *this; }
    BranchProtection & setContextsURL(const std::string &value) { contextsURL = value; return *this; }
    BranchProtection & setProtectionURL(const std::string &value) { protectionURL = value; return *this; }
    BranchProtection & setName(const std::string &value) { name = value; return *this; }
    BranchProtection & setEnforceAdmins(const EnforceAdmins &value) { enforceAdmins = value; return *this; }
    BranchProtection & setRequiredStatusChecks(const RequiredStatusChecks &value) { requiredStatusChecks = value; return *this; }
    BranchProtection & setRequiredPullRequestReviews(const RequiredPullRequestReviews &value) { requiredPullRequestReviews = value; return *this; }
    BranchProtection & setRequiredSignatures(const RequiredSignatures &value) { requiredSignatures = value; return *this; }
    BranchProtection & setRestrictions(const Restrictions &value) { restrictions = value; return *this; }
    BranchProtection & setAllowDeletions(const EnabledFlag &value) { allowDeletions = value; return *this; }
    BranchProtection & setAllowForcePushes(const EnabledFlag &value) { allowForcePushes = value; return *this; }
    BranchProtection & setAllowForkSyncing(const EnabledFlag &value) { allowForkSyncing = value; return *this; }
    BranchProtection & setRequiredLinearHistory(const EnabledFlag &value) { requiredLinearHistory = value; return *this; }
    BranchProtection & setBlockCreations(const EnabledFlag &value) { blockCreations = value; return *this; }
    BranchProtection & setRequiredConversationResolution(const EnabledFlag &value) { requiredConversationResolution = value; return *this; }
    BranchProtection & setLockBranch(const EnabledFlag &value) { lockBranch = value; return *this; }

protected:
    // Enabled appears in the docs, although my testing never shows it being passed.
    // However, we use it to indicate whether we received real data vs. being disabled.
    bool enabled = false;

    std::string url;
    std::string contextsURL;
    std::string protectionURL;
    std::string name;

    EnforceAdmins enforceAdmins;
    RequiredStatusChecks requiredStatusChecks;
    RequiredPullRequestReviews requiredPullRequestReviews;
    RequiredSignatures requiredSignatures;
    Restrictions restrictions;

    EnabledFlag allowDeletions;
    EnabledFlag allowForcePushes;

    /** Whether users can pull changes from upstream when the branch is locked. Set to `true` to allow fork syncing. Set to `false` (default) to prevent fork syncing. */
    EnabledFlag allowForkSyncing;

    EnabledFlag requiredLinearHistory;
    EnabledFlag blockCreations;
    EnabledFlag requiredConversationResolution;

    /** Is read-only? Default == false. */
    EnabledFlag lockBranch;

};
