#include <showlib/CommonUsing.h>

#include "BranchProtection.h"

//======================================================================
// BranchProtection is the top-level return from getting branch protection.
//======================================================================

JSON GitTools::BranchProtection::toJSON() const {
    JSON json = JSON::object();

    setStringValue(json, "url", url);
    setStringValue(json, "name", name);
    setStringValue(json, "contexts_url", contextsURL);
    setStringValue(json, "protection_url", protectionURL);

    json["enabled"] = enabled;

    json["required_status_checks"] = requiredStatusChecks.toJSON();
    json["enforce_admins"] = enforceAdmins.toJSON();
    json["required_pull_request_reviews"] = requiredPullRequestReviews.toJSON();
    json["restrictions"] = restrictions.toJSON();
    json["required_signatures"] = requiredSignatures.toJSON();

    requiredLinearHistory.setJSON_IfSet(json, "required_linear_history");
    allowForcePushes.setJSON_IfSet(json, "allow_force_pushes");
    allowDeletions.setJSON_IfSet(json, "allow_deletions");
    blockCreations.setJSON_IfSet(json, "block_creations");
    requiredConversationResolution.setJSON_IfSet(json, "required_conversation_resolution");
    lockBranch.setJSON_IfSet(json, "lock_branch");
    allowForkSyncing.setJSON_IfSet(json, "allow_fork_syncing");


    return json;
}

void GitTools::BranchProtection::fromJSON(const JSON &json) {
    url = stringValue(json, "url");
    contextsURL = stringValue(json, "contexts_url");
    protectionURL = stringValue(json, "protection_url");
    name = stringValue(json, "name");
    enabled = boolValue(json, "enabled");

    requiredStatusChecks.fromJSON( jsonValue(json, "required_status_checks") );
    enforceAdmins.fromJSON( jsonValue(json, "enforce_admins") );
    requiredPullRequestReviews.fromJSON( jsonValue(json, "required_pull_request_reviews") );
    restrictions.fromJSON( jsonValue(json, "restrictions") );
    requiredSignatures.fromJSON( jsonValue(json, "required_signatures") );

    requiredLinearHistory.fromJSON( jsonValue(json, "required_linear_history") );
    allowForcePushes.fromJSON( jsonValue(json, "allow_force_pushes"));
    allowDeletions.fromJSON( jsonValue(json, "allow_deletions"));
    blockCreations.fromJSON( jsonValue(json, "block_creations"));
    requiredConversationResolution.fromJSON( jsonValue(json, "required_conversation_resolution"));
    lockBranch.fromJSON( jsonValue(json, "lock_branch"));
    allowForkSyncing.fromJSON( jsonValue(json, "allow_fork_syncing"));
}


//======================================================================
//
//======================================================================

JSON GitTools::RequiredPullRequestReviews::toJSON() const {
    JSON json = JSON::object();

    setStringValue(json, "url", url);

    json["dismissal_restrictions"] = dismissalRestrictions.toJSON();
    json["bypass_pull_request_allowances"] = bypassPullRequestAllowances.toJSON();
    json["dismiss_stale_reviews"] = dismissStaleReviews;
    json["require_code_owner_reviews"] = requireCodeOwnerReviews;
    json["required_appriving_review_count"] = requiredApprovingReviewCount;
    json["require_last_push_approval"] = requireLastPushApproval;

    return json;
}

void GitTools::RequiredPullRequestReviews::fromJSON(const JSON &json)
{
    url = stringValue(json, "url");
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
    url = stringValue(json, "url");
    usersURL = stringValue(json, "users_url");
    teamsURL = stringValue(json, "teams_url");
    appsURL = stringValue(json, "apps_url");

    usersTeamsApps.fromJSON(json);
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
