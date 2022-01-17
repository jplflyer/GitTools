#include "Team.h"

using namespace GitTools;

/**
 *
 */
void Team::fromJSON(const JSON &json)
{
    node_id = stringValue(json, "node_id");
    url = stringValue(json, "url");
    html_url = stringValue(json, "html_url");
    name = stringValue(json, "name");
    slug = stringValue(json, "slug");
    description = stringValue(json, "description");
    privacy = stringValue(json, "privacy");
    permission = stringValue(json, "permission");
    members_url = stringValue(json, "members_url");
    repositories_url = stringValue(json, "repositories_url");
    parent = stringValue(json, "parent");
    id = intValue(json, "id");
}

/**
 *
 */
JSON & Team::toJSON(JSON &json) const {
    json["node_id"] = node_id;
    json["url"] = url;
    json["html_url"] = html_url;
    json["name"] = name;
    json["slug"] = slug;
    json["description"] = description;
    json["privacy"] = privacy;
    json["permission"] = permission;
    json["members_url"] = members_url;
    json["repositories_url"] = repositories_url;
    json["parent"] = parent;
    json["id"] = id;

    return json;
}
