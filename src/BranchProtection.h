#pragma once

#include <string>
#include <showlib/JSONSerializable.h>
#include <showlib/StringVector.h>

#include "User.h"
#include "Team.h"

namespace GitTools {
    enum class TriValueBoolean;

    class ExtendedSerializable;

    class RequiredStatusChecks;
    class EnforceAdmins;
    class UsersTeamsApps;
    class DismissalRestrictions;
    class RequiredPullRequestReviews;
    class Restrictions;
    class EnabledFlag;
    class RequiredSignatures;
    class BranchProtection;
    class UpdateBranchProtection;
}

/**
 * This is basically a boolean that might be unset.
 */
enum class GitTools::TriValueBoolean {
    Unset, False, True
};

void setJSON(JSON &json, const std::string &key, GitTools::TriValueBoolean value);

GitTools::TriValueBoolean triValueBoolean(bool value);

/**
 *
 */
class GitTools::ExtendedSerializable : public ShowLib::JSONSerializable {
public:
    void setJSON_IfSet(JSON &json, const std::string &key, bool nullOnEmpty = false) const;

    bool getIsSet() const { return isSet; }
    void markSet() { isSet = true; }
    void markClear() { isSet = false; }

    template <class T>
    void markIfChanged(const T &oldValue, const T &newValue) {
        if (oldValue != newValue) {
            markSet();
        }
    }

protected:
    bool isSet = false;
};

/**
 * This is a generic flag like "foo": { "enabled": true; }
 * This appears in a few places.
 */
class GitTools::EnabledFlag: public ExtendedSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

    // Getters/setters
    bool getEnabled() const { return enabled; }

    // Implicit conversion to boolean.
    operator bool() const { return enabled; }

    // Implicit conversion to TriValueBoolean.
    operator TriValueBoolean() const { return triValueBoolean(enabled); }

    void setEnabled(bool value) { enabled = value; markSet(); }
    void clear() { enabled = false; markClear(); }

protected:
    bool enabled = false;
};

class GitTools::RequiredStatusChecks: public ExtendedSerializable {
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
    // Overrides.
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

    // Getters and setters.
    const std::string & getURL() const { return url; }
    bool getEnabled() const { return enabled; }

    // Provide a simple conversion to boolean.
    operator TriValueBoolean() const { return triValueBoolean(enabled); }

    void enable() { enabled = true; }
    void disable() { enabled = false; }

protected:
    std::string url;
    bool enabled = false;
};

class GitTools::UsersTeamsApps: public ExtendedSerializable {
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

class GitTools::DismissalRestrictions: public ExtendedSerializable {
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
class GitTools::RequiredPullRequestReviews: public ExtendedSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

    // Getters and setters.
    /** If set, then new commits to the branch will force-expire pending Pull Requests. */
    bool getDismissStaleReviews() const { return dismissStaleReviews; }

    /** If set, then a designated code owner must review the Pull Request. */
    bool getRequireCodeOwnerReviews() const { return requireCodeOwnerReviews; }

    /** If set, then the most recent push must be reviewed by someone other than the person who pushed it. */
    bool getRequireLastPushApproval() const { return requireLastPushApproval; }

    /** This sets the number of approvals required. */
    int getRequireApprovingReviewCount() const { return requiredApprovingReviewCount; }

    RequiredPullRequestReviews & setDismissStallReviews(bool value) { markIfChanged (value, dismissStaleReviews); dismissStaleReviews = value; return *this; }
    RequiredPullRequestReviews & setRequireCodeOwnerReviews(bool value) { markIfChanged (value, requireCodeOwnerReviews); requireCodeOwnerReviews = value; return *this; }
    RequiredPullRequestReviews & setRequireLastPushApproval(bool value) { markIfChanged (value, requireLastPushApproval); requireLastPushApproval = value; return *this; }
    RequiredPullRequestReviews & setApprovingReviewCount(int value) { markIfChanged (value, requiredApprovingReviewCount); requiredApprovingReviewCount = value; return *this; }

    RequiredPullRequestReviews & clearAll();

protected:
    std::string url;

    /** Controls who can dismiss pull request reviews. */
    DismissalRestrictions dismissalRestrictions;

    /** This controls who can bypass pull request requirements. */
    UsersTeamsApps bypassPullRequestAllowances;

    bool dismissStaleReviews = false;
    bool requireCodeOwnerReviews = false;
    int requiredApprovingReviewCount = 0;
    bool requireLastPushApproval = false;
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
class GitTools::Restrictions: public ExtendedSerializable {
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

    // Getters and setters. For getters, we have a bunch that are
    // non-const as you might need to call setters on the resulting objects.
    bool getEnabled() const { return enabled; }

    const std::string & getUrl() const { return url; }
    const std::string & getContextsURL() const { return contextsURL; }
    const std::string & getProtectionURL() const { return protectionURL; }
    const std::string & getName() const { return name; }

    const EnabledFlag & getAllowDeletions() const { return allowDeletions; }
    EnabledFlag & getAllowDeletions() { return allowDeletions; }

    const EnabledFlag & getAllowForcePushes() const { return allowForcePushes; }
    EnabledFlag & getAllowForcePushes() { return allowForcePushes; }

    const EnabledFlag & getAllowForkSyncing() const { return allowForkSyncing; }
    EnabledFlag & getAllowForkSyncing() { return allowForkSyncing; }

    const EnabledFlag & getBlockCreations() const { return blockCreations; }
    EnabledFlag & getBlockCreations() { return blockCreations; }

    const EnforceAdmins & getEnforceAdmins() const { return enforceAdmins; }
    EnforceAdmins & getEnforceAdmins() { return enforceAdmins; }

    const EnabledFlag & getLockBranch() const { return lockBranch; }
    EnabledFlag & getLockBranch() { return lockBranch; }

    const EnabledFlag & getRequiredConversationResolution() const { return requiredConversationResolution; }
    EnabledFlag & getRequiredConversationResolution() { return requiredConversationResolution; }

    const EnabledFlag & getRequiredLinearHistory() const { return requiredLinearHistory; }
    EnabledFlag & getRequiredLinearHistory() { return requiredLinearHistory; }

    const RequiredPullRequestReviews & getRequiredPullRequestReviews() const { return requiredPullRequestReviews; }
    RequiredPullRequestReviews & getRequiredPullRequestReviews() { return requiredPullRequestReviews; }

    const RequiredSignatures & getRequiredSignatures() const { return requiredSignatures; }
    RequiredSignatures & getRequiredSignatures() { return requiredSignatures; }

    const RequiredStatusChecks & getRequiredStatusChecks() const { return requiredStatusChecks; }
    RequiredStatusChecks & getRequiredStatusChecks() { return requiredStatusChecks; }

    const Restrictions & getRestrictions() const { return restrictions; }
    Restrictions & getRestrictions() { return restrictions; }

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

class GitTools::UpdateBranchProtection: public ShowLib::JSONSerializable {
public:
    JSON toJSON() const override;
    void fromJSON(const JSON &) override;

    UpdateBranchProtection() = default;
    UpdateBranchProtection(const BranchProtection &);

    UpdateBranchProtection & operator=(const BranchProtection &);

    bool getAllowDeletions() const { return allowDeletions; }

    const TriValueBoolean & getAllowForcePushes() const { return allowForcePushes; }
    const TriValueBoolean & getEnforceAdmins() const { return enforceAdmins; }

    bool getAllowForkSyncing() { return allowForkSyncing; }
    bool getBlockCreations() const { return blockCreations; }
    bool getLockBranch() { return lockBranch; }
    bool getRequiredConversationResolution() { return requiredConversationResolution; }
    bool getRequiredLinearHistory() { return requiredLinearHistory; }

    const RequiredPullRequestReviews & getRequiredPullRequestReviews() const { return requiredPullRequestReviews; }
    RequiredPullRequestReviews & getRequiredPullRequestReviews() { return requiredPullRequestReviews; }

    const RequiredStatusChecks & getRequiredStatusChecks() const { return requiredStatusChecks; }
    RequiredStatusChecks & getRequiredStatusChecks() { return requiredStatusChecks; }

    const Restrictions & getRestrictions() const { return restrictions; }
    Restrictions & getRestrictions() { return restrictions; }

    UpdateBranchProtection & setEnforceAdmins(bool value) { enforceAdmins = value ? TriValueBoolean::True : TriValueBoolean::False; return *this; }
    UpdateBranchProtection & setAllowForcePushes(bool value) { allowForcePushes = value ? TriValueBoolean::True : TriValueBoolean::False; return *this; }

    UpdateBranchProtection & setRequiredStatusChecks(const RequiredStatusChecks &value) { requiredStatusChecks = value; return *this; }
    UpdateBranchProtection & setRequiredPullRequestReviews(const RequiredPullRequestReviews &value) { requiredPullRequestReviews = value; return *this; }
    UpdateBranchProtection & setRestrictions(const Restrictions &value) { restrictions = value; return *this; }
    UpdateBranchProtection & setAllowDeletions(bool value) { allowDeletions = value; return *this; }
    UpdateBranchProtection & setAllowForkSyncing(bool value) { allowForkSyncing = value; return *this; }
    UpdateBranchProtection & setRequiredLinearHistory(bool value) { requiredLinearHistory = value; return *this; }
    UpdateBranchProtection & setBlockCreations(bool value) { blockCreations = value; return *this; }
    UpdateBranchProtection & setRequiredConversationResolution(bool value) { requiredConversationResolution = value; return *this; }
    UpdateBranchProtection & setLockBranch(bool value) { lockBranch = value; return *this; }

protected:
    // These are required.
    TriValueBoolean enforceAdmins = TriValueBoolean::Unset;
    RequiredStatusChecks requiredStatusChecks;
    RequiredPullRequestReviews requiredPullRequestReviews;
    Restrictions restrictions;

    // These are boolean or null.
    TriValueBoolean allowForcePushes = TriValueBoolean::Unset;

    // And these are boolean.
    bool allowDeletions = false;
    bool allowForkSyncing = false;
    bool blockCreations = false;
    bool lockBranch = false;
    bool requiredConversationResolution = false;
    bool requiredLinearHistory = false;
};
