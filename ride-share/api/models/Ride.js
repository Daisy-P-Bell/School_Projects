/*
    Ride Share ORM
    Ride Model
    COS 243
    Daisy Bell & Matthias Dean
*/

const { Model } = require("../db");

class Ride extends Model {
  static get tableName() {
    return "Ride";
  }
  static get idColumn() {
    return "ride_id";
  }
  static get relationMappings() {
    return {
      passengers: {
        relation: Model.HasManyRelation,
        modelClass: require("./Passenger"),
        join: {
          from: "Ride.ride_id",
          to: "Passenger.ride_id",
        },
      },
      drivers: {
        relation: Model.HasManyRelation,
        modelClass: require("./Drivers"),
        join: {
          from: "Ride.ride_id",
          to: "Drivers.ride_id",
        },
      },
      vehicles: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Vehicle"),
        join: {
          from: "Ride.vehicle_id",
          to: "Vehicle.vehicle_id",
        },
      },
      from_locations: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Location"),
        join: {
          from: "Ride.from_location_id",
          to: "Location.location_id",
        },
      },
      to_locations: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Location"),
        join: {
          from: "Ride.to_location_id",
          to: "Location.location_id",
        },
      },
    };
  }
}

module.exports = Ride;
