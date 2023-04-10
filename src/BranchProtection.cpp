#include <showlib/CommonUsing.h>

#include "BranchProtection.h"

//======================================================================
// BranchProtection is the top-level return from getting branch protection.
//======================================================================

JSON GitTools::BranchProtection::toJSON() const {
    JSON json = JSON::object();

    // These fields always appear.
    allowDeletions.setJSON_IfSet(json, "allow_deletions");
    allowForcePushes.setJSON_IfSet(json, "allow_force_pushes");
    allowForkSyncing.setJSON_IfSet(json, "allow_fork_syncing");
    blockCreations.setJSON_IfSet(json, "block_creations");
    json["enforce_admins"] = enforceAdmins.toJSON();
    lockBranch.setJSON_IfSet(json, "lock_branch");
    requiredConversationResolution.setJSON_IfSet(json, "required_conversation_resolution");
    requiredLinearHistory.setJSON_IfSet(json, "required_linear_history");
    json["required_signatures"] = requiredSignatures.toJSON();

    setStringValue(json, "url", url);

    // These fields only appear if enabled.
    requiredPullRequestReviews.setJSON_IfSet(json, "required_pull_request_reviews");
    requiredStatusChecks.setJSON_IfSet      (json, "required_status_checks");
    restrictions.setJSON_IfSet              (json, "restrictions");

    // These aren't being returned in any of my tests.
    setStringValue(json, "name", name);
    setStringValue(json, "contexts_url", contextsURL);
    setStringValue(json, "protection_url", protectionURL);

    return json;
}

void GitTools::BranchProtection::fromJSON(const JSON &json) {
    // We get the message field on various errors. During testing,
    // I saw "Not Found" and "Branch not protected".
    // For now, we'll just return not-enabled and not look for more.
    if (hasKey(json, "message")) {
        return;
    }

    enabled = true;

    // These fields always appear.
    allowDeletions.fromJSON( jsonValue(json, "allow_deletions"));
    allowForcePushes.fromJSON( jsonValue(json, "allow_force_pushes"));
    allowForkSyncing.fromJSON( jsonValue(json, "allow_fork_syncing"));
    blockCreations.fromJSON( jsonValue(json, "block_creations"));
    enforceAdmins.fromJSON( jsonValue(json, "enforce_admins") );
    lockBranch.fromJSON( jsonValue(json, "lock_branch"));
    requiredConversationResolution.fromJSON( jsonValue(json, "required_conversation_resolution"));
    requiredLinearHistory.fromJSON( jsonValue(json, "required_linear_history") );
    requiredSignatures.fromJSON( jsonValue(json, "required_signatures") );

    url = stringValue(json, "url");

    // These fields only appear if enabled.
    requiredStatusChecks.fromJSON( jsonValue(json, "required_status_checks") );
    requiredPullRequestReviews.fromJSON( jsonValue(json, "required_pull_request_reviews") );
    restrictions.fromJSON( jsonValue(json, "restrictions") );

    // These aren't being returned in any of my tests.
    contextsURL = stringValue(json, "contexts_url");
    protectionURL = stringValue(json, "protection_url");
    name = stringValue(json, "name");
}

//======================================================================
//
//======================================================================

JSON GitTools::RequiredPullRequestReviews::toJSON() const {
    JSON json = JSON::object();

    setStringValue(json, "url", url);

    bypassPullRequestAllowances.setJSON_IfSet(json, "bypass_pull_request_allowances");
    dismissalRestrictions.setJSON_IfSet(json, "dismissal_restrictions");

    json["dismiss_stale_reviews"] = dismissStaleReviews;
    json["require_code_owner_reviews"] = requireCodeOwnerReviews;
    json["require_last_push_approval"] = requireLastPushApproval;
    json["required_appriving_review_count"] = requiredApprovingReviewCount;

    return json;
}

void GitTools::RequiredPullRequestReviews::fromJSON(const JSON &json)
{
    if ( ! json.empty() ) {
        enabled = true;

        dismissalRestrictions.fromJSON( jsonValue(json, "dismissal_restrictions") );
        bypassPullRequestAllowances.fromJSON( jsonValue(json, "bypass_pull_request_allowances") );
        url = stringValue(json, "url");
        dismissStaleReviews = boolValue(json, "dismiss_stale_review");
        requireCodeOwnerReviews = boolValue(json, "require_code_owner_reviews");
        requiredApprovingReviewCount = intValue(json, "required_appriving_review_count");
        requireLastPushApproval = boolValue(json, "require_last_push_approvall");
    }
}

/**
 * We set json[key] only if isSet.
 */
void GitTools::RequiredPullRequestReviews::setJSON_IfSet(JSON &json, const std::string &key) const {
    if (enabled) {
        json[key] = toJSON();
    }
}

//======================================================================
// Restritions are who has access to the branch.
//======================================================================

JSON GitTools::Restrictions::toJSON() const {
    JSON json = usersTeamsApps.toJSON();

    setStringValue(json, "url", url);
    setStringValue(json, "users_url", usersURL);
    setStringValue(json, "teams_url", teamsURL);
    setStringValue(json, "apps_url", appsURL);

    return json;
}

void GitTools::Restrictions::fromJSON(const JSON &json) {
    if (!json.empty()) {
        enabled = true;

        url = stringValue(json, "url");
        usersURL = stringValue(json, "users_url");
        teamsURL = stringValue(json, "teams_url");
        appsURL = stringValue(json, "apps_url");

        usersTeamsApps.fromJSON(json);
    }
}

/**
 * We set json[key] only if isSet.
 */
void GitTools::Restrictions::setJSON_IfSet(JSON &json, const std::string &key) const {
    if (enabled) {
        json[key] = toJSON();
    }
}
//======================================================================
// A portion of getting branch protection info.
//======================================================================

JSON GitTools::RequiredSignatures::toJSON() const {
    JSON json = JSON::object();

    json["url"] = url;
    json["enabled"] = enabled;

    return json;
}

void GitTools::RequiredSignatures::fromJSON(const JSON &json) {
    url = stringValue(json, "url");
    enabled = boolValue(json, "enabled");
}

//======================================================================
// This is a generic flag like:
//		"foo": { "enabled": true; }
// This appears in a few places.
//======================================================================

JSON GitTools::EnabledFlag::toJSON() const {
    JSON json = JSON::object();

    json["enabled"] = enabled;

    return json;
}

void GitTools::EnabledFlag::fromJSON(const JSON & json) {
    if (!json.empty()) {
        enabled = boolValue(json, "enabled");
        isSet = true;
    }
}

/**
 * We set json[key] only if isSet.
 */
void GitTools::EnabledFlag::setJSON_IfSet(JSON &json, const std::string &key) const {
    if (isSet) {
        json[key] = toJSON();
    }
}

//======================================================================
//
//======================================================================
JSON GitTools::RequiredStatusChecks::toJSON() const {
    JSON json = JSON::object();

    json["url"] = url;
    json["contexts_url"] = contextsURL;
    json["enforcement_level"] = enforcementLevel;
    json["strict"] = strict;

    json["contexts"] = contexts.toJSON();
    json["checks"] = checks.toJSON();

    return json;
}

void GitTools::RequiredStatusChecks::fromJSON(const JSON &json) {
    url = stringValue(json, "url");
    contextsURL = stringValue(json, "contexts_url");
    enforcementLevel = stringValue(json, "enforcement_level");
    strict = boolValue(json, "strict");

    contexts.fromJSON( jsonArray(json, "contexts") );
    checks.fromJSON( jsonArray(json, "checks") );
}

/**
 * We set json[key] only if isSet.
 */
void GitTools::RequiredStatusChecks::setJSON_IfSet(JSON &json, const std::string &key) const {
    if (enabled) {
        json[key] = toJSON();
    }
}


JSON GitTools::RequiredStatusChecks::Check::toJSON() const {
    JSON json = JSON::object();

    json["context"] = context;
    json["app_id"] = appId;

    return json;
}

void GitTools::RequiredStatusChecks::Check::fromJSON(const JSON & json) {
    context = stringValue(json, "context");
    appId = intValue(json, "app_id");
}

//======================================================================
//
//======================================================================
JSON GitTools::EnforceAdmins::toJSON() const {
    JSON json = JSON::object();

    json["url"] = url;
    json["enabled"] = enabled;

    return json;
}

void GitTools::EnforceAdmins::fromJSON(const JSON &json) {
    url = stringValue(json, "url");
    enabled = boolValue(json, "enabled");
}

//======================================================================
//
//======================================================================
JSON GitTools::UsersTeamsApps::toJSON() const {
    JSON json = JSON::object();

    if ( !users.empty() ) {
        json["users"] = users.toJSON();
    }

    if ( !teams.empty() ) {
        json["teams"] = teams.toJSON();
    }

    return json;
}

void GitTools::UsersTeamsApps::fromJSON(const JSON &json) {
    users.fromJSON( jsonArray(json, "users") );
    teams.fromJSON( jsonArray(json, "teams") );
}

/**
 * We set json[key] only if isSet.
 */
void GitTools::UsersTeamsApps::setJSON_IfSet(JSON &json, const std::string &key) const {
    if ( !users.empty() || !teams.empty() ) {
        json[key] = toJSON();
    }
}



//======================================================================
//
//======================================================================
JSON GitTools::DismissalRestrictions::toJSON() const {
    // This is strange. Users, Teams, and Apps are in our base
    // json object, so we initiate this way.
    JSON json = usersTeamsApps.toJSON();

    setStringValue(json, "url", url);
    setStringValue(json, "users_url", usersURL);
    setStringValue(json, "teams_url", teamsURL);

    return json;
}

void GitTools::DismissalRestrictions::fromJSON(const JSON &json) {
    if (!json.empty()) {
        enabled = true;
        usersTeamsApps.fromJSON(json);
        url = stringValue(json, "url");
        usersURL = stringValue(json, "users_url");
        teamsURL = stringValue(json, "teams_url");
    }
}

/**
 * We set json[key] only if isSet.
 */
void GitTools::DismissalRestrictions::setJSON_IfSet(JSON &json, const std::string &key) const {
    if (enabled) {
        json[key] = toJSON();
    }
}
