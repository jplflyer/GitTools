#include <showlib/CommonUsing.h>

#include "BranchProtection.h"

//======================================================================
// Various helpers.
//======================================================================

/**
 * Tri-value booleans can be unset (in which case we do nothing), or True/False.
 * This updates this JSON object[key] with our value.
 */
void setJSON(JSON &json, const std::string &key, GitTools::TriValueBoolean value) {
    switch (value) {
        case GitTools::TriValueBoolean::Unset: json[key] = nullptr; break;
        case GitTools::TriValueBoolean::False: json[key] = false; break;
        case GitTools::TriValueBoolean::True:  json[key] = true; break;
    }
}

/**
 * We set json[key] only if isSet. nullOnEmpty means we set json[key] to nullptr instead of leaving it untouched.
 * Some of the fields github expects prefer nullptr instead of unset entirely.
 */
void GitTools::ExtendedSerializable::setJSON_IfSet(JSON &json, const std::string &key, bool nullOnEmpty) const {
    if (isSet) {
        json[key] = toJSON();
    }
    else if (nullOnEmpty) {
        json[key] = nullptr;
    }
}

/**
 * Convert a boolean to a TriValueBoolean.
 */
GitTools::TriValueBoolean triValueBoolean(bool value) {
    return value ? GitTools::TriValueBoolean::True : GitTools::TriValueBoolean::False;
}

//======================================================================
// BranchProtection is the top-level return from getting branch protection.
//======================================================================

/**
 * Output to JSON.
 */
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

    // These fields may not be returned to us, but they're needed on uploads.
    json[ "required_pull_request_reviews" ] = requiredPullRequestReviews.toJSON();
    json[ "required_status_checks" ]        = requiredStatusChecks.toJSON();
    json[ "restrictions" ]                  = restrictions.toJSON();

    // These aren't being returned in any of my tests.
    setStringValue(json, "name", name);
    setStringValue(json, "contexts_url", contextsURL);
    setStringValue(json, "protection_url", protectionURL);

    return json;
}

/**
 * Read from JSON.
 */
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

/**
 * Output to JSON.
 */
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

/**
 * Read from JSON.
 */
void GitTools::RequiredPullRequestReviews::fromJSON(const JSON &json)
{
    if ( ! json.empty() ) {
        dismissalRestrictions.fromJSON( jsonValue(json, "dismissal_restrictions") );
        bypassPullRequestAllowances.fromJSON( jsonValue(json, "bypass_pull_request_allowances") );
        url = stringValue(json, "url");
        dismissStaleReviews = boolValue(json, "dismiss_stale_review");
        requireCodeOwnerReviews = boolValue(json, "require_code_owner_reviews");
        requiredApprovingReviewCount = intValue(json, "required_appriving_review_count");
        requireLastPushApproval = boolValue(json, "require_last_push_approvall");

        markSet();
    }
}

/**
 * Set to default values.
 */
GitTools::RequiredPullRequestReviews & GitTools::RequiredPullRequestReviews::clearAll() {
    markIfChanged(false, dismissStaleReviews);
    markIfChanged(false, requireCodeOwnerReviews);
    markIfChanged(0, requiredApprovingReviewCount);
    markIfChanged(false, requireLastPushApproval);

    dismissStaleReviews = false;
    requireCodeOwnerReviews = false;
    requiredApprovingReviewCount = 0;
    requireLastPushApproval = false;

    return *this;
}


//======================================================================
// Restritions are who has access to the branch.
//======================================================================

/**
 * Output to JSON.
 */
JSON GitTools::Restrictions::toJSON() const {
    JSON json = usersTeamsApps.toJSON();

    setStringValue(json, "url", url);
    setStringValue(json, "users_url", usersURL);
    setStringValue(json, "teams_url", teamsURL);
    setStringValue(json, "apps_url", appsURL);

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::Restrictions::fromJSON(const JSON &json) {
    markClear();
    if (!json.empty()) {
        cout << "Setting restrictions from: " << json.dump(2) << endl;
        markSet();

        url = stringValue(json, "url");
        usersURL = stringValue(json, "users_url");
        teamsURL = stringValue(json, "teams_url");
        appsURL = stringValue(json, "apps_url");

        usersTeamsApps.fromJSON(json);
    }
}

//======================================================================
// A portion of getting branch protection info.
//======================================================================

/**
 * Output to JSON.
 */
JSON GitTools::RequiredSignatures::toJSON() const {
    JSON json = JSON::object();

    json["url"] = url;
    json["enabled"] = enabled;

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::RequiredSignatures::fromJSON(const JSON &json) {
    url = stringValue(json, "url");
    enabled = boolValue(json, "enabled");
}

//======================================================================
// This is a generic flag like:
//		"foo": { "enabled": true; }
// This appears in a few places.
//======================================================================

/**
 * Output to JSON.
 */
JSON GitTools::EnabledFlag::toJSON() const {
    JSON json = JSON::object();

    json["enabled"] = enabled;

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::EnabledFlag::fromJSON(const JSON & json) {
    if (!json.empty()) {
        enabled = boolValue(json, "enabled");
        markSet();
    }
}

//======================================================================
//
//======================================================================

/**
 * Output to JSON.
 */
JSON GitTools::RequiredStatusChecks::toJSON() const {
    JSON json = JSON::object();

    setStringValue( json, "url", url);
    setStringValue( json, "contexts_url", contextsURL);
    setStringValue( json, "enforcement_level", enforcementLevel);
    json["strict"] = strict;

    if ( !contexts.empty()) {
        json["contexts"] = contexts.toJSON();
    }
    if ( !checks.empty()) {
        json["checks"] = checks.toJSON();
    }

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::RequiredStatusChecks::fromJSON(const JSON &json) {
    if (!json.empty()) {
        url = stringValue(json, "url");
        contextsURL = stringValue(json, "contexts_url");
        enforcementLevel = stringValue(json, "enforcement_level");
        strict = boolValue(json, "strict");

        contexts.fromJSON( jsonArray(json, "contexts") );
        checks.fromJSON( jsonArray(json, "checks") );

        markSet();
    }
}

/**
 * Output to JSON.
 */
JSON GitTools::RequiredStatusChecks::Check::toJSON() const {
    JSON json = JSON::object();

    json["context"] = context;
    json["app_id"] = appId;

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::RequiredStatusChecks::Check::fromJSON(const JSON & json) {
    context = stringValue(json, "context");
    appId = intValue(json, "app_id");
}

//======================================================================
//
//======================================================================

/**
 * Output to JSON.
 */
JSON GitTools::EnforceAdmins::toJSON() const {
    JSON json = JSON::object();

    json["url"] = url;
    json["enabled"] = enabled;

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::EnforceAdmins::fromJSON(const JSON &json) {
    url = stringValue(json, "url");
    enabled = boolValue(json, "enabled");
}

//======================================================================
//
//======================================================================
/**
 * Output to JSON.
 */
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

/**
 * Read from JSON.
 */
void GitTools::UsersTeamsApps::fromJSON(const JSON &json) {
    users.fromJSON( jsonArray(json, "users") );
    teams.fromJSON( jsonArray(json, "teams") );

    if (users.size() || teams.size()) {
        markSet();
    }
}

//======================================================================
//
//======================================================================
/**
 * Output to JSON.
 */
JSON GitTools::DismissalRestrictions::toJSON() const {
    // This is strange. Users, Teams, and Apps are in our base
    // json object, so we initiate this way.
    JSON json = usersTeamsApps.toJSON();

    setStringValue(json, "url", url);
    setStringValue(json, "users_url", usersURL);
    setStringValue(json, "teams_url", teamsURL);

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::DismissalRestrictions::fromJSON(const JSON &json) {
    if (!json.empty()) {
        usersTeamsApps.fromJSON(json);
        url = stringValue(json, "url");
        usersURL = stringValue(json, "users_url");
        teamsURL = stringValue(json, "teams_url");

        markSet();
    }
}

//======================================================================
// This is used for setting branch protection.
//======================================================================

/**
 * Copy from a BranchProtection.
 */
GitTools::UpdateBranchProtection::UpdateBranchProtection(const BranchProtection &bp) {
    *this = bp;
}

/**
 * Copy from a BranchProtection. This is made fairly simple given some of the implicit conversion methods
 * we have.
 */
GitTools::UpdateBranchProtection & GitTools::UpdateBranchProtection::operator=(const GitTools::BranchProtection &bp) {
    enforceAdmins = bp.getEnforceAdmins();

    requiredStatusChecks = bp.getRequiredStatusChecks();
    requiredPullRequestReviews = bp.getRequiredPullRequestReviews();
    restrictions = bp.getRestrictions();

    allowForcePushes = triValueBoolean(bp.getAllowForcePushes().getEnabled());

    allowDeletions = bp.getAllowDeletions();
    allowForkSyncing = bp.getAllowForkSyncing();
    blockCreations = bp.getAllowDeletions();
    lockBranch = bp.getLockBranch();
    requiredConversationResolution = bp.getRequiredConversationResolution();
    requiredLinearHistory = bp.getRequiredLinearHistory();

    return *this;
}

/**
 * Output to JSON.
 */
JSON GitTools::UpdateBranchProtection::toJSON() const {
    JSON json = JSON::object();

    // These four are required, but they are listed as "object/bool or null".
    requiredStatusChecks.setJSON_IfSet(json, "required_status_checks", true);
    setJSON(json, "enforce_admins", enforceAdmins);
    requiredPullRequestReviews.setJSON_IfSet(json, "required_pull_request_reviews", true);
    restrictions.setJSON_IfSet(json, "restrictions", true);

    // These are all optional.
    if (requiredLinearHistory) {
        json["required_linear_history"] = requiredLinearHistory;
    }
    setJSON(json, "allow_force_pushes", allowForcePushes);
    json["allow_deletions"] = allowDeletions;
    json["block_creations"] = blockCreations;
    json["lock_branch"] = lockBranch;
    json["required_conversation_resolution"] = requiredConversationResolution;
    json["required_linear_history"] = requiredLinearHistory;

    return json;
}

/**
 * Read from JSON.
 */
void GitTools::UpdateBranchProtection::fromJSON(const JSON &) {
}

