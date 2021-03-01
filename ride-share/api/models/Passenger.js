/*
    Ride Share ORM
    Passenger Model
    COS 243
    Daisy Bell & Matthias Dean
*/

const { Model } = require("../db");

class Passenger extends Model {
  static get tableName() {
    return "Passenger";
  }
  static get idColumn() {
    return ["passenger_id", "ride_id"];
  }
  static get relationMappings() {
    return {
      users: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./User"),
        join: {
          from: "Passenger.passenger_id",
          to: "User.user_id",
        },
      },
      rides: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Ride"),
        join: {
          from: "Passenger.ride_id",
          to: "Ride.ride_id",
        },
      },
    };
  }
}

module.exports = Passenger;
