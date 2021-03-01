/*
    Ride Share ORM
    Location Model
    COS 243
    Daisy Bell & Matthias Dean
*/

const { Model } = require("../db");

class Location extends Model {
  static get tableName() {
    return "Location";
  }
  static get idColumn() {
    return "location_id";
  }
  static get relationMappings() {
    return {
      states: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./State"),
        join: {
          from: "Location.state",
          to: "State.abbreviation",
        },
      },
      from_rides: {
        relation: Model.HasManyRelation,
        modelClass: require("./Ride"),
        join: {
          from: "Location.location_id",
          to: "Ride.from_location_id",
        },
      },
      to_rides: {
        relation: Model.HasManyRelation,
        modelClass: require("./Ride"),
        join: {
          from: "Location.location_id",
          to: "Ride.to_location_id",
        },
      },
    };
  }
}

module.exports = Location;
